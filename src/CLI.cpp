#include "../include/CLI.h"

#include <iostream>

#include "../libs/SchedulerLib/include/System_Operations.h"
#include "../include/users/Admin_User.h"
#include "../include/users/Student_User.h"

// main function to run the CLI.
int main() {
	CLI cli{};
	return 0;
}

CLI::CLI() {
	setup();
}

CLI::~CLI() {
	clean_up();
}

void CLI::clean_up() {
	// delete the user object and avoid dangling pointer.
	delete m_user;
	m_user = nullptr;
}

void CLI::setup() {
	// login the user.
	std::cout << "Welcome to the CLI. Please login to continue." << std::endl;

	loop();
}

void CLI::loop() {
	while (is_running()) {
		// wait for the user to login.
		if (!m_user && !login()) {
			std::cout << "Invalid username or password. Please try again." << std::endl;
			continue;
		}
		command_loop();
	}
}

void CLI::command_loop() {
	std::string input{};
	std::cout << "Enter a command (case insensitive) or type 'Help' for more information." << std::endl;
	// while the user is logged in.
	while (m_user) {
		std::cout << "> ";
		// use getline() to read the whole line (std::ws - to wait for input).
		std::getline(std::cin >> std::ws, input);
		process_command(input);
		std::cout << std::endl;
	}
}

bool CLI::login() {
	std::string username{}, password{};
	// ask for the username.
	std::cout << "choose a user (admin or student):" << std::endl;
	std::cin >> username;
	if (username != "admin" && username != "student") { return false; }

	std::string id{};
	if (username == "student") {
		// ask for the student id.
		std::cout << "enter the student id:" << std::endl;
		std::cin >> id;
	}

	// ask for the password.
	std::cout << "enter the password:" << std::endl;
	std::cin >> password;

	// create a new user object based on the username and password.
	if (username == "admin") {
		return process_admin(password);
	}
	if (username == "student") {
		return process_student(id, password);
	}
	return false;
}

void CLI::process_command(const std::string& input) {
	const std::vector query{split_input(input)};
	if (query.empty()) {
		std::cerr << "Error: please enter a command." << std::endl;
		return;
	}
	// the first argument is the command and the rest are the arguments.
	const std::string& command{change_command_case(query[0])};

	std::vector<std::string> args{};
	for (size_t i = 1; i < query.size(); i++) { args.push_back(query[i]); }
	if (args.empty()) {
		if (command == "Exit") {
			set_running(false);
			clean_up();
			return;
		}
		if (command == "Logout") {
			clean_up();
			std::cout << "Logged out." << std::endl;
			return;
		}
	}
	m_user->execute(command, args);
}

std::vector<std::string> CLI::split_input(const std::string& input) {
	std::vector<std::string> query{};
	std::string words{};
	// iterate over the string line and split it into commands.
	for (const char c : input) {
		// if there is a space, push the command into the vector and clear the command.
		if (c == ' ') {
			query.push_back(words);
			words.clear();
		}
		else { words += c; }
	}
	// push the last command into the vector.
	query.push_back(words);
	return query;
}

bool CLI::process_admin(const std::string& password) {
	// check password.
	if (password != admin_password) {
		// log the error.
		std::cerr << "Error: invalid password." << std::endl;
		return false;
	}
	// create a new admin object and downcast it to admin.
	m_user = new Admin_User(password);
	// check if the admin wants to change the password.
	std::cout << "Do you want to change the password? (yes or no)" << std::endl;
	std::string change{};
	std::cin >> change;
	if (change == "yes") {
		std::string new_password{};
		std::cout << "enter the new password:" << std::endl;
		std::cin >> new_password;
		set_admin_password(new_password);
	}
	return true;
}

bool CLI::process_student(const std::string& id, const std::string& password) {
	// authenticate the student.
	if (!System_Operations::authenticate_student(id, password)) {
		return false;
	}
	// create a new student object.
	m_user = new Student_User(id, password);
	return true;
}

bool CLI::is_running() const { return m_running; }

void CLI::set_running(const bool running) { m_running = running; }

void CLI::set_admin_password(const std::string& password) { admin_password = password; }

std::string CLI::change_command_case(const std::string& command) {
	std::string new_command{command};
	// change the first letter to upper case.
	new_command[0] = static_cast<char>(std::toupper(new_command[0]));
	// change the rest of the letters to lower case.
	for (size_t i = 1; i < new_command.size(); i++) {
		new_command[i] = static_cast<char>(std::tolower(new_command[i]));
	}
	return new_command;
}