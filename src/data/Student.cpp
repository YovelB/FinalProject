#include "Student.h"

#include <iomanip>
#include <stdexcept>

#include "../schedule/Schedule_Manager.h"

Student::Student(const std::string& id, const std::string& name, const std::string& password) :
	m_id(valid_id(id)), m_name(valid_name(name)), m_password(valid_password(password)),
	m_schedule_manager(new Schedule_Manager(m_id)) {}

Student::Student(const Student& other) : m_id(other.m_id), m_name(other.m_name), m_password(other.m_password),
                                         m_schedule_manager(other.m_schedule_manager) {}

Student::~Student() {
	// delete the schedule manager object.
	delete m_schedule_manager;
	m_schedule_manager = nullptr;
}

Entity* Student::clone() const {
	// create a new student object and copy the data.
	return new Student(*this);
}

std::vector<std::string> Student::to_csv() const {
	// return the data members as a vector of strings.
	return std::vector{m_id, m_name, m_password};
}

Student* Student::from_csv(const std::vector<std::string>& data) {
	// if the data size doesn't match the number of data members, throw an exception.
	if (data.size() != 3) { throw std::invalid_argument("Invalid CSV data to create a student."); }
	// create a new student object from the data.
	return new Student(valid_id(data[0]), valid_name(data[1]), valid_password(data[2]));
}

std::string Student::get_type() const { return "Student"; }

std::string Student::get_file_name() { return "Students.csv"; }

bool Student::search(const std::string& text) const {
	// use to_csv() method to convert the teacher data to a CSV format.
	// loop through the CSV data and search for the text.
	for (const std::string& str : to_csv()) {
		// str.find(text) method returns the position of the text in the string, or std::string::npos if not found.
		if (str.find(text) != std::string::npos) { return true; }
	}
	return false;
}

std::string Student::get_id() const { return m_id; }
void Student::set_id(const std::string& id) { m_id = valid_id(id); }

std::string Student::get_name() const { return m_name; }
void Student::set_name(const std::string& name) { m_name = valid_name(name); }

std::string Student::get_password() const { return m_password; }
void Student::set_password(const std::string& password) { m_password = valid_password(password); }

Schedule_Manager* Student::get_schedule_manager() const {
	// return the schedule manager object.
	return m_schedule_manager;
}

std::string Student::to_string() const {
	// use ostream and setw() to format the output.
	std::ostringstream out{};
	out << "Student: ID: " << m_id << ", Name: " << std::left << std::setw(7) << m_name << ", Password: " << m_password;
	return out.str();
}

std::ostream& operator<<(std::ostream& os, const Student& student) {
	os << student.to_string();
	return os;
}

std::string Student::valid_id(const std::string& id) {
	// validate id. (must be 9 characters long, and contain only digits).
	if (id.empty()) {
		throw std::invalid_argument("Student id cannot be empty.");
	}
	if (id.size() != 9) {
		throw std::invalid_argument("Student id must be 9 digits.");
	}
	for (const char c : id) {
		if (!std::isdigit(c)) {
			throw std::invalid_argument("Student id must contain only digits.");
		}
	}
	return id;
}

std::string Student::valid_name(const std::string& name) {
	// validate name. (cannot be empty).
	if (name.empty()) {
		throw std::invalid_argument("Student name cannot be empty.");
	}
	return name;
}

std::string Student::valid_password(const std::string& password) {
	// validate password. (must be at least 8 characters long, and contain both letters and digits).
	if (password.empty()) {
		throw std::invalid_argument("Student password cannot be empty.");
	}
	if (password.size() < 8) {
		throw std::invalid_argument("Student password must be at least 8 characters long.");
	}
	bool has_letter = false;
	bool has_digit = false;
	for (const char c : password) {
		if (std::isalpha(c)) {
			has_letter = true;
		}
		if (std::isdigit(c)) {
			has_digit = true;
		}
	}
	if (!has_letter || !has_digit) {
		throw std::invalid_argument("Student password must contain both letters and digits.");
	}
	return password;
}
