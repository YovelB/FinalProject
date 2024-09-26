#ifndef STUDENT_H
#define STUDENT_H

#include "Entity.h"

// forward declaration since it used as a pointer or reference.
class Schedule_Manager;

// Student class represents a row in the Students CSV file.
class Student : public Entity {
	std::string m_id{};
	std::string m_name{};
	std::string m_password{};

	// each Student has an Schedule_Manager pointer to manage the student schedule.
	// since Schedule_Manager is an expensive object to copy, it is preferred to use a pointer.
	Schedule_Manager* m_schedule_manager{};

	/*validation methods.
	note: technically these methods create a copy and add memory,
	but it is preferred so it can be used in list initialization.*/
	static std::string valid_id(const std::string& id);
	static std::string valid_name(const std::string& name);
	static std::string valid_password(const std::string& password);

public:
	// constructors.
	Student(const std::string& id, const std::string& name, const std::string& password);
	Student(const Student& other);
	~Student();

	// copy the student data (uses the copy constructor).
	Entity* clone() const override;

	// convert the student data to a CSV format row.
	std::vector<std::string> to_csv() const override;
	// convert the CSV data to a student object.
	static Student* from_csv(const std::vector<std::string>& data);

	// get the type of the entity (Student).
	std::string get_type() const override;

	// get the file name of the course type.
	static std::string get_file_name();

	// search for a text in the student data.
	bool search(const std::string& text) const override;

	// getters and setters.
	std::string get_id() const override;
	void set_id(const std::string& id) override;
	std::string get_name() const override;
	void set_name(const std::string& name) override;
	std::string get_password() const;
	void set_password(const std::string& password);

	// get the schedule manager (not const since it will be modified).
	Schedule_Manager* get_schedule_manager() const;

	// convert the student data to a string.
	std::string to_string() const override;
	// friend operator to print the student data.
	friend std::ostream& operator<<(std::ostream& os, const Student& student);
};

#endif //STUDENT_H
