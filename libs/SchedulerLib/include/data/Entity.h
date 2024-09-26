#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

// Entity class is an interface and the base class for all entities (Student, Teacher, Course).
class Entity {
protected:
	// protected constructor so the derived classes can call it.
	Entity() = default;

public:
	// virtual destructor so the derived classes destructors are called.
	virtual ~Entity() = default;

	// pure virtual method to copy the entity data.
	virtual Entity* clone() const = 0;

	// virtual method to convert the entity data to a CSV row.
	// since each entity represents a row in the CSV file.
	virtual std::vector<std::string> to_csv() const = 0;

	// dont need from_csv() method since it's not used in the base class.

	// virtual method to get the type of an entity.
	virtual std::string get_type() const = 0;

	// virtual method to search for a text in the entity data.
	virtual bool search(const std::string& text) const = 0;

	// virtual getters and setters for id and name.
	virtual std::string get_id() const = 0;
	virtual void set_id(const std::string& id) = 0;

	virtual std::string get_name() const = 0;
	virtual void set_name(const std::string& name) = 0;

	// virtual method to convert the entity data to a string.
	// needed for the operator<< to print the derived classes data.
	// note: since operator<< is a friend function, it can't be virtual.
	virtual std::string to_string() const = 0;

	// friend operator to print the entity data.
	// note: since operator<< is a friend function, it can't be virtual.
	friend std::ostream& operator<<(std::ostream& os, const Entity& entity) {
		os << entity.to_string();
		return os;
	}
};

#endif //ENTITY_H
