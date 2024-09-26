#ifndef ADMIN_User_H
#define ADMIN_User_H

#include "User.h"

// Admin class represents an admin user.
class Admin_User : public User {
	// default password for the admin.
public:
	// constructors
	Admin_User(const std::string& password);
	Admin_User(const Admin_User& other);
	// theres no need for a destructor, since there is no ptrs or memory allocation (default used).

	// execute a command by given string and arguments.
	bool execute(const std::string& command, const std::vector<std::string>& args) override;

	// override the help method to print the admin help menu.
	void help() const override;
};

#endif //ADMIN_User_H
