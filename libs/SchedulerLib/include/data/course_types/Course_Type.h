#ifndef COURSE_TYPE_H
#define COURSE_TYPE_H

#include <string>
#include <vector>
#include <ctime>

#include "../Entity.h"

// Course_Type class is an abstract class and the base class for all course types (Lecture, Tutorial, Lab).
class Course_Type : public Entity {
	std::string m_id{};
	std::string m_day{}; // day of the week (Monday, Tuesday, ...).
	std::tm m_start_time{}; // start time of the course, in HH:MM format.
	unsigned m_duration{}; // duration of the course type in minutes.
	std::string m_lecturer{}; // lecturer name.
	std::string m_classroom{}; // classroom name.

protected:
	// protected validate methods to be used in the derived classes.
	static std::string validate_id(const std::string& group_id);
	static std::string validate_day(const std::string& day);
	static std::tm validate_start_time(const std::tm& start_time);
	static std::string validate_start_time(const std::string& start_time);
	static unsigned validate_duration(unsigned duration);
	static std::string validate_lecturer(const std::string& lecturer);
	static std::string validate_classroom(const std::string& classroom);

	// protected constructors so the derived classes can call it.
	Course_Type(const std::string& id, const std::string& day, const std::string& start_time,
	            unsigned duration, const std::string& lecturer, const std::string& classroom);
	Course_Type(const Course_Type& other);


public:
	// virtual destructor so the derived classes destructors are called.
	virtual ~Course_Type() = default;

	// pure virtual method to copy the course type object. (since copy constructor cant be virtual).
	virtual Course_Type* clone() const = 0;

	// virtual method get_type() to return the type of the course type.
	virtual std::string get_type() const = 0;

	// convert the course type data to a CSV format.
	// since each course type has the same implementation for this method we can define it here.
	std::vector<std::string> to_csv() const override;
	// static method to convert the CSV data to a course type object.
	// cant be defined here since it is different for each derived class.

	// search for a text in the course type data.
	// since each course type has the same implementation for this method we can define it here.
	bool search(const std::string& text) const override;

	// getters and setters.
	std::string get_id() const override;
	void set_id(const std::string& group_id) override;

	// not naming it get_lecturer and set_lecturer to override the base class get_name and set_name methods.
	std::string get_name() const override;
	void set_name(const std::string& lecturer) override;

	std::tm get_start_time() const;
	void set_start_time(const std::tm& start_time);

	unsigned get_duration() const;
	void set_duration(unsigned duration);

	std::string get_day() const;
	void set_day(const std::string& day);

	std::string get_classroom() const;
	void set_classroom(const std::string& classroom);

	// convert the course type data to a string.
	std::string to_string() const override;
	// friend operator to print the course type data.
	friend std::ostream& operator<<(std::ostream& os, const Course_Type& course_type);

	// helper method to convert the time object to a HH:MM string.
	static std::string time_to_string(const std::tm& start_time);
	// helper method to convert the HH:MM string to a time object.
	static std::tm string_to_time(const std::string& start_time) ;
};

#endif //COURSE_TYPE_H
