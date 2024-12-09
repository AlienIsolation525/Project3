#include "Logger.h"

void Logger::save_message_log(std::string s, std::string sender, std::string receiver) {

	std::ofstream out(filename, std::ios::app);
	out << std::endl << count << ".  " << "user " << sender << " say '" << s
		<< "' to user " << receiver;
	count++;
	out.close();
};
