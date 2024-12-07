#include "User.h"

int winX, winY;
bool server;
User user;

void showMainMenu()
{
    char chatItem;
    bool _isOnline = 1, _loggined = 0;
        // Options
        std::cout << "(1) Change console window size(default-660 700)\n(2) Edit client-server configuration\n(0) End\n";
        std::cin >> chatItem;
        switch (chatItem)
        {
        case '1':
            int q, w;
            std::cin >> q >> w;
            MoveWindow(GetConsoleWindow(), 50, 50, q, w, true);
            break;
        case '2':
            std::cout << "Would you like to go like client or like server?\n(0) Client\n(1) Server\n";
            std::cin >> server;
            break;
        case '0':
            system("cls");
            break;
        default:
            std::cout << "1 or 2...\n";
            break;
        }
        do
        {
         // Login menu
        std::cout << "(1) LogIn\n(2) SignUp\n(0) End\n";
        std::cin >> chatItem;
        switch (chatItem)
        {
        case '1':
            while (true)
            {
                if (user.login())
                {
                    std::cout << " \nAuthorization success";
                    _loggined = 1;
                    break;
                }
                else
                {
                    std::cout << " \nAuthorization unsuccess";
                    break;
                }
            }
            break;
        case '2':
            user.registration();
            break;
        case '0':
            _isOnline = false;
            break;
        default:
            std::cout << "1 or 2...\n";
            break;
        }

    } while (_isOnline && !_loggined);
}

int main() {
    // Connect sql
    connect_sql();

    MoveWindow(GetConsoleWindow(), 50, 50, 660, 700, true);
    User user;
    User test("0");
    Message message(test);

    //showMainMenu();

    message.send_message();
    // Server
    if (server) {
        system("title UDP Server");

        UDPServer udpServer;
        udpServer.start();
    }

    // Client
    else {
        system("title UDP Client");

        UDPClient udpClient;
        udpClient.start();
    }

	// Disconnect sql
	disconnect_sql();
	system("Pause");
}
