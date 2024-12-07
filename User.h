#pragma once
#include "Database.h"
#include "sha256.h"

class User
{
public:
    User(std::string str) { ID = str; };
    User() = default;
    ~User() = default;

    // Assigns hash password 
    void add_hash_pwd(std::string str, std::string hashPwd) {
        // Query
        std::string query;
        query = "INSERT INTO userpwd(user_id, hash_pwd) values('" + str + "', '" + hashPwd + "');";

        // Convert query string to wchar_t
        std::wstring temp = std::wstring(query.begin(), query.end());
        SQLWCHAR* t = (SQLWCHAR*)temp.data();

        // Execute
        if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, t, SQL_NTS)) {
            std::cout << "Error querying SQL Server \n";
            disconnect_sql();
        }

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

        // Convert query string to wchar_t
        std::wstring temp = std::wstring(query.begin(), query.end());
        SQLWCHAR* t = (SQLWCHAR*)temp.data();

        // Execute
        if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, t, SQL_NTS)) {
            std::cout << "Error querying SQL Server \n";
            disconnect_sql();
        }
        else
        {
            system("cls");
            std::cout << "\nUser registration success\n";
        }

        // Get current user id
        SQLCHAR current_id[SQL_RESULT_LEN];
        SQLLEN ptrCurrent_id;
        SQLINTEGER   V_OD_err, V_OD_id;
        std::string curr,
            hashPwd = sha256(_password);

        // Query
        if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT user_ID FROM useru ORDER BY user_ID DESC LIMIT 1", SQL_NTS)) {
            std::cout << "Error querying SQL Server \n";
            disconnect_sql();
        }
        SQLGetData(sqlStmtHandle, 1, SQL_CHAR, current_id, SQL_RESULT_LEN, &ptrCurrent_id);

        V_OD_err = SQLBindCol(sqlStmtHandle, 1, SQL_INTEGER, &V_OD_id, sizeof(V_OD_id), nullptr);
        while (SQLFetch(sqlStmtHandle) != SQL_NO_DATA) {
            curr = std::to_string(V_OD_id);
        }

        // Assigns hash(pwd)
        add_hash_pwd(curr, hashPwd);
    }

    // Find user attempted to login
    std::string get_login_id(std::string _name, std::string _surname, std::string _email) {
        SQLCHAR login_id[SQL_RESULT_LEN];
        SQLLEN ptrLogin_id;
        SQLINTEGER   V_OD_err, V_OD_id;
        std::string ID;

        // Query
        std::string query = "SELECT user_ID FROM useru WHERE name = '"
            + _name + "' AND surname = '" + _surname + "' AND email = '" + _email + "';";
        std::cout << query << std::endl;

        // Convert query string to wchar_t
        std::wstring temp = std::wstring(query.begin(), query.end());
        SQLWCHAR* t = (SQLWCHAR*)temp.data();

        if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, t, SQL_NTS)) {
            std::cout << "Error querying SQL Server \n";
            disconnect_sql();
        }

        SQLGetData(sqlStmtHandle, 1, SQL_CHAR, login_id, SQL_RESULT_LEN, &ptrLogin_id);

        V_OD_err = SQLBindCol(sqlStmtHandle, 1, SQL_INTEGER, &V_OD_id, sizeof(V_OD_id), nullptr);
        while (SQLFetch(sqlStmtHandle) != SQL_NO_DATA) {
            ID = std::to_string(V_OD_id);
        }
        return ID;
    }

    std::string get_password(std::string user_id) {
        const std::string query = "SELECT * FROM userpwd WHERE user_id = "
            + user_id + ";";
        std::cout << query << std::endl;

        // Convert query string to wchar_t
        std::wstring temp = std::wstring(query.begin(), query.end());
        SQLWCHAR* t = (SQLWCHAR*)temp.data();

        std::cout << "\nExecuting T-SQL query...\n";

        if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, t, SQL_NTS)) {
            std::cout << "Error querying SQL Server \n";
            disconnect_sql();
        }
        else {
            //ќбъ€вление структуры данных дл€ результата запроса версии SQL
            SQLLEN sql_str_length;
            SQLINTEGER   V_OD_err, V_OD_id;
            SQLCHAR     V_OD_buffer[200];
            V_OD_err = SQLBindCol(sqlStmtHandle, 2, SQL_CHAR, &V_OD_buffer, SQL_RESULT_LEN, &sql_str_length);

            SQLFetch(sqlStmtHandle);
            std::stringstream ss;
            ss << V_OD_buffer;
            std::string s;
            ss >> s;
            return s;
        }
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
        SQLHSTMT stmtHandle{ nullptr };
        std::string name, surname, email, ID;
};

class Message 
{
public:
    Message(User u) { this->user = u;
    };
    // Add message
    void add_message(std::string s) {
        // Query add message
        std::string query = "INSERT INTO message(text) values('" + s + "');";
        std::cout << query;
        std::wstring temp = std::wstring(query.begin(), query.end());
        SQLWCHAR* t = (SQLWCHAR*)temp.data();

        if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, t, SQL_NTS)) {
            std::cout << "Error querying SQL Server \n";
            disconnect_sql();
        }
        else
        {
            std::cout << "\nMessage sended succesfully\n";
        }

    }

    void send_message()
    {

        std::string text, receiver_id, curr;
        while (true)
        {
            system("cls");
            std::cout << "Enter id of recepient of message:";
            std::cin >> receiver_id;
            std::cout << "\nNow enter text (max 255 symbols): ";
            std::cin >> text;
            if (text.length() <= 255)break;
        }
        add_message(text);
        // Get id of freshly added message
        SQLCHAR current_id[SQL_RESULT_LEN];
        SQLLEN ptrCurrent_id;
        SQLINTEGER   V_OD_err, V_OD_id;

        if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT id FROM message ORDER BY id DESC LIMIT 1", SQL_NTS)) {
            std::cout << "Error querying SQL Server \n";
            disconnect_sql();
        }
        else std::cout << "\ncurrent id query\n";
        SQLGetData(sqlStmtHandle, 1, SQL_CHAR, current_id, SQL_RESULT_LEN, &ptrCurrent_id);

        V_OD_err = SQLBindCol(sqlStmtHandle, 1, SQL_INTEGER, &V_OD_id, sizeof(V_OD_id), nullptr);
        while (SQLFetch(sqlStmtHandle) != SQL_NO_DATA) {
            curr = std::to_string(V_OD_id);
        }

        // Query insert data in message_sended
        std::string query = "INSERT INTO message_sended(sender_id, message_id) values('" + user.get_ID() + "','" + curr + "');";
        std::cout << query;
        std::wstring temp1 = std::wstring(query.begin(), query.end());
        SQLWCHAR* t1 = (SQLWCHAR*)temp1.data();

        // Execute
        if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, t1, SQL_NTS)) {
            std::cout << "Error querying SQL Server \n";
            disconnect_sql();
        }
        else
        {
            std::cout << "\nmessage_sended\n";
        }

        // Query insert data in message_receiver id
        query = "INSERT INTO message_received(receiver_id, message_id) values('" + receiver_id + "','" + curr + "');";
        // Convert query string to wchar_t
        std::wstring temp2 = std::wstring(query.begin(), query.end());
        SQLWCHAR* t2 = (SQLWCHAR*)temp2.data();

        // Execute
        if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, t2, SQL_NTS)) {
            std::cout << "Error querying SQL Server \n";
            disconnect_sql();
        }
        else
        {
            std::cout << "\nmessage_sended\n";
        }

    };
private:
    SQLHSTMT stmtHandle{ nullptr };
    User user;
};

