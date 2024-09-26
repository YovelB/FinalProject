#ifndef LECTURE_H
#define LECTURE_H

#include "Course_Type.h"

// Lecture class represents a row in the xxxxx_Lecture.csv file, where xxxxx is the course id.
class Lecture : public Course_Type {
public:
	// constructors.
	Lecture(const std::string& group_id, const std::string& day, const std::string& start_time,
	        unsigned duration, const std::string& lecturer, const std::string& classroom);
	Lecture(const Lecture& other);
	// default destructor will call the base class destructor. (no need to define it).

	// copy the lecture object.
	Lecture* clone() const override;

	// get the type of the course type (Lecture).
	std::string get_type() const override;

	// get the file name of the course type.
	static std::string get_file_name();

	// convert the CSV data to a lecture object.
	static Lecture* from_csv(const std::vector<std::string>& data);
};

#endif //LECTURE_H
