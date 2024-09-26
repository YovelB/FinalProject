#ifndef SCHEDULE_MANAGER_H
#define SCHEDULE_MANAGER_H

#include <string>
#include <vector>

#include "Schedule.h"

// Schedule_Manager class represents a single instance class that manages the schedules for the student.
class Schedule_Manager {
	// student id.
	std::string m_student_id{};

	/*vector of schedules: that way we get automatic memory management.
	note: dont need pointers of schedules as schedule doesn't have inheritance or virtual functions.*/
	std::vector<Schedule> m_schedules{};

	// counter for the schedule id.
	static unsigned m_id_counter;

	// read the schedules for a student from the file.
	void read_schedules();

	// write the schedules from a student to the file.
	void write_schedules();

	// reset the schedule ids.
	void reset_schedule_ids();

	// check if there are any schedules in the vector.
	void check_schedules() const;

public:
	// constructors and destructor.
	Schedule_Manager(const std::string& id);
	Schedule_Manager(const Schedule_Manager& other);
	~Schedule_Manager();
	// print a schedule by id.
	bool print(const std::string& id) const;
	// print all schedules.
	bool print_all() const;

	// adds an empty schedule to the schedules vector.
	bool add_schedule();
	// removes a schedule by id.
	bool rm_schedule(const std::string& id);

	// add a course to a schedule.
	bool add_course(const std::string& id, const std::string& course_id, const std::string& group_id);
	// remove a course from a schedule.
	bool rm_course(const std::string& id, const std::string& course_id, const std::string& group_id);

	// searches for a course in the schedules.
	bool search(const std::string& course_id) const;

	//print weeky summary of the schedule.
	bool print_weekly_summary(const std::string& id) const;

	// print overlapping courses of the schedule.
	bool check_overlapping_courses(const std::string& id) const;

	// get schedule by id. (returns a reference to the schedule so we dont need to handle with memory management).
	Schedule& get_schedule(unsigned id);

	// get a const version of get_schedule (to not allow changes to the schedule).
	const Schedule& get_schedule(unsigned id) const;
};

#endif //SCHEDULE_MANAGER_H