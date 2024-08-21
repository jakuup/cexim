#include <map>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include "argparse.hpp"

using namespace std;

std::string name;

struct Config {
  vector<string> name;
  int nargs;
};

struct Expected {
  ArgParse::Parsed parsed;
  string outmsg;
};

struct TestRun {
  vector<Config> config;    // addArgument() arguments
  vector<string> arguments; // parse() arguments
  Expected       expected;  // expected result
};

std::map<int, TestRun> tests {
  { 1,
    { {{{"X"}, 1}},
      {},
      {{true, {}}, "Missing positional argument"}
    }
  },
  { 2,
    { {{{"X"}, 1}},
      {"xxx"},
      {{false, { {"X", {"xxx"}} }}, ""}
    }
  },
  { 3,
    { {{{"X"}, 1}, {{"Y"}, 1}},
      {},
      {{true, {}}, "Missing 2 positional arguments"}
    }
  },
  { 4,
    { {{{"X"}, 1}, {{"Y"}, 1}},
      {"xxx"},
      {{true, {}}, "Missing positional argument"}
    }
  },
  { 5,
    { {{{"X"}, 1}, {{"Y"}, 1}},
      {"xxx", "yyy"},
      {{false, { {"X", {"xxx"}}, {"Y", {"yyy"}} }}, ""}
    }
  },
  { 6,
    { {{{"X"}, 1}, {{"-A", "--optA"}, 1}, {{"-B", "--optB"}, 2}},
      {"xxx", "-A", "aaa", "--optB", "bbb", "bbB"},
      {{false, { {"X", {"xxx"}}, {"-A", {"aaa"}}, {"--optA", {"aaa"}}, {"-B", {"bbb", "bbB"}}, {"--optB", {"bbb", "bbB"}} }}, ""}
    }
  },
  { 7,
    { {{{"X"}, 1}, {{"-A", "--optA"}, 1}, {{"--optB", "-B"}, 2}},
      {"xxx", "-A", "aaa", "--optB", "bbb"},
      {{true, {}}, "Missing '--optB' argument"}
    }
  },
  { 8,
    { {{{"X"}, 1}, {{"-A", "--optA"}, 1}, {{"--optB", "-B"}, 2}},
      {"xxx", "-A", "aaa", "--optB"},
      {{true, {}}, "Missing '--optB' 2 arguments"}
    }
  },
  { 9,
    { {{{"--option"}, 0}},
      {"--option"},
      {{false, { {"--option", {}} }}, ""}
    }
  },
  { 10,
    { {{{"--option"}, 0}},
      {"--option", "opt"},
      {{true, {}}, "Unexpected positional argument 'opt'"}
    }
  },
};


int run_test(int id)
{
  int result = 0;
  std::ostringstream oss;
  ArgParse *argparse = new ArgParse(name);

  try {
    TestRun& test = tests.at(id);
    for (auto input : test.config) {
      argparse->addArgument(input.name, input.nargs);
    }
    ArgParse::Parsed& parsed = argparse->parse(test.arguments);
    result |= (parsed != test.expected.parsed);
    oss << *argparse;
    result |= (oss.str() != test.expected.outmsg);
  }
  catch (std::out_of_range) {
    std::cout << "WARNING: unknown test id=" + std::to_string(id) << std::endl;
  }

  delete argparse;
  return result;
}

int main(int argc, const char *argv[])
{
  int result = 0;
#if 0
  map<string, string> a;
  unordered_map<string, string> b;
  unordered_map<string, string> c;
  a["-A"] = "aaa";
  a["-B"] = "bbb";
  a["X"] = "xxx";
  a["--a"] = "aaa";
  a["--b"] = "bbb";
  b["-A"] = "aaa";
  b["-B"] = "bbb";
  b["X"] = "xx";
  b["--a"] = "aaa";
  b["--b"] = "bbb";
  c["X"] = "xxx";
  c["-B"] = "bbb";
  c["-A"] = "aaa";
  c["--b"] = "bbb";
  c["--a"] = "aaa";
  result = (c == b);
  return result;
#endif
  name = *argv;

  if (argc > 1) {
    // run selected test(s)
    while (--argc > 0) {
      int id = std::atoi(*++argv);
      int m_result = run_test(id);
      if (m_result) {
        std::cout << "FAILED: test id=" + std::to_string(id) << std::endl;
        result |= m_result;
      }
    }
  }
  else {
    // run all selected tests
    for (auto i = tests.begin(); i != tests.end(); ++i) {
      int id = i->first;
      int m_result = run_test(id);
      if (m_result) {
        std::cout << "FAILED: test id=" + std::to_string(id) << std::endl;
        result |= m_result;
      }
    }
  }

  return result;
}
