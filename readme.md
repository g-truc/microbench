================================================================================
OpenGL Samples Pack 
--------------------------------------------------------------------------------
ogl-samples.g-truc.net
ogl-samples@g-truc.net

================================================================================
The MIT License
--------------------------------------------------------------------------------
Copyright (c) 2005 - 2015 G-Truc Creation (www.g-truc.net)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

================================================================================
Micro beanchmarks instructions
--------------------------------------------------------------------------------
It is required to generate the solution using enabling AUTOMATED_TESTS option

================================================================================
Visual C++ instructions
--------------------------------------------------------------------------------
A Visual C++ 32-bit or 64-bit solution solution can be generated using CMake.
http://www.cmake.org/cmake/resources/software.html

The OpenGL Samples Pack requires at least Visual C++ 2013.

================================================================================
Linux instructions
--------------------------------------------------------------------------------
- Install the GLFW library included in the external directory.
-- sudo make x11-dist-install
- Run CMake to create a makefile for GCC
- Launch the sample from the build output directory

The OpenGL Samples Pack requires at least GCC 4.7.

================================================================================
MacOS instructions
--------------------------------------------------------------------------------
- Install the GLFW library included in the external directory.
-- sudo make cocoa-dist-install
- Run CMake to create a XCode project
- Launch the sample from XCode

The OpenGL Samples Pack requires at least Clang 3.0.

================================================================================
OpenGL Samples Pack 4.5.4.0: 2017-0X-XX
--------------------------------------------------------------------------------
- Added gl-320-fbo-multisample-mask sample
- Added gl-430-glsl-std430 sample
- Added gl-430-glsl-std140 sample
- Added gl-320-glsl-uniform-align sample
- Added gl-320-fbo-readpixels-staging sample
- Added gl-320-fbo-readpixels-async sample
- Added gl-320-fbo-readpixels-client sample
- Added gl-400-fbo-multisample-interpolate sample
- Added gl-400-fbo-multisample-mask sample

================================================================================
OpenGL Samples Pack 4.5.3.0: 2016-07-10
--------------------------------------------------------------------------------
- Improved test automation: A single template image per test with heuristics
- Improved gl-320-fbo-srgb-blend test
- Improved and fixed many tests

================================================================================
OpenGL Samples Pack 4.5.2.0: 2015-09-22
--------------------------------------------------------------------------------
- Added gl-320-fbo-blend-points sample
- Added gl-400-buffer-uniform-array sample
- Added gl-410-buffer-uniform-array sample
- Added gl-430-texture-fetch-dependent sample
- Many updated samples
- Updated template images for current drivers
- Updated GLM to 0.9.7.1 and GLI to 0.7.1.0
- Fixed gl-320-draw-instanced #55
- Fixed gl-430-program-binary #48

================================================================================
OpenGL Samples Pack 4.5.1.0: 2015-03-28
--------------------------------------------------------------------------------
- Updated gl-320-fbo-srgb sample to be more meaningful
- Added gl-320-fbo-srgb-decode-ext sample
- Added gl-430-fbo-srgb-decode sample
- Added gl-450-conservative-raster-nv sample
- Added gl-320-fbo-blend sample
- Added gl-320-fbo-srgb sample
- Added gl-320-fbo-srgb-blend sample
- Added gl-320-fbo-srgb-decode-ext sample
- Added gl-500-fbo-layered-nv sample
- Added gl-320-texture-lod sample
- Added gl-320-fbo-depth-stencil sample
- Added gl-440-fbo-depth-stencil sample
- Added gl-500-primitive-shading-nv sample
- Added gl-500-shader-blend-intel sample
- Added gl-500-shader-blend-nv sample
- Reorganized vendor and ARB samples into GL5 hardware samples

================================================================================
OpenGL Samples Pack 4.5.0.0: 2014-09-15
--------------------------------------------------------------------------------
- Added 450-buffer-sparse-arb
- Added 450-caps
- Added 450-clip-control
- Added 450-culling
- Added 450-direct-state-access
- Added 450-fbo-multisample-explicit
- Added 450-query-conditional
- Added 450-query-statistics-arb
- Added 450-texture-barrier
- Added 450-texture-derivative
- Added 450-transform-feedback-arb

================================================================================
OpenGL Samples Pack 4.4.3.0: 2014-08-03
--------------------------------------------------------------------------------
- Added draw_array and draw_call microbenchmark tests
- Fixed buffer storage flags
- Fixed samples using newer API than expected
- Added 320-fbo-integer sample
- Added 320-fbo-integer-blit sample
- Added 330-query-counter sample
- Fixed 320-fbo-multisample-integer sample, GL_NEAREST is required
- Updated 4.3 samples to run on Intel 4.2 drivers
- Fixed GL_MAX_UNIFORM_LOCATIONS caps query

================================================================================
OpenGL Samples Pack 4.4.2.0: 2014-03-29
--------------------------------------------------------------------------------
- Added 'caps' samples for each version of OpenGL
- Added in source build option
- Added 440-texture-sparse-arb sample
- Added 440-texture-cube-arb sample
- Added 440-texture-bindless-arb sample
- Added 440-shader-invocation-nv sample
- Added 440-query-occlusion sample
- Added 440-multi-draw-indirect-id-arb sample
- Added 440-multi-draw-indirect-count-arb sample
- Added 440-glsl-vote-arb sample
- Added 440-fbo-without-attachment sample
- Added 440-buffer-storage
- Added 440-atomic-counter
- Added 430-query-occlusion sample
- Added 430-query-conditional sample
- Fixed 430-multi-draw-indirect sample, alignment access in shader issue
- Added 430-fbo-without-attachment sample
- Added 430-fbo-invalidate sample
- Added 400-texture-cube sample
- Added 330-query-occlusion sample
- Added 330-query-conditional sample
- Added 320-primitive-line-msaa sample
- Added 320-primitive-point-clip sample
- Added 320-primitive-point-quad sample
- Updated multiple samples to use shader storage buffer instead of VAOs
- Updated multiple samples to use texture 2d array instead if texture 2d
- Fixed FreeImage link issue
- Fixed linking error with FreeImage
- Updated readme instructions

================================================================================
OpenGL Samples Pack 4.4.1.3: 2014-02-23
--------------------------------------------------------------------------------
- Improve compilation time

================================================================================
OpenGL Samples Pack 4.4.1.2: 2014-02-18
--------------------------------------------------------------------------------
- Added missing image templates

================================================================================
OpenGL Samples Pack 4.4.1.1: 2014-02-17
--------------------------------------------------------------------------------
- Fixed MacOSX and linux build
- Remove shaders and textures copy, loaded from source

================================================================================
OpenGL Samples Pack 4.4.1.0: 2014-02-11
--------------------------------------------------------------------------------
- Added automated tests with reference images
- Fixed many samples

================================================================================
OpenGL Samples Pack 4.4.0.3: 2013-10-17
--------------------------------------------------------------------------------
- Fixed MacOSX port

================================================================================
OpenGL Samples Pack 4.4.0.3: 2013-10-17
--------------------------------------------------------------------------------
- Fixed MacOSX port

================================================================================
OpenGL Samples Pack 4.4.0.2: 2013-10-16
--------------------------------------------------------------------------------
- Fixed Linux port
- Upgraded to GLFW 3.0

================================================================================
OpenGL Samples Pack 4.4.0.1: 2013-09-14
--------------------------------------------------------------------------------
- Various fixes.

================================================================================
OpenGL Samples Pack 4.4.0.0: 2013-08-11
--------------------------------------------------------------------------------
- Added 5 OpenGL 4.4 samples 

================================================================================
OpenGL Samples Pack 4.3.2.2: 2013-05-10
--------------------------------------------------------------------------------
- Fixed various sample bugs

================================================================================
OpenGL Samples Pack 4.3.2.1: 2013-02-24
--------------------------------------------------------------------------------
- Fixed GLFW 2.7.7 source
- Back port many more OpenGL 3.3 samples to OpenGL 3.2
- Fixed OpenGL 3.0 samples which crashed at context creation

================================================================================
OpenGL Samples Pack 4.3.2.0: 2013-02-16
--------------------------------------------------------------------------------
- Added MacOS X support
- Fixed Linux support
- Migrate many samples to OpenGL 3.2
- Removed GLEW
- Replace FreeGLUT by GLFW
- Added 320-glsl-precision sample
- Added 320-glsl-builtin-blocks sample
- Added 320-fbo-shadow sample
- Added 400-fbo-shadow sample

================================================================================
OpenGL Samples Pack 4.3.1.0: 2012-12-22
--------------------------------------------------------------------------------
- Added Intel OpenGL drivers implementation support
- Added 420-draw-image-space-rendering sample

================================================================================
OpenGL Samples Pack 4.3.0.3: 2012-10-31
--------------------------------------------------------------------------------
- Using GLF compiler for all OpenGL 4.2 and 4.3 samples
- Added 430-program-compute-image sample
- Added 430-debug sample
- Added 420-texture-sparse-amd sample
- Added 420-texture-bindless-nv sample
- Updated 430-multi-draw-indirect using a DrawID

================================================================================
OpenGL Samples Pack 4.3.0.2: 2012-09-29
--------------------------------------------------------------------------------
- Added GLSL compiler: -D -I #include 
- Complexified some OpenGL 4.3 samples
- Fixed sample exits
- Fixed multisample position sample
- Clean up legacy files

================================================================================
OpenGL Samples Pack 4.3.0.1: 2012-08-30
--------------------------------------------------------------------------------
- Added 430-texture-view sample
- Added 430-image-sampling sample
- Added 430-program-subroutine sample
- Added 420-primitive-line-aa sample
- Fixed 430-multi-draw-indirect, precision issue
- Fixed 430-interface-matching

================================================================================
OpenGL Samples Pack 4.3.0.0: 2012-08-12
--------------------------------------------------------------------------------
- Added 430-texture-copy sample
- Added 430-program-compute sample
- Added 430-multi-draw-indirect sample
- Added 430-interface-matching sample
- Added 430-image-store sample
- Added 430-draw-without-vertex-attrib sample
- Added 430-direct-state-access sample
- Added 430-atomic-counter sample

================================================================================
OpenGL Samples Pack 4.2.3.0: 2012-01-28
--------------------------------------------------------------------------------
- Added 420-blend-op-amd sample
- Added 420-buffer-pinned-amd sample
- Added 420-debug-output sample
- Added 420-fbo-srgb-decode-ext sample
- Added 420-interface-matching sample
- Added 330-texture-format sample
- Added 330-fbo-multisample-explicit-nv sample
- Updated 410-program-binary sample
- Refactored code

================================================================================
OpenGL Samples Pack 4.2.2.1: 2012-01-24
--------------------------------------------------------------------------------
- Fixed bugs

================================================================================
OpenGL Samples Pack 4.2.2.0: 2011-11-26
--------------------------------------------------------------------------------
- Clean up DSA samples and build a single one
- Added image store sample
- Added clamp separate test
- Take advantage of texture max level
- Generalized pipeline and uniform buffer for OpenGL 4.2 samples
- Fixed dynamic uniform variable use for sampler array and uniform block array
- Tessellation without control shader

================================================================================
OpenGL Samples Pack 4.2.1.1: 2011-08-26
--------------------------------------------------------------------------------
- Fixed CTest default value

================================================================================
OpenGL Samples Pack 4.2.1.0: 2011-08-26
--------------------------------------------------------------------------------
- Added CTest
- Rewritten OpenGL 4.2 samples
- Refactored direct state access samples
- Removed some deprecated/duplicated samples

================================================================================
OpenGL Samples Pack 4.2.0.2: 2011-08-11
--------------------------------------------------------------------------------
- Fixed samples on AMD implementation

================================================================================
OpenGL Samples Pack 4.2.0.1: 2011-08-10
--------------------------------------------------------------------------------
- Fixed samples on NVIDIA implementation

================================================================================
OpenGL Samples Pack 4.2.0.0: 2011-08-09
--------------------------------------------------------------------------------
- Added atomic counter sample
- Added buffer uniform sample
- Added draw base instance sample
- Added GLSL interface matching array sample
- Added depth conservative test sample
- Added pixel store for compressed texture 2d sample
- Added immunable texture sample
- Added compressed texture
- Added texture image sample
- Added transform feedback instancing sample

================================================================================
OpenGL Samples Pack 4.1.7.2: 2011-08-04
--------------------------------------------------------------------------------
- Added point sprite sample
- Added interface matching sample
- Fixed interface matching of some samples
- Fixed transform feedback stream sample
- Fixed GLEW extension loading

================================================================================
OpenGL Samples Pack 4.1.7.1: 2011-07-11
--------------------------------------------------------------------------------
- Added Linux support

================================================================================
OpenGL Samples Pack 4.1.7.0: 2011-07-10
--------------------------------------------------------------------------------
- Removed all the workaround used since OpenGL 3.3 beta drivers release
- Some samples refactoring
- Added more transformed feedback tests
- Added pixel store for texture 2d sample
- Fixed extension loading

================================================================================
OpenGL Samples Pack 4.1.6.1: 2011-06-22
--------------------------------------------------------------------------------
- Added this readme.txt
- Added ogl-330-primitive-front-face sample
- Generalized matching by location
- Fixed bugs

================================================================================
