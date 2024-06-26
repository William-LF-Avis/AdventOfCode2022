// have a live display that updates every 1 second to show the latest move
// have the window be based around the head of the rope (3rd person camera)
// have a pause button
// have a replay mode
// have a smaller post-view of the last 3 moves

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <variant>
#include <thread>

#include "../Common/ProjectPath.h"
#include "RopeBridge.hpp"

using namespace Day09;

auto displayCharacter(const Rope& rope, int i, int j) {
    return [rope, i, j]() -> std::variant<char, int> {

        if (i == 0 and j == 0) return static_cast<char>('O');
        if (i == rope.head[0] and j == rope.head[1]) return static_cast<char>('H');

        // Check if a knot exists at coordinate (i, j)
        auto it = std::find_if(
                rope.knots.begin(),
                rope.knots.end(),
                [i, j](const auto& knot){return knot[0] == i and knot[1] == j;}
        );
        if (it != rope.knots.end()) {
            return static_cast<int>(std::distance(rope.knots.begin(), it) + 1);
        }

        if (i == 0 or j == 0) return static_cast<char>('*');

        return static_cast<char>('.');
    };
}

void displayKnots(Rope& rope) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::stringstream output;
    for (int j = rope.head[1] + 10; j >= rope.head[1] - 10; --j ) {

        for (int i = rope.head[0] - 10; i <= rope.head[0] + 10; ++i) {
            auto findChar = displayCharacter(rope, i, j);
            // Use std::visit to handle the different types in std::variant
            std::visit([&output](auto&& arg) {
                output << ' ' << arg;
            }, findChar());
        }
        output << "\n";
    }
    std::cout << output.str();
    std::cout.flush();
}

int main() {

    std::stringstream error;

    const std::string& location = Common::project_directory + R"(TaskInputs\day9.txt)";
    const int knots = 9;

    Common::WindowsFileHandler fileHandler(location);
    if (!fileHandler.openFileRetryWithExponentialBackoff(error)) {
        std::cerr << error.str();
        return -1;
    }

    auto series_of_motions = parse(fileHandler, error);
    if (!series_of_motions.has_value()) {
        std::cerr << error.str();
        return -1;
    }

    Rope rope_bridge({0, 0}, knots);

    for (auto motion : *series_of_motions) {
        for (; motion.steps > 0; --motion.steps) {

            rope_bridge.displace(motion.direction_vector, error);
            if (not error.str().empty()) {
                std::cerr << error.str();
                return -1;
            }
            displayKnots(rope_bridge);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    return 0;
}