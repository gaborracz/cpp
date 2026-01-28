#include <iostream>
#include <algorithm>
#include <numeric>
#include "seqmap.h"
#include <functional>
#include <string>
#include "seqmap.h"

struct string_size_less
{

  bool operator()( const std::string& lhs,
                   const std::string& rhs ) const
  {
    return lhs.size() < rhs.size();
  }

};

const int max = 1000;

int main()
{
  int your_mark = 1;

  std::map<double, int> a;
  for( int i = 0; i < max; ++i )
  {
    a[ 2.5 * i ] = 2 * i;
  }
  const seqmap<double, int> csa( a );

  std::map<std::string, double> b;
  b[ "Prolog" ] = 1.4;
  b[ "C" ] = 4.7;
  b[ "Fortran" ] = 3.2;
  const seqmap<std::string, double> csb( b );

  if( 3 == csb.size() && 1.6 > csb.at( 2 ) && max == csa.size() &&
      4.3 < csb.at( 0 ) )
  {
    your_mark = csa.at( 1 );
  }

  
  /* 3-as
  seqmap<double, int> sa( a );
  sa.pop_back();
  sa[ 0 ]++;
  sa[ 1 ]++;

  seqmap<std::string, double> sb( b );
  sb.pop_back();

  if( max - 1 == sa.size() && 2 == sb.size() && 0 == b.count( "Prolog" ) &&
      3.5 > csb[ 1 ] )
  {
    your_mark = sa[ 1 ];
  }
  
  /* 4-es
  std::map<std::string, int> x;
  x[ "Hello" ] = 4;
  std::map<std::string, int, string_size_less> y;
  y[ "World" ] = 4;
  y[ "Hello World" ] = 2;
  const seqmap<std::string, int> csx( x );
  seqmap<std::string, int, string_size_less> csy( y );
  csy.pop_back();

  std::map<double, int> c;
  c[ 2.1 ] = 3;
  const seqmap<double, int> csc( c );

  if( csx == csy && !( csc == csa ) )
  {
    your_mark = csy[ 0 ];
  }
  */
  /* 5-os
  std::map<std::string, int> z;
  z[ "Hello" ] = -2;
  z[ "C++" ] = 8;
  z[ "World" ] = -1;
  seqmap<std::string, int> sz( z );

  seqmap<std::string, int>::iterator i = std::find( sz.begin(), sz.end(), 8 );

  if( i != sz.end() && 1 == std::count( csy.begin(), csy.end(), 4 ) )
  {
    your_mark = std::accumulate( sz.begin(), sz.end(), 0 );
  }
  */
  std::cout << "Your mark is " << your_mark << std::endl;
}
