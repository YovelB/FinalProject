#include "CSV_Editor.h"

#include <fstream>
#include <iostream>

std::vector<std::vector<std::string>> CSV_Editor::read_csv(const std::string& file_name) {
	// get the file path.
	const std::string& file_path = "../resources/" + file_name;

	// data vector to store rows.
	std::vector<std::vector<std::string>> data{};

	// open the file for reading.
	std::ifstream file(file_path);
	// if file was not found, create it.
	if (!file.is_open()) {
		create_csv(file_name);
		return data;
	}

	std::string line{};
	while (std::getline(file, line)) {
		// if line is empty, skip
		if (line.empty()) { continue; }

		// Handle BOM (Byte Order Mark) for UTF-8 encoded files.
		if (data.empty() && line.size() >= 3 && line[0] == '\xEF' && line[1] == '\xBB' && line[2] == '\xBF') {
			line.erase(0, 3);
		}

		// row vector to store columns. (split line into columns).
		data.push_back(split(line));
	}
	// if after reading the file, data is empty, do nothing. (to allow the creation of a new files).

	// close the file (not necessary, since file will be closed when it goes out of scope).
	file.close();
	return data;
}

void CSV_Editor::write_csv(const std::string& file_name, const std::vector<std::vector<std::string>>& data) {
	// get the file path.
	const std::string& file_path = "../resources/" + file_name;
	// open the file for writing. (if file doesn't exist, it will be created).
	std::ofstream file(file_path);
	if (!file.is_open()) {
		throw std::runtime_error("Error: could not create file " + file_name);
	}

	for (const std::vector<std::string>& row : data) {
		// join cells into line.
		file << join(row) << '\n';
	}

	// close the file (not necessary, since file will be closed when it goes out of scope).
	file.close();
}

void CSV_Editor::create_csv(const std::string& file_name) {
	// get the file path.
	const std::string& file_path = "../resources/" + file_name;
	// open the file for writing.
	std::ofstream file(file_path);
	// close the file (not necessary, since file will be closed when it goes out of scope).
	file.close();
}

std::vector<std::string> CSV_Editor::split(const std::string& line) {
	std::vector<std::string> row{};
	std::string cell{};
	// iterate over chars in line, and split by comma delimiter.
	for (const char c : line) {
		// if char is delimiter, add cell to row and reset cell.
		if (c == delimiter) {
			row.push_back(cell);
			cell.clear();
		}
		else {
			cell += c;
		}
	}
	// add last cell to row.
	row.push_back(cell);
	return row;
}

std::string CSV_Editor::join(const std::vector<std::string>& row) {
	std::string line{};
	// iterate over cells in row, and join with comma delimiter.
	for (size_t i = 0; i < row.size(); i++) {
		line += row[i];
		// if not last cell, add delimiter.
		if (i < row.size() - 1) {
			line += delimiter;
		}
	}
	return line;
}

void CSV_Editor::delete_csv(const std::string& file_name) {
	// get the file path.
	const std::string& file_path = "../resources/" + file_name;
	// try opening the file to check if it exists.
	std::ifstream file(file_path);
	// if file is not open, throw an exception.
	if (!file.is_open()) {
		throw std::runtime_error("Error: could not open file " + file_name);
	}
	// delete the file.
	if (std::remove(file_path.c_str()) != 0) {
		throw std::runtime_error("Error: could not delete file " + file_name);
	}
}
