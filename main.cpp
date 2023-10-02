#include "myFilepath.h"     // defines project path in separate header
#include "Day1/CalorieCounting.h"
#include "Day2/RockPaperScissors.h"
#include "Day3/RucksackReorganisation.h"
#include "Day4/CampCleanup.h"
#include "Day5/SupplyStacks.h"
#include "Day6/TuningTrouble.h"

int main() {

	const std::string ProjectDirectory = "Enter your project directory here.";
	// remove namespace myFilepath
	const std::string InputDirectory = myFilepath::ProjectDirectory + R"(TaskInputs\)";

	Day1::runTasks(InputDirectory + "day1.txt");
	std::cout << '\n';

	Day2::runTasks(InputDirectory + "day2.txt");

	Day3::Task1(InputDirectory + "day3.txt");
	Day3::Task2(InputDirectory + "day3.txt");

	Day4::Task1(InputDirectory + "day4.txt");
	Day4::Task2(InputDirectory + "day4.txt");


	std::cout << "\n Day 5 - Supply Stacks - Task 1 and 2 \n"
	          << "-------------------------------------- \n";
	Day5::runTasks(
			InputDirectory + "day5_configuration.txt",
			 InputDirectory + "day5_instructions.txt"
	);

	Day6::runTasks(InputDirectory + "day6.txt");

	return 0;
}