#ifndef TRIOMINO_H
#define TRIOMINO_H

#include <iostream>
#include <utility>
#include <vector>
#include <set>
#include <array>
#include <algorithm>

template <typename T>
struct Tile
{
  // Represent the sides of a tile
  std::pair<T, T> a;
  std::pair<T, T> b;
  std::pair<T, T> c;

  bool isAFree{true};
  bool isBFree{true};
  bool isCFree{true};

  Tile(T x, T y, T z) : a{x, y}, b{y, z}, c{z, x} {}

  // 3 values as a soreted triple, order independent for equality check
  std::array<T, 3> values() const
  {
    std::array<T, 3> values{a.first, a.second, b.second};
    std::sort(values.begin(), values.end());

    return values;
  }
};

template <typename T>
class triomino
{
private:
  std::vector<Tile<T>> gameField{};
  int m_size{ 0 };

  bool sideMatch(const std::pair<T, T>& a, const std::pair<T, T>& b)
  {
    return ( a.first == b.first && a.second == b.second) || (a.first == b.second && a.second == b.first);
  }

  void addTile(Tile<T> tileToAdd)
  {
    gameField.push_back(tileToAdd);
    ++m_size;
  }

public:
  bool insert(T x, T y, T z)
  {
    Tile newTile{x, y, z};

    // if the game field is empty accept first tile
    if ( m_size == 0 )
    { 
      gameField.push_back(newTile);
      ++m_size;
      return true;
    }
    
    // check for a matching and free side of a placed tile
    for ( Tile<T>& tile : gameField )
    {
      if ( tile.isAFree )
      {
        if ( sideMatch(tile.a, newTile.a) ) 
        {
          tile.isAFree = false;
          newTile.isAFree = false;
          addTile(newTile);
          return true;
        }
        else if ( sideMatch(tile.a, newTile.b) )
        {
          tile.isAFree = false;
          newTile.isBFree = false;
          addTile(newTile);
          return true;
        }
        else if ( sideMatch(tile.a, newTile.c) )
        {
          tile.isAFree = false;
          newTile.isCFree = false;
          addTile(newTile);
          return true;
        }
      }

      if ( tile.isBFree )
      {
        if ( sideMatch(tile.b, newTile.a) ) 
        {
          tile.isBFree = false;
          newTile.isAFree = false;
          addTile(newTile);
          return true;
        }
        else if ( sideMatch(tile.b, newTile.b) )
        {
          tile.isBFree = false;
          newTile.isBFree = false;
          addTile(newTile);
          return true;
        }
        else if ( sideMatch(tile.b, newTile.c) )
        {
          tile.isBFree = false;
          newTile.isCFree = false;
          addTile(newTile);
          return true;
        }
      }
     
      if ( tile.isCFree )
      {
        if ( sideMatch(tile.c, newTile.a) ) 
        {
          tile.isCFree = false;
          newTile.isAFree = false;
          addTile(newTile);
          return true;
        }
        else if ( sideMatch(tile.c, newTile.b) )
        {
          tile.isCFree = false;
          newTile.isBFree = false;
          addTile(newTile);
          return true;
        }
        else if ( sideMatch(tile.c, newTile.c) )
        {
          tile.isCFree = false;
          newTile.isCFree = false;
          addTile(newTile);
          return true;
        }
      }
    }

    return false;
  }

  int size() const
  {
    return m_size;
  }

  bool contains(T x, T y, T z)
  {
    Tile<T> other{x, y, z};
    std::multiset<T> target{other.a.first, other.a.second, other.b.second};

    for (const Tile<T>& tile : gameField)
    {
      std::multiset<T> values {tile.a.first, tile.a.second, tile.b.second};
      if (values == target) { return true; }
    }

    return false;
  }

  bool equals(const triomino& other) const
  {
    if (m_size != other.m_size) { return false; }

    std::vector<std::array<T, 3>> A, B;
    A.reserve(m_size);
    B.reserve(other.m_size);

    for (const Tile<T>& tile : gameField)
    {
      A.push_back(tile.values());
    }

    for (const Tile<T>& tile : other.gameField)
    {
      B.push_back(tile.values());
    }

    return A == B;
  }

  bool operator==(const triomino<T>& other) const
  {
    return equals(other);
  }
};

#endif
