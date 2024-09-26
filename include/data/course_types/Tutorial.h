#ifndef TUTORIAL_H
#define TUTORIAL_H

#include "Course_Type.h"

// Tutorial class represents a row in the xxxxx_Tutorial.csv file, where xxxxx is the course id.
class Tutorial : public Course_Type {
public:
	// constructors.
	Tutorial(const std::string& group_id, const std::string& day, const std::string& start_time,
	         unsigned duration, const std::string& lecturer, const std::string& classroom);
	Tutorial(const Tutorial& other);
	// default destructor will call the base class destructor. (no need to define it).

	// copy the tutorial object.
	Tutorial* clone() const override;

	// get the type of the course type (Tutorial).
	std::string get_type() const override;

	// get the file name of the course type.
	static std::string get_file_name();

	// convert the CSV data to a tutorial object.
	static Tutorial* from_csv(const std::vector<std::string>& data);
};

#endif //TUTORIAL_H
