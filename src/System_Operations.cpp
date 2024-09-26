#include "System_Operations.h"

#include "schedule/Schedule_Manager.h"
#include "data/Student.h"
#include "data/Teacher.h"

size_t System_Operations::curr_course_index = 0;

Student* System_Operations::curr_student = nullptr;

bool System_Operations::print_course(const std::string& id) {
	try {
		Entity_Manager::get_instance().print_entity<Course>(id);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error printing course: " << e.what() << std::endl;
		return false;
	}
}

bool System_Operations::print_courses(const size_t count, const bool reset_index) {
	try {
		// get entity order of courses.
		const std::vector<std::string>& order = Entity_Manager::get_instance().get_entity_order<Course>();
		// get the minimum of the count and the number of courses.
		const size_t min_count = std::min(curr_course_index + count, order.size());
		// if reset index is true, reset the curr course index. and print message.
		if (reset_index) {
			curr_course_index = 0;
			std::cout << "Printing courses:" << std::endl;
		}
		size_t curr_index{curr_course_index};
		// loop through the courses indexes in order
		for (; curr_index < min_count; curr_index++) {
			// print the course by index.
			print_course(order[curr_index]);
		}
		// update the curr course index.
		curr_course_index = curr_index;
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error printing courses: " << e.what() << std::endl;
		return false;
	}
}

bool System_Operations::print_more_courses(const size_t count) {
	// if the curr course index is greater then the size of the courses, print message.
	try {
		if (curr_course_index >= Entity_Manager::get_instance().get_entity_order<Course>().size()) {
			std::cout << "No more courses to print." << std::endl;
			curr_course_index = 0;
			return true;
		}
		print_courses(count, false);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error printing more courses: " << e.what() << std::endl;
		return false;
	}
}

bool System_Operations::print_all_courses() {
	try {
		Entity_Manager::get_instance().print_entities<Course>();
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error printing all courses: " << e.what() << std::endl;
		return false;
	}
}

bool System_Operations::add_course(const std::string& id, const std::string& name, const std::string& lecturer,
                                   const std::string& points) {
	Course* course{};
	try {
		course = new Course(id, name, lecturer, std::stof(points));
		// use the entity manager to add the course.
		Entity_Manager::get_instance().add_entity<Course>(course);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error adding course: " << e.what() << std::endl;
		delete course; // delete the course when an exception occurs.
		course = nullptr;
		return false;
	}
}

bool System_Operations::rm_course(const std::string& id) {
	try {
		// remove the course from the entity manager.
		Entity_Manager::get_instance().remove_entity<Course>(id);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error removing course: " << e.what() << std::endl;
		return false;
	}
}

bool System_Operations::print_teacher(const std::string& id) {
	try {
		Entity_Manager::get_instance().print_entity<Teacher>(id);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error printing teacher: " << e.what() << std::endl;
		return false;
	}
}

bool System_Operations::print_all_teachers() {
	try {
		Entity_Manager::get_instance().print_entities<Teacher>();
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error printing teachers: " << e.what() << std::endl;
		return false;
	}
}

bool System_Operations::add_lecturer(const std::string& id, const std::string& name) {
	Teacher* teacher{};
	try {
		teacher = new Teacher(id, name);
		// add teacher to the entity manager.
		Entity_Manager::get_instance().add_entity<Teacher>(teacher);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error adding lecturer: " << e.what() << std::endl;
		delete teacher; // delete the teacher when an exception occurs.
		teacher = nullptr;
		return false;
	}
}

bool System_Operations::rm_lecturer(const std::string& id) {
	try {
		// remove the teacher from the entity manager.
		Entity_Manager::get_instance().remove_entity<Teacher>(id);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error removing lecturer: " << e.what() << std::endl;
		return false;
	}
}

bool System_Operations::print_student(const std::string& id) {
	try {
		Entity_Manager::get_instance().print_entity<Student>(id);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error printing student: " << e.what() << std::endl;
		return false;
	}
}

bool System_Operations::print_all_students() {
	try {
		Entity_Manager::get_instance().print_entities<Student>();
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error printing students: " << e.what() << std::endl;
		return false;
	}
}

bool System_Operations::add_student(const std::string& id, const std::string& name, const std::string& password) {
	Student* student{};
	try {
		student = new Student(id, name, password);
		// add student to the entity manager.
		Entity_Manager::get_instance().add_entity<Student>(student);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error adding student: " << e.what() << std::endl;
		delete student; // delete the student when an exception occurs.
		student = nullptr;
		return false;
	}
}

bool System_Operations::rm_student(const std::string& id) {
	try {
		// remove the student from the entity manager.
		Entity_Manager::get_instance().remove_entity<Student>(id);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error removing student: " << e.what() << std::endl;
		return false;
	}
}

bool System_Operations::search(const std::string& text) {
	bool found_any{};
	// try searching for text in entities in order of Course, Teacher, and Student.
	try {
		found_any = Entity_Manager::get_instance().search_entites<Course>(text);
		found_any |= Entity_Manager::get_instance().search_entites<Teacher>(text);
		found_any |= Entity_Manager::get_instance().search_entites<Student>(text);
		// if didn't find any, print a message.
		if (!found_any) {
			std::cerr << "No results found for: " << text << std::endl;
		}
		return found_any;
	}
	catch (const std::exception&) {
		// catch exceptions and return false.
		return false;
	}
}

bool System_Operations::authenticate_student(const std::string& id, const std::string& password) {
	try {
		// get the student entity.
		Student* student = dynamic_cast<Student*>(Entity_Manager::get_instance().get_entity(id));
		// check if downcast was successful, else throw an exception.
		if (!student) { throw std::invalid_argument("Student with id: " + id + " was not found."); }
		// check if the password matches the student password.
		if (student->get_password() != password) { throw std::invalid_argument("Invalid password."); }
		// update the current student id.
		curr_student = student;
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error authenticating student: " << e.what() << std::endl;
		return false;
	}
}

Schedule_Manager* System_Operations::get_student_schedule_manager(const std::string& id) {
	// if the current schedule manager is not null, return it.
	try {
		if (id == curr_student->get_id() && curr_student) {
			return curr_student->get_schedule_manager();
		}
	} catch (const std::exception& e) {
		std::cerr << "Error getting student schedule manager: " << e.what() << std::endl;
	}
	return nullptr;
}
