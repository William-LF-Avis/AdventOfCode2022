#ifndef ADVENTOFCODE2022_DAY4_H
#define ADVENTOFCODE2022_DAY4_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <regex>


namespace Day4 {

	// Size-ordered integer pair (should be declared const to avoid direct re-assignment)
	struct OrderedPair {
		int small_point, large_point;

		explicit OrderedPair(const int &val1, const int &val2) {
			if (val2 < val1) {
				small_point = val2;
				large_point = val1;
			}
			else {
				small_point = val1;
				large_point = val2;
			}
		}
	};

	// determines if one pair fully encapsulates another (not order dependent, if either fully contains the other it will return true)
	static bool ContainsPoints(const OrderedPair &pair1, const OrderedPair &pair2)  {
		if (pair1.small_point < pair2.small_point) {
			if (pair1.large_point < pair2.large_point)
				return false;
		}
		else if (pair2.small_point < pair1.small_point) {
			if (pair2.large_point < pair1.large_point)
				return false;
		}
		return true;
	}

	static bool PointsOverlap(const OrderedPair &pair1, const OrderedPair &pair2) {
		if (pair1.large_point < pair2.small_point)
			return false;
		else if (pair2.large_point < pair1.small_point)
			return false;
		return true;
	}


	static int runTasks(const std::string &location) {

		unsigned int overlaps_part1 = 0, overlaps_part2 = 0, pair_num = 1;
		std::string line;
		std::ifstream in(location);
		std::regex regx("[-,]");
		while (getline(in, line)) {

			//the '-1' is what makes the regex split (-1 := what was not matched)
			std::sregex_token_iterator first{line.begin(), line.end(), regx, -1}, last;
			std::vector<std::string> tokens{first, last};

			if (tokens.size() != 4) {
				std::cout << "ERROR" << std::endl;
				return -1;
			}

			const OrderedPair pair1(std::stoi(tokens[0]), std::stoi(tokens[1])),
						      pair2(std::stoi(tokens[2]), std::stoi(tokens[3]));
			overlaps_part1 += int( ContainsPoints(pair1, pair2) );
			overlaps_part2 += int( PointsOverlap(pair1, pair2) );

			++pair_num;
		}
		std::cout << "There are " << overlaps_part1
				  << " assignment pairs where one range fully contains the other." << std::endl;
		std::cout << "There are " << overlaps_part2
		          << " assignment pairs where the ranges overlap." << std::endl;

		return 0;
	}

};


#endif //ADVENTOFCODE2022_DAY4_H