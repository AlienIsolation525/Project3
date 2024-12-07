#include <iostream>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib") 
#pragma warning(disable:4996) 
#define SERVER "127.0.0.1"  // or "localhost" - ip address of UDP server
#define BUFLEN 512  // max length of answer
#define PORT 8888  // the port on which to listen for incoming data

// ÊCLient
class UDPClient {
public:
    UDPClient() {
        // initialise winsock
        std::cout << "Initialising Winsock...\n";
        if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
            std::cout << "Failed. Error Code: " << WSAGetLastError() << "\n";
            exit(EXIT_FAILURE);
        }
        std::cout << "Initialised.\n";

        // create socket
        if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) {
            std::cout << "socket() failed with error code: " << WSAGetLastError() << "\n";
            exit(EXIT_FAILURE);
        }

        // setup address structure
        memset((char*)&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_port = htons(PORT);
        server.sin_addr.S_un.S_addr = inet_addr(SERVER);
    }

    ~UDPClient() {
        closesocket(client_socket);
        WSACleanup();
    }

    void start() {
        while (true) {
            char message[BUFLEN];
            std::cout << "Enter message: ";
            std::cin.getline(message, BUFLEN);

            // send the message
            if (sendto(client_socket, message, strlen(message), 0, (sockaddr*)&server, sizeof(sockaddr_in)) == SOCKET_ERROR) {
                std::cout << "sendto() failed with error code: " << WSAGetLastError() << "\n";
                exit(EXIT_FAILURE);
            }

            // receive a reply and print it
            // clear the answer by filling null, it might have previously received data
            char answer[BUFLEN] = {};

            // try to receive some data, this is a blocking call
            int slen = sizeof(sockaddr_in);
            int answer_length;
            if ((answer_length = recvfrom(client_socket, answer, BUFLEN, 0, (sockaddr*)&server, &slen)) == SOCKET_ERROR) {
                std::cout << "recvfrom() failed with error code: " << WSAGetLastError() << "\n";
                exit(EXIT_FAILURE);
            }

            //////
            std::cout << "Server's response: " << answer << "\n";
        }
    }

private:
    WSADATA ws;
    SOCKET client_socket;
    sockaddr_in server;
};

 // Server
class UDPServer {
public:
    UDPServer() {
        // initialise winsock
        std::cout << "Initialising Winsock...\n";
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
            std::cout << "Failed. Error Code: " << WSAGetLastError() << "\n";
            exit(0);
        }
        std::cout << "Initialised.\n";

        // create a socket
        if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
            std::cout << "Could not create socket: " << WSAGetLastError() << "\n";
            exit(EXIT_FAILURE);
        }

        // prepare the sockaddr_in structure
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons(PORT);

        // bind
        if (bind(server_socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
            std::cout << "Bind failed with error code: " << WSAGetLastError() << "\n";
            exit(EXIT_FAILURE);
        }
    }

    ~UDPServer() {
        closesocket(server_socket);
        WSACleanup();
    }

    void start() {
        while (!exitRequested) {
            char message[BUFLEN] = {};

            // try to receive some data, this is a blocking call
            int message_len;
            int slen = sizeof(sockaddr_in);
            if ((message_len = recvfrom(server_socket, message, BUFLEN, 0, (sockaddr*)&client, &slen)) == SOCKET_ERROR) {
                std::cout << "recvfrom() failed with error code: " << WSAGetLastError() << "\n";
                exit(0);
            }

            // /////
            std::cout << "Message Received from Client: " << message << "\n";

            std::cout << "Enter response (exit to stop server process): ";
            std::cin.getline(message, BUFLEN);

            // reply to the client with the same data
            if (sendto(server_socket, message, strlen(message), 0, (sockaddr*)&client, sizeof(sockaddr_in)) == SOCKET_ERROR) {
                std::cout << "sendto() failed with error code: " << WSAGetLastError() << "\n";
                exit(EXIT_FAILURE);
            }

            if (strcmp(message, "exit") == 0) {
                std::cout << "Exiting server...\n";
                exitRequested = true;
                break;
            }
        }
    }

private:
    WSADATA wsa{};
    SOCKET server_socket = 0;
    sockaddr_in server{}, client{};
    bool exitRequested = false;
};
