cmake_minimum_required(VERSION 3.5)

project(tic-tac-toe LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

find_package(SDL2 REQUIRED)

add_executable(tic-tac-toe
    src/main.c
    src/App.c
    src/App.h
    src/base/Window.c
    src/base/Window.h
    src/base/Graphics.c
    src/base/Graphics.h
    src/base/Texture.c
    src/base/Texture.h
    src/base/Button.c
    src/base/Button.h
    src/scene_game/SceneGameRect.h
    src/scene_game/SceneGame.c
    src/scene_game/SceneGame.h
    src/scene_game/game_board/GameBoard.c
    src/scene_game/game_board/GameBoard.h
    src/scene_game/game_board/board.c
    src/scene_game/game_board/board.h
    src/scene_game/Sidebar.c
    src/scene_game/Sidebar.h
    src/scene_game/Footer.c
    src/scene_game/Footer.h
    src/scene_game/Header.c
    src/scene_game/Header.h)

target_link_libraries(tic-tac-toe PRIVATE SDL2 SDL2_image SDL2_ttf)
