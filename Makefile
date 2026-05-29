# Makefile — Multi-Client TCP Chat
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread -O2

all: server client

server: server.cpp
	$(CXX) $(CXXFLAGS) -o server server.cpp

client: client.cpp
	$(CXX) $(CXXFLAGS) -o client client.cpp

clean:
	rm -f server client

# Usage:
#   ./server                       # start the server on :8080
#   ./client 127.0.0.1 Alice       # connect as Alice
#   ./client 127.0.0.1 Bob         # connect as Bob (new terminal)