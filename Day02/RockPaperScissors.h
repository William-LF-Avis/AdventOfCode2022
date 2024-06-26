#ifndef ADVENTOFCODE2022_DAY2_H
#define ADVENTOFCODE2022_DAY2_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>

class Day02 {
private:

	/*  'inputSet' and 'outputSet' are 1-1 ordered sets, each containing 3 chars.
	*  If the char 'input' is within 'inputSet', then the function returns the corresponding member of 'outputSet'.
	*/
	static char CharacterMap(const unsigned char &input, const char (&inputSet)[3], const char (&outputSet)[3]) {
		int i = 0;
		for (char letter : inputSet) {
			if (input == letter)
				return outputSet[i];
			++i;
		}
		std::cout << "Input character " << input << " is not contained within the input set specified" << std::endl;
		return ' '; // error return empty char
	}

	/*  Returns your score for a single game of Rock, Paper, Scissors.
	 *      The first char in 'play' should represent the opponents hand (R, P, S).
	 *      The second char should represent either your hand (R, P, S) or the result of the game (L, D, W).
	 *      'is_result' defines the interpretation of the second char, true for result.
	 */
	static int RockPaperScissors(const std::string &play, const bool &is_result) {
		const static std::string permutations[2][3][3]   // the inner 3x3 arrays map 1-1 with the 'scores' array below
				{
						{   // permutations of opponents hand and your hand
								{"RR", "RP", "RS"},
								{"PR", "PP", "PS"},
								{"SR", "SP", "SS"}
						},
						{   // permutations of opponents hand and the result of the game
								{"RD", "RW", "RL"},
								{"PL", "PD", "PW"},
								{"SW", "SL", "SD"}
						}
				};
		/* Points are awarded as the sum of your:
		 *  Hand   - R=1, P=2, S=3
		 *  Result - L=0, D=3, W=6 */
		const static int scores[3][3]
				{
						{4, 8, 3},
						{1, 5, 9},
						{7, 2, 6}
				};
		int i = int(is_result), j = 0;
		for (auto &row : permutations[i]) {
			int k = 0;
			for (auto &str : row) {
				if (play == str)
					return scores[j][k];
				++k;
			}
			++j;
		}
		std::cout << "Input string " << play << " is not contained within the permutation array specified" << std::endl;
		return -1;  // error
	}

public:

	static int runTasks(const std::string &location) {
		unsigned int total_task1 = 0, total_task2 = 0;  // holds the total score for tasks 1 and 2 individually
		unsigned int input_line = 1;
		std::string line;
		std::ifstream in(location);
		while (getline(in, line)) {
			unsigned int alpha_num = 0;
			std::string hand1, hand2, result;
			for (unsigned char sym : line) {
				if (isalpha(sym)) {
					sym = toupper(sym);
					++alpha_num;
					if (alpha_num == 1)
						hand1.push_back( CharacterMap(sym,{'A', 'B', 'C'},{'R', 'P', 'S'}) );
					else if (alpha_num == 2) {
						hand2.push_back( CharacterMap(sym, {'X', 'Y', 'Z'}, {'R', 'P', 'S'}) );
						result.push_back(CharacterMap(sym, {'X', 'Y', 'Z'}, {'L', 'D', 'W'}) );
					}
					else {
						std::cout << "Input Error: " << alpha_num << " character(s) on line " << input_line << ", expected 2" << std::endl;
						return -2;
					}
				}
			}
			if (alpha_num == 2) {
				if ( hand1 != " " and hand2 != " " and result != " " ) {
					int score_task1, score_task2;
					score_task1 = RockPaperScissors(hand1 + hand2, false);
					score_task2 = RockPaperScissors(hand1 + result, true );
					if (score_task1 != -1 and score_task2 != -1) {
						total_task1 += score_task1;
						total_task2 += score_task2;
					}
					else {
						std::cout << "Input Error: Line " << input_line << std::endl
						          << "Critical flaw this error should never occur" << std::endl;
					}
				}
				else {
					std::cout << "Input Error: Line " << input_line << std::endl;
					return -1;
				}
			}
			else {
				std::cout << "Input Error: " << alpha_num << " character(s) on line " << input_line << ", expected 2" << std::endl;
				return -2;
			}
			++input_line;
		}

		std::cout << "Your Total Score for task 1 is " << total_task1 << std::endl;
		std::cout << "Your Total Score for task 2 is " << total_task2 << std::endl;

		return 0;
	}

};
#endif //ADVENTOFCODE2022_DAY2_H