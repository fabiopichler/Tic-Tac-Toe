cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME tic-tac-toe)
project(${PROJECT_NAME} LANGUAGES C)

option(USE_DATA_ZIP "Use data in zip file with PhysicsFS library" OFF)
set(SDL2_INC_DIR "" CACHE STRING "SDL2 include directory")
set(SDL2_LINK_DIR "" CACHE STRING "SDL2 library directory")
set(PHYSFS_INC_DIR "" CACHE STRING "PhysicsFS include directory")
set(PHYSFS_LINK_DIR "" CACHE STRING "PhysicsFS library directory")

if(EXISTS "${CMAKE_SOURCE_DIR}/.env.cmake")
    include("${CMAKE_SOURCE_DIR}/.env.cmake")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT ANDROID)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)
endif()

if(ANDROID)
    add_library(${PROJECT_NAME} SHARED)
else()
    add_executable(${PROJECT_NAME})
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(RENDERER "GLES2" CACHE STRING "OpenGL version")
elseif(ANDROID)
    set(RENDERER "GLES3" CACHE STRING "OpenGL version")
else()
    set(RENDERER "GL3" CACHE STRING "OpenGL version")
endif()

include_directories(${SDL2_INC_DIR} "thirdparty/glad/include")

add_subdirectory(thirdparty)

include(src/sources.cmake)

if(WIN32)
    set(SRC_FILES ${SRC_FILES} rc/app.rc)
endif()

target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})

if(USE_DATA_ZIP)
    include_directories(${PHYSFS_INC_DIR})
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_DATA_ZIP)
endif()

target_link_directories(${PROJECT_NAME} PRIVATE ${SDL2_LINK_DIR})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2main)
elseif(ANDROID)
    set(LIBS ${SDL2_LINK_DIR}/${ANDROID_ABI})

    add_library(SDL2 SHARED IMPORTED)
    set_target_properties(SDL2 PROPERTIES IMPORTED_LOCATION ${LIBS}/libSDL2.so)

    add_library(SDL2_image SHARED IMPORTED)
    set_target_properties(SDL2_image PROPERTIES IMPORTED_LOCATION ${LIBS}/libSDL2_image.so)

    add_library(SDL2_ttf SHARED IMPORTED)
    set_target_properties(SDL2_ttf PROPERTIES IMPORTED_LOCATION ${LIBS}/libSDL2_ttf.so)

    add_library(hidapi SHARED IMPORTED)
    set_target_properties(hidapi PROPERTIES IMPORTED_LOCATION ${LIBS}/libhidapi.so)

    target_link_libraries(${PROJECT_NAME} PRIVATE hidapi)
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "main")
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    # emcmake cmake ..

    set(DEFS "-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s SDL2_IMAGE_FORMATS='[\"png\"]'")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

    add_definitions("${DEFS}")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${DEFS} -s WASM=1 -s MIN_WEBGL_VERSION=1 -s MAX_WEBGL_VERSION=2 --use-preload-plugins --preload-file ${CMAKE_SOURCE_DIR}/assets@")
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 SDL2_image SDL2_ttf)
endif()

if(NOT WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE m)
endif()

if(USE_DATA_ZIP)
    target_link_directories(${PROJECT_NAME} PRIVATE ${PHYSFS_LINK_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE physfs)
endif()

if (${RENDERER} MATCHES "GL2")
    target_compile_definitions(${PROJECT_NAME} PRIVATE RENDERER_GL RENDERER_GL2)
    target_link_libraries(${PROJECT_NAME} PRIVATE glad_gl2)

elseif(${RENDERER} MATCHES "GL3")
    target_compile_definitions(${PROJECT_NAME} PRIVATE RENDERER_GL RENDERER_GL3)
    target_link_libraries(${PROJECT_NAME} PRIVATE glad_gl3)

elseif(${RENDERER} MATCHES "GLES2")
    target_compile_definitions(${PROJECT_NAME} PRIVATE RENDERER_GLES RENDERER_GLES2)
    target_link_libraries(${PROJECT_NAME} PRIVATE glad_gles2)

elseif(${RENDERER} MATCHES "GLES3")
    target_compile_definitions(${PROJECT_NAME} PRIVATE RENDERER_GLES RENDERER_GLES3)
    target_link_libraries(${PROJECT_NAME} PRIVATE glad_gles3)
endif()
