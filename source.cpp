#include "User.h"
#include "Database.h"
#include "sha256.h"

void user_registration() {

    std::string _name, _surname, _email, _password = "1", pwdcheck = "9";
    std::cout << "Greetings! Enter your name: ";
    std::cin >> _name;
    std::cout << "\nyour surname: ";
    std::cin >> _surname;
    std::cout << "\nemail address: ";
    std::cin >> _email;
    while (_password != pwdcheck){

        std::cout << "\npassword: ";
        std::cin >> pwdcheck;
        std::cout << "\nconfirm your password: ";;
        std::cin >> _password;
    }

    // INSERT INTO useru(name, surname, email) registrartion process
    // Forming query string
    const std::string query = "INSERT INTO useru(name, surname, email) values('"
        + _name + "', '" + _surname + "', '" + _email + "');";
    std::cout << query;
    // Convert query string to wchar_t
    std::wstring temp = std::wstring(query.begin(), query.end());
    SQLWCHAR* t = (SQLWCHAR*)temp.data();
    // Execute
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, t, SQL_NTS)) {
        std::cout << "Error querying SQL Server \n";
        disconnect_sql();
    }
    else
        std::cout << "\nUser registration success";

    // Get current user id
    SQLCHAR current_id[SQL_RESULT_LEN];
    SQLLEN ptrCurrent_id;

    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT id FROM useru ORDER BY ID DESC LIMIT 1", SQL_NTS)) {
        std::cout << "Error querying SQL Server \n";
        disconnect_sql(); 
    }


            SQLGetData(sqlStmtHandle, 1, SQL_CHAR, current_id, SQL_RESULT_LEN, &ptrCurrent_id);

            //UPDATE password in 'userpwd' TABLE
            //UPDATE userpwd SET hash_pwd = 'result of hash function' WHERE user_id = current_id;
            
            //sqlchar to string current_id -> Scurrent_id

            char* Ccurrent_id = (char*)current_id;
            char SCcurent_id = Ccurrent_id;
            const std::string query2 = "UPDATE userpwd SET userpwd.hash_pwd='" + sha256(_password) + "' WHERE userpwd.user_id=" + &(char*)current_id + ";";
            std::cout << query2;
            // Convert query string to wchar_t
            std::wstring temp2 = std::wstring(query2.begin(), query2.end());
            SQLWCHAR* t2 = (SQLWCHAR*)temp.data();
            // Execute
            if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, t2, SQL_NTS)) {
                std::cout << "\nError querying SQL Server \n";
                disconnect_sql();
             }

}

int main() {
    using namespace std;

    string username;
    int q;

	// Connect sql
	connect_sql();
	system("Pause");

    // Options

    // Registrarion
	user_registration();

    cout << endl << "Would you like to go client or like server?" << endl << "SERVER - 1   " << "CLIENT - 2" << endl;
    cin >> q;

    // Server
    if (q == 1) {
        system("title UDP Server");

        MoveWindow(GetConsoleWindow(), 50, 50, 500, 500, true);

        UDPServer udpServer;
        udpServer.start();
    }


    // Client
    if (q == 2) {
        system("title UDP Client");

        MoveWindow(GetConsoleWindow(), 50, 50, 500, 500, true);

        UDPClient udpClient;
        udpClient.start();
    }





	// Disconnect sql
	disconnect_sql();
	system("Pause");
}
