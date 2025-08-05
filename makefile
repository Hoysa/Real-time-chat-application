all: server client

server: server.cpp common.cpp
	g++ server.cpp common.cpp -o server -pthread

client: client.cpp common.cpp
	g++ client.cpp common.cpp -o client -pthread

clean:
	rm -f server client chat_log.txt
