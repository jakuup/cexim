
#include "vkmap.hpp"

using namespace std;

template class VKMap<string, void*>;

template<typename K, typename O>
VKMap<K, O>::proxy::proxy(vector<K> t_keys, map<vector<K>, O>& t_map)
: m_keys(t_keys), m_map(t_map)
{
}

template<typename K, typename O>
void VKMap<K, O>::proxy::operator=(O obj)
{
  for (auto k : m_keys) {
    if (search(k)) {
      throw_exception("duplicated key element", k);
      return;
    }
  }
  m_map[m_keys] = obj;
}

template<typename K, typename O>
VKMap<K, O>::proxy::operator O()
{
  O *v = search(m_keys[0]);
  if (!v) {
    throw_exception("invalid key element", m_keys[0]);
  }
  return *v;
}

template<typename K, typename O>
O* VKMap<K, O>::proxy::search(const K& key)
{
  for (auto i = m_map.begin(); i != m_map.end(); ++i) {
    vector<K> kk = i->first;
    for (auto k : kk) {
      if (k == key) {
        return &m_map[kk];
      }
    }
  }
  return nullptr;
}

template<>
void VKMap<string, void*>::proxy::throw_exception(const string msg, const string& key)
{
  throw invalid_argument(msg + ": " + key);
}
