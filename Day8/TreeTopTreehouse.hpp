#ifndef ADVENTOFCODE2022_TREETOPTREEHOUSE_HPP
#define ADVENTOFCODE2022_TREETOPTREEHOUSE_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "../Common/FileHandling.h"
#include "../Common/Vector.hpp"

#include "HeightMap.hpp"

namespace Day08 {

using Common::openFile;

// Build HeightMap from file
std::optional<HeightMap> HeightMap::build(std::istream &file, Vector2d coordinate, std::stringstream &error) {
    HeightMap forest;

    std::string first_line;
    if (not std::getline(file, first_line)) {
        error << "File is empty";
        return std::nullopt;
    }

    forest.origin = coordinate;
    for (auto c: first_line) {
        if (not isdigit(c)) {
            error << "Line " << abs(coordinate[1] - forest.origin[1])
                  << ", character " << abs(coordinate[0] - forest.origin[0])
                  << ": The Character " << c << " is not an integer";
            return std::nullopt;
        }
        forest.height_map.emplace(coordinate, static_cast<uint32_t>(c - '0' + 1));  // char '0' = int 48 to int 1
        ++coordinate[0];
    }
    ++coordinate[1];
    forest.extremal[0] = coordinate[0] - 1;

    for (std::string line; std::getline(file, line);) {

        coordinate[0] = forest.origin[0];
        for (auto c: line) {
            if (not isdigit(c)) {
                error << "Line " << abs(coordinate[1] - forest.origin[1])
                      << ", character " << abs(coordinate[0] - forest.origin[0])
                      << ": The Character " << c << " is not an integer";
                return std::nullopt;
            }
            forest.height_map.emplace(coordinate, static_cast<uint32_t>(c - '0' + 1));  // char '0' = int 48 to int 1
            ++coordinate[0];
        }

        if (abs(coordinate[0] - 1) != forest.extremal[0]) {
            error << "Map is not rectangular";
            return std::nullopt;
        }
        ++coordinate[1];
    }

    // minus one because the last roll of the for loop
    forest.extremal[1] = coordinate[1] - 1;

    return forest;
}

static int run(const std::string &location) {
    std::stringstream error;

    // open file
    std::ifstream input(location);
    if (not openFile(input, error, 5, 100)) {
        std::cerr << error.str();
        return -1;
    }

    // build from file
    auto mapForest = HeightMap::build(input, {0, 0}, error);
    if (not mapForest) {
        std::cerr << error.str();
        return -1;
    }
    const auto forest = *mapForest;

    // task 1
    const auto visibleTrees = edgeVisibleTrees(forest);
    // task 2
    const Vector2d vista = mostScenicView(forest);
    const uint32_t score = evalScenicScore(forest, vista);

    // display task 1
    std::cout << "Number of trees visible from the forest edge: " << visibleTrees.size() << "\n";
    // display task 2
    std::cout << "The tree at coordinate (" << vista[0] << ", " << vista[1] << ")"
              << " has the most scenic view with a score of " << score << "\n";

    input.close();
    return 0;
}

} // Day08

#endif //ADVENTOFCODE2022_TREETOPTREEHOUSE_HPP