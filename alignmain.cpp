#include <iostream>
#include <numeric>
#include <iterator>
#include <string>
//#include "aligner.h"
#include <vector>
#include "aligner.h"
#include <algorithm>

const int max = 1024;

int main()
{
  int your_mark = 1;
  aligner<int>* ali = new center_aligner<int>();
  std::vector<int> v;
  v.push_back( 5 );
  v.push_back( 2 );
  v.push_back( max );

  std::vector<int> a;
  a.push_back( 1 );
  a.push_back( 3 );

  std::vector<int> s;
  s.push_back( 5 );
  s.push_back( 2 );
  s.push_back( 3 );
  s.push_back( 7 );
  s.push_back( 8 );

  ali->add( v );
  ali->add( s );
  ali->add( a );

  ali->align();
  delete ali;

  aligner<std::string>* als = new center_aligner<std::string>();
  std::vector<std::string> fun;
  fun.push_back( "C++" );
  std::vector<std::string> old;
  old.push_back( "Cobol" );
  old.push_back( "Fortran" );
  old.push_back( "Algol" );

  als->add( fun );
  als->add( old );
  als->align();
  delete als;

  if ( fun[ 0 ] == fun[ 2 ] && fun[ 0 ].empty() && 
       fun.size() == old.size() && "Cobol" == old[ 0 ] &&
       a.size() == v.size() && 0 == v[ 0 ] && a[ 0 ] == a[ 4 ] &&
       4 == std::accumulate( a.begin(), a.end(), 0 ) && 5 == s[ 0 ] &&
       5 == v[ 1 ] && max == v[ 3 ] && 8 == s.back() && "C++" == fun[ 1 ] )
  {
    your_mark =  a.size() - old.size();
  }
  v.clear();
  s.clear();
  a.clear();

  v.push_back( 3 );
  v.push_back( 8 );

  s.push_back( 1 );
  s.push_back( 9 );
  s.push_back( 5 );
  s.push_back( 2 );
  s.push_back( 8 );

  a.push_back( 1 );
  ali = new right_aligner<int>();
  ali->add( v );
  ali->add( s );
  ali->add( a );

  ali->align();

  fun.clear();
  fun.push_back( "C++" );

  als = new right_aligner<std::string>();
  als->add( fun );
  als->add( old );

  als->align();

  if ( your_mark == als->count() && "C++" == fun[ 2 ] &&
       "Algol" == old.back() &&  fun.size() == old.size() &&
       8 == v[ 4 ] && 0 == v[ 1 ] && 3 == v[ 3 ] &&
       a.back() == std::accumulate( a.begin(), a.end(), 0 ) )
  {
    your_mark = ali->count();
  }
  delete ali;
  delete als;
  fun.clear();
  fun.push_back( "C++" );
  als = new right_aligner<std::string>();
  als->set_filler( "C++" );
  als->add( fun );
  als->add( old );
  als->align();
  delete als;

  s.clear();
  v.clear();
  a.clear();

  v.push_back( 3 );
  v.push_back( 7 );

  a.push_back( 8 );
  a.push_back( 2 );
  a.push_back( 6 );
  a.push_back( 1 );

  s.push_back( 2 );

  ali = new left_aligner<int>();
  ali->set_filler( max );
  ali->add( s );
  ali->add( v );
  ali->add( a );
  ali->align();
  delete ali;

  if ( fun[ 0 ] == fun[ 2 ] && "C++" == fun[ 1 ] &&
       your_mark == std::count( s.begin(), s.end(), max ) &&
       7 == v[ 1 ] && 2 == a[ 1 ] && max == v.back() )
  {
    your_mark = v.size();
  }
  fun.clear();
  fun.push_back( "C" );
  fun.push_back( "C++" );

  als = new justified_aligner<std::string>();
  als->add( fun );
  als->add( old );
  als->align();
  delete als;

  v.clear();
  s.clear();
  a.clear();

  v.push_back( 2 );
  v.push_back( 4 );
  v.push_back( 8 );

  s.push_back( 7 );
  s.push_back( 1 );
  s.push_back( 3 );
  s.push_back( 8 );
  s.push_back( 9 );

  a.push_back( 3 );
  a.push_back( 5 );

  ali = new justified_aligner<int>();
  ali->set_filler( max );
  ali->add( v );
  ali->add( s );
  ali->add( a );
  ali->align();

  delete ali;

  if ( fun[ 1 ].empty() && "C++" == fun[ 2 ] &&
       3 == a[ 0 ] && 5 == v.size() && 2 == v[ 0 ] &&
       2 == v[ 0 ] && 8 == v[ 4 ] && 4 == v[ 2 ] &&
       2 == std::count( v.begin(), v.end(), max ) &&
       3 == std::count( a.begin(), a.end(), max ) )
  {
    your_mark = a.back();
  }
  std::cout << "Your mark is " << your_mark;
  std::endl( std::cout );
}
