#include "Lab.h"

#include <stdexcept>

// use the base class constructor to init the member variables.
Lab::Lab(const std::string& id, const std::string& day, const std::string& start_time,
         const unsigned duration, const std::string& lecturer, const std::string& classroom) :
	Course_Type{id, day, start_time, duration, lecturer, classroom} {}

Lab::Lab(const Lab& other) : Course_Type{other} {}

Lab* Lab::clone() const {
	// create a new Lab object and return it.
	return new Lab{*this};
}

std::string Lab::get_type() const { return "Lab"; }

std::string Lab::get_file_name() { return "_labs.csv"; }

Lab* Lab::from_csv(const std::vector<std::string>& data) {
	// if the data size doesn't match the number of data members, throw an exception.
	if (data.size() != 6) { throw std::invalid_argument{"Invalid data size to create a lab."}; }

	// create a new Lab object and return it.
	return new Lab(validate_id(data[0]), validate_day(data[1]), validate_start_time(data[2]),
						validate_duration(std::stoi(data[3])), validate_lecturer(data[4]), validate_classroom(data[5]));
}
