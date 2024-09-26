#ifndef LAB_H
#define LAB_H

#include "Course_Type.h"

// Lab class represents a row in the xxxxx_Lab.csv file, where xxxxx is the course id.
class Lab : public Course_Type {
public:
	// constructors and destructor.
	Lab(const std::string& id, const std::string& day, const std::string& start_time,
	    unsigned duration, const std::string& lecturer, const std::string& classroom);
	Lab(const Lab& other);
	// default destructor will call the base class destructor. (no need to define it).

	// copy the lab object.
	Lab* clone() const override;

	// get the type of the course type (Lab).
	std::string get_type() const override;

	// get the file name of the course type.
	static std::string get_file_name();

	// convert the CSV data to a lab object.
	static Lab* from_csv(const std::vector<std::string>& data);
};

#endif //LAB_H
