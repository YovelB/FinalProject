#include "Entity_Manager.h"

#include <iostream>

#include "data/Student.h"
#include "data/Teacher.h"
#include "data/Course.h"

Entity_Manager::Entity_Manager() {
	try {
		// read the entities from the CSV files.
		read_entities<Student>();
		read_entities<Teacher>();
		read_entities<Course>();
	}
	catch (const std::exception&) {
		throw; // rethrow the exception.
	}
}

Entity_Manager::~Entity_Manager() {
	try {
		// write the entities to the CSV files.
		write_entities<Student>();
		write_entities<Teacher>();
		write_entities<Course>();
		// clean up the entities.
		clean_up();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl; // log the error.
	}
}

void Entity_Manager::clean_up() {
	// iterate over the entities and delete them and avoid dangling pointers.
	for (auto& [_, entity] : m_entities) {
		delete entity;
		entity = nullptr;
	}
	m_entities.clear(); // clear the map.

	// iterate over the entities order and clear them.
	for (auto& [_, ids] : m_entity_order) {
		ids.clear(); // clear the vector.
	}
	m_entity_order.clear(); // clear the map.
}

void Entity_Manager::print_all_entities() const {
	// loop through the entities map ordered by file name.
	for (const auto& [file_name, ids] : m_entity_order) {
		// skip the file name if it is not the main files (Students, Teachers, Courses).
		// since each course prints its course types (Lecture, Tutorial, Lab).
		if (file_name != Student::get_file_name() && file_name != Teacher::get_file_name() &&
			file_name != Course::get_file_name()) { continue; }
		// print the entities by the order of the ids.
		std::cout << "File: " << file_name << std::endl;
		for (const std::string& id : ids) {
			const Entity* entity = get_entity(id); // get the entity by id.
			if (entity) {
				std::cout << *entity << std::endl;
			}
			else {
				std::cerr << "Entity with id " << id << " was not found." << std::endl;
			}
		}
		std::cout << std::endl;
	}
}

bool Entity_Manager::entity_exists(const std::string& id, const std::string& file_name) const {
	// first check if entity id exists in the entities map.
	if (get_entity(id)) { return true; }
	// check if the file name key exists in the entity order map.
	// use find() method to check if the key exists.
	const auto it = m_entity_order.find(file_name);
	if (it == m_entity_order.end()) { return false; } // not found.

	// get the order of entities of type T.
	const std::vector<std::string>& order = it->second;
	// check if the entity id exists in the order vector.
	for (const std::string& curr_id : order) {
		if (curr_id == id) { return true; } // found.
	}
	return false; // not found.
}

Entity* Entity_Manager::get_entity(const std::string& id) const {
	// find() method returns an iterator to the element if found, else returns the end iterator.
	if (const auto it = m_entities.find(id); it != m_entities.end())
		return it->second; // return the entity.
	return nullptr; // return nullptr if not found.
}

const std::unordered_map<std::string, Entity*>& Entity_Manager::get_entities() const { return m_entities; }
