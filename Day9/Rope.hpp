#ifndef ADVENTOFCODE2022_ROPE_HPP
#define ADVENTOFCODE2022_ROPE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <regex>
#include <cmath>
#include <variant>
#include <optional>

#include "../Common/Vector.hpp"

namespace Day09 {

/// 2-dimensional vector space which spans the integers
using SquareLattice = Geometry::Vector<int32_t, 2>;

class Rope {

public:
    SquareLattice head;
    std::vector<SquareLattice> knots;

    // all knots start at the same coordinate
    explicit Rope(const std::array<int32_t, 2>& start, const size_t number_of_knots)
            : head(start), knots(number_of_knots, SquareLattice(start)) {}

    // move head of the rope one displace in one direction_vector and UpdatePositions the trailing knots
    void displace(const SquareLattice translation, std::stringstream& error) {

        if (translation.norm() != 1) {
            error << "Invalid movement! The translation is not a basis vector of unit size.";
            return;
        }

        head.add(translation);

        SquareLattice lead = head;
        for (SquareLattice& knot : knots) {

            SquareLattice delta = lead - knot;
            if (delta.norm() < 2) {
                return;
            }
            auto it = displacement_map.find(delta);
            if (it == displacement_map.end()) {
                error << "Rope has snapped! Invalid delta movement.";
                return;
            }
            //knot += it -> second;
            knot.add(it -> second);

            lead = knot;
        }
    }

private:
    const std::unordered_map<SquareLattice, SquareLattice, SquareLattice::Hash> displacement_map = {
            {{ 0, -2}, { 0, -1} },
            {{-1, -2}, {-1, -1} },
            {{-2, -2}, {-1, -1} },
            {{-2, -1}, {-1, -1} },
            {{-2,  0}, {-1,  0} },
            {{-2,  1}, {-1,  1} },
            {{-2,  2}, {-1,  1} },
            {{-1,  2}, {-1,  1} },
            {{ 0,  2}, { 0,  1} },
            {{ 1,  2}, { 1,  1} },
            {{ 2,  2}, { 1,  1} },
            {{ 2,  1}, { 1,  1} },
            {{ 2,  0}, { 1,  0} },
            {{ 2, -1}, { 1, -1} },
            {{ 2, -2}, { 1, -1} },
            {{ 1, -2}, { 1, -1} },
    };
};

}

#endif //ADVENTOFCODE2022_ROPE_HPP
