#include "common.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <csignal>
#include <cstring>
#include <iostream>
#include <thread>

bool exit_flag = false;
std::thread t_send, t_recv;
int client_socket;

void catch_ctrl_c(int signal) {
    char exit_msg[MAX_LEN] = "#exit";
    send(client_socket, exit_msg, sizeof(exit_msg), 0);
    exit_flag = true;
    t_send.detach();
    t_recv.detach();
    close(client_socket);
    exit(signal);
}

void send_message() {
    while (!exit_flag) {
        std::cout << color(1) << "You: " << def_col;
        char str[MAX_LEN];
        std::cin.getline(str, MAX_LEN);
        send(client_socket, str, sizeof(str), 0);
        if (strcmp(str, "#exit") == 0) break;
    }
}

void recv_message() {
    while (!exit_flag) {
        char str[MAX_LEN];
        int bytes_received = recv(client_socket, str, sizeof(str), 0);
        if (bytes_received <= 0) continue;
        std::cout << color(2) << str << def_col << std::endl;
        std::cout << color(1) << "You: " << def_col;
        fflush(stdout);
    }
}

int main() {
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket error");
        return -1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(10000);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect error");
        return -1;
    }

    signal(SIGINT, catch_ctrl_c);

    char name[MAX_LEN];
    std::cout << "Enter your name: ";
    std::cin.getline(name, MAX_LEN);
    send(client_socket, name, sizeof(name), 0);

    std::cout << colors[NUM_COLORS - 1] << "\n\t====== Welcome to the chatroom ======" << def_col << std::endl;
    std::cout << "\nCommands available:\n";
    std::cout << "/pm username message : send private message\n";
    std::cout << "/group groupname : join group chat\n";
    std::cout << "Normal messages are public unless in group.\n\n";

    t_send = std::thread(send_message);
    t_recv = std::thread(recv_message);

    t_send.join();
    t_recv.join();

    return 0;
}
