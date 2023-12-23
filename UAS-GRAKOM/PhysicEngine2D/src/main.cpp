#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>

float translation1 = 1.0f;
float translation2 = 1.0f;
float speed1 = 0.0005f;
float speed2 = 0.0005f;

float color1[] = { 0.0f, 0.0f, 1.0f, 1.0f };
float color2[] = { 0.0f, 1.0f, 0.0f, 1.0f };

GLint color1Loc, color2Loc;

// fungsi tombol
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        std::cout << "Color changed." << std::endl;
        for (int i = 0; i < 4; i++)
        {
            color1[i] = (rand() * 1.0f) / RAND_MAX;
            color2[i] = (rand() * 1.0f) / RAND_MAX;
        }

        glUniform4f(color1Loc, color1[0], color1[1], color1[2], color1[3]);
        glUniform4f(color2Loc, color2[0], color2[1], color2[2], color2[3]);
    }
}

void updateTranslation()
{
    translation1 += speed1;

    if (translation1 > 1.0f || translation1 < -1.0f)
        speed1 = -speed1;

    translation2 += speed2;

    if (translation2 > 1.0f || translation2 < -1.0f)
        speed2 = -speed2;
}

GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
    // Create Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check Vertex Shader Compilation Status
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex Shader Compilation Failed\n" << infoLog << std::endl;
        return 0;
    }

    // Create Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check Fragment Shader Compilation Status
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Fragment Shader Compilation Failed\n" << infoLog << std::endl;
        return 0;
    }

    // Create Shader Program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check Shader Program Linking Status
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader Program Linking Failed\n" << infoLog << std::endl;
        return 0;
    }

    // Delete Shader Objects (already linked to the program)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

std::string readShaderFile(const char* filePath) {
    std::ifstream shaderFile(filePath);
    std::string shaderCode;
    if (shaderFile.is_open()) {
        std::string line;
        while (getline(shaderFile, line)) {
            shaderCode += line + '\n';
        }
        shaderFile.close();
    }
    return shaderCode;
}

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(900, 600, "Two Translating Triangles", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        return -1;
    }

    glfwSetKeyCallback(window, keyCallback);

    // Define shaders for the first triangle
    const std::string vertexShaderSourceStr = readShaderFile("../shader/vertex.vert");
    const char* vertexShaderSource = vertexShaderSourceStr.c_str();

    const std::string fragmentShaderSourceStr = readShaderFile("../shader/fragment.frag");
    const char* fragmentShaderSource = fragmentShaderSourceStr.c_str();

    // Create and use shader program for the first triangle
    GLuint shaderProgram1 = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    if (shaderProgram1 == 0) {
        glfwTerminate();
        return -1;
    }

    // Define shaders for the second triangle
    const std::string vertexShaderSourceStr2 = readShaderFile("../shader/vertex.vert");
    const char* vertexShaderSource2 = vertexShaderSourceStr2.c_str();

    const std::string fragmentShaderSourceStr2 = readShaderFile("../shader/fragment.frag");
    const char* fragmentShaderSource2 = fragmentShaderSourceStr2.c_str();

    // Create and use shader program for the second triangle
    GLuint shaderProgram2 = createShaderProgram(vertexShaderSource2, fragmentShaderSource2);
    if (shaderProgram2 == 0) {
        glfwTerminate();
        return -1;
    }

    float vertices1[] = {
    -0.25f, -0.25f,
     0.25f, -0.25f,
     0.0f,  0.25f
    };


    GLuint VBO1, VAO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);

    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set up vertex data and buffers and configure vertex attributes for the second triangle
    float vertices2[] = {
     -0.25f, 0.25f,
      0.25f, 0.25f,
      0.0f,  0.75f
    };


    GLuint VBO2, VAO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);

    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        // Update translation for the first triangle
        updateTranslation();

        glUseProgram(shaderProgram1);
        GLint uniformLocation1 = glGetUniformLocation(shaderProgram1, "translation");
        if (uniformLocation1 != -1) {
            glUniform1f(uniformLocation1, translation1);
        }

        color1Loc = glGetUniformLocation(shaderProgram1, "color1");
        glUniform4f(color1Loc, color1[0], color1[1], color1[2], color1[3]);

        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Update translation for the second triangle
        updateTranslation();

        glUseProgram(shaderProgram2);
        GLint uniformLocation2 = glGetUniformLocation(shaderProgram2, "translation");
        if (uniformLocation2 != -1) {
            glUniform1f(uniformLocation2, translation2);
        }

        color2Loc = glGetUniformLocation(shaderProgram2, "color2");
        glUniform4f(color2Loc, color2[0], color2[1], color2[2], color2[3]);

        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

    }
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteProgram(shaderProgram1);
    glDeleteProgram(shaderProgram2);


    glfwTerminate();
    return 0;
}