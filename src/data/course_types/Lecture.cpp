#include "Lecture.h"

#include <stdexcept>

// use the base class constructor to init the member variables.
Lecture::Lecture(const std::string& group_id, const std::string& day, const std::string& start_time,
                 const unsigned duration, const std::string& lecturer, const std::string& classroom) :
	Course_Type{group_id, day, start_time, duration, lecturer, classroom} {}

Lecture::Lecture(const Lecture& other) : Course_Type{other} {}

Lecture* Lecture::clone() const {
	// create a new Lecture object and return it.
	return new Lecture{*this};
}

std::string Lecture::get_type() const { return "Lecture"; }

std::string Lecture::get_file_name() { return "_lectures.csv"; }


Lecture* Lecture::from_csv(const std::vector<std::string>& data) {
	// if the data size doesn't match the number of data members, throw an error.
	if (data.size() != 6) {
		throw std::invalid_argument{"Invalid data size for Lecture object."};
	}
	// create a new lecture object from the data.
	return new Lecture(validate_id(data[0]),validate_day(data[1]), validate_start_time(data[2]),
	                   validate_duration(std::stoi(data[3])), validate_lecturer(data[4]), validate_classroom(data[5]));
}
