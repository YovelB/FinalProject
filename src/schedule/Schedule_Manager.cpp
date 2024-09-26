#include "Schedule_Manager.h"

#include <iostream>
#include <ostream>

#include "../CSV_Editor.h"

unsigned Schedule_Manager::m_id_counter = 1;

Schedule_Manager::Schedule_Manager(const std::string& id) : m_student_id(id) {
	try {
		read_schedules(); // read the schedules from the file.
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl; // log the error.
	}
}

Schedule_Manager::Schedule_Manager(const Schedule_Manager& other) : m_student_id(other.m_student_id) {
	// deep copy the schedules.
	for (const Schedule& schedule : other.m_schedules) {
		m_schedules.push_back(schedule);
	}
}

Schedule_Manager::~Schedule_Manager() {
	try {
		write_schedules(); // write the schedules to the file.
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl; // log the error.
	}
}

void Schedule_Manager::read_schedules() {
	try {
		// get the file name.
		const std::string file_name = m_student_id + "_schedules.csv";
		// read data from the file.
		std::vector<std::vector<std::string>> data = CSV_Editor::read_csv(file_name);
		// clear the existing schedules.
		m_schedules.clear();
		// iterate over the data and create schedules.
		for (const std::vector<std::string>& row : data) {
			// create a new schedule object with the data.
			m_schedules.push_back(Schedule::from_csv(row));
		}
		reset_schedule_ids(); // reset the schedule ids.
		data.clear(); // clear the data vector.
	}
	catch (const std::exception& e) {
		// log the error and throw an exception again.
		std::cerr << "Error in reading schedules: " << e.what() << std::endl;
		throw;
	}
}

void Schedule_Manager::write_schedules() {
	try {
		const std::string file_name = m_student_id + "_schedules.csv"; // get the file name.
		// create a vector of vectors of strings to store the data.
		std::vector<std::vector<std::string>> data{};
		// iterate over the schedules and add them to the data vector.
		for (const Schedule& schedule : m_schedules) {
			data.push_back(schedule.to_csv());
		}
		// write the data to the file.
		CSV_Editor::write_csv(file_name, data);
		data.clear(); // clear the data vector.
	}
	catch (const std::exception& e) {
		// log the error and throw an exception again.
		std::cerr << "Error in writing schedules: " << e.what() << std::endl;
		throw;
	}
}

bool Schedule_Manager::print(const std::string& id) const {
	// get the schedule by id.
	try {
		check_schedules(); // check if m_schedules is empty.
		std::cout << get_schedule(std::stoi(id)) << std::endl;
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error printing schedule with id " << id << ": " << e.what() << std::endl;
		return false;
	}
}

bool Schedule_Manager::print_all() const {
	// iterate over the schedules and print them.
	try {
		check_schedules(); // check if m_schedules is empty.
		for (const Schedule& schedule : m_schedules) {
			std::cout << "Schedule id: " << schedule.get_id() << " : " << m_id_counter << std::endl;
			std::cout << schedule << std::endl;
		}
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error printing schedules: " << e.what() << std::endl;
		return false;
	}
}

bool Schedule_Manager::add_schedule() {
	try {
		// create a new schedule and add it to the schedules vector.
		m_id_counter++; // increment the id counter.
		m_schedules.push_back(Schedule(m_id_counter));
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error adding schedule: " << e.what() << std::endl;
		return false;
	}
}

bool Schedule_Manager::rm_schedule(const std::string& id) {
	// find the schedule by id and remove it.
	try {
		check_schedules(); // check if m_schedules is empty.
		for (auto it = m_schedules.begin(); it != m_schedules.end(); ++it) {
			if (it->get_id() == std::stoi(id)) {
				// erase the schedule from the vector.
				m_schedules.erase(it);
				// reset the schedule ids.
				reset_schedule_ids();
				return true;
			}
		}
		throw std::invalid_argument("Schedule with id " + id + " not found.");
	}
	catch (const std::exception& e) {
		std::cerr << "Error removing schedule: " << e.what() << std::endl;
		return false;
	}
}

bool Schedule_Manager::add_course(const std::string& id, const std::string& course_id, const std::string& group_id) {
	try {
		// get the schedule by id and add the course.
		get_schedule(std::stoi(id)).add_course_type(course_id, group_id);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error adding course to schedule " << id << ": " << e.what() << std::endl;
		return false;
	}
}

bool Schedule_Manager::rm_course(const std::string& id, const std::string& course_id, const std::string& group_id) {
	try {
		// get the schedule by id and remove the course.
		get_schedule(std::stoi(id)).remove_course_type(course_id, group_id);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error removing course from schedule " << id << ": " << e.what() << std::endl;
		return false;
	}
}

bool Schedule_Manager::search(const std::string& course_id) const {
	try {
		check_schedules(); // check if m_schedules is empty.
		bool found_any{false};
		// iterate over the schedules and search for the course.
		for (const Schedule& schedule : m_schedules) {
			if (schedule.search(course_id)) {
				found_any = true;
			}
		}
		if (!found_any) {
			std::cerr << "No results found for course id: " << course_id << " in all schedules." << std::endl;
		}
		return found_any;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return false;
	}
}

bool Schedule_Manager::print_weekly_summary(const std::string& id) const {
	try {
		get_schedule(std::stoi(id)).print_weekly_summary();
		return true;
	} catch (const std::exception& e) {
		std::cerr << "Error printing weekly summary for schedule: " << e.what() << std::endl;
		return false;
	}
}

bool Schedule_Manager::check_overlapping_courses(const std::string& id) const {
	try {
		get_schedule(std::stoi(id)).print_overlapping_courses();
		return true;
	} catch (const std::exception& e) {
		std::cerr << "Error checking overlapping courses for schedule: " << e.what() << std::endl;
		return false;
	}
}


Schedule& Schedule_Manager::get_schedule(const unsigned id) {
	// check if m_schedules is empty.
	check_schedules();
	// check id bounds.
	if (id < 1 || id > m_schedules.size()) {
		throw std::invalid_argument("Schedule id " + std::to_string(id) + " out of bounds.");
	}
	// vector index starts from 0, so we need to subtract 1 from the id.
	return m_schedules[id - 1];
}

const Schedule& Schedule_Manager::get_schedule(const unsigned id) const {
	// const_cast just just get the schedule and make it const.
	return const_cast<Schedule_Manager*>(this)->get_schedule(id);
}

void Schedule_Manager::reset_schedule_ids() {
	// iterate over the schedules and reset the ids. (so they are in order).
	unsigned i{0};
	for (; i < m_schedules.size(); i++) {
		m_schedules[i].set_id(i + 1);
	}
	m_id_counter = i; // set the id counter to the max id.
}

void Schedule_Manager::check_schedules() const {
	if (m_schedules.empty()) { throw std::runtime_error("No schedules available."); }
}
