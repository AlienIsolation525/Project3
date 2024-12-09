#pragma once
#include <iostream>
#include <fstream>
#define  filename "log.txt"

class Logger {
public:
    Logger() = default;
    ~Logger() = default;

    //Save message to log(sender_id, message text, receiver id)
    void save_message_log(std::string s, std::string sender, std::string receiver);

private:
    int count = 1;
};