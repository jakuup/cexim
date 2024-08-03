#ifndef __VKMAP_HPP__
#define __VKMAP_HPP__

#include <stdexcept>
#include <map>
#include <memory>
#include <vector>
#include <iostream>

/// @brief Vector-key Map class index [] access of std::map<std::vector<K>, O> container.
/// @tparam K type of key vector elements
/// @tparam O type of object in the container
template<typename K, typename O>
class VKMap {

  class proxy {

  public:

    proxy(std::vector<K> t_keys, std::map<std::vector<K>, O>& t_map);

    void operator=(O obj);

    operator O();

  private:

    std::vector<K> m_keys;
    std::map<std::vector<K>, O>& m_map;

    O* search(const K& key);
    void throw_exception(const std::string msg, const K& key);

  };

  std::map<std::vector<K>, O> m_map;

public:

  proxy operator[](const K key)
  {
    std::vector<K> k = { key };
    return proxy(k, m_map);
  }

  proxy operator[](const std::vector<K> key) {
    return proxy(key, m_map);
  }

};

#endif
