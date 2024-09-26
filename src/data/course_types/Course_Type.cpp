#include "Course_Type.h"

#include <iomanip>
#include <sstream>

Course_Type::Course_Type(const std::string& id, const std::string& day, const std::string& start_time,
                         const unsigned duration, const std::string& lecturer, const std::string& classroom) :
	m_id{validate_id(id)}, m_day{validate_day(day)}, m_start_time{validate_start_time(string_to_time(start_time))},
	m_duration{validate_duration(duration)}, m_lecturer{validate_lecturer(lecturer)},
	m_classroom{validate_classroom(classroom)} {}

Course_Type::Course_Type(const Course_Type& other) :
	m_id{other.m_id}, m_day{other.m_day}, m_start_time{other.m_start_time},
	m_duration{other.m_duration}, m_lecturer{other.m_lecturer}, m_classroom{other.m_classroom} {}

std::vector<std::string> Course_Type::to_csv() const {
	// using to_string to convert the integer values to strings.
	return std::vector{
		m_id, m_day, time_to_string(m_start_time),
		std::to_string(m_duration), m_lecturer, m_classroom
		};
}

bool Course_Type::search(const std::string& text) const {
	// to simplify the search, we convert the course type data to a CSV format and search in it.
	// loop through the CSV data and search for the text.
	for (const std::string& str : to_csv()) {
		// if the text is found in the string, return true.
		if (str.find(text) != std::string::npos) {
			return true;
		}
	}
	return false;
}

std::string Course_Type::get_id() const { return m_id; }
void Course_Type::set_id(const std::string& group_id) { m_id = validate_id(group_id); }

std::string Course_Type::get_name() const { return m_lecturer; }
void Course_Type::set_name(const std::string& lecturer) { m_lecturer = validate_lecturer(lecturer); }

std::tm Course_Type::get_start_time() const { return m_start_time; }
void Course_Type::set_start_time(const std::tm& start_time) { m_start_time = validate_start_time(start_time); }

unsigned Course_Type::get_duration() const { return m_duration; }
void Course_Type::set_duration(const unsigned duration) { m_duration = validate_duration(duration); }

std::string Course_Type::get_day() const { return m_day; }
void Course_Type::set_day(const std::string& day) { m_day = validate_day(day); }

std::string Course_Type::get_classroom() const { return m_classroom; }
void Course_Type::set_classroom(const std::string& classroom) { m_classroom = validate_classroom(classroom); }

std::string Course_Type::to_string() const {
	// use a string stream and setw() to format the output.
	std::ostringstream out{};
	out << std::setw(8) << "ID: " << m_id << ", Day: " << std::left << std::setw(9) << m_day <<
		", Start Time: " << time_to_string(m_start_time) << ", Duration: " << std::left << std::setw(3) << m_duration <<
		", Lecturer: " << std::left << std::setw(13) << m_lecturer << ", Classroom: " << m_classroom;
	return out.str();
}

std::ostream& operator<<(std::ostream& os, const Course_Type& course_type) {
	os << course_type.to_string();
	return os;
}

std::string Course_Type::validate_id(const std::string& group_id) {
	// validate group_id (must be 2 characters long, and contain only digits).
	if (group_id.empty()) {
		throw std::invalid_argument("Course group id cannot be empty.");
	}
	if (group_id.size() != 2) {
		throw std::invalid_argument("Course group id must be 2 characters long.");
	}
	for (const char c : group_id) {
		if (!std::isdigit(c)) {
			throw std::invalid_argument("Course group id must contain only digits.");
		}
	}
	return group_id;
}

std::string Course_Type::validate_day(const std::string& day) {
	// validate day (must be a valid day).
	if (day.empty()) {
		throw std::invalid_argument("Course day cannot be empty.");
	}
	if (day != "Sunday" && day != "Monday" && day != "Tuesday" && day != "Wednesday" &&
		day != "Thursday" && day != "Friday" && day != "Saturday") {
		throw std::invalid_argument("Course day must be a valid day.");
	}
	return day;
}

std::tm Course_Type::validate_start_time(const std::tm& start_time) {
	// validate start_time (must be a valid time).
	if (start_time.tm_hour < 0 || start_time.tm_hour > 23) {
		throw std::invalid_argument("Course start time must be between 0 and 23.");
	}
	if (start_time.tm_min < 0 || start_time.tm_min > 59) {
		throw std::invalid_argument("Course start time must be between 0 and 59.");
	}
	// no need to validate start_time.tm_sec since it's not used.
	return start_time;
}

std::string Course_Type::validate_start_time(const std::string& start_time) {
	// check if the start_time is empty.
	if (start_time.empty()) { throw std::invalid_argument("Course start time cannot be empty."); }
	// validate start_time (must be in HH:MM format).
	if (start_time.size() != 5 || start_time[2] != ':') {
		throw std::invalid_argument("Course start time must be in HH:MM format.");
	}
	return start_time;
}

unsigned Course_Type::validate_duration(const unsigned duration) {
	// validate duration (must be positive).
	if (duration == 0) {
		throw std::invalid_argument("Course duration must be positive.");
	}
	return duration;
}

std::string Course_Type::validate_lecturer(const std::string& lecturer) {
	// validate lecturer (cannot be empty).
	if (lecturer.empty()) {
		throw std::invalid_argument("Course lecturer cannot be empty.");
	}
	return lecturer;
}

std::string Course_Type::validate_classroom(const std::string& classroom) {
	// validate classroom (cannot be empty).
	if (classroom.empty()) {
		throw std::invalid_argument("Course classroom cannot be empty.");
	}
	return classroom;
}

std::string Course_Type::time_to_string(const std::tm& start_time) {
	// ostringstream is for writing the hours and minutes to the string. (output only stream).
	std::ostringstream out;
	out << (start_time.tm_hour < 10 ? "0" : "") << start_time.tm_hour << ":" <<
		(start_time.tm_min < 10 ? "0" : "") << start_time.tm_min;
	return out.str();
}
std::tm Course_Type::string_to_time(const std::string& start_time) {
	// string stream is for reading the hours and minutes from the string. (input and output stream).
	std::tm tm{};
	std::stringstream ss{start_time};
	char delimter{':'};
	// read the hours and minutes from the string stream.
	ss >> tm.tm_hour >> delimter >> tm.tm_min;
	// validate the input.
	if (ss.fail() || delimter != ':' || tm.tm_hour < 0 || tm.tm_hour > 23 || tm.tm_min < 0 || tm.tm_min > 59) {
		throw std::invalid_argument("Invalid start time format.");
	}
	return tm;
}
