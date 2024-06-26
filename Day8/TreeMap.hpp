#ifndef ADVENTOFCODE2022_TREEMAP_HPP
#define ADVENTOFCODE2022_TREEMAP_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>

#include "../Common/FileHandling.h"
#include "../Common/Vector.hpp"

// 2-dimensional vector space which spans the positive integers (single quadrant)
using QuadrantLattice = Geometry::Vector<uint32_t, 2>;

const QuadrantLattice zero_vector {0, 0};
const QuadrantLattice unit_vector_x {1, 0};
const QuadrantLattice unit_vector_y {0, 1};
using ScalerField = std::map<QuadrantLattice, uint32_t>;


struct TreeMap {
    // maps tree height for coordinates (x, y)
    std::unordered_map<QuadrantLattice, uint32_t, QuadrantLattice::Hash> height_map;
    uint32_t width = 0, depth = 0;
};

// Text parser
std::istream &operator>>(std::istream &file, TreeMap &forest) {

    QuadrantLattice coordinate = zero_vector;
    for (std::string line; std::getline(file, line);) {
        coordinate[0] = 0;
        coordinate.add(unit_vector_y);
        for (auto c: line) {
            ++coordinate[0];
            forest.height_map.emplace(coordinate, c); // map needs to be unordered to map on equality (uniqueness)
        }
        // TODO: explicit cast coordinate from char to int
    }
    forest.width = coordinate[0];
    forest.depth = coordinate[1];

    return file;
}

void isEdgeVisible(
        const TreeMap &forest,
        const QuadrantLattice& coordinate,
        uint32_t &view_max,
        std::unordered_set<QuadrantLattice, QuadrantLattice::Hash> &visible_trees) {

    std::cout << "x: " << coordinate[0] << " and y:" << coordinate[1] <<  std::endl;

    // TODO: No error handling -> potential unvalidated operation
    auto it = forest.height_map.find(coordinate);

    const uint32_t height = it -> second;
    if (view_max < height) {
        visible_trees.insert(coordinate);
        view_max = height;
    }
}

// Cross-section scan
std::unordered_set<QuadrantLattice, QuadrantLattice::Hash> findVisibleTrees(const TreeMap &forest) {
    std::unordered_set<QuadrantLattice, QuadrantLattice::Hash> visible_trees;


    std::cout << forest.depth << std::endl;
    std::cout << unit_vector_y.scale(forest.depth)[1] << std::endl;

    // count left-to-right and right-to-left
    for (QuadrantLattice coordinate = unit_vector_y; coordinate != unit_vector_y.scale(forest.depth); coordinate.add(unit_vector_y)) {

        uint32_t view_max = 0;
        QuadrantLattice update = coordinate + unit_vector_x;

        // Count trees visible from left
        while (view_max < 57 and update[0] < forest.width) {
            isEdgeVisible(forest, update, view_max, visible_trees);
            update.add(unit_vector_x);
        }

        view_max = 0;
        update = coordinate + unit_vector_x.scale(forest.width);
        // count trees visible from right
        while (view_max < 57 and update[0] >= 1) {
            isEdgeVisible(forest, update, view_max, visible_trees);
            --update[0];
        }

    }


    /* count left-to right and right-to-left
    for (uint32_t j = 1; j <= forest.depth; ++j) {
        uint32_t line_of_sight_maximum = 0;
        // l-r
        for (uint32_t i = 1; i <= forest.width; ++i) {
            isEdgeVisible(forest, QuadrantLattice{i, j}, line_of_sight_maximum, visible_trees);
        }
        line_of_sight_maximum = 0;
        // r-l
        for (uint32_t i = forest.width; i >= 1; --i) {
            isEdgeVisible(forest, QuadrantLattice{i, j}, line_of_sight_maximum, visible_trees);
        }
    }
    */

    // count bottom-to-top and top-to-bottom
    for (uint32_t i = 1; i <= forest.width; ++i) {
        uint32_t line_of_sight_maximum = 0;
        // b-t
        for (uint32_t j = 1; j <= forest.depth; ++j) {
            isEdgeVisible(forest, QuadrantLattice{i, j}, line_of_sight_maximum, visible_trees);
        }
        line_of_sight_maximum = 0;
        //t-b
        for (uint32_t j = forest.depth; j >= 1; --j) {
            isEdgeVisible(forest, QuadrantLattice{i, j}, line_of_sight_maximum, visible_trees);
        }
    }

    return visible_trees;
}

uint32_t treeScenicScore(const TreeMap &forest, unsigned tree_height, QuadrantLattice tree_coordinates) {

    // find the number of visible trees in the x plus direction_vector
    unsigned visible_x_plus = 0;
    for (unsigned x = tree_coordinates[0] + 1; x <= forest.width; x++) {
        auto it = forest.height_map.find(QuadrantLattice {x, tree_coordinates[1]});
        QuadrantLattice current_coords = it->first;
        unsigned current_height = it->second;

        // sum of the number of visible trees
        // check if tree is visible and if so add to count
        ++visible_x_plus;
        if (tree_height <= current_height) break;
    }

    // find the number of visible trees in the x minus direction_vector
    unsigned visible_x_minus = 0;
    for (unsigned x = tree_coordinates[0] - 1; x >= 1; x--) {
        auto it = forest.height_map.find(QuadrantLattice {x, tree_coordinates[1]});
        QuadrantLattice current_coords = it->first;
        unsigned current_height = it->second;

        // sum of the number of visible trees
        // check if tree is visible and if so add to count
        ++visible_x_minus;
        if (tree_height <= current_height) break;
    }

    // find the number of visible trees in the y plus direction_vector
    unsigned visible_y_plus = 0;
    for (unsigned y = tree_coordinates[1] + 1; y <= forest.depth; y++) {
        auto it = forest.height_map.find(QuadrantLattice {tree_coordinates[0], y});
        QuadrantLattice current_coords = it->first;
        unsigned current_height = it->second;

        // sum of the number of visible trees
        // check if tree is visible and if so add to count
        ++visible_y_plus;
        if (tree_height <= current_height) break;
    }

    // find the number of visible trees in the y minus direction_vector
    unsigned visible_y_minus = 0;
    for (unsigned y = tree_coordinates[1] - 1; y >= 1; y--) {
        auto it = forest.height_map.find(QuadrantLattice {tree_coordinates[0],y});
        QuadrantLattice current_coords = it -> first;
        unsigned current_height = it -> second;

        // sum of the number of visible trees
        // check if tree is visible and if so add to count
        ++visible_y_minus;
        if (tree_height <= current_height) break;
    }

    return visible_x_plus * visible_x_minus * visible_y_plus * visible_y_minus;
}

struct ScenicScore {
    uint32_t score;
    std::pair<uint32_t, uint32_t> coordinates;
};

uint32_t highestScenicScore(const TreeMap &forest) {
    uint32_t highest_scenic_score = 0;

    // how many trees can be seen in each direction_vector
    // run through knots
    for (unsigned j = 2; j <= forest.depth - 1; ++j) {

        for (unsigned i = 2; i <= forest.width - 1; ++i) {

            auto location = forest.height_map.find(QuadrantLattice {i, j});
            unsigned location_height = location -> second;
            QuadrantLattice coords {i, j};

            unsigned current_scenic_score = treeScenicScore(forest, location_height, coords);
            if (current_scenic_score > highest_scenic_score) highest_scenic_score = current_scenic_score;

        }

    }

    return highest_scenic_score;

}


namespace day8 {

static int runTasks(const std::string &location) {

        std::ifstream input(location);
        std::string read_error;
        if (!Common::tryReadFile(input, read_error, 5, 100)) {
            std::cerr << read_error;
            return -1;
        }

        // parse file
        TreeMap forest;
        input >> forest;

        std::unordered_set<QuadrantLattice, QuadrantLattice::Hash> visible_trees = findVisibleTrees(forest);

        std::cout << "The number of trees visible from the forest edge are " << visible_trees.size() << "\n";
        std::cout << "The highest scenic score of all the trees is " << highestScenicScore(forest) << "\n";
        return 0;
}

} // namespace day8

#endif //ADVENTOFCODE2022_TREEMAP_HPP