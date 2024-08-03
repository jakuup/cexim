
#include <stdexcept>
#include <map>
#include <vector>
#include <iostream>

#include "vkmap.hpp"

using namespace std;

// ****************************************************************************
int test__string_voidptr()
{
  int error_cnt = 0;
  string *value;
  VKMap<string, void*> vkmap;

  unique_ptr<string[]> data {new string[5]};
  #define _A_  data[0]
  #define _B_  data[1]
  #define _C_  data[2]
  #define _nA_ data[3]
  #define _nB_ data[4]
  _A_  = "_A_";
  _B_  = "_B_";
  _C_  = "_C_";
  _nA_ = "!A!";
  _nB_ = "!B!";

  try {
    vkmap["a"] = &_A_;
  }
  catch (invalid_argument exception) {
    cout << "unexpected exception @" << __LINE__ << endl;
    ++error_cnt;
  }

  try {
    vkmap[vector<string>({"b"})] = &_B_;
  }
  catch (invalid_argument exception) {
    cout << "unexpected exception @" << __LINE__ << endl;
    ++error_cnt;
  }

  try {
    vkmap[vector<string>({"c", "cccc"})] = &_C_;
  }
  catch (invalid_argument exception) {
    cout << "unexpected exception @" << __LINE__ << endl;
    ++error_cnt;
  }

  try {
    vkmap[vector<string>({"a"})] = &_nA_;
    cout << "no exception @" << __LINE__ << endl;
    ++error_cnt;
  }
  catch (invalid_argument exception) {
    if (string(exception.what()) != string("duplicated key element: a")) {
      cout << "wrong exception @" << __LINE__ << ": " << exception.what() << endl;
      ++error_cnt;
    }
  }
  try {
    vkmap[vector<string>({"b", "c"})] = &_nB_;
    cout << "no exception @" << __LINE__ << endl;
    ++error_cnt;
  }
  catch (invalid_argument exception) {
    if (string(exception.what()) != string("duplicated key element: b")) {
      cout << "wrong exception @" << __LINE__ << ": " << exception.what() << endl;
      ++error_cnt;
    }
  }

  try {
    value = (string*)(void*)vkmap["a"];
    if (*value != _A_) {
      cout << "value error @" << __LINE__ << ": " << value << endl;
      ++error_cnt;
    }
  }
  catch (invalid_argument exception) {
    cout << "unexpected exception @" << __LINE__ << endl;
    ++error_cnt;
  }

  try {
    value = (string*)(void*)vkmap[vector<string>({"a"})];
    if (*value != _A_) {
      cout << "value error @" << __LINE__ << ": " << value << endl;
      ++error_cnt;
    }
  }
  catch (invalid_argument exception) {
    cout << "unexpected exception @" << __LINE__ << endl;
    ++error_cnt;
  }

  try {
    value = (string*)(void*)vkmap["b"];
    if (*value != _B_) {
      cout << "value error @" << __LINE__ << ": " << value << endl;
      ++error_cnt;
    }
  }
  catch (invalid_argument exception) {
    cout << "unexpected exception @" << __LINE__ << endl;
    ++error_cnt;
  }

  try {
    value = (string*)(void*)vkmap["c"];
    if (*value != _C_) {
      cout << "value error @" << __LINE__ << ": " << value << endl;
      ++error_cnt;
    }
  }
  catch (invalid_argument exception) {
    cout << "unexpected exception @" << __LINE__ << endl;
    ++error_cnt;
  }

  try {
    value = (string*)(void*)vkmap["cccc"];
    if (*value != _C_) {
      cout << "value error @" << __LINE__ << ": " << value << endl;
      ++error_cnt;
    }
  }
  catch (invalid_argument exception) {
    cout << "unexpected exception @" << __LINE__ << endl;
    ++error_cnt;
  }

  try {
    value = (string*)(void*)vkmap[vector<string>({"cccc", "h"})];
    if (*value != _C_) {
      cout << "value error @" << __LINE__ << ": " << value << endl;
      ++error_cnt;
    }
  }
  catch (invalid_argument exception) {
    cout << "unexpected exception @" << __LINE__ << endl;
    ++error_cnt;
  }

  try {
    value = (string*)(void*)vkmap[vector<string>({"h", "cccc"})];
    cout << "no exception @" << __LINE__ << endl;
    ++error_cnt;
  }
  catch (invalid_argument exception) {
    if (string(exception.what()) != string("invalid key element: h")) {
      cout << "wrong exception @" << __LINE__ << ": " << exception.what() << endl;
      ++error_cnt;
    }
  }

  try {
    value = (string*)(void*)vkmap["ccc"];
    cout << "no exception @" << __LINE__ << endl;
    ++error_cnt;
  }
  catch (invalid_argument exception) {
    if (string(exception.what()) != string("invalid key element: ccc")) {
      cout << "wrong exception @" << __LINE__ << ": " << exception.what() << endl;
      ++error_cnt;
    }
  }

  return error_cnt;
}

int main()
{
  return test__string_voidptr();
}
