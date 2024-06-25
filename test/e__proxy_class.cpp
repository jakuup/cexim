// https://stackoverflow.com/questions/994488/what-is-proxy-class-in-c

#include <exception>
#include <vector>
#include <iostream>

struct Data
{
  std::string name;
  int value;

  Data(std::string t_name, int t_value)
    : name(t_name), value(t_value)
  {}

  Data &operator=(Data &t_data) {
    name  = t_data.name;
    value = t_data.value;
    return *this;
  }
};

template <typename T>
struct DataMapperProxy
{
  int index;
  std::vector<T>& data;

  DataMapperProxy<T>(int t_index, std::vector<T> &r_data)
    : index(t_index), data(r_data)
  {}

  void operator=(T t_value) {
    try {
      data.at(index) = t_value;
    }
    catch (std::out_of_range) {
      data.push_back(t_value);
    }
  }

  operator T() {
    return data.at(index);
  }
};

template <typename T>
struct DataMapper
{
  std::vector<T> data;

  DataMapperProxy<T> operator[](int t_index) {
    if (t_index > data.size()) {
      throw std::out_of_range("index=" + std::to_string(t_index) + " of DataMapper[]");
    }
    return DataMapperProxy<T>(t_index, data);
  }
};

int main()
{
  // --- int ---
  DataMapper<int> map_int;
  map_int[0] = 10;
  map_int[1] = 11;
  map_int[2] = 11;
  int int_test_2 = map_int[2];
  int int_test_1 = map_int[1];
  int int_test_0 = map_int[0];
  std::cout << "[2]=" + std::to_string(int_test_2) << std::endl;
  std::cout << "[1]=" + std::to_string(int_test_1) << std::endl;
  std::cout << "[0]=" + std::to_string(int_test_0) << std::endl;

  // --- struct/class ---
  DataMapper<Data> map_data;
  map_data[0] = Data("aa", 10);
  map_data[1] = Data("bb", 11);
  map_data[2] = Data("cc", 12);
  Data data_test_2 = map_data[2];
  Data data_test_1 = map_data[1];
  Data data_test_0 = map_data[0];
  std::cout << "[2]=" + data_test_2.name + ":" + std::to_string(data_test_2.value) << std::endl;
  std::cout << "[1]=" + data_test_1.name + ":" + std::to_string(data_test_1.value) << std::endl;
  std::cout << "[0]=" + data_test_0.name + ":" + std::to_string(data_test_0.value) << std::endl;

}
