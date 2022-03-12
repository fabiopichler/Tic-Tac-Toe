cmake_minimum_required(VERSION 3.5)

include(.env.cmake)

project(tic-tac-toe LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT ANDROID)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)
endif()

if(WIN32)
    include_directories(${WIN32_INC_DIR})
elseif(ANDROID)
    include_directories(${ANDROID_INC_DIR})
endif()

if(ANDROID)
    add_library(tic-tac-toe SHARED)
else()
    add_executable(tic-tac-toe)
endif()

include(src/sources.cmake)

if(WIN32)
    set(SRC_FILES ${SRC_FILES} rc/app.rc)
endif()

target_sources(tic-tac-toe PRIVATE ${SRC_FILES})

if(WIN32)
    target_link_directories(tic-tac-toe PRIVATE ${WIN32_LINK_DIR})
    target_link_libraries(tic-tac-toe PRIVATE SDL2main)
elseif(ANDROID)
    set(LIBS ${ANDROID_LINK_DIR}/${ANDROID_ABI})

    add_library(SDL2 SHARED IMPORTED)
    set_target_properties(SDL2 PROPERTIES IMPORTED_LOCATION ${LIBS}/libSDL2.so)

    add_library(SDL2_image SHARED IMPORTED)
    set_target_properties(SDL2_image PROPERTIES IMPORTED_LOCATION ${LIBS}/libSDL2_image.so)

    add_library(SDL2_ttf SHARED IMPORTED)
    set_target_properties(SDL2_ttf PROPERTIES IMPORTED_LOCATION ${LIBS}/libSDL2_ttf.so)

    add_library(hidapi SHARED IMPORTED)
    set_target_properties(hidapi PROPERTIES IMPORTED_LOCATION ${LIBS}/libhidapi.so)

    target_link_libraries(tic-tac-toe PRIVATE hidapi)
    set_target_properties(tic-tac-toe PROPERTIES OUTPUT_NAME "main")
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    # emcmake cmake ..

    set(DEFS "-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s SDL2_IMAGE_FORMATS='[\"png\"]'")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

    add_definitions("${DEFS}")
    set_target_properties(tic-tac-toe PROPERTIES LINK_FLAGS "${DEFS} -s WASM=1 -s MIN_WEBGL_VERSION=1 -s MAX_WEBGL_VERSION=2 --use-preload-plugins --preload-file ${CMAKE_SOURCE_DIR}/assets@")
else()
    target_link_libraries(tic-tac-toe PRIVATE SDL2 SDL2_image SDL2_ttf)
endif()

if(NOT WIN32)
    target_link_libraries(tic-tac-toe PRIVATE m)
endif()
