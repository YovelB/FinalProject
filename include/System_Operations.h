#ifndef SYSTEM_OPERATIONS_H
#define SYSTEM_OPERATIONS_H

#include <string>
#include <vector>

#include "Entity_Manager.h"
#include "data/Student.h"
#include "schedule/Schedule_Manager.h"

// forward declarations since they are used as pointers or references.

// System Operations is a static utility class to perform operations on the csv files data.
// it does this by using the Entity Manager to manage the entities and Schedule Manager to manage the schedules.
class System_Operations {
	// static member to keep track of the curr course index.
	static size_t curr_course_index;

	// static memeber to keep track of the curr Schedule_Manager instace for a student.
	static Student* curr_student;

	// private constructor and destructor to prevent instantiation.
	System_Operations() = default;
	~System_Operations() = default;

	// validate string to number conversion.
	static void validate_number(const std::string& str) {
		try {
			// try to convert the string to a number.
			std::stol(str);
		}
		catch (const std::exception&) {
			std::cerr << "Error: invalid number: " << str << std::endl;
		}
	}

public:
	// course operations.
	// print a course by id.
	static bool print_course(const std::string& id);
	// print the first count courses, if reset index is true, reset the index to 0.
	static bool print_courses(size_t count = 10, bool reset_index = true);
	// print the next count courses.
	static bool print_more_courses(size_t count = 10);
	// print all courses.
	static bool print_all_courses();

	// add and remove a course.
	static bool add_course(const std::string& id, const std::string& name, const std::string& lecturer,
	                       const std::string& points);
	static bool rm_course(const std::string& id);

	// add and remove a lecture, tutorial, or lab course to a course id.
	template <typename T>
	static bool add_course_type(const std::string& course_id, const std::string& group_id,
	                            const std::string& m_day, const std::string& m_start_time,
	                            const std::string& m_duration,
	                            const std::string& m_lecturer, const std::string& m_classroom) {
		Course_Type* course_type{};
		try {
			// create a new lecture course type.
			course_type = new T(group_id, m_day, m_start_time, std::stol(m_duration), m_lecturer, m_classroom);
			// get the course entity and downcast it to the Course type.
			Course* course = dynamic_cast<Course*>(Entity_Manager::get_instance().get_entity(course_id));
			// check if course exists in entity manager.
			if (!course) { throw std::invalid_argument("Course with id: " + course_id + " does not exist."); }
			// add the course type to the course.
			Entity_Manager::get_instance().add_entity<T>(course_type, course);
			return true;
		}
		catch (const std::exception& e) {
			std::cerr << "Error adding course type: " << e.what() << std::endl;
			delete course_type; // delete the course type when an exception occurs.
			course_type = nullptr;
			return false;
		}
	}

	template <typename T>
	static bool rm_course_type(const std::string& course_id, const std::string& group_id) {
		try {
			// get the course entity and downcast it to the Course type.
			Entity* course = dynamic_cast<Course*>(Entity_Manager::get_instance().get_entity(course_id));
			// check if course exists in entity manager.
			if (!course) { throw std::invalid_argument("Course with id: " + course_id + " does not exist."); }
			// remove the course type from the course.
			Entity_Manager::get_instance().remove_entity<T>(group_id, course);
			return true;
		}
		catch (const std::exception& e) {
			std::cerr << "Error removing course type: " << e.what() << std::endl;
			return false;
		}
	}

	// teacher operations.
	// print a teacher by id.
	static bool print_teacher(const std::string& id);
	// print all teachers.
	static bool print_all_teachers();
	// add and remove a teacher.
	static bool add_lecturer(const std::string& id, const std::string& name);
	static bool rm_lecturer(const std::string& id);

	// student operations.
	// print a student by id.
	static bool print_student(const std::string& id);
	// print all students.
	static bool print_all_students();
	// add and remove a student.
	static bool add_student(const std::string& id, const std::string& name, const std::string& password);
	static bool rm_student(const std::string& id);

	// search for text in all entities (courses, teachers, students, etc).
	static bool search(const std::string& text);

	// authenticate a student by id and password (returns true if student exists in record).
	static bool authenticate_student(const std::string& id, const std::string& password);
	// get a pointer to the student schedule manager by id.
	static Schedule_Manager* get_student_schedule_manager(const std::string& id);
};

#endif //SYSTEM_OPERATIONS_H
