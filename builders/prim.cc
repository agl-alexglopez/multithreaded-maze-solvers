#include "maze_algorithms.hh"
#include "disjoint_set.hh"

#include <climits>
#include <queue>
#include <random>
#include <unordered_map>

namespace {

struct Priority_cell
{
  Maze::Point cell;
  int priority;
  bool operator==( const Priority_cell& rhs ) const
  {
    return this->priority == rhs.priority && this->cell == rhs.cell;
  }
  bool operator!=( const Priority_cell& rhs ) const { return !( *this == rhs ); }
  bool operator<( const Priority_cell& rhs ) const { return this->priority < rhs.priority; }
  bool operator>( const Priority_cell& rhs ) const { return this->priority > rhs.priority; }
  bool operator<=( const Priority_cell& rhs ) const { return !( *this > rhs ); }
  bool operator>=( const Priority_cell& rhs ) const { return !( *this < rhs ); }
};

Maze::Point pick_random_odd_point( Maze& maze )
{
  std::uniform_int_distribution<int> rand_row( 1, ( maze.row_size() - 2 ) / 2 );
  std::uniform_int_distribution<int> rand_col( 1, ( maze.col_size() - 2 ) / 2 );
  std::mt19937 generator( std::random_device {} () );
  int odd_row = 2 * rand_row( generator ) + 1;
  int odd_col = 2 * rand_col( generator ) + 1;
  return { odd_row, odd_col };
}

std::unordered_map<Maze::Point, int> randomize_cell_costs( Maze& maze )
{
  std::unordered_map<Maze::Point, int> cell_cost = {};
  std::uniform_int_distribution<int> random_cost( 0, 100 );
  std::mt19937 generator( std::random_device {} () );
  for ( int row = 1; row < maze.row_size(); row += 2 ) {
    for ( int col = 1; col < maze.col_size(); col += 2 ) {
      cell_cost[ { row, col } ] = random_cost( generator );
    }
  }
  return cell_cost;
}

} // namespace

void generate_prim_maze( Maze& maze )
{
  maze.fill_maze_with_walls();
  std::unordered_map<Maze::Point, int> cell_cost = randomize_cell_costs( maze );
  Maze::Point odd_point = pick_random_odd_point( maze );
  Priority_cell start = { odd_point, cell_cost[odd_point] };
  std::priority_queue<Priority_cell, std::vector<Priority_cell>, std::greater<>> cells;
  cells.push( start );
  while ( !cells.empty() ) {
    const Maze::Point& cur = cells.top().cell;
    maze[cur.row][cur.col] |= Maze::builder_bit_;
    Maze::Point min_neighbor = {};
    int min_weight = INT_MAX;
    for ( const Maze::Point& p : Maze::generate_directions_ ) {
      Maze::Point next = { cur.row + p.row, cur.col + p.col };
      if ( maze.can_build_new_square( next ) ) {
        int weight = cell_cost[next];
        if ( weight < min_weight ) {
          min_weight = weight;
          min_neighbor = next;
        }
      }
    }
    if ( min_neighbor.row ) {
      maze.join_squares( cur, min_neighbor );
      cells.push( { min_neighbor, min_weight } );
    } else {
      cells.pop();
    }
  }
}

void animate_prim_maze( Maze& maze )
{
  maze.fill_maze_with_walls_animated();
  std::unordered_map<Maze::Point, int> cell_cost = randomize_cell_costs( maze );
  Maze::Point odd_point = pick_random_odd_point( maze );
  Priority_cell start = { odd_point, cell_cost[odd_point] };
  std::priority_queue<Priority_cell, std::vector<Priority_cell>, std::greater<>> cells;
  cells.push( start );
  while ( !cells.empty() ) {
    const Maze::Point& cur = cells.top().cell;
    maze[cur.row][cur.col] |= Maze::builder_bit_;
    Maze::Point min_neighbor = {};
    int min_weight = INT_MAX;
    for ( const Maze::Point& p : Maze::generate_directions_ ) {
      Maze::Point next = { cur.row + p.row, cur.col + p.col };
      if ( maze.can_build_new_square( next ) ) {
        int weight = cell_cost[next];
        if ( weight < min_weight ) {
          min_weight = weight;
          min_neighbor = next;
        }
      }
    }
    if ( min_neighbor.row ) {
      maze.join_squares_animated( cur, min_neighbor );
      cells.push( { min_neighbor, min_weight } );
    } else {
      cells.pop();
    }
  }
}