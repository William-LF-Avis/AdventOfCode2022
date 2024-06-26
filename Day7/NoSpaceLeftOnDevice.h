#ifndef ADVENTOFCODE2022_NOSPACELEFTONDEVICE_H
#define ADVENTOFCODE2022_NOSPACELEFTONDEVICE_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <regex>
#include <cmath>
#include <thread>
#include <stdexcept>
#include "/../Common/FileHandling.h"
#include "Terminal.h"


namespace day7 {

	int runTasks(std::string &location) {


        std::ifstream input(location);
        std::string read_error;
        if (!common::tryReadFile(input, read_error, 5, 100))
            return -1;

        // parse commands and instantiate folder Tree with ptr
        std::string parse_error;
        std::unique_ptr<terminal::Tree> file_system = terminal::constructTree(input, parse_error);

		return 0;
	}

};



#endif //ADVENTOFCODE2022_NOSPACELEFTONDEVICE_H