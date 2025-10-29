#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <cmath>
#include <GL/glut.h>
#include "GLMatrix.h"

template<typename T>
using Matrix = std::vector<std::vector<T>>;

// Global variables for camera control
float cameraAngleX = 30.0f;
float cameraAngleY = 45.0f;
float cameraDistance = 500.0f;
int lastMouseX = 0, lastMouseY = 0;
bool isRotating = false;

// Visualization mode
int visualizationMode = 0; // 0: original, 1: X-reflection, 2: Y-reflection, 3: Z-reflection, 4: origin reflection, 5: all

template<typename T>
Matrix<T> multiplyMatrices(const Matrix<T>& A, const Matrix<T>& B) {
    if (A.empty() || B.empty() || A[0].empty() || B[0].empty()) {
        throw std::invalid_argument("Matrices cannot be empty");
    }

    size_t rows_A = A.size();
    size_t cols_A = A[0].size();
    size_t rows_B = B.size();
    size_t cols_B = B[0].size();

    if (cols_A != rows_B) {
        throw std::invalid_argument("Matrix multiplication not possible: columns of first matrix ("
                                  + std::to_string(cols_A) + ") must equal rows of second matrix ("
                                  + std::to_string(rows_B) + ")");
    }

    for (const auto& row : A) {
        if (row.size() != cols_A) {
            throw std::invalid_argument("First matrix has inconsistent row sizes");
        }
    }
    for (const auto& row : B) {
        if (row.size() != cols_B) {
            throw std::invalid_argument("Second matrix has inconsistent row sizes");
        }
    }

    Matrix<T> result(rows_A, std::vector<T>(cols_B, T{}));

    for (size_t i = 0; i < rows_A; ++i) {
        for (size_t j = 0; j < cols_B; ++j) {
            for (size_t k = 0; k < cols_A; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return result;
}

template<typename T>
Matrix<T> createReflectionX() {
    Matrix<T> reflection = {
        {-1, 0,  0,  0},
        {0,  1,  0,  0},
        {0,  0,  1,  0},
        {0,  0,  0,  1}
    };
    return reflection;
}

template<typename T>
Matrix<T> createReflectionY() {
    Matrix<T> reflection = {
        {1,  0,  0,  0},
        {0, -1,  0,  0},
        {0,  0,  1,  0},
        {0,  0,  0,  1}
    };
    return reflection;
}

template<typename T>
Matrix<T> createReflectionZ() {
    Matrix<T> reflection = {
        {1,  0,  0,  0},
        {0,  1,  0,  0},
        {0,  0, -1,  0},
        {0,  0,  0,  1}
    };
    return reflection;
}

template<typename T>
Matrix<T> createReflectionOrigin() {
    Matrix<T> reflection = {
        {-1,  0,  0,  0},
        {0,  -1,  0,  0},
        {0,   0, -1,  0},
        {0,   0,  0,  1}
    };
    return reflection;
}

void drawText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set up camera
    gluLookAt(cameraDistance * cos(cameraAngleY * M_PI / 180.0f) * cos(cameraAngleX * M_PI / 180.0f),
              cameraDistance * sin(cameraAngleX * M_PI / 180.0f),
              cameraDistance * sin(cameraAngleY * M_PI / 180.0f) * cos(cameraAngleX * M_PI / 180.0f),
              0.0f, 50.0f, 40.0f,  // Look at point (center of cube approximately)
              0.0f, 1.0f, 0.0f);

    // Draw grid and axes
    drawGrid();
    drawAxes();

    // Draw original cube (always visible with lower opacity in "all" mode)
    if (visualizationMode == 0 || visualizationMode == 5) {
        float alpha = (visualizationMode == 5) ? 0.3f : 1.0f;
        glColor4f(1.0f, 1.0f, 1.0f, alpha);
        drawCube(1.0f, 1.0f, 1.0f, alpha);
    }

    // Apply transformations based on mode
    const auto reflectX = createReflectionX<float>();
    const auto reflectY = createReflectionY<float>();
    const auto reflectZ = createReflectionZ<float>();
    const auto reflectOrigin = createReflectionOrigin<float>();

    // Draw reflected cubes
    if (visualizationMode == 1 || visualizationMode == 5) {
        // X-axis reflection (YZ plane) - Cyan
        glPushMatrix();
        float glMatrix[16];
        matrixToGLFormat(reflectX, glMatrix);
        glMultMatrixf(glMatrix);
        drawCube(0.0f, 1.0f, 1.0f); // Cyan
        glPopMatrix();
    }

    if (visualizationMode == 2 || visualizationMode == 5) {
        // Y-axis reflection (XZ plane) - Magenta
        glPushMatrix();
        float glMatrix[16];
        matrixToGLFormat(reflectY, glMatrix);
        glMultMatrixf(glMatrix);
        drawCube(1.0f, 0.0f, 1.0f); // Magenta
        glPopMatrix();
    }

    if (visualizationMode == 3 || visualizationMode == 5) {
        // Z-axis reflection (XY plane) - Yellow
        glPushMatrix();
        float glMatrix[16];
        matrixToGLFormat(reflectZ, glMatrix);
        glMultMatrixf(glMatrix);
        drawCube(1.0f, 1.0f, 0.0f); // Yellow
        glPopMatrix();
    }

    if (visualizationMode == 4 || visualizationMode == 5) {
        // Origin reflection - Orange
        glPushMatrix();
        float glMatrix[16];
        matrixToGLFormat(reflectOrigin, glMatrix);
        glMultMatrixf(glMatrix);
        drawCube(1.0f, 0.5f, 0.0f); // Orange
        glPopMatrix();
    }

    // Draw UI text
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f);
    std::string modeText;
    switch (visualizationMode) {
        case 0: modeText = "Mode: Original Cube (White)"; break;
        case 1: modeText = "Mode: X-axis Reflection (Cyan) - YZ Plane"; break;
        case 2: modeText = "Mode: Y-axis Reflection (Magenta) - XZ Plane"; break;
        case 3: modeText = "Mode: Z-axis Reflection (Yellow) - XY Plane"; break;
        case 4: modeText = "Mode: Origin Reflection (Orange)"; break;
        case 5: modeText = "Mode: All Reflections"; break;
    }
    drawText(10, glutGet(GLUT_WINDOW_HEIGHT) - 20, modeText);
    drawText(10, glutGet(GLUT_WINDOW_HEIGHT) - 40, "Keys: 0-5 (modes), Mouse drag to rotate, Scroll to zoom");
    drawText(10, glutGet(GLUT_WINDOW_HEIGHT) - 60, "Axes: X=Red, Y=Green, Z=Blue");

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 2000.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case '0': visualizationMode = 0; break;
        case '1': visualizationMode = 1; break;
        case '2': visualizationMode = 2; break;
        case '3': visualizationMode = 3; break;
        case '4': visualizationMode = 4; break;
        case '5': visualizationMode = 5; break;
        case 27: // ESC
            exit(0);
            break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isRotating = true;
            lastMouseX = x;
            lastMouseY = y;
        } else {
            isRotating = false;
        }
    }

    // Mouse wheel for zoom
    if (button == 3) { // Scroll up
        cameraDistance *= 0.9f;
        glutPostRedisplay();
    } else if (button == 4) { // Scroll down
        cameraDistance *= 1.1f;
        glutPostRedisplay();
    }
}

void motion(int x, int y) {
    if (isRotating) {
        cameraAngleY += (x - lastMouseX) * 0.5f;
        cameraAngleX += (y - lastMouseY) * 0.5f;

        // Clamp vertical angle
        if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
        if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;

        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}

void init() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

int main(int argc, char** argv) {
    try {
        // Print transformation info to console
        std::cout << "=== Matrix Reflection Visualizer ===\n";
        std::cout << "Controls:\n";
        std::cout << "  0-5: Switch visualization modes\n";
        std::cout << "  Mouse drag: Rotate view\n";
        std::cout << "  Mouse wheel: Zoom in/out\n";
        std::cout << "  ESC: Exit\n\n";

        std::cout << "Original Cube vertices:\n";
        std::cout << "  p0: (40, 30, 0)\n";
        std::cout << "  p1: (40, 130, 0)\n";
        std::cout << "  p2: (40, 130, 80)\n";
        std::cout << "  p3: (40, 30, 80)\n";
        std::cout << "  p4: (140, 30, 0)\n";
        std::cout << "  p5: (140, 130, 0)\n";
        std::cout << "  p6: (140, 130, 80)\n";
        std::cout << "  p7: (140, 30, 80)\n\n";

        // Initialize GLUT
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowSize(1024, 768);
        glutCreateWindow("3D Matrix Reflection Visualizer");

        init();

        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutKeyboardFunc(keyboard);
        glutMouseFunc(mouse);
        glutMotionFunc(motion);

        glutMainLoop();

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}