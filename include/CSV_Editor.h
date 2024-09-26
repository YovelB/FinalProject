#ifndef CSV_EDITOR_H
#define CSV_EDITOR_H

#include <string>
#include <vector>

// CSV Editor is a static utility class to read and write csv files.
// it does this by using std::vector<std::vector<std::string>> as a vector of rows (each row is a vector of cells).
class CSV_Editor {
	// delimiter for csv files.
	static const char delimiter = ',';

	// private constructor and destructor to prevent instantiation.
	CSV_Editor() = default;
	~CSV_Editor() = default;

	// split line into cells.
	static std::vector<std::string> split(const std::string& line);
	// join cells into line.
	static std::string join(const std::vector<std::string>& row);

public:
	// read csv file and return data as vector of rows (each row is a vector of cells).
	static std::vector<std::vector<std::string>> read_csv(const std::string& file_name);

	// write csv file with data (vector of rows, each row is a vector of cells).
	// if file exists, it will be overwritten else a new file will be created.
	static void write_csv(const std::string& file_name, const std::vector<std::vector<std::string>>& data);

	// create a new empty csv file.
	static void create_csv(const std::string& file_name);

	// delete csv file.
	static void delete_csv(const std::string& file_name);
};
#endif // CSV_EDITOR_H
