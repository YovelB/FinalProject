#ifndef STUDENT_User_H
#define STUDENT_User_H

#include "User.h"

class Schedule_Manager; // forward declaration since it used as a pointer or reference.

// Student class represents a student user.
class Student_User : public User {
	const std::string m_id{}; // student id.
	bool is_schedule_menu{false}; // flag to check if the student is in the schedule menu.
public:
	// constructors
	Student_User(const std::string& id, const std::string& password);
	Student_User(const Student_User& other);
	// theres no need for a destructor, since there is no ptrs or memory allocation (default used).
	// note: dont need to delete the schedule manager since it is managed by the system.

	// execute a command by given string and arguments (return true if execution completed).
	bool execute(const std::string& command, const std::vector<std::string>& args) override;

	// override the help method to print the student help menu.
	void help() const override;

	// student only methods.
	// schedule command execute menu (print, print all, add, remove, search, back).
	bool schedule_execute(const std::string& command, const std::vector<std::string>& args);
};

#endif //STUDENT_User_H
