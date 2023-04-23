#include "maze.hh"
#include "maze_algorithms.hh"
#include "thread_solvers.hh"
#include <iostream>
#include <functional>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

const std::unordered_set<std::string> argument_flags
  = { "-r", "-c", "-b", "-s", "-h", "-g", "-d", "-m", "-sa", "-ba" };

const std::unordered_map<std::string, std::function<void(Maze&)>> builder_table = {
  { "rdfs", Maze::Builder_algorithm::randomized_depth_first },
  { "wilson", Maze::Builder_algorithm::randomized_loop_erased },
  { "wilson-walls", Maze::Builder_algorithm::randomized_loop_erased_walls },
  { "fractal", Maze::Builder_algorithm::randomized_fractal },
  { "kruskal", Maze::Builder_algorithm::randomized_kruskal },
  { "prim", Maze::Builder_algorithm::randomized_prim },
  { "grid", Maze::Builder_algorithm::randomized_grid },
  { "arena", Maze::Builder_algorithm::arena },
};

const std::unordered_map<std::string, Maze::Maze_modification> modification_table = {
  { "none", Maze::Maze_modification::none },
  { "cross", Maze::Maze_modification::add_cross },
  { "x", Maze::Maze_modification::add_x },
};

const std::unordered_map<std::string, Thread_solvers::Solver_algorithm> solver_table = {
  { "dfs", Thread_solvers::Solver_algorithm::depth_first_search },
  { "rdfs", Thread_solvers::Solver_algorithm::randomized_depth_first_search },
  { "bfs", Thread_solvers::Solver_algorithm::breadth_first_search },
};

const std::unordered_map<std::string, Maze::Maze_style> style_table = {
  { "sharp", Maze::Maze_style::sharp },
  { "round", Maze::Maze_style::round },
  { "doubles", Maze::Maze_style::doubles },
  { "bold", Maze::Maze_style::bold },
  { "contrast", Maze::Maze_style::contrast },
  { "spikes", Maze::Maze_style::spikes },
};

const std::unordered_map<std::string, Thread_solvers::Maze_game> game_table = {
  { "hunt", Thread_solvers::Maze_game::hunt },
  { "gather", Thread_solvers::Maze_game::gather },
  { "corners", Thread_solvers::Maze_game::corners },
};

const std::unordered_map<std::string, Thread_solvers::Solver_speed> solver_animation_table = {
  { "0", Thread_solvers::Solver_speed::instant },
  { "1", Thread_solvers::Solver_speed::speed_1 },
  { "2", Thread_solvers::Solver_speed::speed_2 },
  { "3", Thread_solvers::Solver_speed::speed_3 },
  { "4", Thread_solvers::Solver_speed::speed_4 },
  { "5", Thread_solvers::Solver_speed::speed_5 },
  { "6", Thread_solvers::Solver_speed::speed_6 },
  { "7", Thread_solvers::Solver_speed::speed_7 },
};

const std::unordered_map<std::string, Maze::Builder_speed> builder_animation_table = {
  { "0", Maze::Builder_speed::instant },
  { "1", Maze::Builder_speed::speed_1 },
  { "2", Maze::Builder_speed::speed_2 },
  { "3", Maze::Builder_speed::speed_3 },
  { "4", Maze::Builder_speed::speed_4 },
  { "5", Maze::Builder_speed::speed_5 },
  { "6", Maze::Builder_speed::speed_6 },
  { "7", Maze::Builder_speed::speed_7 },
};

void set_relevant_arg( Maze::Maze_args& maze_args,
                       Thread_solvers::Solver_args& solver_args,
                       std::string_view flag,
                       std::string_view arg );
void print_usage();

int main( int argc, char** argv )
{
  Maze::Maze_args maze_args = {};
  Thread_solvers::Solver_args solver_args = {};
  if ( argc > 1 ) {
    const std::vector<std::string_view> args( argv + 1, argv + argc );
    bool process_current = false;
    std::string_view prev_flag = {};
    for ( const std::string_view& arg : args ) {
      if ( process_current ) {
        set_relevant_arg( maze_args, solver_args, prev_flag, arg );
        process_current = false;
      } else {
        const auto& found_arg = argument_flags.find( arg.data() );
        if ( found_arg == argument_flags.end() ) {
          std::cerr << "Invalid argument flag: " << arg << std::endl;
          print_usage();
          std::abort();
        }
        if ( *found_arg == "-h" ) {
          print_usage();
          return 0;
        } else {
          process_current = true;
          prev_flag = arg;
        }
      }
    }
  }
  Maze maze( maze_args );
  Thread_solvers( maze, solver_args ).solve_maze();
  return 0;
}

void set_relevant_arg( Maze::Maze_args& maze_args,
                       Thread_solvers::Solver_args& solver_args,
                       std::string_view flag,
                       std::string_view arg )
{
  if ( flag == "-r" ) {
    maze_args.odd_rows = std::stoi( arg.data() );
    if ( maze_args.odd_rows % 2 == 0 ) {
      maze_args.odd_rows++;
    }
    if ( maze_args.odd_rows < 7 ) {
      std::cerr << "Smallest maze possible is 7x7" << std::endl;
      std::abort();
    }
  } else if ( flag == "-c" ) {
    maze_args.odd_cols = std::stoi( arg.data() );
    if ( maze_args.odd_cols % 2 == 0 ) {
      maze_args.odd_cols++;
    }
    if ( maze_args.odd_cols < 7 ) {
      std::cerr << "Smallest maze possible is 3x7" << std::endl;
      std::abort();
    }
  } else if ( flag == "-b" ) {
    const auto found = builder_table.find( arg.data() );
    if ( found == builder_table.end() ) {
      std::cerr << "Invalid builder argument: " << arg << std::endl;
      print_usage();
      std::abort();
    }
    maze_args.builder = found->second;
  } else if ( flag == "-m" ) {
    const auto found = modification_table.find( arg.data() );
    if ( found == modification_table.end() ) {
      std::cerr << "Invalid modification argument: " << arg << std::endl;
      print_usage();
      std::abort();
    }
    maze_args.modification = found->second;
  } else if ( flag == "-s" ) {
    const auto found = solver_table.find( arg.data() );
    if ( found == solver_table.end() ) {
      std::cerr << "Invalid solver argument: " << arg << std::endl;
      print_usage();
      std::abort();
    }
    solver_args.solver = found->second;
  } else if ( flag == "-g" ) {
    const auto found = game_table.find( arg.data() );
    if ( found == game_table.end() ) {
      std::cerr << "Invalid game argument: " << arg << std::endl;
      print_usage();
      std::abort();
    }
    solver_args.game = found->second;
  } else if ( flag == "-d" ) {
    const auto found = style_table.find( arg.data() );
    if ( found == style_table.end() ) {
      std::cerr << "Invalid drawing argument: " << arg << std::endl;
      print_usage();
      std::abort();
    }
    maze_args.style = found->second;
  } else if ( flag == "-sa" ) {
    const auto found = solver_animation_table.find( arg.data() );
    if ( found == solver_animation_table.end() ) {
      std::cerr << "Invalid solver animation argument: " << arg << std::endl;
      print_usage();
      std::abort();
    }
    solver_args.speed = found->second;
  } else if ( flag == "-ba" ) {
    const auto found = builder_animation_table.find( arg.data() );
    if ( found == builder_animation_table.end() ) {
      std::cerr << "Invalid builder animation argument: " << arg << std::endl;
      print_usage();
      std::abort();
    }
    maze_args.builder_speed = found->second;
  } else {
    std::cerr << "Invalid flag past the first defense? " << flag << std::endl;
    print_usage();
    std::abort();
  }
}

void print_usage()
{
  std::cout << "┌──────────────────────────────────────────────────────┐\n"
            << "│                                                      │\n"
            << "├────────────Thread Maze Usage Instructions────────────┤\n"
            << "│                                                      │\n"
            << "│  Use flags, followed by arguments, in any order:     │\n"
            << "│                                                      │\n"
            << "│  -r Rows flag. Set rows for the maze.                │\n"
            << "│      Any number > 7. Zoom out for larger mazes!      │\n"
            << "│  -c Columns flag. Set columns for the maze.          │\n"
            << "│      Any number > 7. Zoom out for larger mazes!      │\n"
            << "│  -b Builder flag. Set maze building algorithm.       │\n"
            << "│      rdfs - Randomized Depth First Search.           │\n"
            << "│      kruskal - Randomized Kruskal's algorithm.       │\n"
            << "│      prim - Randomized Prim's algorithm.             │\n"
            << "│      wilson - Loop-Erased Random Path Carver.        │\n"
            << "│      wilson-walls - Loop-Erased Random Wall Adder.   │\n"
            << "│      fractal - Randomized recursive subdivision.     │\n"
            << "│      grid - A random grid pattern.                   │\n"
            << "│      arena - Open floor with no walls.               │\n"
            << "│  -m Modification flag. Add shortcuts to the maze.    │\n"
            << "│      cross - Add crossroads through the center.      │\n"
            << "│      x - Add an x of crossing paths through center.  │\n"
            << "│  -s Solver flag. Set maze solving algorithm.         │\n"
            << "│      dfs - Depth First Search                        │\n"
            << "│      rdfs - Randomized Depth First Search            │\n"
            << "│      bfs - Breadth First Search                      │\n"
            << "│  -g Game flag. Set the game for the threads to play. │\n"
            << "│      hunt - 4 threads race to find one finish.       │\n"
            << "│      gather - 4 threads gather 4 finish squares.     │\n"
            << "│      corners - 4 threads race to the center.         │\n"
            << "│  -d Draw flag. Set the line style for the maze.      │\n"
            << "│      sharp - The default straight lines.             │\n"
            << "│      round - Rounded corners.                        │\n"
            << "│      doubles - Sharp double lines.                   │\n"
            << "│      bold - Thicker straight lines.                  │\n"
            << "│      contrast - Full block width and height walls.   │\n"
            << "│      spikes - Connected lines with spikes.           │\n"
            << "│  -sa Solver Animation flag. Watch the maze solution. │\n"
            << "│      Any number 1-7. Speed increases with number.    │\n"
            << "│  -ba Builder Animation flag. Watch the maze build.   │\n"
            << "│      Any number 1-7. Speed increases with number.    │\n"
            << "│  -h Help flag. Make this prompt appear.              │\n"
            << "│      No arguments.                                   │\n"
            << "│                                                      │\n"
            << "│  If any flags are omitted, defaults are used.        │\n"
            << "│                                                      │\n"
            << "│  Examples:                                           │\n"
            << "│  ./run_maze                                          │\n"
            << "│  ./run_maze -r 51 -c 111 -b random-dfs -s bfs -hunt  │\n"
            << "│  ./run_maze -c 111 -s bfs -g gather                  │\n"
            << "│  ./run_maze -s bfs -g corners -d round -b fractal    │\n"
            << "│  ./run_maze -s dfs -ba 4 -sa 5 -b kruskal -m x       │\n"
            << "│                                                      │\n"
            << "└──────────────────────────────────────────────────────┘\n";
  std::cout << std::endl;
}
