#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

// User class represents an abstract class for all users.
class User {
protected:
	// protected field and methods so they can be used in derived classes.
	std::string m_password{}; // password of the user.

	// constructors and destructor.
	User(const std::string& password);
	User(const User& other);

	// validate args size by expected size and print error if not.
	static bool validate_arg_size(size_t arg_size, size_t expected_size, const std::string& cmd);

public:
	// always make the destructor virtual (default is used).
	virtual ~User() = default;

	// execute a command by given string and arguments.
	virtual bool execute(const std::string& command, const std::vector<std::string>& args);

	// print the help menu for the user.
	virtual void help() const;

	// clear the screen.
	static void clear_screen();

	// getter for password.
	std::string get_password() const;
};

#endif //USER_H
