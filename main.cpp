#include <algorithm>
#include <charconv>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using Value = long long;
using Matrix = std::vector<std::vector<Value>>;

// Parse a whole decimal token; leading zeroes do not mean octal.
Value parseInteger(const std::string& token) {
    Value value = 0;
    const char* first = token.data();
    const char* last = first + token.size();
    if (first != last && *first == '+') {
        ++first;
        if (first == last || *first == '-' || *first == '+') {
            throw std::runtime_error("Invalid integer: " + token);
        }
    }
    const auto result = std::from_chars(first, last, value);
    if (result.ec != std::errc{} || result.ptr != last) {
        throw std::runtime_error("Invalid or out-of-range integer: " + token);
    }
    return value;
}

Value readInteger(std::istream& input) {
    std::string token;
    if (!(input >> token)) {
        throw std::runtime_error("Input file is empty or missing matrix values.");
    }
    return parseInteger(token);
}

// Problem 1: size dynamically after reading N, with no fixed lab-size limit.
std::pair<Matrix, Matrix> loadMatrices(const std::string& filename) {
    std::ifstream input(filename);
    if (!input) {
        throw std::runtime_error("Cannot open input file: " + filename);
    }
    const Value dimension = readInteger(input);
    if (dimension <= 0) {
        throw std::runtime_error("Matrix size N must be positive.");
    }
    if (static_cast<unsigned long long>(dimension) > Matrix{}.max_size()) {
        throw std::runtime_error("Matrix size exceeds the supported container size.");
    }
    const auto n = static_cast<std::size_t>(dimension);
    Matrix a(n), b(n);
    for (Matrix* matrix : {&a, &b}) {
        for (auto& row : *matrix) {
            row.resize(n);
            for (Value& value : row) {
                value = readInteger(input);
                if (value < std::numeric_limits<int>::min() ||
                    value > std::numeric_limits<int>::max()) {
                    throw std::runtime_error("Matrix entries must fit in a C++ int.");
                }
            }
        }
    }
    std::string extra;
    if (input >> extra) {
        throw std::runtime_error("Unexpected data after the two matrices.");
    }
    if (input.bad()) {
        throw std::runtime_error("Could not finish reading the input file.");
    }
    return {std::move(a), std::move(b)};
}

void printMatrix(const Matrix& matrix) {
    std::size_t width = 4;
    for (const auto& row : matrix) {
        for (Value value : row) {
            width = std::max(width, std::to_string(value).size() + 1);
        }
    }
    for (const auto& row : matrix) {
        for (Value value : row) {
            std::cout << std::setw(static_cast<int>(width)) << value;
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

Value checkedAdd(Value a, Value b) {
    const Value minimum = std::numeric_limits<Value>::min();
    const Value maximum = std::numeric_limits<Value>::max();
    if ((b > 0 && a > maximum - b) || (b < 0 && a < minimum - b)) {
        throw std::overflow_error("Matrix arithmetic exceeds the long long range.");
    }
    return a + b;
}

// Problem 2: corresponding entries add independently.
Matrix addMatrices(const Matrix& a, const Matrix& b) {
    Matrix result(a.size(), std::vector<Value>(a.size()));
    for (std::size_t i = 0; i < a.size(); ++i) {
        for (std::size_t j = 0; j < a.size(); ++j) {
            result[i][j] = checkedAdd(a[i][j], b[i][j]);
        }
    }
    return result;
}

// Problem 3: row i of A dotted with column j of B.
Matrix multiplyMatrices(const Matrix& a, const Matrix& b) {
    Matrix result(a.size(), std::vector<Value>(a.size(), 0));
    for (std::size_t i = 0; i < a.size(); ++i) {
        for (std::size_t j = 0; j < a.size(); ++j) {
            for (std::size_t k = 0; k < a.size(); ++k) {
                // Input entries fit in int; their product fits in long long.
                result[i][j] = checkedAdd(result[i][j], a[i][k] * b[k][j]);
            }
        }
    }
    return result;
}

// Problem 4: the centre of an odd-sized matrix belongs to both sums.
std::pair<Value, Value> diagonalSums(const Matrix& matrix) {
    Value main = 0;
    Value secondary = 0;
    for (std::size_t i = 0; i < matrix.size(); ++i) {
        main = checkedAdd(main, matrix[i][i]);
        secondary = checkedAdd(secondary, matrix[i][matrix.size() - 1 - i]);
    }
    return {main, secondary};
}

bool validIndex(const Matrix& matrix, Value index) {
    return index >= 0 && static_cast<unsigned long long>(index) < matrix.size();
}

// Problems 5-7 validate both indices before modifying anything.
bool swapRows(Matrix& matrix, Value first, Value second) {
    if (!validIndex(matrix, first) || !validIndex(matrix, second)) {
        return false;
    }
    std::swap(matrix[static_cast<std::size_t>(first)],
              matrix[static_cast<std::size_t>(second)]);
    return true;
}

bool swapColumns(Matrix& matrix, Value first, Value second) {
    if (!validIndex(matrix, first) || !validIndex(matrix, second)) {
        return false;
    }
    for (auto& row : matrix) {
        std::swap(row[static_cast<std::size_t>(first)],
                  row[static_cast<std::size_t>(second)]);
    }
    return true;
}

bool updateElement(Matrix& matrix, Value row, Value column, Value value) {
    if (!validIndex(matrix, row) || !validIndex(matrix, column)) {
        return false;
    }
    matrix[static_cast<std::size_t>(row)][static_cast<std::size_t>(column)] = value;
    return true;
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 1 && argc != 2 && argc != 9) {
            throw std::runtime_error(
                "Usage: ./main [filename [row1 row2 col1 col2 update_row update_col new_value]]");
        }
        std::string filename;
        if (argc == 1) {
            std::cout << "Enter input filename: \n";
            if (!std::getline(std::cin, filename)) {
                throw std::runtime_error("No input filename provided.");
            }
        } else {
            filename = argv[1];
        }
        Value row1 = 0, row2 = 2, col1 = 0, col2 = 2;
        Value updateRow = 1, updateCol = 2, newValue = 99;
        if (argc == 9) {
            row1 = parseInteger(argv[2]);
            row2 = parseInteger(argv[3]);
            col1 = parseInteger(argv[4]);
            col2 = parseInteger(argv[5]);
            updateRow = parseInteger(argv[6]);
            updateCol = parseInteger(argv[7]);
            newValue = parseInteger(argv[8]);
        }
        const auto matrices = loadMatrices(filename);
        const Matrix& a = matrices.first;
        const Matrix& b = matrices.second;
        std::cout << "Matrix A:\n";
        printMatrix(a);
        std::cout << "Matrix B:\n";
        printMatrix(b);
        std::cout << "A + B:\n";
        printMatrix(addMatrices(a, b));
        std::cout << "A * B:\n";
        printMatrix(multiplyMatrices(a, b));
        const auto sums = diagonalSums(a);
        std::cout << "Diagonal sums for Matrix A:\nMain diagonal sum: " << sums.first
                  << "\nSecondary diagonal sum: " << sums.second << "\n\n";

        Matrix edited = a;
        const bool rowsValid = swapRows(edited, row1, row2);
        std::cout << "Problem 5 - Rows " << row1 << " and " << row2 << " swapped:\n";
        if (!rowsValid) {
            std::cout << "Invalid row indices; matrix unchanged.\n";
        }
        printMatrix(edited);

        edited = a;
        const bool columnsValid = swapColumns(edited, col1, col2);
        std::cout << "Problem 6 - Columns " << col1 << " and " << col2 << " swapped:\n";
        if (!columnsValid) {
            std::cout << "Invalid column indices; matrix unchanged.\n";
        }
        printMatrix(edited);

        edited = a;
        const bool updateValid = updateElement(edited, updateRow, updateCol, newValue);
        std::cout << "Problem 7 - Updated matrix:\n";
        if (!updateValid) {
            std::cout << "Invalid element indices; matrix unchanged.\n";
        }
        printMatrix(edited);
    } catch (const std::bad_alloc&) {
        std::cerr << "Error: Not enough memory for these matrices.\n";
        return 1;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
