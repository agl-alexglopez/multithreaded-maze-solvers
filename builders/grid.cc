#include "maze_algorithms.hh"

#include <algorithm>
#include <chrono>
#include <random>
#include <stack>
#include <thread>

namespace {

constexpr int run_limit = 4;

struct Run_start {
  Maze::Point cur;
  Maze::Point direction;
};

void complete_run( Maze& maze, std::stack<Maze::Point>& dfs, Run_start run )
{
  // This allows us to run over previous paths which is what makes this algorithm unique.
  Maze::Point next = { run.cur.row + run.direction.row, run.cur.col + run.direction.col };
  // Create the "grid" by running in one direction until wall or limit.
  int cur_run = 0;
  while ( maze.is_square_within_perimeter_walls( next ) && cur_run < run_limit ) {
    maze.join_squares( run.cur, next );
    run.cur = next;
    dfs.push( next );
    next.row += run.direction.row;
    next.col += run.direction.col;
    cur_run++;
  }
}

void animate_run( Maze& maze, std::stack<Maze::Point>& dfs, Run_start run )
{
  Maze::Point next = { run.cur.row + run.direction.row, run.cur.col + run.direction.col };
  int cur_run = 0;
  while ( maze.is_square_within_perimeter_walls( next ) && cur_run < run_limit ) {
    maze.join_squares_animated( run.cur, next );
    run.cur = next;
    dfs.push( next );
    next.row += run.direction.row;
    next.col += run.direction.col;
    cur_run++;
  }
}

} // namespace

void generate_grid_maze( Maze& maze )
{
  maze.fill_maze_with_walls();
  std::mt19937 generator( std::random_device {} () );
  std::uniform_int_distribution row_random( 1, maze.row_size() - 2 );
  std::uniform_int_distribution col_random( 1, maze.col_size() - 2 );
  Maze::Point start = { 2 * ( row_random( generator ) / 2 ) + 1, 2 * ( col_random( generator ) / 2 ) + 1 };
  std::stack<Maze::Point> dfs( { start } );
  std::vector<int> random_direction_indices( Maze::generate_directions_.size() );
  std::iota( begin( random_direction_indices ), end( random_direction_indices ), 0 );
  while ( !dfs.empty() ) {
    Maze::Point cur = dfs.top();
    std::shuffle( begin( random_direction_indices ), end( random_direction_indices ), generator );
    bool branches_remain = false;
    for ( const int& i : random_direction_indices ) {
      const Maze::Point& direction = Maze::generate_directions_.at( i );
      Maze::Point next = { cur.row + direction.row, cur.col + direction.col };
      if ( maze.can_build_new_square( next ) ) {
        complete_run( maze, dfs, { cur, direction } );
        branches_remain = true;
        break;
      }
    }
    if ( !branches_remain ) {
      dfs.pop();
    }
  }

}

void animate_grid_maze( Maze& maze )
{
  maze.fill_maze_with_walls_animated();
  std::mt19937 generator( std::random_device {} () );
  std::uniform_int_distribution row_random( 1, maze.row_size() - 2 );
  std::uniform_int_distribution col_random( 1, maze.col_size() - 2 );
  Maze::Point start = { 2 * ( row_random( generator ) / 2 ) + 1, 2 * ( col_random( generator ) / 2 ) + 1 };
  std::stack<Maze::Point> dfs( { start } );
  std::vector<int> random_direction_indices( Maze::generate_directions_.size() );
  std::iota( begin( random_direction_indices ), end( random_direction_indices ), 0 );
  while ( !dfs.empty() ) {
    Maze::Point cur = dfs.top();
    shuffle( begin( random_direction_indices ), end( random_direction_indices ), generator );
    bool branches_remain = false;
    for ( const int& i : random_direction_indices ) {
      const Maze::Point& direction = Maze::generate_directions_.at( i );
      Maze::Point next = { cur.row + direction.row, cur.col + direction.col };
      if ( maze.can_build_new_square( next ) ) {
        animate_run( maze, dfs, { cur, direction } );
        branches_remain = true;
        break;
      }
    }
    if ( !branches_remain ) {
      maze.flush_cursor_maze_coordinate( cur.row, cur.col );
      std::this_thread::sleep_for( std::chrono::microseconds( maze.build_speed() ) );
      dfs.pop();
    }
  }
}