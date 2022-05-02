# Orion dependencies

This directory holds the necessary dependencies required to use Orion.

### Core dependencies

Here is list of dependencies of the core Orion library:
 - [Dav1dde](https://github.com/Dav1dde)[/**glad**](https://github.com/dav1dde/glad): for loading
 [OpenGL](https://www.opengl.org/) functions.
 - [glfw](https://github.com/glfw)[/**glfw**](https://github.com/glfw/glfw): only used when including
 `<orionwin.h>` *(but using GLFW is still encouraged, regardless of if you use Orion function
 abstractions or not)*.

### Test/example dependencies

If you build test or example executables, the following dependencies are also built with Orion:
 - [nothings](https://github.com/nothings)[/**stb**](https://github.com/nothings/stb/blob/master/stb_image.h)
 **(stb_image)**: for loading images.
 - [jabenuk](https://github.com/jabenuk)[/**zetaml**](https://github.com/jabenuk/zetaml): for
 mathematical operations.

(They are located in the [execdeps](execdeps/) directory)
