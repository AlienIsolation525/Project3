#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#define  filename "log.txt"

class Logger {
public:
    Logger() {
        file.open(filename, std::ios_base::app);
    }
    ~Logger() {
        file.close();
    }

    // Save message to log(sender_id, message text, receiver id)
    void save_message_log(std::string s, std::string sender, std::string receiver);

    // Read line from log
    std::string readLine();

    // Return number of lines in log file
    int countLines();

private:
    int count = 1;
    std::mutex mutex;
    std::ofstream file;
    std::string line;
};