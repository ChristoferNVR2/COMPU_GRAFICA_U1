#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>

template<typename T>
using Matrix = std::vector<std::vector<T>>;

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
Matrix<T> createTranslationMatrix(T tx, T ty, T tz) {
    Matrix<T> translation = {
        {1, 0, 0, tx},
        {0, 1, 0, ty},
        {0, 0, 1, tz},
        {0, 0, 0, 1}
    };
    return translation;
}

template<typename T>
Matrix<T> createScaleMatrix(T sx, T sy, T sz) {
    Matrix<T> scale = {
        {sx, 0,  0,  0},
        {0,  sy, 0,  0},
        {0,  0,  sz, 0},
        {0,  0,  0,  1}
    };
    return scale;
}

// template<typename T>
// Matrix<T> createScaleMatrixAroundPoint(T sx, T sy, T sz, T px, T py, T pz) {
//     // Translate to origin -> Scale -> Translate back
//     Matrix<T> translateToOrigin = createTranslationMatrix(-px, -py, -pz);
//     Matrix<T> scale = createScaleMatrix(sx, sy, sz);
//     Matrix<T> translateBack = createTranslationMatrix(px, py, pz);
//
//     Matrix<T> temp = multiplyMatrices(scale, translateToOrigin);
//     return multiplyMatrices(translateBack, temp);
// }

// template<typename T>
// Matrix<T> createScaleMatrixAroundPoint(T sx, T sy, T sz, T px, T py, T pz) {
//     Matrix<T> scale = createScaleMatrix(sx, sy, sz);
//     Matrix<T> translate = createTranslationMatrix(px * (1 - sx), py * (1 - sy), pz * (1 - sz));
//     return multiplyMatrices(translate, scale);
// }

template<typename T>
Matrix<T> createScaleMatrixAroundPoint(T sx, T sy, T sz, T px, T py, T pz) {
    return {
        {sx, 0,  0,  px * (1 - sx)},
        {0,  sy, 0,  py * (1 - sy)},
        {0,  0,  sz, pz * (1 - sz)},
        {0,  0,  0,  1}
    };
}


template<typename T>
void printMatrix(const Matrix<T>& matrix, const std::string& name) {
    std::cout << name << ":\n";
    for (const auto& row : matrix) {
        for (const auto& element : row) {
            // std::cout << std::setw(8) << std::setprecision(2) << element << " ";
            std::cout << std::setw(8) << element << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
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


int main() {
    try {
        // std::cout << "=== Transformation Matrices ===\n\n";
        //
        // auto translation = createTranslationMatrix(2.0f, 3.0f, 1.0f);
        // printMatrix(translation, "Translation Matrix (2, 3, 1)");
        //
        // auto scale = createScaleMatrix(2.0f, 1.5f, 0.5f);
        // printMatrix(scale, "Scale Matrix (2x, 1.5y, 0.5z)");
        //
        // auto scaleAroundPoint = createScaleMatrixAroundPoint(2.0f, 2.0f, 2.0f, 1.0f, 1.0f, 1.0f);
        // printMatrix(scaleAroundPoint, "Scale 2x around point (1, 1, 1)");





        // std::cout << "=== Performing Transformations ===\n\n";
        // Matrix<float> point = {{2.0f}, {2.0f}, {2.0f}, {1.0f}};
        // printMatrix(point, "Original Point (2, 2, 2)");
        //
        // auto translatedPoint = multiplyMatrices(translation, point);
        // printMatrix(translatedPoint, "After Translation");
        //
        // auto scaledPoint = multiplyMatrices(scale, point);
        // printMatrix(scaledPoint, "After Scale from Origin");
        //
        // auto scaledAroundPointResult = multiplyMatrices(scaleAroundPoint, point);
        // printMatrix(scaledAroundPointResult, "After Scale around Point (1,1,1)");





        // NOTE: Matrices are multiplied in reverse order of application
        // So, the order is Translate -> Scale (application order)
        // Theoretically: Transformed = [Translation] * [Rotation] * [Scale] * [Original Vector]
        // std::cout << "=== Performing Multiple Transformations ===\n";
        // auto combined = multiplyMatrices(scale, translation);
        // printMatrix(combined, "Combined: Scale * Translation");
        //
        // auto combinedResult = multiplyMatrices(combined, point);
        // printMatrix(combinedResult, "Point after Combined Transform");

        // Mathematically equivalent to:
        // auto temp = multiplyMatrices(scale, point);
        // auto temp2 = multiplyMatrices(temp, translation);



        //
        // auto scaleOneThird = createScaleMatrix(1.0f/3.0f, 1.0f/3.0f, 1.0f/3.0f);
        // printMatrix(scaleOneThird, "Reduce to 1/3 relative to origin");
        //
        // std::vector<Matrix<float>> cubeVertices = {
        //     {{40.0f}, {30.0f}, {0.0f}, {1.0f}},   // p0
        //     {{40.0f}, {130.0f}, {0.0f}, {1.0f}},  // p1
        //     {{40.0f}, {130.0f}, {80.0f}, {1.0f}}, // p2
        //     {{40.0f}, {30.0f}, {80.0f}, {1.0f}},  // p3
        //     {{140.0f}, {30.0f}, {0.0f}, {1.0f}},  // p4
        //     {{140.0f}, {130.0f}, {0.0f}, {1.0f}}, // p5
        //     {{140.0f}, {130.0f}, {80.0f}, {1.0f}},// p6
        //     {{140.0f}, {30.0f}, {80.0f}, {1.0f}}  // p7
        // };
        //
        // std::cout << "Original Cube Vertices:\n";
        // for (int i = 0; i < cubeVertices.size(); ++i) {
        //     printMatrix(cubeVertices[i], "p" + std::to_string(i) +
        //                " (" + std::to_string((int)cubeVertices[i][0][0]) + ", " +
        //                std::to_string((int)cubeVertices[i][1][0]) + ", " +
        //                std::to_string((int)cubeVertices[i][2][0]) + ")");
        // }
        //
        // std::cout << "Transformed Cube Vertices (1/3 scale from origin):\n";
        // for (int i = 0; i < cubeVertices.size(); ++i) {
        //     auto transformedVertex = multiplyMatrices(scaleOneThird, cubeVertices[i]);
        //     printMatrix(transformedVertex, "p" + std::to_string(i) + " scaled");
        // }



        const auto reflectX = createReflectionX<float>();
        const auto reflectY = createReflectionY<float>();
        const auto reflectZ = createReflectionZ<float>();
        const auto reflectOrigin = createReflectionOrigin<float>();

        const std::vector<Matrix<float>> cubeVertices = {
            {{40.0f}, {30.0f}, {0.0f}, {1.0f}},   // p0
            {{40.0f}, {130.0f}, {0.0f}, {1.0f}},  // p1
            {{40.0f}, {130.0f}, {80.0f}, {1.0f}}, // p2
            {{40.0f}, {30.0f}, {80.0f}, {1.0f}},  // p3
            {{140.0f}, {30.0f}, {0.0f}, {1.0f}},  // p4
            {{140.0f}, {130.0f}, {0.0f}, {1.0f}}, // p5
            {{140.0f}, {130.0f}, {80.0f}, {1.0f}},// p6
            {{140.0f}, {30.0f}, {80.0f}, {1.0f}}  // p7
        };

        std::cout << "=== Original Cube ===\n";
        for (int i = 0; i < cubeVertices.size(); ++i) {
            printMatrix(cubeVertices[i], "p" + std::to_string(i) +
                       " (" + std::to_string(static_cast<int>(cubeVertices[i][0][0])) + ", " +
                       std::to_string(static_cast<int>(cubeVertices[i][1][0])) + ", " +
                       std::to_string(static_cast<int>(cubeVertices[i][2][0])) + ")");
        }

        std::cout << "=== Reflection across X-axis (YZ plane) ===\n";
        for (int i = 0; i < cubeVertices.size(); ++i) {
            auto reflected = multiplyMatrices(reflectX, cubeVertices[i]);
            printMatrix(reflected, "p" + std::to_string(i) + " reflected X");
        }

        std::cout << "=== Reflection across Y-axis (XZ plane) ===\n";
        for (int i = 0; i < cubeVertices.size(); ++i) {
            auto reflected = multiplyMatrices(reflectY, cubeVertices[i]);
            printMatrix(reflected, "p" + std::to_string(i) + " reflected Y");
        }

        std::cout << "=== Reflection across Z-axis (XY plane) ===\n";
        for (int i = 0; i < cubeVertices.size(); ++i) {
            auto reflected = multiplyMatrices(reflectZ, cubeVertices[i]);
            printMatrix(reflected, "p" + std::to_string(i) + " reflected Z");
        }

        std::cout << "=== Reflection through Origin ===\n";
        for (int i = 0; i < cubeVertices.size(); ++i) {
            auto reflected = multiplyMatrices(reflectOrigin, cubeVertices[i]);
            printMatrix(reflected, "p" + std::to_string(i) + " reflected origin");
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }

    return 0;
}
