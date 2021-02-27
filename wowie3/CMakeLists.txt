project(wowie3)

add_executable(wowie3)
target_link_libraries(wowie3 PRIVATE RheelEngine)
target_include_directories(wowie3 PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_sources(wowie3 PRIVATE
        Maze.cpp Maze.h
        MazeGenerator.cpp MazeGenerator.h
        PlayerController.cpp PlayerController.h
        Wowie3.cpp Wowie3.h)
