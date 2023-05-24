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



class Day6 {

private:
	static bool isUnique(const std::deque<char> &container) {

		for (auto it1 = container.begin(); it1 != container.end(); ++it1) {
			for (auto it2 = it1 + 1; it2 != container.end(); ++it2)
					if (*it2 == *it1)
						return false;
		}
		return true;
	}


public:
	static unsigned runTasks(const std::string &taskInput) {

		std::cout << "\n Day 5 - Supply Stacks - Task 1 and 2 \n"
		          << "--------------------------------------- \n";

		std::ifstream in(taskInput);
		std::string line;
		while (getline(in, line)){
			unsigned N = 13;
			std::deque<char> msg;
			for (auto it = line.begin(); it != line.begin() + N; ++it)
				msg.push_back(*it);

			//std::deque<char> msg {line[0], line[1], line[2]};
			for (auto it = line.begin() + N; it != line.end(); ++it) {

				msg.push_back(*it);

				if (isUnique(msg))
					std::cout << N << '\n';

				msg.pop_front();

				++N;
			}

		}

		return 0;
	}
};


#endif //ADVENTOFCODE2022_TUNINGTROUBLE_H
