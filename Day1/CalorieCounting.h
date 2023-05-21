#ifndef ADVENTOFCODE2022_CALORIECOUNTING_H
#define ADVENTOFCODE2022_CALORIECOUNTING_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// Defines an elf with ID and the number of calories being carried in food
struct Elves {
	unsigned int id = 0, calories = 0;
};

class Day1 {
private:

// Find the top elf carrying the most calories from of a list of elves
	static Elves FindTopGatherer(const std::vector<Elves> &elves) {
		Elves top;
		for (Elves elf: elves) {
			if (elf.calories > top.calories)
				top = elf;
		}
		return top;
	}

// Find the top 3 elves carrying the most calories from of a list of elves
	static std::vector<Elves> FindTop3Gatherers(const std::vector<Elves> &elves) {
		std::vector<Elves> top3(3);  // initialise 3 elves
		for (Elves elf: elves)
			if (elf.calories > top3[0].calories) {
				top3[2] = top3[1];
				top3[1] = top3[0];
				top3[0] = elf;
			} else if (elf.calories > top3[1].calories) {
				top3[2] = top3[1];
				top3[1] = elf;
			} else if (elf.calories > top3[2].calories)
				top3[2] = elf;

		return top3;
	}

public:
	static int runTasks(const std::string &taskInput)  {
		Elves elf;
		std::vector <Elves> record;
		// read file line by line
		std::string line;
		std::ifstream in(taskInput);
		while (getline(in, line)) {
			if (line.empty()) {
				record.push_back(elf);
				elf.id += 1;   // define elf id by the order in which they are input
				elf.calories = 0;
			} else
				elf.calories += stoi(line);  // convert string to int and add
		}
		unsigned int num_elves = record.size();
		std::cout << "There are " << num_elves << " elves in total" << '\n';

		std::vector <Elves> top3 = FindTop3Gatherers(record);
		std::cout << "The top 3 elves to retrieve the most food are:" << '\n';
		unsigned int sum = 0;
		for (Elves top_elves: top3) {
			std::cout << "\t" << "ID-" << top_elves.id << " : " << top_elves.calories << " cal" << '\n';
			sum += top_elves.calories;
		}
		std::cout << "In total between them there are " << sum << " calories" << std::endl;

		return 0;
	}
};


#endif //ADVENTOFCODE2022_CALORIECOUNTING_H
