#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

// Clipping window
float xmin = 200, xmax = 800;
float ymin = 150, ymax = 550;

// Single line
float x1 = 100, y1 = 100;
float x2 = 900, y2 = 600;

// Region codes
const int INSIDE = 0;
const int LEFT   = 1;
const int RIGHT  = 2;
const int BOTTOM = 4;
const int TOP    = 8;

int computeCode(float x, float y) {
    int code = INSIDE;

    if (x < xmin) code |= LEFT;
    else if (x > xmax) code |= RIGHT;

    if (y < ymin) code |= BOTTOM;
    else if (y > ymax) code |= TOP;

    return code;
}

bool cohenSutherland(float &x1, float &y1, float &x2, float &y2) {

    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);

    while (true) {

        if ((code1 | code2) == 0) {
            return true;   // completely inside
        }
        else if (code1 & code2) {
            return false;  // completely outside
        }
        else {
            float x, y;
            int codeOut = code1 ? code1 : code2;

            if (codeOut & TOP) {
                x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
                y = ymax;
            }
            else if (codeOut & BOTTOM) {
                x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
                y = ymin;
            }
            else if (codeOut & RIGHT) {
                y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
                x = xmax;
            }
            else { // LEFT
                y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
                x = xmin;
            }

            if (codeOut == code1) {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            } else {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }
}

int main() {

    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(1000, 700, "Cohen Sutherland - Tejaswi Acharya", NULL, NULL);
    glfwMakeContextCurrent(window);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1000, 0, 700, -1, 1);

    // Store original line
    float ox1 = x1, oy1 = y1;
    float ox2 = x2, oy2 = y2;

    bool visible = cohenSutherland(x1, y1, x2, y2);

    while (!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT);

        // RED Clipping window
        glColor3f(1, 0, 0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(xmin, ymin);
        glVertex2f(xmax, ymin);
        glVertex2f(xmax, ymax);
        glVertex2f(xmin, ymax);
        glEnd();

        //BLUE Original line
        glColor3f(0, 0, 1);
        glBegin(GL_LINES);
        glVertex2f(ox1, oy1);
        glVertex2f(ox2, oy2);
        glEnd();

        // GREEN Clipped line (if visible)
        if (visible) {
            glColor3f(0, 1, 0);
            glBegin(GL_LINES);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
            glEnd();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}