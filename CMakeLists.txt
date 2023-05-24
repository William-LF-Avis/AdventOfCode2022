cmake_minimum_required(VERSION 3.25)
project(AdventOfCode2022)

set(CMAKE_CXX_STANDARD 23)

add_executable( AdventOfCode2022
        main.cpp
        myFilepath.h
        Day1/CalorieCounting.h
        Day2/RockPaperScissors.h
        Day3/RucksackReorganisation.h
        Day4/CampCleanup.h
        Day5/SupplyStacks.h
        Day6/TuningTrouble.h)
