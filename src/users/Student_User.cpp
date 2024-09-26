#include "../../include/users/Student_User.h"

#include <iostream>

#include "../../libs/SchedulerLib/include/System_Operations.h"

Student_User::Student_User(const std::string& id, const std::string& password) : User(password), m_id(id) {
}

Student_User::Student_User(const Student_User& other) : User(other), m_id(other.m_id) {}

bool Student_User::execute(const std::string& command, const std::vector<std::string>& args) {
	// check if the user is in the schedule menu.
	if (is_schedule_menu) { return schedule_execute(command, args); }
	// then check the shared commands.
	bool found{true};
	try { User::execute(command, args); }
	catch (const std::invalid_argument&) { found = false; } // catch the error and continue.
	if (command == "Schedule") {
		if (validate_arg_size(args.size(), 0, command)) {
			is_schedule_menu = true;
			schedule_execute("Help", {});
			return true;
		}
	}
	// command not found, log error.
	if (!found) { std::cerr << "Error: command not found: " << command << std::endl; }
	return false;
}

bool Student_User::schedule_execute(const std::string& command, const std::vector<std::string>& args) {
	if (command == "Help") {
		help();
		return true;
	}
	if (command == "Print") {
		return validate_arg_size(args.size(), 1, command) &&
			System_Operations::get_student_schedule_manager(m_id)->print(args[0]);
	}
	if (command == "Printall") {
		return validate_arg_size(args.size(), 0, command) &&
			System_Operations::get_student_schedule_manager(m_id)->print_all();
	}
	if (command == "Addschedule") {
		return validate_arg_size(args.size(), 0, command) &&
			System_Operations::get_student_schedule_manager(m_id)->add_schedule();
	}
	if (command == "Rmschedule") {
		return validate_arg_size(args.size(), 1, command) &&
			System_Operations::get_student_schedule_manager(m_id)->rm_schedule(args[0]);
	}
	if (command == "Add") {
		return validate_arg_size(args.size(), 3, command) &&
			System_Operations::get_student_schedule_manager(m_id)->add_course(args[0], args[1], args[2]);
	}
	if (command == "Rm") {
		return validate_arg_size(args.size(), 3, command) &&
			System_Operations::get_student_schedule_manager(m_id)->rm_course(args[0], args[1], args[2]);
	}
	if (command == "Search") {
		return validate_arg_size(args.size(), 1, command) &&
			System_Operations::get_student_schedule_manager(m_id)->search(args[0]);
	}
	if (command == "Printsummary") {
		return validate_arg_size(args.size(), 1, command) &&
			System_Operations::get_student_schedule_manager(m_id)->print_weekly_summary(args[0]);
	}
	if (command == "Checkoverlap") {
		return validate_arg_size(args.size(), 1, command) &&
			System_Operations::get_student_schedule_manager(m_id)->check_overlapping_courses(args[0]);
	}
	if (command == "Back") {
		if (validate_arg_size(args.size(), 0, command)) {
			is_schedule_menu = false;
			return true;
		}
	}
	// command not found, log error.
	std::cerr << "Error: command not found: " << command << std::endl;
	return false;
}

void Student_User::help() const {
	User::help();
	if (!is_schedule_menu) {
		std::cout << "\nStudent commands:" << std::endl;
		std::cout << "Help - print the student menu." << std::endl;
		std::cout << "Schedule - go to the schedule menu." << std::endl;
	}
	else {
		std::cout << "\nSchedule menu:" << std::endl;
		std::cout << "Help - print the schedule menu." << std::endl;
		std::cout << "Print [schedule_id] - print the schedule." << std::endl;
		std::cout << "PrintAll - print all schedules." << std::endl;
		std::cout << "AddSchedule - add a schedule." << std::endl;
		std::cout << "RmSchedule [schedule_id] - remove a schedule." << std::endl;
		std::cout << "Add [schedule_id] [course_id] [group_id] - add a course to a schedule." << std::endl;
		std::cout << "Rm [schedule_id] [course_id] [group_id] - remove a course from a schedule." << std::endl;
		std::cout << "Search [course_id] [group_id] - search and print courses from a schedule." << std::endl;
		std::cout << "PrintSummary [schedule_id] - print the weekly summary of the schedule." << std::endl;
		std::cout << "Back - go back to the main menu." << std::endl;
	}
}
