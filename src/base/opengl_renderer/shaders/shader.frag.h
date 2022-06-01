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

static const char frag[] =
                                                                                                                    "\n\
#ifdef GL_ES                                                                                                         \n\
    precision highp float;                                                                                           \n\
#endif                                                                                                               \n\
                                                                                                                     \n\
uniform sampler2D uSampler;                                                                                          \n\
                                                                                                                     \n\
varying vec2 vUV;                                                                                                    \n\
varying vec4 vColor;                                                                                                 \n\
                                                                                                                     \n\
void main()                                                                                                          \n\
{                                                                                                                    \n\
#ifdef hasTextureBGRA                                                                                                \n\
    gl_FragColor = texture2D(uSampler, vUV).bgra;                                                                    \n\
#elif hasTexture                                                                                                     \n\
    gl_FragColor = texture2D(uSampler, vUV);                                                                         \n\
#else                                                                                                                \n\
    gl_FragColor = vColor / 255.0;                                                                                   \n\
#endif                                                                                                               \n\
}                                                                                                                    \n\
                                                                                                                     \n";
