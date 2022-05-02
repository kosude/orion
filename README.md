# Orion Graphics Library

## Summary

Orion is a C remake of my older C++ graphics library, **Juno/GL**. From the Juno/GL documentation:
 > Juno/GL is primarily intended for any graphical projects where you want to run OpenGL without the
 annoyances of raw OpenGL code.

However, there are key differences between Orion's design and the older Juno/GL design philosophy.
For example, GLFW was previously _bundled in_ and you were essentially _forced_ to use it as part of
the core library. This is not the case with Orion - however, there are GLFW _abstractions_ you can
use that add some quality-of-life features (for example automatic memory management with the
`oriTerminate()` function). See the [usage](#orion-windowing-interface) section for information on
using these features.

## Compilation and usage

Orion is built with CMake. To configure it, use the command
`cmake . -B build/ -DCMAKE_BUILD_TYPE=(Debug|Release)` while the project's folder is the current
working directory.

#### CMake build flags
 - `-DCMAKE_BUILD_TYPE=(Debug|Release)` is **required**. Specifies the build configuration to use.
 - `-DBUILD_SHARED_LIBS=(ON|OFF)` is **optional** (defaults to ON). Specifies whether to build Orion
 as a static or shared library.
 - `-DORION_BUILD_LIB=(ON|OFF)` is **optional** (defaults to ON). Choose whether to build the core
 Orion library.
 - `-DORION_BUILD_TESTS=(ON|OFF)` is **optional** (defaults to OFF). Choose whether to build Orion
 test executables.
 - `-DORION_BUILD_EXAMPLES=(ON|OFF)` is **optional** (defaults to OFF). Choose whether to build Orion
 usage examples.
 - `-DORION_BUILD_DOCS=(ON|OFF)` is **optional** (defaults to ON). Choose whether to build Orion HTML
 documentation. requires Doxygen!

### Orion GL
The Orion Graphics Library is the main part of the Orion library, and can be used by including
`<oriongl.h>`.

### Orion windowing interface
The Orion windowing interface is an optional part of Orion, and can be used by including
`<orionwin.h>`. This module includes abstractions of GLFW functions with the added ability to use the
included `oriWindow` struct, which is managed internally by Orion and is automatically freed in calls
to `oriTerminate()`. Keep in mind that Orion has also been tested with basic GLFW (without orionwin)
and it works just as well.

## Contributing

See the [dependencies](deps/) directory read-me for information about Orion's dependencies.

### Using Valgrind to eliminate memory issues

Whenever something is contributed to Orion, regardless of its significance, it should always be tested
with **[Valgrind](https://valgrind.org/)** first. There are a few memory leaks that are caused by
graphics drivers, windowing systems (e.g. X11), and bugs within Valgrind itself. In this case, you can
safely ignore *(suppress, even)* any leaks that don't originate within Orion.
