#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "CSV_Editor.h"
#include "data/Course.h"
#include "data/Entity.h"
#include "data/course_types/Lecture.h"
#include "data/course_types/Tutorial.h"
#include "data/course_types/Lab.h"

// forward declarations for classes (since they are only used as pointers or references).

// Entity_Manager class represents a signel instance class to manage entities.
// it manages entities of main types (Student, Teacher, Course) and sub types (Lecture, Tutorial, Lab).
class Entity_Manager {
	/*map to store entities of main types (Student, Teacher, Course).
	keys - entity ids, values - pointers to entities.*/
	std::unordered_map<std::string, Entity*> m_entities{};
	/*map to store order of entities of all types (Student, Teacher, Course, Lecture, Tutorial, Lab).
	to keep read and write order of all csv files.
	keys - file names, values - vector of ids in order.*/
	std::unordered_map<std::string, std::vector<std::string>> m_entity_order{};

	/*private constructor and destructor to prevent object creation (single instance class).
	constructor to read entities from csv files.
	no need for a copy constructor since there is only one instance (should be marked public and deleted).
	Entity_Manager(const Entity_Manager&) = delete;
	destructor to write entities to csv files.*/
	Entity_Manager();
	~Entity_Manager();

	// clean up function to delete all entities and order maps.
	void clean_up();

	/**
	 * read entities of type T from csv file into m_entities and m_entity_order maps.
	 * if course is provided, read course types (Lecture, Tutorial, Lab) for the course.
	 * @tparam T - type of entity (Student, Teacher, Course, Lecture, Tutorial, Lab).
	 * @param course - optional pointer to course object to read course types.
	 */
	template <typename T>
	void read_entities(Course* course = nullptr) {
		// get the file name for the entity type.
		const std::string file_name = get_file_name<T>(course);
		try {
			// read data from csv file.
			std::vector<std::vector<std::string>> data = CSV_Editor::read_csv(file_name);
			for (std::vector<std::string>& row : data) {
				// process entity from csv row and add to m_entities and m_entity_order maps.
				process_entity<T>(file_name, row, course);
			}
			data.clear(); // clear the data vector.
		}
		catch (const std::exception& e) {
			// log the error and throw the exception again.
			std::cerr << "Error reading file: " << file_name << ": " << e.what() << std::endl;
		}
	}

	/**
	 * write entities of type T to csv file from m_entities and m_entity_order maps.
	 * if course is provided, write course types (Lecture, Tutorial, Lab) for the course.
	 * @tparam T - type of entity (Student, Teacher, Course, Lecture, Tutorial, Lab).
	 * @param course - optional pointer to course object to write course types.
	 */
	template <typename T>
	void write_entities(Course* course = nullptr) {
		// get file name for the entity type.
		std::string file_name = get_file_name<T>(course);
		try {
			// prepare entity data for writing to csv file.
			std::vector<std::vector<std::string>> data = prepare_data<T>(file_name, course);
			// write data to csv file.
			CSV_Editor::write_csv(file_name, data);
			data.clear(); // clear the data vector.
		}
		catch (const std::exception& e) {
			// log the error and throw the exception again.
			std::cerr << "Error writing file: " << file_name << ": " << e.what() << std::endl;
		}
	}

	/**
	 * helper function to process entity from csv row and add to m_entities and m_entity_order maps.
	 * can process entities of main types (Student, Teacher, Course) optional parameter course=nullptr.
	 * and can process course types (Lecture, Tutorial, Lab) optional parameter course provided.
	 * @tparam T - type of entity (Student, Teacher, Course, Lecture, Tutorial, Lab).
	 * @param file_name - name of the csv file.
	 * @param row - vector of strings representing csv row.
	 * @param course - optional pointer to course object to process course types.
	 */
	template <typename T>
	void process_entity(const std::string& file_name, std::vector<std::string>& row,
	                    Course* course = nullptr) {
		T* entity{dynamic_cast<T*>(T::from_csv(row))}; // create entity from csv row and downcast to T*.
		// process entity to m_entities and m_entity_order maps.
		add_entity_to_collections(entity, file_name, course, false);
	}

	/**
	 * helper function to process course object from csv row and adds all course types to the course.
	 * @param course - pointer to course object to assign course types.
	 */
	void process_course(Course* course) {
		try {
			// check if the downcast from T* to Course* was successful.
			if (!course) { throw std::invalid_argument("Failed to downcast Entity* to Course*."); }
			// read and assign course types to current course.
			read_entities<Lecture>(course);
			read_entities<Tutorial>(course);
			read_entities<Lab>(course);
		}
		catch (const std::exception&) {
			throw; // rethrow the exception.
		}
	}

	/**
	 * helper function to prepare data for writing to csv file from m_entities and m_entity_order maps.
	 * @tparam T - type of entity (Student, Teacher, Course, Lecture, Tutorial, Lab).
	 * @param file_name - name of the csv file.
	 * @param course - optional pointer to course object to prepare course types.
	 * @return the data vector of rows to write to the csv file.
	 */
	template <typename T>
	std::vector<std::vector<std::string>> prepare_data(std::string& file_name, Course* course = nullptr) {
		std::vector<std::vector<std::string>> data{}; // create a data vector to store rows.
		const std::vector<std::string>& order = m_entity_order[file_name]; // get the order of entities.
		// iterate through entity_order and add only entities of type T to the data vector.
		for (const std::string& id : order) {
			T* entity{}; // create a pointer to entity.
			if (!course) {
				// if preparing an entity of main types (Student, Teacher, Course).
				// get entity with id from the entities map and downcast it to T*.
				entity = dynamic_cast<T*>(get_entity(id));
				if (entity->get_type() == "Course") {
					// downcast T* to Course* and prepare course.
					prepare_course(dynamic_cast<Course*>(entity));
				}
			}
			else {
				// if preparing an entity of course types (Lecture, Tutorial, Lab).
				entity = dynamic_cast<T*>(course->get_course_type(id));
			}
			// check if the downcast was successful.
			check_entity(entity, "Entity was not found.");
			// add course type to data.
			data.push_back(entity->to_csv());
		}
		return data;
	}

	/**
	 * helper function to prepare course and course types (Lecture, Tutorial, Lab) for writing to csv file.
	 * @param course - pointer to course object to prepare course types.
	 */
	void prepare_course(Course* course) {
		try {
			// check if the downcast from Entity* to Course* was successful.
			if (!course) { throw std::invalid_argument("Failed to downcast Entity* to Course*."); }
			// write course types to file.
			write_entities<Lecture>(course);
			write_entities<Tutorial>(course);
			write_entities<Lab>(course);
		}
		catch (const std::exception&) {
			// log the error and throw the exception again.
			std::cerr << "Error writing course types for course." << std::endl;
			throw;
		}
	}

	/**
	 * helper function to remove all course types of type T from the course.
	 * @param course - pointer to course object to remove course types.
	 */
	void remove_course(Course* course) {
		try {
			// check if downcast was successful.
			if (!course) { throw std::invalid_argument("Failed to downcast Entity* to Course*."); }
			// remove all course types from the course.
			remove_all_course_types<Lecture>(course);
			remove_all_course_types<Tutorial>(course);
			remove_all_course_types<Lab>(course);
		}
		catch (const std::exception&) {
			// log the error and rethrow the exception.
			std::cerr << "Error removing course types from course." << std::endl;
			throw;
		}
	}

	/**
	 * helper function to add entity to entities map and order.
	 * @param entity - pointer to entity object to add.
	 * @param file_name - name of the csv file.
	 * @param course - optional pointer to course object to add course types.
	 * @param is_addition - flag to check if the entity is being added or processed.
	 */
	void add_entity_to_collections(Entity* entity, const std::string& file_name, Course* course = nullptr,
	                               bool is_addition = true) {
		check_entity(entity, "Failed to create entity.");
		const std::string& id = entity->get_id();
		if (entity_exists(id, file_name)) { throw std::invalid_argument("Entity with id " + id + " already exists."); }

		m_entity_order[file_name].push_back(id); // add id to entity order map.
		if (!course) {
			// main types (Student, Teacher, Course).
			m_entities[id] = entity; // add entity to entities map.
			if (entity->get_type() == "Course") {
				if (is_addition) {
					prepare_course(dynamic_cast<Course*>(entity)); // downcast T* to Course* and prepare course.
				} else {
					process_course(dynamic_cast<Course*>(entity)); // downcast T* to Course* and process course.
				}
			}
		}
		else {
			// course types (Lecture, Tutorial, Lab).
			course->add_course_type(dynamic_cast<Course_Type*>(entity)); // add course type to course.
		}
	}


	/**
	 * helper function to remove entity from entities map and order.
	 * @param id - id of the entity to remove.
	 * @param file_name - name of the csv file.
	 * @param course - optional pointer to course object to remove course types.
	 */
	void remove_entity_from_collections(const std::string& id, const std::string& file_name, Course* course = nullptr) {
		std::vector<std::string>& order = m_entity_order[file_name]; // get the order of entities.
		// use an iterator to find the id from the order vector and remove it.
		for (auto it = order.begin(); it != order.end(); ++it) {
			if (*it == id) {
				// if id was found.
				if (!course) {
					// removing entity of main types (Student, Teacher, Course).
					Entity* entity = m_entities[id]; // get the entity from entities map.
					m_entities.erase(id); // remove entity from entities map.
					// if the entity is of type Course, remove the course types (Lecture, Tutorial, Lab).
					if (entity->get_type() == "Course") {
						// downcast Entity* to Course* and remove course.
						remove_course(dynamic_cast<Course*>(entity));
					} // delete the entity object and avoid dangling pointer.
					delete entity;
					entity = nullptr;
				}
				else {
					// removing entity of course types (Lecture, Tutorial, Lab).
					course->remove_course_type(id);
				}
				order.erase(it); // remove id from the entity order map.
				break; // break the loop.
			}
		}
	}

	// helper function to get the file name for the entity type.
	template <typename T>
	static std::string get_file_name(const Course* course = nullptr) {
		return course ? course->get_id() + T::get_file_name() : T::get_file_name();
	}

	// helper function to check entity and throw an exception if not null.
	template <typename T>
	static void check_entity(const T* entity, const std::string& error_msg) {
		if (!entity) { throw std::invalid_argument(error_msg); }
	}

public:
	/*static method to get the the only instance of Entity_Manager.
	note: this method should be private and only used by System_Operations class with friend keyword.
	(but for simplicity, it is public).*/
	static Entity_Manager& get_instance() {
		// since static var are defined only once, there will be only one instnace.
		static Entity_Manager instance;
		return instance;
	}

	/**
	 * add entity of type T to entities map and order.
	 * @tparam T - type of entity (Student, Teacher, Course, Lecture, Tutorial, Lab).
	 * @param entity - pointer to entity object to add.
	 * @param course - optional pointer to course object to add course types.
	 */
	template <typename T>
	void add_entity(Entity* entity, Course* course = nullptr) {
		// get the file name for the entity type.
		const std::string file_name = get_file_name<T>(course);
		// add entity to entities map and order.
		add_entity_to_collections(dynamic_cast<T*>(entity), file_name, course);
	}

	/**
	 * remove entity of type T from entities map and order.
	 * @tparam T - type of entity (Student, Teacher, Course, Lecture, Tutorial, Lab).
	 * @param id - id of the entity to remove.
	 * @param course - optional pointer to course object to remove course types.
	 */
	template <typename T>
	void remove_entity(const std::string& id, Course* course = nullptr) {
		// get the file name for the entity type.
		const std::string file_name = get_file_name<T>(course);
		// remove entity from entities map and order.
		remove_entity_from_collections(id, file_name, course);
	}

	/**
	 * remove all course types of type T from entity order map and delete the csv file.
	 * example: remove_all_course_types<Lecture>(course) to remove all lectures from the course.
	 * @tparam T - type of course type (Lecture, Tutorial, Lab).
	 * @param course - pointer to course object to remove course types.
	 */
	template <typename T>
	void remove_all_course_types(Course* course) {
		// get file name for course type csv file.
		const std::string file_name = course->get_id() + T::get_file_name();
		// remove course type order from the entity order map.
		for (const std::string& id : m_entity_order[file_name]) {
			remove_entity_from_collections(id, file_name, course);
		}
		m_entity_order.erase(file_name); // erase the course type order.
		CSV_Editor::delete_csv(file_name); // delete the course type csv file.
	}

	/**
	 * print entity of type T by id.
	 * @tparam T - type of entity (Student, Teacher, Course, Lecture, Tutorial, Lab).
	 * @param id - id of the entity to print.
	 * @param course - optional pointer to course object to print course types.
	 * @return true if entity was printed successfully, false otherwise.
	 */
	template <typename T>
	void print_entity(const std::string& id, const Course* course = nullptr) const {
			// get the file name for the entity type.
			const std::string file_name = get_file_name<T>(course);
			// check if the entity exists, else throw an exception.
			if (!entity_exists(id, file_name)) {
				throw std::invalid_argument("Entity with id: " + id + " was not found.");
			}

			// get the entity and downcast it to the derived type.
			const Entity* entity = dynamic_cast<const T*>(get_entity(id));
			// check if downcast was successful, else throw an exception.
			check_entity(entity, "Entity was not found.");
			std::cout << *entity << std::endl; // print the entity.
	}

	/**
	 * print entities of type T in the order of the entity type.
	 * @tparam T - type of entity (Student, Teacher, Course, Lecture, Tutorial, Lab).
	 */
	template <typename T>
	void print_entities(Course* course = nullptr) const {
		// get the file name for the entity type.
		const std::string file_name = get_file_name<T>(course);
		const std::vector<std::string>& order = m_entity_order.at(file_name); // get the order of entities.
		// iterate over the order of T
		for (const std::string& id : order) {
			// get the base class entity and downcast it to the derived type.
			const T* entity = dynamic_cast<const T*>(get_entity(id));
			// check if downcast was successful, else throw an exception.
			check_entity(entity, "Entity was not found.");
			std::cout << *entity << std::endl; // print the entity.
		}
	}

	template <typename T>
	bool search_entites(const std::string& text) {
		bool found{};
		// get the entity manager instance and from it get the entity order.
		// iterate over the order of specified type of entity.
		try {
			const std::vector<std::string>& order = get_entity_order<T>();
			// if order is empty, return false.
			for (const std::string& id : order) {
				// get the base class entity and downcast it to the derived type.
				const T* entity = dynamic_cast<const T*>(get_entity(id));
				// check if downcast was successful, else throw an exception.
				if (!entity) {
					throw std::invalid_argument("Entity with id: " + id + " was not found.");
				}
				// use the derived entity to search for text.
				if (entity->search(text)) {
					if (!found) {
						std::cout << "Found in " << T::get_file_name() << ":" << std::endl;
						found = true;
					}
					std::cout << *entity << std::endl; // print the entity.
				}
			}
		}
		catch (const std::exception&) {
			return false;
		}
		return found;
	}


	/**
     * get the order of entities of type T from the entity order map.
     * @tparam T - type of entity (Student, Teacher, Course, Lecture, Tutorial, Lab).
     * @return vector of ids in order of the entity type.
     */
	template <typename T>
	const std::vector<std::string>& get_entity_order() const {
		// if entity order map is empty, throw an exception.
		if (m_entity_order.empty()) { throw std::invalid_argument("Entity order map is empty."); }
		// check if the entity type exists in the entity order map.
		if (m_entity_order.find(T::get_file_name()) == m_entity_order.end()) {
			throw std::invalid_argument("Entity type not found in entity order.");
		}
		// use at() to access the element with the specified key.
		// at() throws an exception if the key does not exist.
		return m_entity_order.at(T::get_file_name());
		// log the error and throw the exception again.
	}

	// print all entities by order.
	void print_all_entities() const;

	/**
	 * check if entity with id exists in both entities map and order map.
	 * search for the id in the order of entities of key file_name.
	 * finds entities of main types (Student, Teacher, Course)
	 * and course types (Lecture, Tutorial, Lab).
	 * note: since if we only check the order map there can be duplicates in id for teacher and student.
	 * @param id - id of the entity.
	 * @param file_name - name of the csv file.
	 * @return true if entity exists, false otherwise.
	 */
	bool entity_exists(const std::string& id, const std::string& file_name) const;

	/**
	 * get entity by id from the entities map.
	 * note: gets entities of only main types (Student, Teacher, Course).
	 * @param id - id of the entity.
	 * @return pointer to the entity if found, nullptr otherwise.
	 */
	Entity* get_entity(const std::string& id) const;


	// get entities map.
	const std::unordered_map<std::string, Entity*>& get_entities() const;
};

#endif //ENTITY_MANAGER_H
