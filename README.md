# Simple_C_Networking_Library

## Introduction
A simple library that offers socket wrappers and functions to perform networking operations in C.

## Features
- **Socket wrapper:** A simple struct that contains a socket file descriptor as well as other structures to contain the data related to a networking endpoint (socket fd, address storage, socket type).
- **Full TCP and partial UDP support:** The sockets are capable of making connections and sending data through TCP or simply sending data to an address through UDP. In future updates, full UDP support will be added.
- **Functions to change byte order:** A simple set of functions that allow users to determine the endianness of their computer and swap the byte order of data structures of any length.
- **Functions to setup and perform connections:** Functions that allow users to connect to a host by specifying an address and port and that allow users to host with a socket on a specified port. 
- **Partial IP version agnostic support:** In most certain platforms (mainly unix systems), the current code allows for IPv6 capable sockets that permit IPv4 connections, making hosting and connecting completely IP version agnostic. This depends on multiple conditions on the system, so it requires some polish to get fully functional in all systems.
- **Protocol agnostic send and recv functions:** Functions that allow the user to send and receive data without having to specify what protocol you are using. This means that sockets created for UDP or TCP can directly work with code created for the other protocol with minimal change.
- **Operating system support:** The library works on POSIX compliant systems, as well as Windows (min version is Windows Vista). 

## Usage
Compile the C source files.
Include the header "dra_net.h" in your project.
Link the created object files.
Link against operating system specific dynamically linked libraries (example: winsock2 in Windows) 
```gcc my_program.c *.o -lws2_32 -o program.exe```

## Planned Features
- **Full IP version agnostic support**
- **Simplified build steps**
- **Full TCP and UDP support**
- **C++ wrappers**
- **Select and poll mechanics**
  
