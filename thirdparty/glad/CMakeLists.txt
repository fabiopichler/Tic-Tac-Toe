
project(glad LANGUAGES C)

include_directories(include)

# OpenGL 2.1

add_library(glad_gl2 STATIC src/gl2.c include/glad/gl2.h)

# OpenGL 3.3

add_library(glad_gl3 STATIC src/gl3.c include/glad/gl3.h)

# OpenGL ES 2.0

add_library(glad_gles2 STATIC src/gles2.c include/glad/gles2.h)

# OpenGL ES 3.0

add_library(glad_gles3 STATIC src/gles3.c include/glad/gles3.h)
