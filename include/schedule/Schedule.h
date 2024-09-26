#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <string>
#include <vector>
#include <unordered_map>

// forward declaration since only using pointers or references to the class.
class Course_Type;

// Schedule class to store the course ids and the added associated course types (lecture, tutorial, lab).
class Schedule {
	// id of the schedule.
	unsigned m_id{};
	/*map of courses as keys are the course ids and values are the course_type objects.
	since we need both the course id and the associated course types (lecture, tutorial, lab).*/
	std::unordered_map<std::string, std::vector<Course_Type*>> m_courses{};

	// check if the course id exists in the m_courses map.
	bool course_exists(const std::string& course_id) const;

	// check if the course type exists in the m_courses map.
	bool course_type_exists(const std::string& course_id, const std::string& group_id) const;

	// clean up the schedule data.
	void clean_up();
	// copy the schedule data from another schedule.
	void deep_copy_schedule(const Schedule& other);

	//validate course and course type.
	void validate_course_and_type(const std::string& course_id, const std::string& group_id) const;

	// helper method to search for course id by type.
	bool search_by_type(const std::string& course_id, const std::string& type) const;

	// check if the schedule is empty.
	void check_empty() const;

	// helper method to check overlapping courses.
	static bool find_overlapping_courses(const Course_Type* course1, const Course_Type* course2);

	// helper functions for to_string() function.
	// creates schedule data and populate it with course types.
	std::unordered_map<std::string, std::vector<std::vector<std::string>>> create_schedule_Data(
		const std::vector<std::string>& days, unsigned hours_size) const;

	// populate the schedule data with course types.
	void populate_schedule_data(std::unordered_map<std::string, std::vector<std::vector<std::string>>>& schedule_data,
	                            unsigned hours_size) const;

	// add course types to the schedule data.
	static void add_course_type_to_schedule(
		std::unordered_map<std::string, std::vector<std::vector<std::string>>>& schedule_data,
		const std::string& course_id, const Course_Type* course_type, unsigned hours_size);

	// print the header (days) of the schedule.
	static void print_header(std::ostringstream& out, const std::vector<std::string>& days);

	// helper function to print the schedule body.
	static void print_schedule_body(std::ostringstream& out,
	                                const std::unordered_map<std::string, std::vector<std::vector<std::string>>>&
	                                schedule_data,
	                                const std::vector<std::string>& days, const std::vector<unsigned>& hours);

	// get the max number of courses for an hour (overlapping courses).
	static unsigned get_max_courses_for_hour(
		const std::unordered_map<std::string, std::vector<std::vector<std::string>>>& schedule_data,
		const std::vector<std::string>& days, unsigned hour_index);

public
:
	// constructors and destructor.
	Schedule(unsigned id);
	Schedule(const Schedule& other);
	~Schedule();
	// since we have memory allocation so for clone() function we need operator=.
	Schedule& operator=(const Schedule& other);

	/**
	 * create a schedule from a csv data.
	 * @param data - the csv data to create the schedule.
	 * @return the created schedule.
	 */
	static Schedule from_csv(const std::vector<std::string>& data);

	/**
	 * convert the schedule to csv data.
	 * @return the schedule as csv data.
	 */
	std::vector<std::string> to_csv() const;

	/**
	 * add a course type (lecture, tutorial, lab) to the schedule.
	 * @param course_id - the course id with the associated course type.
	 * @param course_type - the course_type to add.
	 */
	void add_course_type(const std::string& course_id, Course_Type* course_type);
	/**
	 * add a course type (lecture, tutorial, lab) to the schedule.
	 * @param course_id - the course id with the associated course type.
	 * @param group_id - the course_type with the group id.
	 */
	void add_course_type(const std::string& course_id, const std::string& group_id);
	/**
	 * remove a course type (lecture, tutorial, lab) from the schedule.
	 * @param course_id - the course id with the associated course type.
	 * @param group_id - the course_type with the group id.
	 */
	void remove_course_type(const std::string& course_id, const std::string& group_id);
	/**
	 * search for a course in the schedule and print the course types.
	 * @param course_id - the course id to search for.
	 * @return true if found any course types in schedule, false otherwise.
	 */
	bool search(const std::string& course_id) const;

	/**
	 * prints the weekly hours and points for all courses in the schedule.
	 */
	void print_weekly_summary() const;

	/**
	 * print the overlapping courses in the schedule.
	 */
	void print_overlapping_courses() const;

	// getters and setters.
	unsigned get_id() const;
	void set_id(unsigned id);

	const Course_Type* get_course_type(const std::string& course_id, const std::string& group_id) const;

	/**
	 * print the schedule in a table format and return it as a string.
	 * in the table, the columns are the days and the rows are the hours.
	 * the values are the course types (lecture, tutorial, lab). info about course_type (course_id, type, classroom).
	 * @return the schedule as a string.
	 */
	std::string to_string() const;
	// friend operator to print the schedule.
	friend std::ostream& operator<<(std::ostream& os, const Schedule& schedule);
};

#endif //SCHEDULE_H
