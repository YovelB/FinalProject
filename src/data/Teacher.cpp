#include "Teacher.h"

#include <iomanip>
#include <stdexcept>

Teacher::Teacher(const std::string& id, const std::string& name) : m_id(validate_id(id)), m_name(validate_name(name)) {}

Teacher::Teacher(const Teacher& other) : m_id(other.m_id), m_name(other.m_name) {}

Entity* Teacher::clone() const {
	// create a new teacher object and copy the data.
	return new Teacher(*this);
}

std::vector<std::string> Teacher::to_csv() const {
	// return the data members as a vector of strings.
	return std::vector{m_id, m_name};
}

Teacher* Teacher::from_csv(const std::vector<std::string>& data) {
	// if the data size doesn't match the number of data members, throw an exception.
	if (data.size() != 2) { throw std::invalid_argument("Invalid CSV data to create a teacher."); }
	// create a new teacher object from the data.
	return new Teacher(validate_id(data[0]), validate_name(data[1]));
}

std::string Teacher::get_type() const { return "Teacher"; }

std::string Teacher::get_file_name() { return "Teachers.csv"; }

bool Teacher::search(const std::string& text) const {
	// use to_csv() method to convert the teacher data to a CSV format.
	// loop through the CSV data and search for the text.
	for (const std::string& str : to_csv()) {
		// str.find(text) method returns the position of the text in the string, or std::string::npos if not found.
		if (str.find(text) != std::string::npos) { return true; }
	}
	return false;
}

std::string Teacher::get_id() const { return m_id; }
void Teacher::set_id(const std::string& id) { m_id = validate_id(id); }

std::string Teacher::get_name() const { return m_name; }
void Teacher::set_name(const std::string& name) { m_name = validate_name(name); }

std::string Teacher::to_string() const {
	// use ostringstream and setw() to format the output.
	std::ostringstream out{};
	out << "Teacher: ID: " << m_id << ", Name: " << m_name;
	return out.str();
}

std::ostream& operator<<(std::ostream& os, const Teacher& teacher) {
	os << teacher.to_string();
	return os;
}

std::string Teacher::validate_id(const std::string& id) {
	// validate id. (must be 9 characters long, and contain only digits).
	if (id.empty()) {
		throw std::invalid_argument("Teacher id cannot be empty.");
	}
	if (id.size() != 9) {
		throw std::invalid_argument("Teacher id must be 9 digits.");
	}
	for (const char c : id) {
		if (!std::isdigit(c)) {
			throw std::invalid_argument("Teacher id must contain only digits.");
		}
	}
	return id;
}
std::string Teacher::validate_name(const std::string& name) {
	// validate name. (cannot be empty).
	if (name.empty()) {
		throw std::invalid_argument("Teacher name cannot be empty.");
	}
	return name;
}
