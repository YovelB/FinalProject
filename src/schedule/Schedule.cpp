#include "Schedule.h"

#include <algorithm>

#include "../Entity_Manager.h"
#include "../data/Course.h"
#include "../data/course_types/Course_Type.h"

#include <iomanip>

Schedule::Schedule(const unsigned id) : m_id(id) {}

Schedule::Schedule(const Schedule& other) : m_id(other.m_id) {
	deep_copy_schedule(other);
}

Schedule::~Schedule() {
	// iterate over the courses and delete them.
	clean_up();
}

Schedule& Schedule::operator=(const Schedule& other) {
	if (this != &other) {
		// clear the current courses.
		clean_up();

		// copy the data from the other schedule.
		deep_copy_schedule(other);
	}
	return *this;
}

void Schedule::deep_copy_schedule(const Schedule& other) {
	for (const auto& [course_id, course_type_vector] : other.m_courses) {
		// iterate over the course types and clone them.
		for (const Course_Type* course_type : course_type_vector) {
			// if the course type is not nullptr, clone it and add it to the course.
			if (course_type) {
				// use clone() method to copy the instance of the course type.
				m_courses[course_id].push_back(course_type->clone());
			}
		}
	}
}

void Schedule::clean_up() {
	for (auto& [_, course_type_vector] : m_courses) {
		for (const Course_Type* course_type : course_type_vector) {
			delete course_type; // delete the course type and avoid dangling pointers.
			course_type = nullptr;
		}
		// clear the vector.
		course_type_vector.clear();
	}
	// clear the map.
	m_courses.clear();
}

Schedule Schedule::from_csv(const std::vector<std::string>& data) {
	// check if the data size is valid.
	if (data.empty()) { throw std::invalid_argument("Invalid CSV data to create a schedule."); }
	// create a new schedule object with the id.
	// note: stoi() - converts string to integer.
	//print data
	Schedule schedule(std::stoi(data[0]));

	const unsigned num_of_elements = 8;
	if ((data.size() - 1) % num_of_elements != 0) {
		throw std::invalid_argument("Invalid CSV data to create a schedule.");
	}
	// iterate over the data and add the course types to the schedule.
	for (unsigned i = 1; i < data.size(); i += num_of_elements) {
		const std::string& course_id = data[i];
		const std::string& type = data[i + 1];
		std::vector<std::string> course_type_data{data.begin() + i + 2, data.begin() + i + num_of_elements};

		Course_Type* course_type{};

		// depending on the type add the course type to the schedule.
		if (type == "Lecture") {
			course_type = Lecture::from_csv(course_type_data);
		}
		else if (type == "Tutorial") {
			course_type = Tutorial::from_csv(course_type_data);
		}
		else if (type == "Lab") {
			course_type = Lab::from_csv(course_type_data);
		} else {
			throw std::invalid_argument("Invalid course type: " + type);
		}
		// add the course type to the schedule.
		schedule.add_course_type(course_id, course_type);
	}
	return schedule;
}


std::vector<std::string> Schedule::to_csv() const {
	std::vector<std::string> data{};
	// add the schedule id to the data vector.
	//print data to the csv file.
	for (const std::string& value : data) {
		std::cout << value << ",";
	}
	std::cout << std::endl;

	data.push_back(std::to_string(m_id));
	// iterate over the courses and add the course types to the data vector.
	for (const auto& [course_id, course_type_vector] : m_courses) {
		for (const Course_Type* course_type : course_type_vector) {
			if (course_type != nullptr) {
				// print the course type
				data.push_back(course_id); // add the course id.
				data.push_back(course_type->get_type()); // add the course type.
				// add the course type data to the data vector.
				std::vector<std::string> course_type_data = course_type->to_csv();
				// use insert() method to add the course type data to the data vector.
				data.insert(data.end(), course_type_data.begin(), course_type_data.end());
			}
		}
	}
	return data;
}

void Schedule::add_course_type(const std::string& course_id, Course_Type* course_type) {
	// dont validated the course and course type since they are being cloned.
	// add the course type to the schedule.
	m_courses[course_id].push_back(course_type);
}

void Schedule::add_course_type(const std::string& course_id, const std::string& group_id) {
	// validate the course and course type.
	validate_course_and_type(course_id, group_id);

	// get the Course* from the system (entity manager).
	const Course* course = dynamic_cast<Course*>(Entity_Manager::get_instance().get_entity(course_id));
	// get the Course_Type* from the course.
	Course_Type* course_type = course->get_course_type(group_id)->clone();

	// add the course type to the schedule.
	m_courses[course_id].push_back(course_type);
}

void Schedule::remove_course_type(const std::string& course_id, const std::string& group_id) {
	// check if course exists in the schedule.
	const auto course_it = m_courses.find(course_id);
	if (course_it == m_courses.end()) {
		throw std::invalid_argument("Course with id: " + course_id + " does not exist in the schedule.");
	}

	// get the course type vector from the map.
	std::vector<Course_Type*>& course_types = course_it->second;
	bool found = false;

	// iterate over the course types and remove the course type.
	for (auto it = course_types.begin(); it != course_types.end(); ++it) {
		if ((*it)->get_id() == group_id) {
			delete *it; // Delete the course type to avoid memory leak
			course_types.erase(it); // Erase the course type from the vector
			found = true;
			break; // Break the loop after removing the course type
		}
	}

	// If the course type vector is now empty, remove the course from the map
	if (course_types.empty()) {
		m_courses.erase(course_it);
	}

	// If the course type wasn't found, throw an exception
	if (!found) {
		throw std::invalid_argument("Course_Type with id: " + group_id + " does not exist in the schedule.");
	}
}

bool Schedule::search(const std::string& course_id) const {
	bool found_any{};
	found_any = search_by_type(course_id, "Lecture");
	found_any |= search_by_type(course_id, "Tutorial");
	found_any |= search_by_type(course_id, "Lab");
	// if didn't find any, throw an exception.
	if (!found_any) {
		return false;
	}
	return true;
}

bool Schedule::search_by_type(const std::string& course_id, const std::string& type) const {
	bool found{};
	for (const auto& [id, course_type_vector] : m_courses) {
		if (id == course_id) {
			// if course_type_vector is empty, throw an exception.
			if (course_type_vector.empty()) {
				throw std::invalid_argument("No " + type + "s found for course with id: " + course_id);
			}
			// print the course types with the given type.
			for (const Course_Type* course_type : course_type_vector) {
				if (course_type->get_type() == type) {
					if (!found) {
						std::cout << "Schedule with id: " << m_id << std::endl;
						std::cout << "Found in " << type << "s:" << std::endl;
						found = true;
					}
					std::cout << *course_type << "\n"; // print the course type.
				}
			}
			return true; // found.
		}
	}
	return false; // not found.
}


void Schedule::print_weekly_summary() const {
	check_empty();
	double weekly_hours{}; // total weekly hours for all courses.
	float total_points{}; // total points for all courses.

	for (const auto& [course_id, course_type_vector] : m_courses) {
		// iterate over the course types and calculate the total weekly hours.
		for (const Course_Type* course_type : course_type_vector) {
			// sum hours (since it is a double the mins are factored in).
			weekly_hours += course_type->get_duration() / 60.0;

			// get the course and dynamic cast it from Entity* to Course*.
			const Course* course = dynamic_cast<Course*>(Entity_Manager::get_instance().get_entity(course_id));
			total_points += course->get_points(); // add the points to the total points.
		}
	}
	// print the weekly hours and total points.
	std::cout << "Total weekly hours: " << weekly_hours << std::endl;
	std::cout << "Total points: " << total_points << std::endl;
}

void Schedule::print_overlapping_courses() const {
	// create a vector of pairs to store the overlapping courses.
	std::vector<std::pair<const Course_Type*, const Course_Type*>> overlaps{};

	// loop over all course types and check for overlaps.
	for (const auto& [course_id1, course_type_vector1] : m_courses) {
		for (const Course_Type* course1 : course_type_vector1) {
			// compare with all other course types.
			for (const auto& [course_id2, course_type_vector2] : m_courses) {
				for (const Course_Type* course2 : course_type_vector2) {
					// skip if the same course type.
					if (course1 == course2) { continue; }
					// check if the course types are the same or if they overlap.
					if (course1->get_day() == course2->get_day() &&
						find_overlapping_courses(course1, course2)) {
						// add the overlapping course types to the vector.
						overlaps.push_back({course1, course2});
					}
				}
			}
		}
	}
	// print the overlapping courses.
	if (overlaps.empty()) {
		std::cout << "No overlapping courses found." << std::endl;
	}
	else {
		std::cout << "Overlapping courses:" << std::endl;
		for (const auto& [course1, course2] : overlaps) {
			std::cout << "Course 1: " << *course1 << std::endl;
			std::cout << "Course 2: " << *course2 << std::endl;
		}
	}
}

bool Schedule::find_overlapping_courses(const Course_Type* course1, const Course_Type* course2) {
	// calculate the start and end times for the courses.
	const unsigned start1 = course1->get_start_time().tm_hour * 60 + course1->get_start_time().tm_min;
	const unsigned end1 = start1 + course1->get_duration();
	const unsigned start2 = course2->get_start_time().tm_hour * 60 + course2->get_start_time().tm_min;
	const unsigned end2 = start2 + course2->get_duration();

	// check if the courses overlap.
	return (start1 < end2 && start2 < end1);
}

void Schedule::check_empty() const {
	if (m_courses.empty()) {
		throw std::invalid_argument("No courses in the schedule.");
	}
}

const Course_Type* Schedule::get_course_type(const std::string& course_id, const std::string& group_id) const {
	// find() method returns an iterator to the element if found, else returns the end iterator.
	if (const auto it = m_courses.find(course_id); it != m_courses.end()) {
		for (const Course_Type* course_type : it->second) {
			if (course_type->get_id() == group_id) {
				return course_type;
			}
		}
	}
	return nullptr; // not found, return nullptr.
}

bool Schedule::course_exists(const std::string& course_id) const {
	// check if the course id exists in the m_courses map.
	// find() method returns an iterator to the element if found, else returns the end iterator.
	return m_courses.find(course_id) != m_courses.end();
}

bool Schedule::course_type_exists(const std::string& course_id, const std::string& group_id) const {
	// check if the course type exists in the m_courses map.
	// find() method returns an iterator to the element if found, else returns the end iterator.
	if (const auto it = m_courses.find(course_id); it != m_courses.end()) {
		for (const Course_Type* course_type : it->second) {
			if (course_type) {
				if (course_type->get_id() == group_id) { return true; } // found.
			}
		}
	}
	return false; // not found.
}

unsigned Schedule::get_id() const { return m_id; }

void Schedule::set_id(const unsigned id) { m_id = id; }

void Schedule::validate_course_and_type(const std::string& course_id, const std::string& group_id) const {
	const Course* course = dynamic_cast<Course*>(Entity_Manager::get_instance().get_entity(course_id));
	// check if course exists in system (entity manager).
	if (!course) { throw std::invalid_argument("Course with id: " + course_id + " does not exist."); }

	Course_Type* course_type = course->get_course_type(group_id);
	// check if course type exists in the system (course).
	if (!course_type) { throw std::invalid_argument("Course_Type with id: " + group_id + " does not exist."); }

	// check if the course type already exist in the schedule.
	if (course_type_exists(course_id, group_id)) {
		throw std::invalid_argument("Course_Type with id: " + group_id + " already exists in the schedule.");
	}
}


std::string Schedule::to_string() const {
	// create a string stream to store the schedule data.
	std::ostringstream out{};

	// define the days and hours for the schedule.
	const std::vector<std::string> days = {
		"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
		};
	const std::vector<unsigned> hours = {7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21};

	/*prepare a map of schedule data to store course types.
	keys are the days and values are 2d vectors representing the hours and course types.
	the 2d vector is used to store multiple course types for the same hour (since there can be overlap).*/
	const std::unordered_map<std::string, std::vector<std::vector<std::string>>> schedule_data =
		create_schedule_Data(days, hours.size());

	// Print header (days) of the schedule.
	print_header(out, days);

	// print schedule body.
	print_schedule_body(out, schedule_data, days, hours);

	return out.str();
}

std::ostream& operator<<(std::ostream& os, const Schedule& schedule) {
	os << schedule.to_string();
	return os;
}

void Schedule::populate_schedule_data(
	std::unordered_map<std::string, std::vector<std::vector<std::string>>>& schedule_data,
	const unsigned hours_size) const {
	for (const auto& [course_id, course_type_vector] : m_courses) {
		// iterate over the course types and add them to the schedule data.
		for (const Course_Type* course_type : course_type_vector) {
			if (course_type) {
				add_course_type_to_schedule(schedule_data, course_id, course_type, hours_size);
			}
		}
	}
}

void Schedule::add_course_type_to_schedule(
	std::unordered_map<std::string, std::vector<std::vector<std::string>>>& schedule_data, const std::string& course_id,
	const Course_Type* course_type, const unsigned hours_size) {
	const std::string& curr_day = course_type->get_day();
	const unsigned start_hour = course_type->get_start_time().tm_hour;
	const unsigned start_min = course_type->get_start_time().tm_min;
	const unsigned duration = course_type->get_duration();
	// calculate the end hour of the course type.
	unsigned end_hour = (start_hour * 60 + start_min + duration) / 60;
	if (start_hour == end_hour) end_hour++; // if the course duration is less than an hour, set it to 1 hour.
	if (duration == 0) { return; } // if the duration is 0, skip the course type.

	// loop through the hours and add the course type to the schedule data (21 is the end hour).
	for (unsigned curr_hour = start_hour; curr_hour < end_hour && curr_hour <= 21; curr_hour++) {
		const unsigned curr_hour_index = curr_hour - 7; // (7 - is the start hour, so the index will be ref to 0).
		if (curr_hour_index < hours_size) {
			// add the course type to the schedule data. (course_id, course_type, classroom)
			schedule_data[curr_day][curr_hour_index].push_back(
				course_id + " " + course_type->get_type() + " " + course_type->get_classroom());
		}
	}
}

std::unordered_map<std::string, std::vector<std::vector<std::string>>> Schedule::create_schedule_Data(
	const std::vector<std::string>& days, const unsigned hours_size) const {
	std::unordered_map<std::string, std::vector<std::vector<std::string>>> schedule_data{};

	// loop through the days and create the schedule data.
	for (const std::string& day : days) {
		// Initialize the schedule data map
		schedule_data[day] = std::vector<std::vector<std::string>>(hours_size);
	}
	// populate the schedule data with course types.
	populate_schedule_data(schedule_data, hours_size);
	return schedule_data;
}

void Schedule::print_header(std::ostringstream& out, const std::vector<std::string>& days) {
	// print the header of the schedule.
	out << std::setw(8) << "Time";
	for (const std::string& day : days) {
		out << " | " << std::setw(20) << std::left << day;
	}
	out << "\n" << std::string(8 + 23 * days.size(), '-') << std::endl;
}

void Schedule::print_schedule_body(std::ostringstream& out,
                                   const std::unordered_map<std::string, std::vector<std::vector<std::string>>>&
                                   schedule_data,
                                   const std::vector<std::string>& days, const std::vector<unsigned>& hours) {
	// loop through the hours and days to print the schedule data.
	for (const unsigned int hour : hours) {
		out << std::setw(5) << std::right << hour << ":00";
		unsigned max_courses = get_max_courses_for_hour(schedule_data, days, hour - 7);

		// Print courses for this hour
		for (unsigned course_index = 0; course_index < std::max(1u, max_courses); ++course_index) {
			if (course_index > 0) {
				out << std::setw(8) << ""; // Indent for additional courses in the same hour
			}

			for (const auto& day : days) {
				out << " | ";
				const auto& day_schedule = schedule_data.find(day);
				if (day_schedule != schedule_data.end() && hour - 7 < day_schedule->second.size() &&
					course_index < day_schedule->second[hour - 7].size()) {
					out << std::setw(20) << std::left << day_schedule->second[hour - 7][course_index];
				}
				else {
					out << std::setw(20) << "";
				}
			}
			out << "\n";
		}

		// Add a separator line between hours
		out << std::string(8 + 23 * days.size(), '-') << "\n";
	}
}

unsigned Schedule::get_max_courses_for_hour(
	const std::unordered_map<std::string, std::vector<std::vector<std::string>>>& schedule_data,
	const std::vector<std::string>& days, const unsigned hour_index) {
	unsigned max_courses{0};
	// basic loop to get the max number of courses for an hour.
	for (const std::string& day : days) {
		// get the course types for the current day and hour.
		// note: horrible usage of map.at() and vector.at() without checking if the key/index exists.
		const std::vector<std::string>& course_types = schedule_data.at(day).at(hour_index);
		if (course_types.size() > max_courses) {
			max_courses = course_types.size();
		}
	}
	return max_courses;
}
