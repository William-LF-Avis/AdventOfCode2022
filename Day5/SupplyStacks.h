#ifndef ADVENTOFCODE2022_DAY5_H
#define ADVENTOFCODE2022_DAY5_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>
#include <cmath>
#include <thread>
#include <stdexcept>
#include "../Common/FileHandling.h"

namespace day5 {


	class StackConfigurationReader {
	public:
		// Read and parse stack configuration file to initialise SupplyStacks
		static std::vector<std::vector<char>> readConfiguration(const std::string &location, std::string &error) {
			std::vector<std::vector<char>> stacks;

			std::ifstream file(location);
			if (!Common::tryReadFile(file, error))
				return stacks;

			unsigned int line_no = 1;
			std::string line;
			while (getline(file, line)) {
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

		/// Construct SupplyStacks using a template StackReader
		explicit SupplyStacks(const std::string &location, std::string &read_error) {
			supply_stacks = StackReader::readConfiguration(location, read_error);
		}

		/// Check if moving n crates from stack A to stack B is an invalid operation
		[[nodiscard]] bool isMoveInvalid(const size_t number, const size_t from_stack, const size_t to_stack, std::string &move_error) {
			if (from_stack < 1 or from_stack > supply_stacks.size()) {
				move_error = "Stack " + std::to_string(from_stack) + " is out of range \n";
				return true;
			} else if (to_stack < 1 or to_stack > supply_stacks.size()) {
				move_error = "Stack " + std::to_string(to_stack)   + " is out of range \n";
				return true;
			} else if (number > supply_stacks[from_stack - 1].size()) {
				move_error = "Cannot move " + std::to_string(number)
						     + " crates from stack " + std::to_string(from_stack)
				             + " because it only contains " + std::to_string(supply_stacks[from_stack - 1].size()) + " crates \n";
				return true;
			}
			return false;
		}

		// Task 1
		void moveCrates(const unsigned int number, const size_t from_stack, const size_t to_stack) {
			// references to the internal stacks being modified
			std::vector<char> &source = supply_stacks[from_stack - 1];
			std::vector<char> &target = supply_stacks[to_stack - 1];

			// perform move from back to front
			auto end  = std::end(source);
			auto iter = std::prev(end, number);
			std::reverse_copy(iter, end, std::back_inserter(target));
			source.erase(iter, end);
		}

		// Task 2
		void moveCratesReverse(const unsigned int number, const size_t from_stack, const size_t to_stack) {
			// references to the internal stacks being modified
			std::vector<char> &source = supply_stacks[from_stack - 1];
			std::vector<char> &target = supply_stacks[to_stack - 1];

			// perform move from front to back
			auto end  = std::end(source);
			auto iter = std::prev(end, number);
			std::copy(iter, end, std::back_inserter(target));
			source.erase(iter, end);
		}

		// Return SupplyStacks formatted as a string in the style of the configuration file
		std::string formatStackAsString() {
			std::string s;
			for (auto row = findLargestStack(); row != 0; --row) {
				for (auto &stack: supply_stacks)
					s += ( (stack.size() >= row) ? ("[" + std::string(1, stack[row - 1]) + "] ") : "    " );
				s += '\n';
			}
			for (auto num = 1; num != supply_stacks.size() + 1; ++num)
				s += " " + std::to_string(num) + "  ";
			return s + '\n';
		}

		// Return the crates at the top of each stack formatted as a string
		std::string formatTopOfStackAsString() {
			std::string s;
			for (auto &stack : supply_stacks)
				s += ( (stack.empty()) ? ' '  :  stack[stack.size()-1]);
			return s + '\n';
		}

	};


	int runTasks(const std::string &configuration, const std::string &instructions) {

		// initialise SupplyStacks with an implementation defined StackReader
		std::string read_error;
		SupplyStacks<StackConfigurationReader> stacks_part1(configuration, read_error), stacks_part2 = stacks_part1;
		if (!read_error.empty()) {
			std::cerr << read_error;
			return -1;
		}
		std::cout << "The initial stack configuration is: \n \n" << stacks_part1.formatStackAsString() << '\n';

		std::ifstream file(instructions);
		if (!Common::tryReadFile(file, read_error))
			return -1;

		int line_no = 1;
		for (std::string line; getline(file, line);) {

            // parse instructions using regex
			static std::regex pattern(R"(move (\d+) from (\d+) to (\d+))");
			std::smatch matches;
			if (!std::regex_match(line, matches, pattern)) {
				std::cerr << "Line " << line_no << ": Invalid instruction encountered " << "'" << line << "'\n";
				return -1;
			}
			int num  = std::stoi(matches[1]);
			int from = std::stoi(matches[2]);
			int to   = std::stoi(matches[3]);

			// check that move is valid
			// (parts 1 and 2 are equivalent since stack sizes remain the same under the move operations)
			std::string move_error;
			if (stacks_part1.isMoveInvalid(num, from, to, move_error)) {
				std::cerr << "Line " << line_no << " : " << move_error << '\n';
				return -1;
			}
			// perform move instruction
			stacks_part1.moveCrates(num, from, to);
			stacks_part2.moveCratesReverse(num, from, to);
			++line_no;
		}
		file.close(); // safely close file
		std::cout << "Task 1: After " << line_no << " moves the resulting crates at the top of each stack are "
			  << stacks_part1.formatTopOfStackAsString() << '\n';
		std::cout << "Task 2: After " << line_no << " moves the resulting crates at the top of each stack are "
	          << stacks_part2.formatTopOfStackAsString() << '\n';
		return 0;
	}
}


#endif //ADVENTOFCODE2022_DAY5_H