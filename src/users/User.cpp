#include "../../include/users/User.h"

#include <iostream>

#include "../../libs/SchedulerLib/include/System_Operations.h"

User::User(const std::string& password) : m_password{password} {}

User::User(const User& other) : m_password{other.m_password} {}

bool User::execute(const std::string& command, const std::vector<std::string>& args) {
	// shared commands for all users.
	if (command == "Help") {
		if (validate_arg_size(args.size(), 0, command)) {
			help();
			return true;
		}
	}
	if (command == "Clear") {
		return validate_arg_size(args.size(), 0, command) && (clear_screen(), true);
	}
	if (command == "Printcourse") {
		if (args.empty()) { return System_Operations::print_courses(10, true); }
		return validate_arg_size(args.size(), 1, command) && System_Operations::print_course(args[0]);
	}
	if (command == "Printlecturer") {
		return validate_arg_size(args.size(), 1, command) && System_Operations::print_teacher(args[0]);
	}
	if (command == "Printstudent") {
		return validate_arg_size(args.size(), 1, command) && System_Operations::print_student(args[0]);
	}
	if (command == "Printallcourses") {
		return validate_arg_size(args.size(), 0, command) && System_Operations::print_all_courses();
	}
	if (command == "Printalllecturers") {
		return validate_arg_size(args.size(), 0, command) && System_Operations::print_all_teachers();
	}
	if (command == "Printallstudents") {
		return validate_arg_size(args.size(), 0, command) && System_Operations::print_all_students();
	}
	if (command == "More") {
		return validate_arg_size(args.size(), 0, command) && System_Operations::print_more_courses();
	}
	// if a command was not found, throw an error.
	throw std::invalid_argument("Command: " + command + " was not found.");
}

void User::help() const {
	// print the available commands.
	std::cout << "Available commands:" << std::endl;
	std::cout << "Help - prints this menu." << std::endl;
	std::cout << "Clear - clear the screen." << std::endl;
	std::cout << "Logout - logout from the system." << std::endl;
	std::cout << "Exit - exit the system." << std::endl;
	std::cout << "Print(Course/lecturer/student) [id] - print the entity with the given id." << std::endl;
	std::cout << "Printall(Courses/lecturers/students) - print all entities of specific type." << std::endl;
	std::cout << "Print - print the the first 10 courses." << std::endl;
	std::cout << "More - print 10 more courses. (if available)" << std::endl;
}

void User::clear_screen() {
	// clear the screen.
	std::cout << "\033[2J\033[1;1H";
}

std::string User::get_password() const { return m_password; }

bool User::validate_arg_size(const size_t arg_size, const size_t expected_size, const std::string& cmd) {
	if (arg_size != expected_size) {
		std::cerr << "Error: invalid number of arguments for " << cmd << " command." << std::endl;
		return false;
	}
	return true;
}
