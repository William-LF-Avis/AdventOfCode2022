#ifndef ADVENTOFCODE2022_TUNINGTROUBLE_H
#define ADVENTOFCODE2022_TUNINGTROUBLE_H

#include <iostream>
#include <utility>
#include <vector>
#include <deque>    // double-ended queue
#include <string>
#include <fstream>
#include <regex>
#include <cmath>


namespace day6 {

	static bool containsUniqueElements(const std::deque<char> &set) {
		for (auto it1 = set.begin(); it1 != set.end(); ++it1) {
			for (auto it2 = it1 + 1; it2 != set.end(); ++it2)
					if (*it2 == *it1)
						return false;
		}
		return true;
	}

	static unsigned runTask(unsigned int distinct_chars, const std::string &task_input) {

		std::ifstream in(task_input);
		std::string line;
		while (getline(in, line)){
			std::deque<char> message;
			for (auto it = line.begin(); it != line.begin() + distinct_chars; ++it)
				message.push_back(*it);

			for (auto it = line.begin() + distinct_chars; it != line.end(); ++it) {
				if (containsUniqueElements(message)) {
                    std::cout << distinct_chars << "\n";
                    return 0;
                }
				message.push_back(*it);
				message.pop_front();
				++distinct_chars;
			}
		}
        std::cout << "No distinct characters exist";
		return 0;
	}
};


#endif //ADVENTOFCODE2022_TUNINGTROUBLE_H