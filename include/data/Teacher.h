#ifndef TEACHER_H
#define TEACHER_H

#include "Entity.h"

// Teacher class represents a row in the Teachers CSV file.
class Teacher : public Entity {
	std::string m_id{};
	std::string m_name{};

	// validation methods.
	// note: technically these methods create a copy and add memory,
	// but it is preferred so it can be used in list initialization.
	static std::string validate_id(const std::string& id);
	static std::string validate_name(const std::string& name);

public:
	// constructors.
	Teacher(const std::string& id, const std::string& name);
	Teacher(const Teacher& other);
	// destructor is not needed since there are no pointers or memory allocation (default destructor is used).

	// copy the teacher data.
	Entity* clone() const override;

	// convert the teacher data to a CSV format.
	std::vector<std::string> to_csv() const override;
	// convert the CSV data to a teacher object.
	static Teacher* from_csv(const std::vector<std::string>& data);

	// get the type of the entity (Teacher).
	std::string get_type() const override;

	// get the file name of the course type.
	static std::string get_file_name();

	// search for a text in the teacher data.
	bool search(const std::string& text) const override;

	// getters and setters.
	std::string get_id() const override;
	void set_id(const std::string& id) override;
	std::string get_name() const override;
	void set_name(const std::string& name) override;

	// override to_string method.
	std::string to_string() const override;
	// friend operator to print the teacher data.
	friend std::ostream& operator<<(std::ostream& os, const Teacher& teacher);
};

#endif //TEACHER_H
