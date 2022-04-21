#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

static void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* source_code = source.c_str();
    glShaderSource(id, 1, &source_code, nullptr);
    glCompileShader(id);

    // ERROR HANDLING
    int shader_debug;
    glGetShaderiv(id, GL_COMPILE_STATUS, &shader_debug);
    if (shader_debug == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_COMPILE_STATUS, &length);
        char* message = (char*) (alloca(sizeof(char) * length));

        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Shader compilation failed for " << (type == GL_VERTEX_SHADER ? "vertex shader " : "fragment shader ") <<
            message << std::endl;

        glDeleteShader(id);
        return 0;
    }

        
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    if (program != GL_FALSE)
    {
        unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vs);
        glAttachShader(program, fs);

        glLinkProgram(program);
        glValidateProgram(program);

        int program_link;
        glGetProgramiv(program, GL_LINK_STATUS, &program_link);
        if (program_link == GL_FALSE)
        {
            int length;
            glGetProgramiv(program, GL_LINK_STATUS, &length);
            char* message = (char*)alloca(sizeof(char) * length);

            glGetProgramInfoLog(program, length, &length, message);
            std::cout << "Program linking failed with error message:  " << message << std::endl;

            return 0;

        }

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;

    }

    return 0;

}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "GLEW was not initialized" << std::endl;

    float positions[6] = {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    unsigned int buffer = 1;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    // to specify the layout of our vertices
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    const std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout (location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "  gl_Position = position;\n"
        "}\n";

    const std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "  color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
        "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}