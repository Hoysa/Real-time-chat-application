#include "server.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

std::vector<Client> clients;
std::mutex cout_mtx, clients_mtx;

void broadcast_message(const std::string& message, int sender_id, const std::string& group) {
    std::lock_guard<std::mutex> lock(clients_mtx);
    for (auto& client : clients) {
        if (client.id != sender_id && (group.empty() || client.group == group)) {
            send(client.socket, message.c_str(), message.length() + 1, 0);
        }
    }
}

void private_message(const std::string& message, const std::string& receiver, int sender_id) {
    std::lock_guard<std::mutex> lock(clients_mtx);
    for (auto& client : clients) {
        if (client.name == receiver) {
            send(client.socket, message.c_str(), message.length() + 1, 0);
            break;
        }
    }
}

void shared_print(const std::string& message) {
    std::lock_guard<std::mutex> lock(cout_mtx);
    std::cout << message << std::endl;
}

void set_name(int id, const char* name) {
    std::lock_guard<std::mutex> lock(clients_mtx);
    for (auto& client : clients) {
        if (client.id == id) {
            client.name = name;
            break;
        }
    }
}

void end_connection(int id) {
    std::lock_guard<std::mutex> lock(clients_mtx);
    for (auto it = clients.begin(); it != clients.end(); ++it) {
        if (it->id == id) {
            it->th.detach();
            close(it->socket);
            clients.erase(it);
            break;
        }
    }
}

void handle_client(int client_socket, int id) {
    char name[MAX_LEN], str[MAX_LEN];
    recv(client_socket, name, sizeof(name), 0);
    set_name(id, name);

    std::string welcome_msg = std::string(name) + " has joined.";
    broadcast_message(welcome_msg, id);
    shared_print(color(id) + welcome_msg + def_col);
    log_message(welcome_msg);

    while (true) {
        int bytes_received = recv(client_socket, str, sizeof(str), 0);
        if (bytes_received <= 0) break;

        std::string message = std::string(str);
        if (message == "#exit") {
            std::string leave_msg = std::string(name) + " has left.";
            broadcast_message(leave_msg, id);
            shared_print(color(id) + leave_msg + def_col);
            log_message(leave_msg);
            end_connection(id);
            break;
        } else if (message.rfind("/group ", 0) == 0) {
            std::string group_name = message.substr(7);
            for (auto& client : clients) {
                if (client.id == id) client.group = group_name;
            }
            std::string group_msg = std::string(name) + " joined group: " + group_name;
            broadcast_message(group_msg, id);
            shared_print(color(id) + group_msg + def_col);
            log_message(group_msg);
        } else if (message.rfind("/pm ", 0) == 0) {
            size_t pos = message.find(' ', 4);
            if (pos != std::string::npos) {
                std::string receiver = message.substr(4, pos - 4);
                std::string pm_msg = message.substr(pos + 1);
                std::string formatted_msg = "[PM from " + std::string(name) + "]: " + pm_msg;
                private_message(formatted_msg, receiver, id);
                shared_print(color(id) + formatted_msg + def_col);
                log_message(formatted_msg);
            }
        } else {
            std::string full_message = std::string(name) + " : " + message;
            std::string sender_group;
            {
                std::lock_guard<std::mutex> lock(clients_mtx);
                for (auto& client : clients) {
                    if (client.id == id) {
                        sender_group = client.group;
                        break;
                    }
                }
            }
            broadcast_message(full_message, id, sender_group);
            shared_print(color(id) + full_message + def_col);
            log_message(full_message);
        }
    }
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket error");
        return -1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(10000);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind error");
        return -1;
    }

    if (listen(server_socket, 10) == -1) {
        perror("listen error");
        return -1;
    }

    shared_print("Server started on port 10000");

    int id_counter = 0;
    while (true) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_len);
        if (client_socket == -1) {
            perror("accept error");
            continue;
        }

        id_counter++;
        std::thread t(handle_client, client_socket, id_counter);
        {
            std::lock_guard<std::mutex> lock(clients_mtx);
            clients.push_back({id_counter, "Anonymous", client_socket, "", std::move(t)});
        }
    }

    close(server_socket);
    return 0;
}
