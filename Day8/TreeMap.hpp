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

/*
using QuadrantLattice = GridVector;
const QuadrantLattice a {0, 0};;
*/

// 2-dimensional vector space which spans the positive integers (single quadrant)
using GridVector = Geometry::Vector<uint32_t, 2>;

const GridVector Zero_Vector {0, 0};
const GridVector Basis_Vector_X {1, 0};
const GridVector Basis_Vector_Y {0, 1};

struct TreeMap {
    // maps tree height for coordinates (x, y)
    std::unordered_map<GridVector, uint32_t, GridVector::Hash> height_map;
    uint32_t width = 0, depth = 0;
};

// Text parser
std::istream &operator>>(std::istream &file, TreeMap &forest) {

    GridVector coordinate = Zero_Vector;
    for (std::string line; std::getline(file, line); coordinate.add(Basis_Vector_Y)) {
        coordinate[0] = 0;
        for (auto c: line) {
            forest.height_map.emplace(coordinate, c); // map needs to be unordered to map on equality (uniqueness)
            coordinate.add(Basis_Vector_X);
        }
        // TODO: explicit cast coordinate from char to int
    }
    forest.width = coordinate[0] - 1;  // not number of elements
    forest.depth = coordinate[1] - 1;  // minus one because at the end of the loop an extra unit vector is added

    return file;
}

void isEdgeVisible(
        const TreeMap &forest,
        const GridVector& coordinate,
        int32_t &view_max,
        std::unordered_set<GridVector, GridVector::Hash> &visible_trees) {

    std::cout << "x: " << coordinate[0] << " and y:" << coordinate[1] <<  std::endl;

    // TODO: No error handling -> potential unvalidated operation
    auto it = forest.height_map.find(coordinate);
    if (it == forest.height_map.end()) {
        std::cout << "We dead with this one";
        //return;
    }

    const uint32_t height = it -> second;
    if (view_max < height) {
        visible_trees.insert(coordinate);
        view_max = height;
    }
}

// Cross-section scan
std::unordered_set<GridVector, GridVector::Hash> findVisibleTrees(const TreeMap &forest) {
    std::unordered_set<GridVector, GridVector::Hash> visible_trees;

    // TODO: Investigate synchronous cross grip compare
    std::cout << Basis_Vector_X.scale(forest.width)[0] << std::endl;
    std::cout << Basis_Vector_Y.scale(forest.depth)[1] << std::endl;

    /* count left-to-right and right-to-left
    for (GridVector coordinate = Zero_Vector; coordinate != Basis_Vector_Y.scale(forest.depth); coordinate.add(Basis_Vector_Y)) {

        uint32_t view_max = 0;
        GridVector update = coordinate;

        // Count trees visible from left
        while (view_max < 57 and update[0] <= forest.width) {
            isEdgeVisible(forest, update, view_max, visible_trees);
            update.add(Basis_Vector_X);
        }

        //std::cout << "x: " << update[0] << " and y:" << update[1] << std::endl;

        view_max = 0;
        update = coordinate + Basis_Vector_X.scale(forest.width);
        // count trees visible from right
        while (view_max < 57 and update[0] >= 1) {
            isEdgeVisible(forest, update, view_max, visible_trees);
            --update[0];  // TODO: buffer overflow if not cautious
        }

        //std::cout << "x: " << update[0] << " and y:" << update[1] << std::endl;

    }*/


    // count left-to right and right-to-left
    for (uint32_t j = 1; j != forest.depth; ++j) {
        int32_t line_of_sight_maximum = -1;
        // l-r
        for (uint32_t i = 0; i <= forest.width; ++i) {
            isEdgeVisible(forest, GridVector{i, j}, line_of_sight_maximum, visible_trees);
        }
        line_of_sight_maximum = -1;
        // r-l
        for (uint32_t i = forest.width; i > 1; --i) {
            isEdgeVisible(forest, GridVector{i, j}, line_of_sight_maximum, visible_trees);
        }
    }



    // count bottom-to-top and top-to-bottom
    for (uint32_t i = 0; i != forest.width; ++i) {
        int32_t line_of_sight_maximum = -1;
        // b-t
        for (uint32_t j = 0; j <= forest.depth; ++j) {
            isEdgeVisible(forest, {i, j}, line_of_sight_maximum, visible_trees);
        }
        line_of_sight_maximum = -1;
        //t-b
        for (uint32_t j = forest.depth; j > 1; --j) {
            isEdgeVisible(forest, {i, j}, line_of_sight_maximum, visible_trees);
        }

        return visible_trees;
    }
}

uint32_t treeScenicScore(const TreeMap &forest, unsigned tree_height, GridVector tree_coordinates) {

    // find the number of visible trees in the x plus direction_vector
    unsigned visible_x_plus = 0;
    for (unsigned x = tree_coordinates[0] + 1; x <= forest.width; x++) {
        auto it = forest.height_map.find(GridVector {x, tree_coordinates[1]});
        GridVector current_coords = it->first;
        unsigned current_height = it->second;

        // sum of the number of visible trees
        // check if tree is visible and if so add to count
        ++visible_x_plus;
        if (tree_height <= current_height) break;
    }

    // find the number of visible trees in the x minus direction_vector
    unsigned visible_x_minus = 0;
    for (unsigned x = tree_coordinates[0] - 1; x >= 1; x--) {
        auto it = forest.height_map.find(GridVector {x, tree_coordinates[1]});
        GridVector current_coords = it->first;
        unsigned current_height = it->second;

        // sum of the number of visible trees
        // check if tree is visible and if so add to count
        ++visible_x_minus;
        if (tree_height <= current_height) break;
    }

    // find the number of visible trees in the y plus direction_vector
    unsigned visible_y_plus = 0;
    for (unsigned y = tree_coordinates[1] + 1; y <= forest.depth; y++) {
        auto it = forest.height_map.find(GridVector {tree_coordinates[0], y});
        GridVector current_coords = it->first;
        unsigned current_height = it->second;

        // sum of the number of visible trees
        // check if tree is visible and if so add to count
        ++visible_y_plus;
        if (tree_height <= current_height) break;
    }

    // find the number of visible trees in the y minus direction_vector
    unsigned visible_y_minus = 0;
    for (unsigned y = tree_coordinates[1] - 1; y >= 1; y--) {
        auto it = forest.height_map.find(GridVector {tree_coordinates[0], y});
        GridVector current_coords = it -> first;
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

            auto location = forest.height_map.find(GridVector {i, j});
            unsigned location_height = location -> second;
            GridVector coords {i, j};

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

        std::unordered_set<GridVector, GridVector::Hash> visible_trees = findVisibleTrees(forest);

        std::cout << "The number of trees visible from the forest edge are " << visible_trees.size() << "\n";
        //std::cout << "The highest scenic score of all the trees is " << highestScenicScore(forest) << "\n";
        return 0;
}

} // namespace day8

#endif //ADVENTOFCODE2022_TREEMAP_HPP