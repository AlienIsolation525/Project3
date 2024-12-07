#pragma once

#include <iostream>
#include "sha256.h"
#include "mysql.h"
MYSQL mysql;
MYSQL_RES* res;
MYSQL_ROW row;



void disconnect_sql() {
    mysql_close(&mysql);
    std::cout << "\nDisconnected from sql server, press any key to exit...";
    system("pause");
}

void connect_sql() {

    mysql_init(&mysql);// Connection descriptor
    if (&mysql == nullptr) {
        std::cout << "Error: can't create MySQL-descriptor\n";
    }
    // Databse login - guest
    if (!mysql_real_connect(&mysql, "localhost", "guest_user", "privetasd", "chat", NULL, NULL, 0)) {
        std::cout << "Error: can't connect to database " << mysql_error(&mysql) << std::endl;
    }
    else {
        std::cout << "\nDatabase connection established\n";
    }

    mysql_set_character_set(&mysql, "utf8");// Char set (default - latin1)
    std::cout << "connection characterset: " << mysql_character_set_name(&mysql) << std::endl;

}

class User
{
public:
    User(std::string str) { this->ID = str; };
    User() = default;
    ~User() = default;

    void show_all_users() {
        mysql_query(&mysql, "SELECT * FROM useru");
        if (res = mysql_store_result(&mysql)) {
            while (row = mysql_fetch_row(res)) {
                for (int i = 0; i < mysql_num_fields(res); i++) {
                    std::cout << row[i] << "  ";
                }
                std::cout << std::endl;
            }
        }
        else
            std::cout << "ERROR: " << mysql_error(&mysql);
    }

    void show_all_messages() {
        std::string query;
        int count = 1;
        query = "select text from message join message_received on message.id = message_received.message_id where message_received.receiver_id = '" +
            ID + "';";
        const char* ptr = query.c_str();
        mysql_query(&mysql, ptr);
        if (res = mysql_store_result(&mysql)) {
            while (row = mysql_fetch_row(res)) {
                for (int i = 0; i < mysql_num_fields(res); i++) {
                    std::cout << count << ". " << row[i] << "  \n";
                    count++;
                }
                std::cout << std::endl;
            }
        }
        else
            std::cout << "ERROR: " << mysql_error(&mysql);
    }

    void add_message(std::string s) {
        // Query add message
        std::string query = "INSERT INTO message(text) values('" + s + "');";
        const char* ptr = query.c_str();
        mysql_query(&mysql, ptr);

    }

    std::string current_user() {
        mysql_query(&mysql, "SELECT id FROM message ORDER BY id DESC LIMIT 1");
        if (res = mysql_store_result(&mysql)) {
            while (row = mysql_fetch_row(res)) {
                for (int i = 0; i < mysql_num_fields(res); i++)
                return row[i];
            }
        }
        else
            std::cout << "ERROR: " << mysql_error(&mysql);
    }

    std::string current_message() {
        mysql_query(&mysql, "SELECT id FROM message ORDER BY id DESC LIMIT 1");
        if (res = mysql_store_result(&mysql)) {
            while (row = mysql_fetch_row(res)) {
                for (int i = 0; i < mysql_num_fields(res); i++)
                    return row[i];
            }
        }
        else
            std::cout << "ERROR: " << mysql_error(&mysql);
    }

    void send_message()
    {

        std::string text, receiver_id, curr;
        while (true)
        {
            while(true)
            {
                system("cls");
                std::cout << "Enter id of recepient of message:";
                std::cin >> receiver_id;
                if (receiver_id > current_user()) { 
                    std::cout << "User with this id doesn't exists yet\n";
                    system("pause"); 
                }
                else break;
            }
            std::cout << "\nNow enter text (max 255 symbols): ";
            std::cin >> text;
            if (text.length() <= 255)break;
        }
        add_message(text);

        // Get id of freshly added message
        curr = current_message();

        // Query insert data in message_sended
        std::string query = "INSERT INTO message_sended(sender_id, message_id) values('" + ID + "','" + curr + "');";
        const char* ptr = query.c_str();
        mysql_query(&mysql, ptr);

        // Query insert data in message_receiver id
        query = "INSERT INTO message_received(receiver_id, message_id) values('" + receiver_id + "','" + curr + "');";
        ptr = query.c_str();
        mysql_query(&mysql, ptr);
    };

    // Assigns hash password 
    void add_hash_pwd(std::string str, std::string hashPwd) {
        // Query
        std::string query;
        query = "INSERT INTO userpwd(user_id, hash_pwd) values('" + str + "', '" + hashPwd + "');";
        const char* ptr = query.c_str();
        mysql_query(&mysql, ptr);
    }

    // Create user and assigns password
    void registration() {

        std::string _name, _surname, _email, _password = "1", pwdcheck = "9";
        std::cout << "Greetings! Enter your name: ";
        std::cin >> _name;
        std::cout << "\nyour surname: ";
        std::cin >> _surname;
        std::cout << "\nemail address: ";
        std::cin >> _email;
        while (_password != pwdcheck) {

            std::cout << "\npassword: ";
            std::cin >> pwdcheck;
            std::cout << "\nconfirm your password: ";;
            std::cin >> _password;
        }

        // Query
        std::string query = "INSERT INTO useru(name, surname, email) values('"
            + _name + "', '" + _surname + "', '" + _email + "');";
        const char* ptr = query.c_str();
        mysql_query(&mysql, ptr);

        std::string curr = current_user();
        std::string hashPwd = sha256(_password);

        // Assigns hash(pwd)
        add_hash_pwd(curr, hashPwd);

        }

        // Find user attempted to login
    std::string get_login_id(std::string _name, std::string _surname, std::string _email) {
            // Query
            std::string query = "SELECT user_ID FROM useru WHERE name = '"
                + _name + "' AND surname = '" + _surname + "' AND email = '" + _email + "';";
            std::string id;
            const char* ptr = query.c_str();
            mysql_query(&mysql, ptr);
            if (res = mysql_store_result(&mysql)) {
                while (row = mysql_fetch_row(res)) {
                    for (int i = 0; i < mysql_num_fields(res); i++)
                        id = row[i];
                    return id;
                }
            }
            else
                std::cout << "Ошибка MySql номер " << mysql_error(&mysql);
        }

    std::string get_password(std::string user_id) {
            const std::string query = "SELECT * FROM userpwd WHERE user_id = "
                + user_id + ";";
            std::string pwd;
            const char* ptr = query.c_str();
            mysql_query(&mysql, ptr);
            if (res = mysql_store_result(&mysql)) {
                while (row = mysql_fetch_row(res)) {
                    for (int i = 0; i < mysql_num_fields(res); i++)
                        pwd = row[i];
                    return pwd;
                }
            }
            else
                std::cout << "Ошибка MySql номер " << mysql_error(&mysql);
        }

    // Function ask login data and check the password of selected user
    bool login() {
            std::string _name, _surname, _email, _password, hashpwd;
            std::cout << "User authorization \n Welcome! Enter your name: ";
            std::cin >> _name;
            std::cout << "\nyour surname: ";
            std::cin >> _surname;
            std::cout << "\nemail address: ";
            std::cin >> _email;
            std::cout << "\npassword: ";
            std::cin >> _password;
            std::string str = get_login_id(_name, _surname, _email);
            std::string pwd = get_password(str);
            if (pwd == sha256(_password))
            {
                name = _name;
                surname = _surname;
                email = _email;
                ID = str;
                return true;
            }
            else
                return false;
        };

    std::string get_ID() {
            return ID;
        }
private:
    std::string name, surname, email, ID;
    };



