#include <iostream> // For std::cout, std::cerr, std::endl
#include <fstream>	// For std::ifstream
#include <sstream>	// For std::stringstream
#include <string>	// For std::string
#include <map>		// For std::map
#include <dirent.h> // For DIR, dirent, opendir, readdir, closedir

// Function to check if a database file exists and is readable - We will use this function to check if data.csv is
// present
bool checkFile(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: " << filename << " does not exist or is not readable." << std::endl;
		return false;
	}
	return true;
}

bool validateCSVStructure(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
		return false;

	std::string line;
	bool firstLine = true;
	while (std::getline(file, line))
	{
		if (firstLine)
		{
			if (line != "date,exchange_rate")
			{
				std::cerr << "Error: Invalid CSV file header." << std::endl;
				return false;
			}
			firstLine = false;
			continue;
		}
		std::stringstream ss(line);
		std::string date;
		float rate;
		if (std::getline(ss, date, ',') && ss >> rate)
		{
			// Valid line
			continue;
		}
		else
		{
			// Invalid line
			return false;
		}
		return true;
	}
}

std::string findCSVDatabase()
{
	const std::string defaultDatabaseFilename = "data.csv";
	if (checkFile(defaultDatabaseFilename) && validateCSVStructure(defaultDatabaseFilename))
	{
		return defaultDatabaseFilename;
	}
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(".")) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			std::string filename = ent->d_name;
			if (filename.size() > 4 && filename.substr(filename.size() - 4) == ".csv")
			{
				if (checkFile(filename) && validateCSVStructure(filename))
				{
					closedir(dir);
					return filename;
				}
			}
		}
		closedir(dir);
	}
	return "";
}

int main(int argc, char *argv[])
{
	std::string DBFilename = findCSVDatabase();
	if (DBFilename.empty())
	{
		std::cerr << "Error: No valid CSV database found." << std::endl;
		return 1;
	}
	std::cout << "Using database: " << DBFilename << std::endl;
	return 0;
}