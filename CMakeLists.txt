cmake_minimum_required(VERSION 3.25)
project(AdventOfCode2022)

set(CMAKE_CXX_STANDARD 23)

add_executable( AdventOfCode2022
        main.cpp
        Common/ProjectPath.h
        Day01/CalorieCounting.h
        Day02/RockPaperScissors.h
        Day03/RucksackReorganisation.h
        Day4/CampCleanup.h
        Day5/SupplyStacks.h
        Day6/TuningTrouble.h
        Day7/NoSpaceLeftOnDevice.h Day7/Directory.h Day7/Terminal.h
        Common/FileHandling.h
        Day8/TreeMap.hpp
        Common/TextBanners.hpp
        Day9/RopeBridge.hpp
        Common/Vector.hpp
        Common/quote_price.hpp Day9/Rope.hpp)

add_executable( Day9_RopeSimulation
        Day9/RopeSimulator.cpp
        Day9/RopeBridge.hpp
        Common/ProjectPath.h
        Day9/Rope.hpp)
