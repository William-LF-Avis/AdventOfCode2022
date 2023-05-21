#ifndef ADVENTOFCODE2022_DAY3_H
#define ADVENTOFCODE2022_DAY3_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class Day3 {
private:

	static std::vector<char> FindSharedCharacters(const std::string &str1, const std::string &str2) {
		std::vector<char> sharedChars;
		for (char sym1 : str1) {
			for (char sym2 : str2) {
				if (sym1 == sym2) {
					bool found = false;
					for (char element : sharedChars) {
						if (sym1 == element)
							found = true;
					}
					if (!found)
						sharedChars.push_back(sym1);
				}
			}
		}
		return sharedChars;
	}

	/*
	 *
	 */
	static int FindItemPriority(const char &item) {
		const std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		int i = 1;
		for (char alpha : alphabet) {
			if (item == alpha)
				return i;
			++i;
		}
		return -1;
	}



public:
	/*
	 *
	 */
	static int Task1(const std::string &location) {

		std::cout << std::endl
		          << "Day 3 - Rucksack Reorganisation - Task 1" << std::endl
		          << "----------------------------------------" << std::endl;

		unsigned int priority_sum = 0, rucksack_num = 1;
		std::string line;
		std::ifstream in(location);
		while (getline(in, line)) {
			std::string half1, half2;
			if (line.size() % 2 == 0) {
				half1 = line.substr(0, (line.size()/2));
				half2 = line.substr((line.size()/2));
			}
			else {
				std::cout << "Input Error: Rucksack " << rucksack_num
				          << " doesn't have an equal number of items in each compartment." << std::endl
				          << "             Expected an even number of items." << std::endl;
				return -1;  // exit
			}

			std::vector<char> shared = FindSharedCharacters(half1, half2);
			if (shared.size() == 1) {
				char duplicate_item = shared[0];
				if (std::isalpha(duplicate_item))
					priority_sum += FindItemPriority(duplicate_item);
				else {
					std::cout << "Input Error : Rucksack " << rucksack_num
					          << " contains a duplicate item which isn't labeled correctly." << std::endl
					          << "              Expected alphabetical labels only... this item cannot be prioritised."
					          << std::endl;
					return -1; //exit
				}
			}

			else if (shared.empty()) {
				std::cout << "Input Error: Rucksack " << rucksack_num
				          << " doesn't contain a duplicate item in each compartment." << std::endl
				          << "             Expected one duplicate in each compartment... this rucksack doesn't need reorganised."
				          << std::endl;
				priority_sum += 0;
			}
			else {
				std::cout << "Input Error: Rucksack " << rucksack_num << " contains " << shared.size()
				          << " duplicate items in each compartment." << std::endl
				          << "             Expected only a single duplicate per rucksack... what was the elf playing at."
				          << std::endl;
				return -1;  // exit
			}
			++rucksack_num;
		}
		std::cout << "The sum of the priorities is " << priority_sum << "." << std::endl;
		return 0;
	}

	/*
	 *
	 */
	static int Task2(const std::string &location) {

		std::cout << std::endl
		          << "Day 3 - Rucksack Reorganisation - Task 2" << std::endl
		          << "----------------------------------------" << std::endl;

		const std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		unsigned int priority_common_sum = 0, rucksack_num = 1, count = 0;
		std::string tmp_str1, tmp_str2;   //
		std::vector<char> shared;
		std::string line;
		std::ifstream in(location);
		while (getline(in, line)) {
			//std::cout << rucksack_num << " : ";
			++count;
			if (count == 1)
				tmp_str1 = line;
			else if (count == 2) {
				tmp_str2 = line;
				shared = FindSharedCharacters(tmp_str1, tmp_str2);
				tmp_str1.clear();
				for (auto chr : shared) {
					tmp_str1.push_back(chr);
				}
				//std::cout << tmp_str1 << " : ";
			}
			else {
				tmp_str2 = line;
				shared = FindSharedCharacters(tmp_str1, tmp_str2);
				//for (auto tt : shared)
					//std::cout << tt;
				//std::cout << std::endl;
				char duplicate_item = shared[0];
				priority_common_sum += FindItemPriority(duplicate_item);
				count = 0;
			}
			++rucksack_num;
		}
		std::cout << "The sum of the priorities is " << priority_common_sum << "." << std::endl;
		return 0;
	}
};

#endif //ADVENTOFCODE2022_DAY3_H