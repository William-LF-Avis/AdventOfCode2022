#ifndef ADVENTOFCODE2022_DAY5_H
#define ADVENTOFCODE2022_DAY5_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <regex>
#include <cmath>
#include <thread>
#include <stdexcept>

class StackConfigurationReader {
private:
	static const unsigned int READ_ATTEMPTS = 10;

public:
	// Read and parse stack configuration file to initialise SupplyStacks
	static std::vector<std::vector<char>> readConfiguration(const std::string &location, std::string &error) {
		std::vector<std::vector<char>> stacks;

		std::ifstream in(location);
		// try and read the file
		unsigned int tries = 0;
		while (!in) {
			if (tries >= READ_ATTEMPTS) {
				error = "Tries have exceeded the maximum number of " + std::to_string(READ_ATTEMPTS) + " attempts \n";
				error += "Could not read the stack configuration file: " + location + "\n";
				return stacks;   // empty stack
			}
			// if unsuccessful wait for 1 second before retrying
			std::this_thread::sleep_for(std::chrono::seconds(1));
			++tries;
		}

		unsigned int line_no = 1;
		std::string line;
		while (getline(in, line)) {
			// parse line by every 4th char starting at the 2nd char
			for (size_t i = 1; i < line.length(); i += 4) {

				// initialise new stack
				if (stacks.size() <= i / 4)
					stacks.emplace_back();

				// if char is alphabetical add element to current stack
				if (std::isalpha(line[i]))
					stacks[std::floor(i / 4)].emplace_back(line[i]);
				// ignore whitespace or digits, otherwise return char error
				else if (!std::isspace(line[i]) and !std::isdigit(line[i])) {
					error  = "Line " + std::to_string(line_no) + ": Invalid character '" + line[i] + "' encountered at index " + std::to_string(i) + "\n";
					error += "Could not parse the stack configuration file: " + location + "\n";
					return std::vector<std::vector<char>> {};   // empty stack
				}
			}
			++line_no;
		}

		// reverse stack since the elements have been read top to bottom
		for (auto &stack : stacks)
			std::reverse(stack.begin(), stack.end());

		return stacks;
	}
};

template<typename StackReader>
class SupplyStacks {
private:
	// SupplyStacks data structure
	std::vector<std::vector<char>> supply_stacks;

	// Return the number of crates in the largest stack
	size_t findLargestStack() {
		size_t largestStackSize = 0;
		for (auto &stack: supply_stacks) {
			if (stack.size() > largestStackSize)
				largestStackSize = stack.size();
		}
		return largestStackSize;
	}

public:

	// Construct SupplyStacks using a template StackReader
	explicit SupplyStacks(const std::string &location, std::string &read_error) {
		supply_stacks = StackReader::readConfiguration(location, read_error);
	}

	// Check if moving n crates from stack A to stack B is an invalid operation
	bool isMoveInvalid(const size_t number, const size_t from_stack, const size_t to_stack, std::string &move_error) {
		if (from_stack < 1 or from_stack > supply_stacks.size()) {
			move_error = "Stack " + std::to_string(from_stack) + " is out of range" + '\n';
			return true;
		} else if (to_stack < 1 or to_stack > supply_stacks.size()) {
			move_error = "Stack " + std::to_string(to_stack) + " is out of range" + '\n';
			return true;
		} else if (number > supply_stacks[from_stack - 1].size()) {
			move_error = "Cannot move " + std::to_string(number) + " crates from stack " + std::to_string(from_stack)
			             + " because it only contains " + std::to_string(supply_stacks[from_stack - 1].size()) + " crates"
			             + '\n';
			return true;
		}
		return false;
	}

	// Part 1
	void moveCrates(const unsigned int number, const size_t from_stack, const size_t to_stack) {
		// references to the internal stacks being modified
		std::vector<char> &source = supply_stacks[from_stack - 1];
		std::vector<char> &target = supply_stacks[to_stack - 1];

		// perform move end to front
		auto end  = std::end(source);
		auto iter = std::prev(end, number);
		std::reverse_copy(iter, end, std::back_inserter(target));
		source.erase(iter, end);
	}

	// Part 2
	void moveCratesReverse(const unsigned int number, const size_t from_stack, const size_t to_stack) {
		// references to the internal stacks being modified
		std::vector<char> &source = supply_stacks[from_stack - 1];
		std::vector<char> &target = supply_stacks[to_stack - 1];

		// perform move front to end
		auto end  = std::end(source);
		auto iter = std::prev(end, number);
		std::copy(iter, end, std::back_inserter(target));
		source.erase(iter, end);
	}

	// Return SupplyStacks formatted as a string in the style of the configuration file
	std::string StacksToString() {
		std::string s;
		for (auto row = findLargestStack(); row != 0; --row) {
			for (auto &stack: supply_stacks)
				s += ( (stack.size() >= row) ? ("[" + std::string(1,stack[row - 1]) + "] ") : "    " );
			s += '\n';
		}
		for (auto num = 1; num != supply_stacks.size() + 1; ++num)
			s += " " + std::to_string(num) + "  ";
		return s + '\n';
	}

	// Return the crates at the top of each stack formatted as a string
	std::string TopCratesToString() {
		std::string s;
		for (auto &stack : supply_stacks)
			s += ( (stack.empty()) ? ' '  :  stack[stack.size()-1]);
		return s + '\n';
	}

};


namespace Day5 {

	int runTasks(const std::string &configuration, const std::string &instructions) {

		// initialise SupplyStacks with an implementation defined StackReader
		std::string read_error;
		SupplyStacks<StackConfigurationReader> stacks_part1(configuration, read_error), stacks_part2 = stacks_part1;
		if (!read_error.empty()) {
			std::cerr << read_error;
			return -1;
		}
		std::cout << "The initial stack configuration is: \n \n" << stacks_part1.StacksToString() << '\n';

		std::ifstream in(instructions);
		// try and read the file
		unsigned int tries = 0;
		while (!in) {
			// if tries exceeds 10 attempts return read error
			if (tries >= 10) {
				std::cerr << "Tries have exceeded the maximum number of " << 10 << " attempts \n";
				std::cerr << "Could not read the instructions file: " + instructions + "\n";
				return -1;
			}
			// if unsuccessful wait for 1 second before retrying
			std::this_thread::sleep_for(std::chrono::seconds(1));
			++tries;
		}

		int line_no = 1;
		std::string line;
		while (getline(in, line)) {
			// parse instructions using regex
			std::regex regex_move(R"(move (\d+) from (\d+) to (\d+))");
			std::smatch match;
			if (!std::regex_match(line, match, regex_move)) {
				std::cerr << "Line " << line_no << ": Invalid instruction encountered " << "'" << line << "'\n";
				return -1;
			}
			int num  = std::stoi(match[1].str());
			int from = std::stoi(match[2].str());
			int to   = std::stoi(match[3].str());

			// check that move is valid
			// (parts 1 and 2 are equivalent since stack sizes remain the same under the move operations)
			std::string move_error;
			if (stacks_part1.isMoveInvalid(num, from, to, move_error)) {
				std::cerr << "Line " << line_no << " : " << move_error << '\n';;
				return -1;
			}
			// perform move instruction
			stacks_part1.moveCrates(num, from, to);
			stacks_part2.moveCratesReverse(num, from, to);
			++line_no;
		}
		in.close(); // safely close file
		std::cout << "After " << line_no << " moves task 1 configures the stack into: \n"
			<< stacks_part1.TopCratesToString() << '\n';
		std::cout << "After " << line_no << " moves task 2 configures the stack into: \n"
	          << stacks_part2.TopCratesToString() << '\n';
		return 0;
	}
}


#endif //ADVENTOFCODE2022_DAY5_H