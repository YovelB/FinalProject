#include "../../include/users/Admin_User.h"

#include <iostream>

#include "../../libs/SchedulerLib/include/System_Operations.h"

Admin_User::Admin_User(const std::string& password) : User(password) {}

Admin_User::Admin_User(const Admin_User& other) : User(other) {}

bool Admin_User::execute(const std::string& command, const std::vector<std::string>& args) {
	// check the shared commands.
	bool found{true};
	try { User::execute(command, args); }
	catch (const std::invalid_argument&) { found = false; } // catch the error and continue.

	if (command == "Addcourse") {
		return validate_arg_size(args.size(), 4, command) && System_Operations::add_course(
			args[0], args[1], args[2], args[3]);
	}
	if (command == "Rmcourse") {
		return validate_arg_size(args.size(), 1, command) &&
			System_Operations::rm_course(args[0]);
	}
	if (command == "Addlecturer") {
		return validate_arg_size(args.size(), 2, command) &&
			System_Operations::add_lecturer(args[0], args[1]);
	}
	if (command == "Rmlecturer") {
		return validate_arg_size(args.size(), 1, command) &&
			System_Operations::rm_lecturer(args[0]);
	}
	if (command == "Addstudent") {
		return validate_arg_size(args.size(), 3, command) &&
			System_Operations::add_student(args[0], args[1], args[2]);
	}
	if (command == "Rmstudent") {
		return validate_arg_size(args.size(), 1, command) &&
			System_Operations::rm_student(args[0]);
	}
	if (command == "Search") {
		return validate_arg_size(args.size(), 1, command) &&
			System_Operations::search(args[0]);
	}
	if (command == "Addlecture") {
		return validate_arg_size(args.size(), 7, command) &&
			System_Operations::add_course_type<Lecture>(args[0], args[1], args[2],
			                                            args[3],
			                                            args[4],
			                                            args[5], args[6]);
	}
	if (command == "Addtutorial") {
		return validate_arg_size(args.size(), 7, command) &&
			System_Operations::add_course_type<Tutorial>(args[0], args[1], args[2],
			                                             args[3],
			                                             args[4],
			                                             args[5], args[6]);
	}

	if (command == "Addlab") {
		return validate_arg_size(args.size(), 7, command) &&
			System_Operations::add_course_type<Lab>(args[0], args[1], args[2],
			                                        args[3],
			                                        args[4],
			                                        args[5], args[6]);
	}
	// command not found, log error.
	if (!found) { std::cerr << "Error: command not found: " << command << std::endl; }
	return false;
}

void Admin_User::help() const {
	User::help(); // print the shared commands.
	std::cout << "\nAdmin commands:" << std::endl;
	std::cout << "AddCourse [id] [course_name] [lecturer] [points] - add a course to the records." << std::endl;
	std::cout << "RmCourse [id] - remove a course from the records." << std::endl;
	std::cout << "AddLecturer [id] [lecturer_name] - add a lecturer to the records." << std::endl;
	std::cout << "RmLecturer [id] - remove a lecturer from the records." << std::endl;
	std::cout << "AddStudent [id] [student_name] [password] - add a student to the records." << std::endl;
	std::cout << "RmStudent [id] - remove a student from the records." << std::endl;
	std::cout << "Search [text] - search in the database and print the results." << std::endl;
	std::cout <<
		"Add(Lecture/Tutorial/Lab) [course_id] [group_id] [day] [HH:MM] [duration(min)] [lecturer] [classroom] - add a lecture to a course."
		<< std::endl;
}
