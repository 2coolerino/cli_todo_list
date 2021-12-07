// Copyright Scott Hutchins
#include <iostream>
#include <unordered_map>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

/**
 * Converts string to bool value
 * @param s the string to be converted
 */
bool to_bool(std::string const &s)
{
	return s != "0";
}

/**
 * loads the data from the inputted file data and returns the map of the data
 * @param filename
 */
std::map<std::string, bool> loadData(const std::string &filename)
{
	std::map<std::string, bool> result;
	std::string line, word;
	bool checked;
	std::ifstream input(filename);
	while (std::getline(input, line))
	{
		std::istringstream iss(line);
		std::getline(iss, word);
		std::string s(1, word.back());
		checked = to_bool(s);
		std::cout << "Checked:" << checked << std::endl;
		word = word.substr(0, word.length() - 2);
		result[word] = checked;
	}
	return result;
}

/**
 * Method to print the contents of the map
 * @param todo
 */
void printList(std::map<std::string, bool> &todo)
{
	for (auto &entry : todo)
	{
		std::cout << entry.first << ": Finished: " << entry.second << std::endl;
	}
}

/**
 * Writes the changes made to the map to the corresponding file
 * @param todo the map the user modified
 * @param fileName the file to write to
 */
void saveChanges(std::map<std::string, bool> &todo, std::string fileName)
{
	std::ofstream out;
	out.open(fileName);
	for (auto &entry : todo)
	{
		out << entry.first << " " << entry.second << std::endl;
	}
	out.close();
}

/**
 * The main loop that handles all the interactions
 * @param todo the map to be modified
 */
void main_loop(std::map<std::string, bool> &todo)
{
	std::string userInput = "";
	std::cout << "Enter 'exit' to save and quit" << std::endl;
	// If you remove this the program breaks and I dont know why lol
	std::getline(std::cin, userInput);

	while (userInput != "exit")
	{
		int pid = fork();
		if (pid == 0)
		{
			execlp("clear", "clear", 0);
		}
		else
		{
			waitpid(pid, nullptr, 0);
		}
		printList(todo);
		std::cout << ">";
		std::getline(std::cin, userInput);
		if (userInput == "exit")
		{
			break;
		}
		else
		{
			std::istringstream iss(userInput);
			std::string command, word;
			iss >> command;
			std::getline(iss, word);
			word = word.substr(1);
			if (command == "add")
			{
				todo[word] = false;
			}
			else if (command == "finish" || command == "done")
			{
				todo[word] = true;
			}
			else if (command == "remove" || command == "del")
			{
				if (todo.find(word) != std::end(todo))
				{
					todo.erase(word);
				}
			}
			else
			{
				std::cout << "Enter a valid command" << std::endl;
			}
		}
	}
}

/**
 * @brief
 *
 * @return int
 */
int main()
{
	std::ifstream input_file;
	std::string fileName;
	std::cout << "Enter file name to open (if the file does not exsit a new one will be created)" << std::endl;
	std::cin >> fileName;
	fileName = fileName + ".txt";
	input_file.open(fileName);
	if (input_file)
	{
		std::map<std::string, bool> list = loadData(fileName);
		main_loop(list);
		saveChanges(list, fileName);
	}
	else
	{
		std::ofstream output(fileName);
		output.close();
		std::map<std::string, bool> list;
		main_loop(list);
		saveChanges(list, fileName);
	}
	return 0;
}
