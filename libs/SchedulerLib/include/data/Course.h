#ifndef COURSE_H
#define COURSE_H

#include <unordered_map>

#include "Entity.h"

class Course_Type;

// Course class represents a row in the Courses CSV file.
class Course : public Entity {
	std::string m_id{};
	std::string m_name{};
	std::string m_lecturer{};
	float m_points{};

	// each course can have multiple course types (lecture, tutorial, lab).
	// note:: can have multiple lectures, tutorials, and labs. or none at all.
	std::unordered_map<std::string, Course_Type*> m_course_types{};

	// validation methods.
	static std::string validate_id(const std::string& id);
	// for both name and lecturer.
	static std::string validate_name(const std::string& name);
	static float validate_points(float points);

public:
	// constructors.
	Course(const std::string& id, const std::string& name, const std::string& lecturer, float points);
	void deep_copy_course_types(const Course& other);
	Course(const Course& other);
	void clean_up();
	~Course() override;
	// since we have memory allocation so for clone() function we need operator=.
	Course& operator=(const Course& other);

	// copy the course data.
	Entity* clone() const override;

	// convert the course data to a CSV format.
	std::vector<std::string> to_csv() const override;
	// convert the CSV data to a course object.
	static Course* from_csv(const std::vector<std::string>& data);

	// get the type of the entity (Course).
	std::string get_type() const override;
	// get the file name of the course type.
	static std::string get_file_name();

	// search for a text in the course data.
	bool search(const std::string& text) const override;

	// getters and setters.
	std::string get_id() const override;
	void set_id(const std::string& id) override;

	std::string get_name() const override;
	void set_name(const std::string& name) override;

	float get_points() const;
	void set_points(float points);

	// convert the course data to a string.
	std::string to_string() const override;
	// friend operator to print the course data.
	friend std::ostream& operator<<(std::ostream& os, const Course& course);

	// get course type by id.
	Course_Type* get_course_type(const std::string& id) const;

	// set course type by id.
	void set_course_type(const std::string& id, Course_Type* course_type);

	// add course type to the course.
	// if the course type already exists, throws an exception.
	void add_course_type(Course_Type* course_type);

	// remove course type from the course.
	void remove_course_type(const std::string& course_type_id);
};

#endif //COURSE_H
