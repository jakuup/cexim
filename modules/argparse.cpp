#include <stdexcept>
#include <memory>

#include "vkmap.hpp"
#include "argparse.hpp"

using namespace std;

struct ArgData {
  vector<string> name;
  int nargs;
};

struct ArgParse::impl {

  vector<unique_ptr<ArgData>> _argdata;
  vector<void*> _argpos;        // vector with pointers to positional arguments metadata
  VKMap<string, void*> _argopt; // vector key map with pointers to optional arguments metadata

  string _exec;
  Parsed _result;

  string _tmp_argname = "";
  ArgData* _tmp_argdata = nullptr;

  // **************************************************************************
  void _addArgument(const vector<string>& name, int& nargs)
  {
    if (name.size() == 1 && name[0][0] != '-') {
      // positional argument, nargs is always 1
      _argdata.push_back(unique_ptr<ArgData>(new ArgData {name, 1}));
      _argpos.push_back(_argdata.back().get());
    }
    else {
      // option
      _argdata.push_back(unique_ptr<ArgData>(new ArgData {name, nargs}));
      _argopt[name] = _argdata.back().get();
    }
  }

  // **************************************************************************
  void _parse(const vector<string>& argv)
  {
    int posn = 0;
    int optn = 0;

    for (auto a : argv) {
      if (a[0] != '-') {
        if (!_tmp_argdata) {
          // positional argument
          if (posn < _argpos.size()) {
            ArgData* argdata = (ArgData*)_argpos[posn];
            _result.args[argdata->name[0]].push_back(a);
            ++posn;
          }
          else {
            _exec = "Unexpected positional argument '" + a + "'";
            _handleError();
            break;
          }
        }
        else {
          if (optn < _tmp_argdata->nargs) {
            // argument of option (AoO)
            for (auto an : _tmp_argdata->name) {
              _result.args[an].push_back(a);
            }
            ++optn;
            if (optn == _tmp_argdata->nargs) {
              _tmp_argdata = nullptr;
              _tmp_argname = "";
            }
          }
          else {
            _exec = "Unexpected option's " + _tmp_argname + " argument #" + to_string(optn + 1);
            _handleError();
            break;
          }
        }
      }
      else {
        // option
        if (_tmp_argdata) {
          _handleMissingOpt(optn);
          break;
        }

        try {
          _tmp_argdata = (ArgData*)(void*)_argopt[a];
        }
        catch (out_of_range) {
          _exec = "Unexpected option: " + a;
          _handleError();
          break;
        }
        if (_tmp_argdata->nargs) {
          // options with arguments will be added to _result later (see AoO)
          _tmp_argname = a;
          optn = 0;
        }
        else {
          // options with no arguments are added to _result here
          for (auto an : _tmp_argdata->name) {
            _result.args[an] = {};
          }
          _tmp_argdata = nullptr;
          _tmp_argname = "";
        }
      }
    }

    _handleMissingPos(posn);

    if (_tmp_argdata) {
      _handleMissingOpt(optn);
    }
  }

  // **************************************************************************
  void _handleMissingPos(int posn)
  {
    int count = _argpos.size() - posn;
    if (count) {
      if (count == 1) {
        _exec = "Missing positional argument";
      }
      else {
        _exec = "Missing " + to_string(count) + " positional arguments";
      }
      // dodać info których nazw brakuje...
      _handleError();
    }
  }

  // **************************************************************************
  void _handleMissingOpt(int optn)
  {
    if (_tmp_argdata) {
      int count = _tmp_argdata->nargs - optn;
      if (count == 1) {
        _exec = "Missing '" + _tmp_argname + "' argument";
      }
      else {
        _exec = "Missing '" + _tmp_argname + "' " + to_string(count) + " arguments";
      }
      _handleError();
    }
  }

  void _handleError()
  {
    _result.error = true;
    _result.args.clear();
  }

};


ArgParse::ArgParse(const string& exec)
: pimpl(unique_ptr<impl>(new impl))
{
}

ArgParse::~ArgParse()
{
}

void ArgParse::addArgument(const vector<string>& name, int nargs)
{
  pimpl->_addArgument(name, nargs);
}

ArgParse::Parsed& ArgParse::parse(const vector<string>& argv)
{
  pimpl->_parse(argv);
  return pimpl->_result;
}

bool operator==(const ArgParse::Parsed &p1, const ArgParse::Parsed &p2)
{
  return (p1.error == p2.error) && (p1.args == p2.args);
}

bool operator!=(const ArgParse::Parsed& p1, const ArgParse::Parsed& p2) {
  return !(p1 == p2);
}

ostream &operator<<(ostream &os, const ArgParse &ap)
{
  os << ap.pimpl->_exec;
  return os;
}
