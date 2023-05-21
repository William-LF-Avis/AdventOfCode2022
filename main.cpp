#include "myFilepath.h"     // defines project path
#include "Day1/CalorieCounting.h"
#include "Day2/RockPaperScissors.h"
#include "Day3/RucksackReorganisation.h"
#include "Day4/CampCleanup.h"
#include "Day5/SupplyStacks.h"

int main() {

	const std::string PROJECTDIR = "Enter your project directory here.";

	// remove namespace myFilepath
	const std::string INPUTDIR = myFilepath::PROJECTDIR + R"(TaskInputs\)";

	Day1::runTasks(INPUTDIR + "day1.txt)");
	std::cout << '\n';

	Day2::runTasks(INPUTDIR + "day2.txt)");

	Day3::Task1(INPUTDIR + "day3.txt)");
	Day3::Task2(INPUTDIR + "day3.txt)");

	Day4::Task1(INPUTDIR + "day4.txt)");
	Day4::Task2(INPUTDIR + "day4.txt)");

	Day5::runTasks(INPUTDIR + "day5_start.txt)", INPUTDIR + "day5_procedure.txt)");

	return 0;
}

