add_library(maze_algorithms_debug maze_algorithms.hh arena.cc grid.cc kruskal.cc prim.cc recursive_backtracker.cc recursive_subdivision.cc wilson_path_carver.cc wilson_wall_adder.cc eller.cc)
target_link_libraries(maze_algorithms_debug PRIVATE maze disjoint_set maze_utilities print_utilities)

add_library(maze_algorithms_sanitized maze_algorithms.hh arena.cc grid.cc kruskal.cc prim.cc recursive_backtracker.cc recursive_subdivision.cc wilson_path_carver.cc wilson_wall_adder.cc eller.cc)
target_link_libraries(maze_algorithms_sanitized PRIVATE maze disjoint_set maze_utilities print_utilities)
target_compile_options(maze_algorithms_sanitized PUBLIC ${SANITIZING_FLAGS})

add_library(maze_algorithms_optimized maze_algorithms.hh arena.cc grid.cc kruskal.cc prim.cc recursive_backtracker.cc recursive_subdivision.cc wilson_path_carver.cc wilson_wall_adder.cc eller.cc)
target_link_libraries(maze_algorithms_optimized PRIVATE maze disjoint_set maze_utilities print_utilities)
target_compile_options(maze_algorithms_optimized PUBLIC "-O2")
