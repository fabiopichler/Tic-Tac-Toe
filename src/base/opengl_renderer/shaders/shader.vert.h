//-------------------------------------------------------------------------------
// Copyright (c) 2020-2022 Fábio Pichler
/*-------------------------------------------------------------------------------

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-------------------------------------------------------------------------------*/

static const char vert[] =
                                                                                                                    "\n\
attribute vec2 aPosition;                                                                                            \n\
attribute vec2 aUV;                                                                                                  \n\
attribute vec4 aColor;                                                                                               \n\
                                                                                                                     \n\
uniform mat3 uTransform;                                                                                             \n\
uniform mat4 uProjection;                                                                                            \n\
uniform vec4 uSourcePosition;                                                                                        \n\
                                                                                                                     \n\
varying vec2 vUV;                                                                                                    \n\
varying vec4 vColor;                                                                                                 \n\
                                                                                                                     \n\
void main()                                                                                                          \n\
{                                                                                                                    \n\
    vColor = aColor;                                                                                                 \n\
    gl_Position = uProjection * vec4(uTransform * vec3(aPosition, 1.0), 1.0);                                        \n\
                                                                                                                     \n\
#if hasTexture                                                                                                       \n\
    vUV = uSourcePosition.xy + vec2(aUV.x * uSourcePosition.z, aUV.y * uSourcePosition.w);                           \n\
#endif                                                                                                               \n\
}                                                                                                                    \n\
                                                                                                                     \n";
