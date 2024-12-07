#include "User.h"

int winX, winY;
bool server;
User user;

void showMainMenu()
{
    char chatItem;
    bool _isOnline = 1, _loggined = 0;

        // Login menu
        do{
            system("cls");
        std::cout << "(1) LogIn\n(2) SignUp\n(0) End\n";
        std::cin >> chatItem;
        switch (chatItem)
        {
        case '1':
            while (true)
            {
                if (user.login())
                {
                    std::cout << " \nAuthorization success\n";
                    _loggined = 1;
                    break;
                }
                else
                    std::cout << " \nAuthorization unsuccess\n";
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

        // User actions menu
        do
        {
            system("cls");
            std::cout << "(1) Show users list\n(2) Show messages list\n(3) Send message\n(0) End\n";
            std::cin >> chatItem;
            switch (chatItem)
            {
            case '1':
                user.show_all_users();
                system("pause");
                break;
            case '2':
                user.show_all_messages();
                system("pause");
                break;
            case '3':
                user.send_message();
                break;
            case '0':
                _isOnline = false;
                break;
            default:
                std::cout << "1 2 or 3...\n";
                break;
            }

        } while (_isOnline);

}


int main() {
    // Connect sql
    connect_sql();

    MoveWindow(GetConsoleWindow(), 50, 50, 600, 550, true);
    User user;
    showMainMenu();

	// Disconnect sql
	disconnect_sql();
	system("Pause");
    return 0;
}
