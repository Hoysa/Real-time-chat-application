#  Terminal-Based Multi-Client Chat Application
### *Sasken Internship Program – 2025*

> **Built with C++ | TCP Sockets | Multithreading | Linux**

This project delivers a functional, terminal-based multi-client chat system. It enables real-time communication through broadcast, group, and private messaging using TCP sockets and multithreading in C++. Designed to demonstrate scalable and concurrent client-server architecture, this application provides a strong foundation for advanced networked applications.

---


This project emphasizes core principles in systems programming, including socket communication, concurrency, thread safety, and structured logging.
##  Objective

Create a powerful yet minimalist **terminal-based chat system** that allows multiple users to:

- Chat in real-time
- Send private messages
- Join dynamic groups
- Log all activities on the server
- Enjoy a colorful user experience via the command line


---


## Features

 **Real-Time Group Chat**  
 **Private Messaging via `/pm`**  
 **Dynamic Group Creation with `/group`**  
 **Thread-safe Multi-client Handling**  
 **Color-coded Terminal Messages**  
 **Chat History Logging on Server Side**

---

##  How It Works

### Server Logic

- Listens on port `10000` for incoming connections.
- Creates a **dedicated thread** for every client using `std::thread`.
- Stores client data: Username, Socket, Group name, Thread ID.
- Supports:
  -  Broadcast Messaging (everyone)
  -  Group Messaging (same group only)
  -  Private Messaging (one-to-one)
  -  Chat Logging (`chat_log.txt`)

###  Client Logic

- Prompted to enter a **username** upon connection.
- Clients can:
  - Chat with all users
  - Join/create groups using `/group groupname`
  - Send private messages via `/pm username message`
  - Exit chat using `#exit`
- Uses **two threads**:
  - One for sending messages
  - One for receiving messages (non-blocking)

---

##  Commands Reference

| Command                       | Description                                     |
|------------------------------|-------------------------------------------------|
| `/group <groupname>`         | Join or create a group                         |
| `/pm <username> <message>`   | Send a private message                         |
| `#exit`                      | Exit the chat gracefully                       |

>  *Groups are created dynamically. A user can only belong to one group at a time.*

---

##  Sample Chat Flow

```
Client1: Hello everyone! 
→ Broadcasted to all clients

Client2: /group devs
→ Joined group "devs"

Client2: Hello devs team! 
→ Message visible only to users in "devs"

Client1: /pm Client2 Hey, need a quick sync?
→ Private message sent to Client2 only

Server Log: All of this is recorded in `chat_log.txt`
```

---

##  Build & Run

###  Compile

```bash
g++ server.cpp -o server -pthread
g++ client.cpp -o client -pthread
```

###  Run

**Start Server**
```bash
./server
```

**Start Clients (in separate terminals)**
```bash
./client
```

---

## Technical Concepts

| Concept            | Description |
|-------------------|-------------|
| **TCP Sockets**   | Ensures reliable communication between server and clients |
| **Multithreading**| Enables simultaneous client handling using `std::thread` |
| **Mutex**         | Protects shared resources with `std::mutex` |
| **File I/O**      | Chat logs written to `chat_log.txt` for traceability |
| **ANSI Colors**   | Enhances message readability with terminal colors |

---

##  Future Enhancements

Here’s what can make this project even cooler:

-  **User Authentication**
-  **File Sharing Support**
-  **Admin/Moderator Roles**
-  **Persistent Chat History**
-  **WebSocket or GUI Interface**
-  **Mobile/GUI Version using Qt or ncurses**

---

##  Conclusion

This terminal-based chat app is more than just a messaging tool — it’s a hands-on demonstration of:

-  Multithreading & Synchronization
-  TCP Socket Programming
-  Real-time Client-Server Interaction
-  Structured Logging and Clean CLI UX



 
*For the Sasken Internship Program 2025*


