#include "Logger.h"

void Logger::save_message_log(std::string s, std::string sender, std::string receiver) {

    if (file.is_open())
    {
        std::lock_guard<std::mutex> lock(mutex);
        file << (countLines() + 1) << ".  " << "user " << sender << " say '" << s
            << "' to user " << receiver << std::endl;
    }
};

std::string Logger::readLine() {
    std::ifstream file(filename);
    std::string line;

    if(file.is_open())
    {
        std::lock_guard<std::mutex> lock(mutex);
        std::getline(file, line);
    }

    return line;
}

int Logger::countLines() {
    std::ifstream file(filename);
    std::string str;
    int res = 0;

    if (file.is_open())
    {
        std::lock_guard<std::mutex> lock(mutex);
        for (std::string line; getline(file, str); ) {
            res++;
        }
    }

    return res;
}