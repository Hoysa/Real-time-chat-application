#pragma once
#include "common.hpp"
#include <unordered_map>

struct Client {
    int id;
    std::string name;
    int socket;
    std::string group;
    std::thread th;
};

extern std::vector<Client> clients;
extern std::mutex cout_mtx, clients_mtx;

void broadcast_message(const std::string& message, int sender_id, const std::string& group="");
void private_message(const std::string& message, const std::string& receiver, int sender_id);
void shared_print(const std::string& message);
void set_name(int id, const char* name);
void end_connection(int id);
void handle_client(int client_socket, int id);
