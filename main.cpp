#include "Common/ProjectPath.h"     // defines project path in separate header (remove)
#include "Day01/CalorieCounting.h"
#include "Day02/RockPaperScissors.h"
#include "Day03/RucksackReorganisation.h"
#include "Day4/CampCleanup.h"
#include "Day5/SupplyStacks.h"
#include "Day6/TuningTrouble.h"
#include "Day8/TreeMap.hpp"
#include "Day9/RopeBridge.hpp"

#include "Common/quote_price.hpp"

int main() {

	const std::string project_folder = "Enter your project directory here.";
	// remove namespace myFilepath
	const std::string input_folder = Common::project_directory + R"(TaskInputs\)";

    Day01::runTasks(input_folder + "day1.txt");
	std::cout << '\n';

	Day02::runTasks(input_folder + "day2.txt");

	Day03::Task1(input_folder + "day3.txt");
	Day03::Task2(input_folder + "day3.txt");

	std::cout << "\n Day 4 - Camp Cleanup \n"
	          << "---------------------- \n";
	Day4::runTasks(input_folder + "day4.txt");

	std::cout << "\n * Day 5 - Supply Stacks * \n"
	          << " ------------------------- \n";
	day5::runTasks(input_folder + "day5_configuration.txt", input_folder + "day5_instructions.txt");

	std::cout << "\n Day 6 - Tuning Trouble \n"
	          << "------------------------ \n";
    day6::runTask(4, input_folder + "day6.txt");
	day6::runTask(14, input_folder + "day6.txt");

    std::cout << "\n * Day 8 - Treetop Tree House * \n";
    day8::runTasks(input_folder + "day8.txt");

    std::cout << "\n * Day 9 - Snake Bridge * \n \n";
    Day09::runTasks(R"(C:\Users\willi\C++Projects\day9.txt)", 9);

	return 0;

}