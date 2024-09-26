#include "Tutorial.h"

#include <stdexcept>

// use the base class constructor to init the member variables.
Tutorial::Tutorial(const std::string& group_id, const std::string& day, const std::string& start_time,
                   const unsigned duration, const std::string& lecturer, const std::string& classroom) :
	Course_Type{group_id, day, start_time, duration, lecturer, classroom} {}

Tutorial::Tutorial(const Tutorial& other) : Course_Type{other} {}

Tutorial* Tutorial::clone() const {
	// create a new Tutorial object and return it.
	return new Tutorial{*this};
}

std::string Tutorial::get_type() const { return "Tutorial"; }

std::string Tutorial::get_file_name() { return "_tutorials.csv"; }

Tutorial* Tutorial::from_csv(const std::vector<std::string>& data) {
	// if the data size doesn't match the number of data members, throw an exception.
	if (data.size() != 6) { throw std::invalid_argument{"Invalid data size to create a tutorial."}; }
	// create a new Tutorial object and return it.
	return new Tutorial(validate_id(data[0]), validate_day(data[1]), validate_start_time(data[2]),
	                    validate_duration(std::stoi(data[3])), validate_lecturer(data[4]), validate_classroom(data[5]));
}
