### GLAD
We’re still not quite there yet, since there is one other thing we still need to do. Because OpenGL is
only really a standard/specification it is up to the driver manufacturer to implement the specification
to a driver that the specific graphics card supports. Since there are many different versions of
OpenGL drivers, the location of most of its functions is not known at compile-time and needs to be
queried at run-time. It is then the task of the developer to retrieve the location of the functions he/she
needs and store them in function pointers for later use. Retrieving those locations is OS-specific 1 . In
Windows it looks something like this:

```
    // define the function’s prototype
    typedef void (*GL_GENBUFFERS) (GLsizei, GLuint*);
    
    // find the function and assign it to a function pointer
    GL_GENBUFFERS glGenBuffers =
    (GL_GENBUFFERS)wglGetProcAddress("glGenBuffers");
    
    // function can now be called as normal
    unsigned int buffer;
    glGenBuffers(1, &buffer);

```

As you can see the code looks complex and it’s a cumbersome process to do this for each
function you may need that is not yet declared. Thankfully, there are libraries for this purpose as
well where GLAD is a popular and up-to-date library.