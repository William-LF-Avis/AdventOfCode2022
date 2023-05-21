#ifndef ADVENTOFCODE2022_DAY4_H
#define ADVENTOFCODE2022_DAY4_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <regex>

class Day4 {
private:
	// size-ordered integer pair
	class OrderedPair {
	private:
		int small_element = 0, large_element = 0;
	public:
		void setPair(const int &val1, const int &val2) {
			if (val2 < val1) {
				small_element = val1;
				large_element = val1;
			}
			else {
				small_element = val1;
				large_element = val2;
			}
		}
		[[nodiscard]] int getSmallInt() const {
			return small_element;
		}
		[[nodiscard]] int getLargeInt() const {
			return large_element;
		}
		[[nodiscard]] bool isEqual() const {
			if (small_element == large_element)
				return true;
			else
				return false;
		}
	};

	// determines if one pair fully encapsulates another (not order dependent, if either fully contains the other it will return true)
	static bool ContainsPoints(const OrderedPair &pair1, const OrderedPair &pair2)  {
		if (pair1.getSmallInt() < pair2.getSmallInt()) {
			if (pair1.getLargeInt() < pair2.getLargeInt())
				return false;
		}
		else if (pair2.getSmallInt() < pair1.getSmallInt()) {
			if (pair2.getLargeInt() < pair1.getLargeInt())
				return false;
		}
		return true;
	}

	static bool PointsOverlap(const OrderedPair &pair1, const OrderedPair &pair2) {
		if (pair1.getLargeInt() < pair2.getSmallInt())
			return false;
		else if (pair2.getLargeInt() < pair1.getSmallInt())
			return false;
		return true;
	}


public:
	static int Task1(const std::string &location) {

		std::cout << std::endl
		          << "Day 4 - Camp Cleanup - Task 1" << '\n'
		          << "-----------------------------" << '\n';

		unsigned int overlaps = 0, pair_num = 1;
		std::string line;
		std::ifstream in(location);
		std::regex regx("[-,]");
		while (getline(in, line)) {

			//the '-1' is what makes the regex split (-1 := what was not matched)
			std::sregex_token_iterator first{line.begin(), line.end(), regx, -1}, last;
			std::vector<std::string> tokens{first, last};

			if (tokens.size() == 4) {
				OrderedPair pair1, pair2;
				pair1.setPair(std::stoi(tokens[0]), std::stoi(tokens[1]) );
				pair2.setPair(std::stoi(tokens[2]), std::stoi(tokens[3]) );
				overlaps += int( ContainsPoints(pair1, pair2) );
				/*std::cout << pair1.getSmallInt() << " - " << pair1.getLargeInt()
				          << " : "
				          << pair2.getSmallInt() << " - " << pair2.getLargeInt()
				          << " :: " << overlaps << std::endl; */
			}
			else {
				std::cout << "ERROR" << std::endl;
			}

			++pair_num;
		}
		std::cout << "There are " << overlaps
				  << " assignment pairs where one range fully contains the other." << std::endl;

		return 0;
	}

	/*
	 *
	 */
	static int Task2(const std::string &location) {

		std::cout << std::endl
		          << "Day 4 - Camp Cleanup - Task 2" << '\n'
		          << "-----------------------------" << '\n';

		unsigned int overlaps = 0, pair_num = 1;
		std::string line;
		std::ifstream in(location);
		while (getline(in, line)) {

			std::regex regx("[-,]");
			//the '-1' is what makes the regex split (-1 := what was not matched)
			std::sregex_token_iterator first{line.begin(), line.end(), regx, -1}, last;
			std::vector<std::string> tokens{first, last};

			if (tokens.size() == 4) {
				OrderedPair pair1, pair2;
				pair1.setPair(std::stoi(tokens[0]), std::stoi(tokens[1]) );
				pair2.setPair(std::stoi(tokens[2]), std::stoi(tokens[3]) );
				overlaps += int( PointsOverlap(pair1, pair2) );
				/*std::cout << pair1.getSmallInt() << " - " << pair1.getLargeInt()
				          << " : "
				          << pair2.getSmallInt() << " - " << pair2.getLargeInt()
				          << " :: " << overlaps << std::endl; */
			}
			else {
				std::cout << "ERROR" << std::endl;
			}

			++pair_num;
		}

		std::cout << "There are " << overlaps
		          << " assignment pairs where the ranges overlap." << std::endl;
		return 0;
	}

};


#endif //ADVENTOFCODE2022_DAY4_H