#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>

template<typename T>
using Matrix = std::vector<std::vector<T>>;

template<typename T>
Matrix<T> multiplyMatrices(const Matrix<T>& A, const Matrix<T>& B) {
    // Check if matrices are empty
    if (A.empty() || B.empty() || A[0].empty() || B[0].empty()) {
        throw std::invalid_argument("Matrices cannot be empty");
    }

    size_t rows_A = A.size();
    size_t cols_A = A[0].size();
    size_t rows_B = B.size();
    size_t cols_B = B[0].size();

    // Check if multiplication is possible (columns of A must equal rows of B)
    if (cols_A != rows_B) {
        throw std::invalid_argument("Matrix multiplication not possible: columns of first matrix ("
                                  + std::to_string(cols_A) + ") must equal rows of second matrix ("
                                  + std::to_string(rows_B) + ")");
    }

    // Verify that all rows in each matrix have the same number of columns
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

    // Initialize result matrix with zeros
    Matrix<T> result(rows_A, std::vector<T>(cols_B, T{}));

    // Perform matrix multiplication
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
void printMatrix(const Matrix<T>& matrix, const std::string& name) {
    std::cout << name << ":\n";
    for (const auto& row : matrix) {
        for (const auto& element : row) {
            std::cout << std::setw(8) << std::setprecision(2) << element << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

int main() {
    try {
        // Example 1: Integer matrices (3x2) * (2x3) = (3x3)
        Matrix<int> A1 = {
            {1, 2},
            {3, 4},
            {5, 6}
        };

        Matrix<int> B1 = {
            {7, 8, 9},
            {10, 11, 12}
        };

        std::cout << "=== Integer Matrix Multiplication ===\n";
        printMatrix(A1, "Matrix A1 (3x2)");
        printMatrix(B1, "Matrix B1 (2x3)");

        Matrix<int> result1 = multiplyMatrices(A1, B1);
        printMatrix(result1, "Result A1 * B1 (3x3)");

        // Example 2: Double matrices (2x3) * (3x2) = (2x2)
        Matrix<double> A2 = {
            {1.5, 2.5, 3.5},
            {4.5, 5.5, 6.5}
        };

        Matrix<double> B2 = {
            {1.1, 2.1},
            {3.1, 4.1},
            {5.1, 6.1}
        };

        std::cout << "=== Double Matrix Multiplication ===\n";
        printMatrix(A2, "Matrix A2 (2x3)");
        printMatrix(B2, "Matrix B2 (3x2)");

        Matrix<double> result2 = multiplyMatrices(A2, B2);
        printMatrix(result2, "Result A2 * B2 (2x2)");

        // Example 3: Demonstrate error handling - incompatible dimensions
        std::cout << "=== Error Handling Example ===\n";
        Matrix<int> A3 = {
            {1, 2, 3},
            {4, 5, 6}
        };

        Matrix<int> B3 = {
            {1, 2},
            {3, 4}
        };

        std::cout << "Attempting to multiply incompatible matrices:\n";
        printMatrix(A3, "Matrix A3 (2x3)");
        printMatrix(B3, "Matrix B3 (2x2)");

        Matrix<int> result3 = multiplyMatrices(A3, B3);  // This will throw an exception

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n\n";
    }

    // Example 4: Float matrices - square matrices
    try {
        std::cout << "=== Float Matrix Multiplication (Square Matrices) ===\n";
        Matrix<float> A4 = {
            {1.0f, 2.0f},
            {3.0f, 4.0f}
        };

        Matrix<float> B4 = {
            {5.0f, 6.0f},
            {7.0f, 8.0f}
        };

        printMatrix(A4, "Matrix A4 (2x2)");
        printMatrix(B4, "Matrix B4 (2x2)");

        Matrix<float> result4 = multiplyMatrices(A4, B4);
        printMatrix(result4, "Result A4 * B4 (2x2)");

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }

    return 0;
}
