#ifndef ADVENTOFCODE2022_TERMINAL_H
#define ADVENTOFCODE2022_TERMINAL_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <regex>
#include <thread>
#include <stdexcept>

namespace day7::terminal {

	struct Directory {
		std::string name;
		Directory *parent;
		std::vector<Directory*> children;
		std::map<std::string, int> files;

		explicit Directory(const std::string &folder_name, Directory *folder_parent = nullptr) {
			name   = folder_name;
			parent = folder_parent;
		}

	};

	std::string findDirectoryPath(Directory folder) {
		// unwind with * and construct the path with the folder names
		return "";
	}

	struct Tree {
		//! TODO: *?
		std::vector<Directory> directories;


		// when encountering a new folder, add to Tree::directories
		void cd(std::string &name, Directory *parent = nullptr) {
			Directory folder(name);
			if (name == "/") directories.push_back(folder);
			else {
				folder.parent = parent;
				directories.push_back(folder);
			}

		}

		// when encountering a new file, add to Tree::&dir::files
		void ls() {

		}

	};

	// Construct the directory Tree from file stream
	std::unique_ptr<Tree> constructTree(std::ifstream &file, std::string &error) {
		auto tree = std::make_unique<Tree>();

		int line_no = 1;
		bool is_ls = true;
		std::string current_path;
		std::string line;
		while (getline(file, line)) {

			// do something involving peak
			// create directory upon cd or ls?


			if (line[0] == '$') {

				// change directory or list files
				std::regex regex_cd(R"($ cd (\d+))"), regex_ls(R"($ ls)");

				// parse instructions using regex
				std::smatch match_cd, match_ls;
				if (std::regex_match(line, match_cd, regex_cd)) {
					// cd to folder must exist
					current_path += match_cd[1].str();

				} else if (std::regex_match(line, match_ls, regex_ls)) {
					is_ls = true;
					std::cout << is_ls;
				}

			} else if (is_ls) {

				std::regex regex_dir(R"(dir (\d+))"), regex_file(R"((\d+) (\d+))");

				std::smatch match_dir, match_file;
				if (std::regex_match(line, match_dir, regex_dir)) {
					// create directory
					Directory new_folder(match_dir[1].str());
				} else if (std::regex_match(line, match_file, regex_file)) {
					// link file to current directory with size

				} else {
					// raise unexpected command error
					return nullptr;
				}

			}
				/// expected command
			else {
				std::cerr << "Line " << line_no << ": Invalid instruction encountered " << "'" << line << "'\n";
				return nullptr;
			}
		}
		std::cout << "Task 1: After " << line_no << " moves the resulting crates at the top of each stack are "
				  << "" << '\n';
		std::cout << "Task 2: After " << line_no << " moves the resulting crates at the top of each stack are "
				  << "" << '\n';
		return nullptr;


			++line_no;
		}


	}







#endif //ADVENTOFCODE2022_TERMINAL_H
