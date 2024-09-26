#ifndef CLI_H
#define CLI_H

#include <string>

#include "users/User.h"

// CLI class represents a utility class to manage the command line interface.
class CLI {
	// current user using the CLI.
	User* m_user{};

	std::string admin_password{"admin"}; // default admin password.

	// flag to check if program is running.
	bool m_running{true};

	// process for creating a admin or student object.
	bool process_admin(const std::string& password);
	bool process_student(const std::string& id, const std::string& password);

	// process for creating a command object and execute it.
	void process_command(const std::string& input);

	// split a input into command and arguments.
	static std::vector<std::string> split_input(const std::string& input);

	// change command case specific (first letter upper case, rest lower case).
	static std::string change_command_case(const std::string& command);

public:
	// constructor and destructor.
	CLI();
	// no eed for a copy constructor since we don't want to copy the CLI object.
	~CLI();
	void loop();

	// start method to start the CLI.
	void setup();

	// login method to authenticate the user.
	bool login();
	void clean_up();

	// loop to check the commands.
	void command_loop();

	// get and set methods.
	bool is_running() const;
	void set_running(bool running);

	// set admin password.
	void set_admin_password(const std::string& password);

};

#endif //CLI_H