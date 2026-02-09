#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>

using namespace std;

int xc = 0, yc = 0;
int r = 100;

vector<float> points;

void addPoint(int x, int y)
{
    points.push_back(x / 400.0f);
    points.push_back(y / 300.0f);
}

void drawCirclePoints(int xc, int yc, int x, int y)
{
    addPoint(xc + x, yc + y);
    addPoint(xc - x, yc + y);
    addPoint(xc + x, yc - y);
    addPoint(xc - x, yc - y);
    addPoint(xc + y, yc + x);
    addPoint(xc - y, yc + x);
    addPoint(xc + y, yc - x);
    addPoint(xc - y, yc - x);
}

void midpointCircle()
{
    int x = 0;
    int y = r;
    int p = 1 - r;

    while (x <= y)
    {
        drawCirclePoints(xc, yc, x, y);
        x++;
        if (p < 0)
            p = p + 2 * x + 1;
        else
        {
            y--;
            p = p + 2 * x + 1 - 2 * y;
        }
    }
}

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"void main(){ gl_Position = vec4(aPos, 0.0, 1.0); }\n";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){ FragColor = vec4(1.0, 1.0, 1.0, 1.0); }\n";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Midpoint Circle", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    midpointCircle();

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), &points[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glPointSize(2.0);
        glDrawArrays(GL_POINTS, 0, points.size() / 2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
