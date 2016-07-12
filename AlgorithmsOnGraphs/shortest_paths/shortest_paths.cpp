#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>
#include <vector>

using std::vector;
using std::unordered_map;
using std::priority_queue;
using std::queue;
using std::pair;

bool relax( int u, int v, vector< long long > &prev,
            unordered_map< int, long long > &dist,
            const vector< vector< int > > &cost )
{
  long long new_w = dist[ u ] + cost[ u ][ v ];
  if ( dist[ v ] > new_w )
  {
    dist[ v ] = new_w;
    prev[ v ] = u;
    return true;
  }
  return false;
}

int negative_cycle( vector< vector< int > > &adj, vector< vector< int > > &cost,
                    int s )
{
  std::unordered_map< int, long long > dist;
  std::vector< long long > prev( adj.size(), -1 );
  for ( size_t i = 0; i < adj.size(); ++i )
  {
    dist[ i ] = std::numeric_limits< int >::max();
  }

  dist[ s ] = 0;

  int V = adj.size();
  for ( int i = 0; i < V - 1; ++i )
  {
    for ( size_t j = 0; j < adj.size(); ++j )
    {
      int u = j;
      const auto &es = adj[ u ];
      for ( int v : es )
      {
        relax( u, v, prev, dist, cost );
      }
    }
  }

  for ( size_t j = 0; j < adj.size(); ++j )
  {
    int u = j;
    const auto &es = adj[ u ];
    for ( int v : es )
    {
      if ( relax( u, v, prev, dist, cost ) )
        return 1;
    }
  }
  return 0;
}

int get_distance( vector< vector< int > > &adj, vector< vector< int > > &cost,
                  int s, int t )
{
  // write your code her
  std::unordered_map< int, long long > dist;
  std::vector< long long > prev( adj.size(), -1 );
  for ( size_t i = 0; i < adj.size(); ++i )
  {
    dist[ i ] = std::numeric_limits< int >::max();
  }

  dist[ s ] = 0;

  auto cmp = [&dist]( int left, int right ) {
    return dist[ left ] > dist[ right ];
  };
  std::priority_queue< int, std::vector< int >, decltype( cmp ) > h( cmp );

  for ( size_t i = 0; i < adj.size(); ++i )
  {
    h.push( i );
  }

  while ( !h.empty() )
  {
    int u = h.top();
    h.pop();

    const auto &edges = adj[ u ];
    for ( auto v : edges )
    {
      int w = cost[ u ][ v ];
      if ( dist[ v ] == std::numeric_limits< int >::max() )
      {
        dist[ v ] = dist[ u ] + w;
        prev[ v ] = u;
        h.push( v );
      }
      else
      {
        if ( dist[ v ] > ( dist[ u ] + w ) )
        {
          dist[ v ] = dist[ u ] + w;
          prev[ v ] = u;
          h.push( v );
        }
      }
    }
  }

  int tmp = t;
  int sum = 0;
  while ( tmp != s )
  {
    if ( tmp == -1 || prev[ tmp ] == -1 )
      return -1;
    int w = cost[ prev[ tmp ] ][ tmp ];
    if ( w == -1 )
      return -1;
    sum += 1;
    tmp = prev[ tmp ];
  }
  return sum;
}

void explore( const vector< vector< int > > &adj, vector< bool > &visited,
              int v )
{
  visited[ v ] = true;
  const auto &es = adj[ v ];

  for ( const auto &w : es )
  {

    if ( !visited[ w ] )
    {
      explore( adj, visited, w );
    }
  }
}

int reach( vector< vector< int > > &adj, int x, int y )
{
  std::vector< bool > visited( adj.size(), false );

  explore( adj, visited, x );

  if ( visited[ y ] )
    return 1;

  return 0;
}

void shortest_paths( vector< vector< int > > &adj,
                     vector< vector< int > > &cost, int s,
                     vector< long long > &distance, vector< int > &reachable,
                     vector< int > &shortest )
{
  // write your code here
  // test reachability
  bool negCycle = negative_cycle( adj, cost, s ) == 1;

  for ( size_t i = 0; i < adj.size(); ++i )
  {
    if ( i == s )
    {
      reachable[ i ] = 1;
      distance[ i ] = 0;
      continue;
    }

    int u = i;
    int r = reach( adj, s, u );

    if ( r == 1 )
    {
      reachable[ i ] = r;

      if ( negCycle )
      {
        distance[ i ] = get_distance( adj, cost, s, u );
      }
      else
      {
        shortest[ i ] = 0;
      }
    }
  }
}

int main()
{
  int n, m, s;
  std::cin >> n >> m;
  vector< vector< int > > adj( n, vector< int >() );
  vector< vector< int > > cost( n, vector< int >( n, -1 ) );
  for ( int i = 0; i < m; i++ )
  {
    int x, y, w;
    std::cin >> x >> y >> w;
    adj[ x - 1 ].push_back( y - 1 );
    cost[ x - 1 ][ y - 1 ] = w;
  }
  std::cin >> s;
  s--;
  vector< long long > distance( n, std::numeric_limits< long long >::max() );
  vector< int > reachable( n, 0 );
  vector< int > shortest( n, 1 );
  shortest_paths( adj, cost, s, distance, reachable, shortest );
  for ( int i = 0; i < n; i++ )
  {
    if ( !reachable[ i ] )
    {
      std::cout << "*\n";
    }
    else if ( !shortest[ i ] )
    {
      std::cout << "-\n";
    }
    else
    {
      std::cout << distance[ i ] << "\n";
    }
  }

  return 0;
}
