#ifndef ADVENTOFCODE2022_ROPEBRIDGE_HPP
#define ADVENTOFCODE2022_ROPEBRIDGE_HPP

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

#include "../Common/FileHandling.h"
#include "../Common/Vector.hpp"
#include "Rope.hpp"


namespace Day09 {

// 2-dimensional coordinate vector
using SquareLattice = Geometry::Vector<int32_t, 2>;
using Common::WindowsFileHandler;

std::optional<std::vector<SquareLattice>> parse(std::istream& file, std::stringstream &error) {

    static const std::unordered_map<char, SquareLattice> direction_map = {
            {'R', { 1,  0} },
            {'L', {-1,  0} },
            {'U', { 0,  1} },
            {'D', { 0, -1} }
    };
    static const std::regex pattern(R"(([RLUD]) (\d+))");

    size_t number = 0;
    std::vector<SquareLattice> series_of_motions;
    for (std::string line; std::getline(file, line); ++number) {

        std::smatch matches;
        if (!std::regex_match(line, matches, pattern)) {
            error << "(line " << number << ")" << " Error: Invalid input instruction encountered '" << line << "'\n";
            return std::nullopt;
        }
        const char dir = matches[1].str()[0];
        const int32_t steps = std::stoi(matches[2]);

        auto translation = direction_map.find(dir) -> second;
        std::fill_n(std::back_inserter(series_of_motions), steps, translation);
    }

    return series_of_motions;
}

const int READ_RETRY_LIMIT = 5;
const int READ_WAIT_MILLIS = 100;

static int runTasks(const std::string& location, const unsigned knots) {
    std::stringstream error;
    Common::WindowsFileHandler fileHandler(location);


    // TODO: METH 1
    std::ifstream file(location);
    auto file_stream1 = Common::tryReadFile(file, error);
    if (not file_stream1) {
        std::cerr << error.str();
        return -1;
    }
    auto series_of_motions = parse(file, error);
    if (not series_of_motions) {
        std::cerr << error.str();
        return -1;
    }

    /* TODO: METH 2
    auto file_stream2 = fileHandler.openFileRetryWithExponentialBackoff(error);
    if (not file_stream2.has_value()) {
        std::cerr << error.str();
    }
    auto series_of_motions2 = parse(file_stream2.value(), error);
    if (not series_of_motions2.has_value()) {
        std::cerr << error.str();
    }
    */

    Rope rope_bridge({0, 0}, knots);
    // unique points that the head and tail of the rope visit respectively MOVE
    std::unordered_set<SquareLattice, SquareLattice::Hash> unique_tail_positions;

    for (auto motion : *series_of_motions) {
        rope_bridge.displace(motion, error);
        if (!error.str().empty()) {
            std::cerr << error.str();
            return -1;
        }
        unique_tail_positions.emplace(rope_bridge.knots[knots - 1]);
    }

    std::cout << unique_tail_positions.size();
    return 0;
}

} // namespace day09

#endif //ADVENTOFCODE2022_ROPEBRIDGE_HPP