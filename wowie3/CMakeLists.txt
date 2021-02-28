project(wowie3)

add_executable(Memomaze)
target_link_libraries(Memomaze PRIVATE RheelEngine)
target_include_directories(Memomaze PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_sources(Memomaze PRIVATE
        CameraShaker.cpp CameraShaker.h
        GameOverOverlay.cpp GameOverOverlay.h
        MainMenu.cpp MainMenu.h
        Maze.cpp Maze.h
        MazeGenerator.cpp MazeGenerator.h
        PlayerController.cpp PlayerController.h
        TutorialOverlay.cpp TutorialOverlay.h
        Wowie3.cpp Wowie3.h)
