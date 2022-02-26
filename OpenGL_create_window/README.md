#### GLFW

**GLFW** is a lightweight utility library for use with [OpenGL](https://en.wikipedia.org/wiki/OpenGL "OpenGL"). GLFW stands for Graphics Library Framework. It provides programmers with the ability to create and manage windows and OpenGL contexts, as well as handle [joystick](https://en.wikipedia.org/wiki/Joystick "Joystick"), [keyboard](https://en.wikipedia.org/wiki/Computer_keyboard "Computer keyboard") and [mouse](https://en.wikipedia.org/wiki/Mouse_(computing)) 

The API provides a thin, multi-platform abstraction layer, primarily for applications whose sole graphics output is through the OpenGL API.

A possible reason that libraries like GLFW are needed is that OpenGL by itself does not provide any mechanisms for creating the necessary context, managing windows, user input, timing, etc.

There are several other libraries available for aiding OpenGL development. The most common ones are [FreeGLUT](https://en.wikipedia.org/wiki/FreeGLUT "FreeGLUT") (an Open Source implementation of GLUT) and SDL. However, FreeGLUT is mostly concerned with providing a stable clone of GLUT, while SDL is too large for some people and has never had OpenGL as its main focus. GLFW is predicated on the assumption that there is room for a lightweight, modern library for managing OpenGL contexts, windows, and input.


GLFW is by design not

* **a user interface library**. It allows the programmer to create top-level windows with OpenGL contexts. No menus, no buttons.
* a Windows-only library. Requests for features that cannot be portably implemented will be denied unless they are unobtrusive, like the Windows port looking for a GLFW_ICON resource at window creation.
* **a threading library.** There are already good cross-platform threading libraries and threading has been added to both the C11 and C++11 standard libraries.
* **an image loading library.** There are already good cross-platform image loading libraries.
* **capable of rendering text.** There are already several libraries that render text with OpenGL and consistent cross-platform text rendering cannot depend on the platform’s text rendering facilities anyway.
* **capable of rendering anything at all.** Rendering is up to the programmer and/or other libraries.
* integrated with any user interface toolkit on any platform.
* **Able to playback sound.**
* [GLUT](https://en.wikipedia.org/wiki/OpenGL_Utility_Toolkit "OpenGL Utility Toolkit") or [SDL](https://en.wikipedia.org/wiki/Simple_DirectMedia_Layer "Simple DirectMedia Layer").
