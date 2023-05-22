#ifndef ADVENTOFCODE2022_DAY5_H
#define ADVENTOFCODE2022_DAY5_H

#include <iostream>
#include <utility>
#include <vector>
#include <deque>    // double-ended queue
#include <string>
#include <fstream>
#include <regex>
#include <cmath>

class SupplyStacks {
private:

	std::vector<std::deque<char>> supplyStacks;

	// Return the number of crates in the largest stack
	size_t findLargestStack() {
		size_t largestStackSize = 0;
		for (auto &stack: supplyStacks) {
			if (stack.size() > largestStackSize)
				largestStackSize = stack.size();
		}
		return largestStackSize;
	}

	void readInput(const std::string &location) {
		std::string line;
		std::ifstream in(location);
		while (getline(in, line)) {
			for (size_t i = 1; i < line.length(); i += 4) {

				if (supplyStacks.size() <= i / 4)
					supplyStacks.emplace_back();

				if (isalpha(line[i]))
					supplyStacks[std::floor(i / 4)].emplace_front(line[i]);

			}
		}
	}

public:

	explicit SupplyStacks(const std::string &location) {
		readInput(location);
	}

	/*
	 * explicit SupplyStacks(const SupplyStackParser &parser (&location)) {
	 *      supplyStack = parser.Parse();
	 * }
	 */

	// Check if moving x crates from stack A to stack B is a valid operation
	bool isMoveValid(const size_t &numberOfCrates, const size_t &fromStack, const size_t &toStack) {
		if (numberOfCrates > supplyStacks[fromStack - 1].size()) {
			std::cout << "Error: Cannot move " << numberOfCrates << " crates from stack " << fromStack
			          << " because it only contains " << supplyStacks[fromStack - 1].size() << " crates"
			          << '\n';
			return false;
		} else if (fromStack < 1 or fromStack > supplyStacks.size()) {
			std::cout << "Error: Stack " + std::to_string(fromStack) + " is out of range" << '\n';
			return false;
		} else if (toStack < 1 or toStack > supplyStacks.size()) {
			std::cout << "Error: Stack " + std::to_string(toStack) + " is out of range" << '\n';
			return false;
		}
		return true;
	}

	void moveCratesIndividually(const unsigned int &numberOfCrates, const size_t &fromStack, const size_t &toStack) {
		// references to the internal stacks being modified
		std::deque<char> &source = supplyStacks[fromStack - 1];
		std::deque<char> &target = supplyStacks[toStack - 1];
		// perform move end to front
		auto end  = std::end(source);
		auto iter = std::prev(end, numberOfCrates);
		std::reverse_copy(iter, end, std::back_inserter(target));
		source.erase(iter, end);
	}

	void moveCratesByBlock(const unsigned int &numberOfCrates, const size_t &fromStack, const size_t &toStack) {
		// references to the internal stacks being modified
		std::deque<char> &source = supplyStacks[fromStack - 1];
		std::deque<char> &target = supplyStacks[toStack - 1];
		// perform move front to end
		auto end  = std::end(source);
		auto iter = std::prev(end, numberOfCrates);
		std::copy(iter, end, std::back_inserter(target));
		source.erase(iter, end);
	}

	// Return entire Supply Stack formatted as a string
	std::string StackToString() {
		std::string s;
		for (auto row = findLargestStack(); row != 0; --row) {
			for (auto &stack: supplyStacks)
				s += ( (stack.size() >= row) ? ("[" + std::string(1,stack[row - 1]) + "] ") : "    " );
			s += '\n';
		}
		for (auto num = 1; num != supplyStacks.size() + 1; ++num)
			s += " " + std::to_string(num) + "  ";
		return s + '\n';
	}

	// Return top of the Supply Stack formatted as a string
	std::string TopStackToString() {
		std::string s;
		for (auto &stack : supplyStacks)
			s += ( (stack.empty()) ? ' '  :  stack[stack.size()-1]);
		return s + '\n';
	}

};


class Day5 {

public:

	static int runTasks(const std::string &start, const std::string &instructions) {

		SupplyStacks supplyStack1(start), supplyStack2 = supplyStack1;

		std::cout << "\n Day 5 - Supply Stacks - Task 1 and 2 \n"
		          << "--------------------------------------- \n";

		std::cout << supplyStack1.StackToString() << '\n';

		size_t cnt = 0, from = 0, to = 0;
		std::string line;
		std::ifstream in(instructions);
		//std::regex regx("[-,]");
		int moveNo = 1;
		while (getline(in, line)) {
			sscanf_s(line.c_str(), "move %d from %d to %d", &cnt, &from, &to);

			if (supplyStack1.isMoveValid(cnt, from, to))
				supplyStack1.moveCratesIndividually(cnt, from, to);
			else
				return -1;

			if (supplyStack2.isMoveValid(cnt, from, to))
				supplyStack2.moveCratesByBlock(cnt, from, to);
			else
				return -2;

			++moveNo;
		}
		std::cout << "After " << moveNo << " moves task 1 configures the stack into: \n"
		          << supplyStack1.TopStackToString() << '\n';
		std::cout << "After " << moveNo << " moves task 2 configures the stack into: \n"
		          << supplyStack2.TopStackToString() << '\n';
		return 0;
	}

};


#endif //ADVENTOFCODE2022_DAY5_H