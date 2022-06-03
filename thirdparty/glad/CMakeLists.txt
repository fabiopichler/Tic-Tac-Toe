
project(glad LANGUAGES C)

include_directories(include)

# OpenGL 3.3 / OpenGL 2.1

add_library(glad_gl STATIC src/gl.c include/glad/gl.h)

# OpenGL ES 3.0 / OpenGL ES 2.0

add_library(glad_gles STATIC src/gles2.c include/glad/gles2.h)
