#include "Course.h"

#include <iomanip>
#include <iostream>
#include <sstream>

#include "course_types/Course_Type.h"

Course::Course(const std::string& id, const std::string& name, const std::string& lecturer, const float points) :
	m_id{validate_id(id)}, m_name{validate_name(name)}, m_lecturer{validate_name(lecturer)},
	m_points{validate_points(points)} {}

Course::Course(const Course& other) : m_id{other.m_id}, m_name{other.m_name},
                                      m_lecturer{other.m_lecturer}, m_points{other.m_points} {
	deep_copy_course_types(other);
}

Course::~Course() {
	clean_up();
}

Course& Course::operator=(const Course& other) {
	if (this != &other) {
		// copy the course data.
		m_id = other.m_id;
		m_name = other.m_name;
		m_lecturer = other.m_lecturer;
		m_points = other.m_points;
		// clean up the course types and copy the course types.
		clean_up();
		deep_copy_course_types(other);
	}
	return *this;
}

void Course::clean_up() {
	// delete the course types and avoid dangling pointers.
	for (auto& [_, curr_course_type] : m_course_types) {
		delete curr_course_type;
	}
	// clear the vector.
	m_course_types.clear();
}

void Course::deep_copy_course_types(const Course& other) {
	// copy the course types.
	for (const auto& [id, course_type] : other.m_course_types) {
		// if the course type is not nullptr, clone it and add it to the course.
		if (course_type) {
			// use clone() method to copy the instance of the course type.
			m_course_types[id] = course_type->clone();
		}
	}
}

Entity* Course::clone() const {
	// create a new course object and copy the data.
	return new Course(*this);
}

std::vector<std::string> Course::to_csv() const {
	return std::vector{m_id, m_name, m_lecturer, std::to_string(m_points)};
}

Course* Course::from_csv(const std::vector<std::string>& data) {
	// if the data size doesn't match the number of data members, throw an exception.
	if (data.size() != 4) { throw std::invalid_argument{"Invalid data size to create a course."}; }
	// stof() method converts a string to a float number.
	return new Course(validate_id(data[0]), validate_name(data[1]),
	                  validate_name(data[2]), validate_points(std::stof(data[3])));
}

std::string Course::get_type() const { return "Course"; }

std::string Course::get_file_name() { return "Courses.csv"; }

bool Course::search(const std::string& text) const {
	// use to_csv() method to convert the course data to a CSV format.
	// loop through the CSV data and search for the text.
	for (const std::string& str : to_csv()) {
		// str.find(text) method returns the position of the text in the string, or std::string::npos if not found.
		if (str.find(text) != std::string::npos) {
			return true;
		}
	}
	// then search in each course type.
	for (const auto& [_, curr_course_type] : m_course_types) {
		// if the course type is not nullptr and the text is found, return true.
		if (curr_course_type && curr_course_type->search(text)) {
			return true;
		}
	}
	return false;
}

std::string Course::get_id() const { return m_id; }
void Course::set_id(const std::string& id) { m_id = validate_id(id); }

std::string Course::get_name() const { return m_name; }
void Course::set_name(const std::string& name) { m_name = validate_name(name); }

float Course::get_points() const { return m_points; }
void Course::set_points(const float points) { m_points = validate_points(points); }

std::string Course::to_string() const {
	// using ostringstream and setw() to format the output.
	// setprecision() to show only 1 decimal point.
	std::ostringstream out{};
	out << "Course: ID: "
		<< m_id << ", Name: " << m_name << ", Lecturer: " << m_lecturer << ", Points: " <<
		std::fixed << std::setprecision(1) << m_points;

	// create a map of course types to sort them by type for printing.
	std::unordered_map<std::string, std::vector<Course_Type*>> sorted_course_types{};
	for (const auto& [_, course_type] : m_course_types) {
		sorted_course_types[course_type->get_type()].push_back(course_type);
	}
	// define the order of course types.
	const std::vector<std::string> course_type_order{"Lecture", "Tutorial", "Lab"};
	// loop through the course type order and print the course types.
	for (const auto& type : course_type_order) {
		// use the sorted map to print the course types in order. (if the course type doesn't exist, it will be skipped).
		// find() checks if the key exists in the map and returns an iterator to it.
		if (auto it = sorted_course_types.find(type); it != sorted_course_types.end()) {
			out << "\n" << type << "s:";
			for (const auto& curr : it->second) {
				out << "\n" << *curr;
			}
		}
	}
	return out.str();
}

std::ostream& operator<<(std::ostream& os, const Course& course) {
	os << course.to_string();
	return os;
}

Course_Type* Course::get_course_type(const std::string& id) const {
	// find() method returns an iterator to the element if found, else returns the end iterator.
	if (const auto it = m_course_types.find(id); it != m_course_types.end()) {
		return it->second; // return the course type.
	}
	return nullptr; // return nullptr if not found.
}

void Course::set_course_type(const std::string& id, Course_Type* course_type) {
	// check if the id and course type exist.
	if (!get_course_type(id)) { throw std::invalid_argument("Course type doesn't exist."); }
	if (!course_type) { throw std::invalid_argument("Course type cannot be nullptr."); }
	// set the course type by id. (if the course type doesn't exist, it will throw an exception).
	m_course_types[id] = course_type;
}

void Course::add_course_type(Course_Type* course_type) {
	// check if the course type is nullptr.
	if (!course_type) {
		throw std::invalid_argument("Course type cannot be nullptr.");
	}
	const std::string id = course_type->get_id();
	// check if the course type already exists.
	if (get_course_type(id)) {
		throw std::invalid_argument("Course type with group id: " + id + " already exists.");
	}
	// add the course type to the course.
	m_course_types[id] = course_type;
}

void Course::remove_course_type(const std::string& course_type_id) {
	// check if the course doesn't exist.
	if (!get_course_type(course_type_id)) {
		throw std::invalid_argument("Course type with id: " + course_type_id + " doesn't exist.");
	}
	// delete the course type and avoid dangling pointers and erase it from the map.
	delete m_course_types[course_type_id];
	m_course_types[course_type_id] = nullptr;
	m_course_types.erase(course_type_id);
}

std::string Course::validate_id(const std::string& id) {
	// validate id (must be 5 characters long, and contain only digits).
	if (id.empty()) {
		throw std::invalid_argument("Course id cannot be empty.");
	}
	if (id.size() != 5) {
		throw std::invalid_argument("Course id must be 5 characters long.");
	}
	for (const char c : id) {
		if (!std::isdigit(c)) {
			throw std::invalid_argument("Course id must contain only digits.");
		}
	}
	return id;
}

std::string Course::validate_name(const std::string& name) {
	// validate name (cannot be empty).
	if (name.empty()) {
		throw std::invalid_argument("Course name cannot be empty.");
	}
	return name;
}

float Course::validate_points(const float points) {
	// validate points (must be positive).
	if (points <= 0) {
		throw std::invalid_argument("Course points must be positive.");
	}
	// check if the number has more than one decimal points.
	// multiply by 10 cast to int to round it down.
	const float frac_part = points * 10 - static_cast<int>(points * 10);
	if (frac_part != 0) { throw std::invalid_argument("Course points must have only one decimal point."); }

	// check if decimal points are .5 or .0.
	// multiply by 10 cast to int to round it down.
	if (static_cast<int>(points * 10) % 5 != 0) { throw std::invalid_argument("Course points must have .5 or .0 decimal points."); }
	return points;
}
