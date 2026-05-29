# Linux Multi-Client Chat Application

A multi-client chat application built using C++, Linux socket programming, and multithreading. The project demonstrates client-server communication, TCP networking, concurrent programming, and synchronization using mutexes.

## Features

* Multi-client communication
* TCP socket-based networking
* Client-server architecture
* Concurrent client handling using multithreading
* Thread-safe shared resource management using mutexes
* Message broadcasting to connected clients
* Client disconnect handling

## Technologies Used

* C++
* Linux
* POSIX Sockets
* Multithreading (std::thread)
* Mutex Synchronization
* Makefile
* Git & GitHub

## Project Structure

```text
.
├── server.cpp
├── client.cpp
├── Makefile
└── README.md
```

## Build Instructions

Compile using Makefile:

```bash
make
```

Or compile manually:

```bash
g++ server.cpp -o server -pthread
g++ client.cpp -o client -pthread
```

## Running the Application

Start the server:

```bash
./server
```

Start a client:

```bash
./client 127.0.0.1 Alice
```

Open another terminal and start a second client:

```bash
./client 127.0.0.1 Bob
```

## Example

Alice sends:

```text
Hello Bob
```

Bob receives:

```text
Alice: Hello Bob
```

## Concepts Demonstrated

* Socket Programming
* TCP/IP Networking
* Client-Server Architecture
* Concurrent Programming
* Multithreading
* Mutex Synchronization
* Linux System Programming

## Future Enhancements

* Private messaging
* User authentication
* Multiple chat rooms
* Message timestamps
* Chat history storage
* Epoll-based scalability
* Thread pool implementation


