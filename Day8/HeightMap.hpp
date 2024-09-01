#ifndef ADVENTOFCODE2022_HEIGHTMAP_HPP
#define ADVENTOFCODE2022_HEIGHTMAP_HPP

#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "../Common/FileHandling.h"
#include "../Common/Vector.hpp"

// using QuadrantLattice = GridVector = Vector2d;

namespace Day08 {

// 2-dimensional vector space which spans the 32-bit integers
using Vector2d = Geometry::Vector<int32_t, 2>;

// directional unit vectors
const Vector2d UNIT_VECTOR_X {1, 0};
const Vector2d UNIT_VECTOR_Y {0, 1};
const Vector2d UNIT_VECTOR_NEGATIVE_X {-1, 0};
const Vector2d UNIT_VECTOR_NEGATIVE_Y {0, -1};

// dataclass (accessible and modifiable with non-guarded state)
struct HeightMap {
    Vector2d origin   {0, 0},  // lower-left vertex
             extremal {0, 0};  // upper-right vertex
    std::unordered_map<Vector2d, uint16_t, Vector2d::Hash> height_map;

    static std::optional<HeightMap> build(std::istream& file, Vector2d coordinate, std::stringstream& error);
};

std::vector<Vector2d> linearFov(const HeightMap& forest, Vector2d coordinate, const Vector2d& step,
                                const uint16_t viewHeight, const uint16_t maxHeight) {
    std::vector<Vector2d> visibleTrees {};

    coordinate.add(step);
    auto iter = forest.height_map.find(coordinate);

    uint16_t highest = 0;
    while (highest < maxHeight and iter != forest.height_map.end()) {

        uint16_t height = iter -> second;
        if (height > highest) {
            highest = height;
            visibleTrees.push_back(coordinate);
        }
        else if (height < viewHeight) {
            visibleTrees.push_back(coordinate);
        }

        iter = forest.height_map.find(coordinate.add(step));
    }

    return visibleTrees;
}

/// Cross-section scan returning set of visible trees from the edge of HeightMap
std::unordered_set<Vector2d, Vector2d::Hash> edgeVisibleTrees(const HeightMap &forest) {
    std::unordered_set<Vector2d, Vector2d::Hash> visibleTrees;

    // count left-to-right and right-to-left
    for (int32_t y = forest.origin[1]; y != forest.extremal[1] + 1; ++y) {

        std::ranges::for_each(
            linearFov(forest, {forest.origin[0] - 1, y}, UNIT_VECTOR_X, 0, 10),
            [&visibleTrees](const Vector2d& coord){visibleTrees.insert(coord);}
        );

        std::ranges::for_each(
            linearFov(forest, {forest.extremal[0] + 1, y}, UNIT_VECTOR_NEGATIVE_X, 0, 10),
            [&visibleTrees](const Vector2d& coord){visibleTrees.insert(coord);}
        );
    }

    // count bottom-to-top and top-to-bottom
    for (int32_t x = forest.origin[0]; x != forest.extremal[0] + 1; ++x) {

        std::ranges::for_each(
            linearFov(forest, {x, forest.origin[1] - 1}, UNIT_VECTOR_Y, 0, 10),
            [&visibleTrees](const Vector2d& coord){visibleTrees.insert(coord);}
        );

        std::ranges::for_each(
            linearFov(forest, {x, forest.extremal[1] + 1}, UNIT_VECTOR_NEGATIVE_Y, 0, 10),
            [&visibleTrees](const Vector2d& coord){visibleTrees.insert(coord);}
        );
    }

    return visibleTrees;
}

uint32_t evalScenicScore(const HeightMap &forest, const Vector2d& coordinate) {
    uint32_t scenicScore = 1;

    const auto iter = forest.height_map.find(coordinate);
    if (iter == forest.height_map.end()) {
        return 0;
    }
    const uint16_t height = iter -> second;

    const std::array<Vector2d, 4> directions {
        UNIT_VECTOR_X,
        UNIT_VECTOR_Y,
        UNIT_VECTOR_NEGATIVE_X,
        UNIT_VECTOR_NEGATIVE_Y
    };
    for (auto direction : directions) {
        scenicScore *= linearFov(forest, coordinate, direction, height, height).size();
    }

    return scenicScore;
}

Vector2d mostScenicView(const HeightMap &forest) {
    Vector2d mostScenicView;

    uint32_t highest = 0;
    for (int32_t y = forest.origin[1]; y != forest.extremal[1] + 1; ++y) {

        for (int32_t x = forest.origin[0]; x != forest.extremal[0] + 1; ++x) {

            const uint32_t score = evalScenicScore(forest, {x, y});
            if (score > highest) {
                highest = score;
                mostScenicView = {x, y};
            }
        }

    }

    return mostScenicView;
}

} // Day08

#endif //ADVENTOFCODE2022_HEIGHTMAP_HPP