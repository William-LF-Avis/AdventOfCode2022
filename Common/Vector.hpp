#ifndef ADVENTOFCODE2022_VECTOR_HPP
#define ADVENTOFCODE2022_VECTOR_HPP

#include <cstdint>
#include <cmath>
#include <vector>
#include <array>
#include <utility>
#include <optional>

namespace Geometry {

void translate(std::pair<int32_t, int32_t> &coordinate, std::pair<int32_t, int32_t> translation_vector) {
    coordinate.first += translation_vector.first;
    coordinate.second += translation_vector.second;
}

struct Coordinate {
    int32_t x, y;

    explicit Coordinate(int32_t x = 0, int32_t y = 0) : x(x), y(y) {}

    void translate(const Coordinate translation_vector) {
        x += translation_vector.x;
        y += translation_vector.y;
    }
};

/// Coordinate vector V of fixed dimension N containing arithmetic elements T
template<typename X, std::size_t N>
class Vector {
    std::array<X, N> V;

public:
    Vector() = default;
    explicit Vector(const std::array<X, N>& vector) : V(vector) {}

    Vector(std::initializer_list<X> initializer) {
        std::move(initializer.begin(), initializer.end(), V.begin());
    }

    // Access vector elements
    const X& operator[](std::size_t n) const {
        return V[n];
    }

    // Access and modify vector coordinates
    X& operator[](std::size_t n) {
        return V[n];
    }

    // Dimension of the vector
    [[nodiscard]] std::size_t dim() const {
        return N;
    }

    // Euclidean norm (p-norm for p=2)
    [[nodiscard]] double_t norm() const {
        double_t sum_of_squares = 0;
        for (auto x : V) {
            sum_of_squares += pow(x, 2);
        }
        return sqrt(sum_of_squares);
    }

    bool operator==(const Vector& W) const {
        return V == W.V;
    }

    // Vector addition for vectors with elements of the same type
    Vector operator+(const Vector& W) const {
        Vector sum;
        for (std::size_t n = 0; n < N; ++n) {
            sum[n] = V[n] + W[n];
        }
        return sum;
    }

    /// Vector Subtraction which is only defined for the set of elements X >= I > N
    template<typename S = X>
    std::enable_if_t<std::is_signed<S>::value, Vector> operator-(const Vector& W) const {
        Vector difference;
        for (std::size_t n = 0; n < N; ++n) {
            difference[n] = V[n] - W[n];
        }
        return difference;
    }

    // Define a add operator
    Vector& add(const Vector& W) {
        for (std::size_t n = 0; n < N; ++n) {
            V[n] += W[n];
        }

        return *this; // Return a reference to the modified object
    }

    // Define the -= operator
    template<typename S = X>
    std::enable_if_t<std::is_signed<S>::value, Vector&> subtract(const Vector& W) {
        for (std::size_t n = 0; n < N; ++n) {
            V[n] -= W[n];
        }
        return *this; // Return a reference to the modified object
    }

    // Scalar multiplication
    Vector scale(const X scalar) const {
        Vector result;
        std::transform(V.begin(), V.end(), result.V.begin(),
                       [scalar](const X& a) { return a * scalar; });
        return result;
    }

    // Hash functor for Vector class which defines a hash for a unique vector
    struct Hash {
        std::size_t operator()(const Vector<X, N>& W) const {
            std::size_t hash = 0;
            for (const auto& n : W.V) {
                hash ^= std::hash<X>{}(n) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            return hash;
        }
    };

};

}

#endif //ADVENTOFCODE2022_VECTOR_HPP