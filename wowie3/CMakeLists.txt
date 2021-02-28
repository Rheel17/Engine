project(wowie3)

add_executable(wowie3)
target_link_libraries(wowie3 PRIVATE RheelEngine)
target_include_directories(wowie3 PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_sources(wowie3 PRIVATE
        MainMenu.cpp MainMenu.h
        Maze.cpp Maze.h
        MazeGenerator.cpp MazeGenerator.h
        PlayerController.cpp PlayerController.h
        TutorialOverlay.cpp TutorialOverlay.h
        Wowie3.cpp Wowie3.h)
