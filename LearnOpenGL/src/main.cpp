#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
       x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
    // Clear all the errors
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError()) // runs while the error has value/ not false
    {
        std::cout << "[OpenGL Error] (" << error << ")" << " " << function <<
            " " << file << ":" << line << std::endl;
        return false;
    }

    return true;
}

struct ShaderProgramSource
{
    std::string VertexShader;
    std::string FragmentShader;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                // vertex shader
            {
                type = ShaderType::VERTEX;
            }

            else if (line.find("fragment") != std::string::npos)
                // fragment shader
            {
                type = ShaderType::FRAGMENT;
            }
        }   
        else
        {
            if (type != ShaderType::NONE)
                ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str() , ss[1].str() };
}

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
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "GLEW was not initialized" << std::endl;

    float positions[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f
    };

    // for the index buffer so there is no repetition
    unsigned int indices[] = {
       0, 1, 2, 
       2, 3, 0
    };

    // VERTEX ARRAY OBJECT
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // ARRAY BUFFER
    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

    // to specify the layout of our vertices
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    // INDEX BUFFER 
    unsigned int index_buffer;
    GLCall(glGenBuffers(1, &index_buffer));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    ShaderProgramSource shaderSource = ParseShader("res/shaders/basic.shader");

    unsigned int shader = CreateShader(shaderSource.VertexShader, shaderSource.FragmentShader);
    GLCall(glUseProgram(shader));

    // UNIFORM 
    int location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != -1);
    GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

    float r = 0.0f;
    float increment = 0.05f;

    GLCall(glBindVertexArray(0));
    GLCall(glUseProgram(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glUseProgram(shader));
        // animating the color of the rectangle
        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
    
        GLCall(glBindVertexArray(vao));
        // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer));

        // function used most of the times
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;

        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    } 


    glfwTerminate();
    return 0;
}