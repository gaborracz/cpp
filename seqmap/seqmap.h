#ifndef SEQMAP_H
#define SEQMAP_H

#include <map>
#include <vector>
#include <cstddef>

template <class Key, class T>
class seqmap
{
public:
  seqmap(const std::map<Key, T>& m)
  {
    typename std::map<Key, T>::const_iterator it = m.begin();
    typename std::map<Key, T>::const_iterator end = m.end();
    while (it != end)
    {
      m_data.push_back(&it->second);
      ++it;
    }
  }

  std::size_t size() const
  {
    return m_data.size();
  }

  const T& at(std::size_t idx) const
  {
    return *m_data.at(idx);
  }

private:
  std::vector<const T*> m_data;
};

#endif

