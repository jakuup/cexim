#ifndef __ARGPARSE_HPP__
#define __ARGPARSE_HPP__

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class ArgParse {

public:

  /// @brief 
  struct Parsed {
    bool error;
    std::unordered_map<std::string, std::vector<std::string>> args;
    Parsed() : error(false), args() {}
  // test purposes only
    Parsed(bool error, std::unordered_map<std::string, std::vector<std::string>> args) : error(error), args(args) {}
    friend bool operator==(const Parsed& p1, const Parsed& p2);
    friend bool operator!=(const Parsed& p1, const Parsed& p2);
  };

  /// @brief
  /// @param t_exec  path to executable
  ArgParse(const std::string& exec);
  ~ArgParse();

  /// @brief 
  /// @param name 
  /// @param nargs 
  void addArgument(const std::vector<std::string>& name, int nargs);

  /// @brief 
  /// @param argv 
  /// @return 
  Parsed& parse(const std::vector<std::string>& argv);

private:

  struct impl;
  std::unique_ptr<impl> pimpl;

  // the main data structures
//  struct ArgData {
//    std::vector<std::string> names;
//    int nargs;
//    ArgData(int nargs, std::vector<std::string> names)
//    : nargs(nargs), names(names) {}
//  };
//  std::vector<ArgData*> m_argdata;
//  std::map<std::string, ArgData*> m_argname;
//  std::vector<std::string> m_argposn;

  // the parse() method execution results
//  std::string m_exec;
//  Parsed m_result;

  // temporary elements used by the parse() method
//  ArgData * tmp_argdata;
//  std::string tmp_argname;
//  void handleMissingPos(int posn);
//  void handleMissingOpt(int optn);
//  void handleError();

  /// @brief   The operator streams result of last parse() call that can be
  ///          printed out. It can be: error message along with usage help,
  ///          tbd
  friend std::ostream& operator<<(std::ostream& os, const ArgParse& ap);

};

#endif