// https://stackoverflow.com/questions/994488/what-is-proxy-class-in-c

#include <exception>
#include <vector>
#include <iostream>

struct Data
{
  std::string name;
  int value;

  Data &operator=(Data &t_data) {
    name  = t_data.name;
    value = t_data.value;
    return *this;
  }
};

struct DataMapperProxy
{
  int index;
  std::vector<int>& data;

  DataMapperProxy(int t_index, std::vector<int> &r_data)
    : index(t_index), data(r_data)
  {}

  void operator=(int t_value) {
    try {
      data.at(index) = t_value;
    }
    catch (std::out_of_range) {
      data.push_back(t_value);
    }
  }

  operator int() {
    return data.at(index);
  }

  operator std::string() {
    return "string(" + std::to_string(data.at(index)) + ")";
  }
};

struct DataMapper
{
  std::vector<int> data;

  DataMapperProxy operator[](int t_index) {
    if (t_index > data.size()) {
      throw std::out_of_range("index=" + std::to_string(t_index) + " of DataMapper[]");
    }
    return DataMapperProxy(t_index, data);
  }
};

int main()
{
  DataMapper mapper;

  mapper[0] = 10;
  mapper[1] = 11;
  mapper[2] = 12;

  std::cout << "[2]=" << std::to_string(mapper[2]) << std::endl;
  std::cout << "[1]=" << std::to_string(mapper[1]) << std::endl;
  std::cout << "[0]=" << std::to_string(mapper[0]) << std::endl;

  std::string str = mapper[1];
  std::cout << "[1]=" << str << std::endl;
}
