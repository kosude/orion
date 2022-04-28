# Orion Graphics Library

## Summary

Orion is a C remake of my older C++ graphics library, **Juno/GL**. From the Juno/GL documentation:
 > Juno/GL is primarily intended for any graphical projects where you want to run OpenGL without the annoyances of raw OpenGL code.

However, there are key differences between Orion's design and the older Juno/GL design philosophy. For example, GLFW was previously _bundled in_ and you were essentially _forced_ to use it as part of the core library. This is not the case with Orion - however, there are GLFW _abstractions_ you can use that add some quality-of-life features (for example automatic memory management with the `oriTerminate()` function). See the [usage](#orion-windowing-interface) section for information on using these features.

## Usage

Orion is built with CMake. To configure it, use the command `cmake . -B build/` whilst the project's folder is the current working directory. **You should also specify some build options with the following flags:**
 - `-DCMAKE_BUILD_TYPE=(Debug|Release)` is **required**. Specifies the build configuration to use.
 - `-DBUILD_SHARED_LIBS=(ON|OFF)` is **optional** (defaults to ON). Specifies whether to build Orion as a static or shared library.
 - `-DORION_BUILD_LIB=(ON|OFF)` is **optional** (defaults to ON). Choose whether to build the core Orion library.
 - `-DORION_BUILD_TESTS=(ON|OFF)` is **optional** (defaults to OFF). Choose whether to build Orion test executables.
 - `-DORION_BUILD_EXAMPLES=(ON|OFF)` is **optional** (defaults to OFF). Choose whether to build Orion usage examples.
 - `-DORION_BUILD_DOCS=(ON|OFF)` is **optional** (defaults to ON). Choose whether to build Orion HTML documentation. requires Doxygen!

### Orion GL
The Orion Graphics Library is the main part of the Orion library, and can be used by including `<oriongl.h>`.

### Orion windowing interface
The Orion windowing interface is an optional part of Orion, and can be used by including `<orionwin.h>`. This module includes abstractions of GLFW functions with the added ability to use the included `oriWindow` struct, which is managed internally by Orion and is automatically freed in calls to `oriTerminate()`. Keep in mind that Orion has also been tested with basic GLFW (without orionwin) and it works fine too. Use it if you want; you don't have to.
