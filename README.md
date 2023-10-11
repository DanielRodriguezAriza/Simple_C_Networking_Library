# Simple_C_Networking_Library

## Introduction
A simple library that offers socket wrappers and functions to perform networking operations in C.

## Features
- **Socket wrapper:** A simple struct that contains a socket file descriptor as well as other information related to the socket (IP version and wether the protocol is TCP or UDP).
- **Address wrapper**: A simple data structure that contains the information of an address. Can be used to retrieve the IP of an accepted connection or any other purpose.
- **Full TCP and partial UDP support:** The sockets are capable of making connections and sending data through TCP or simply sending data to an address through UDP. In future updates, full UDP support will be added.
- **Functions to change byte order:** A simple set of functions that allow users to determine the endianness of their computer and swap the byte order of data structures of any length.
- **Functions to setup and perform connections:** Functions that allow users to connect to a host by specifying an address and port and that allow users to host with a socket on a specified port. 
- **Full IP version agnostic support:** Full dual stack support for systems that allow it (example: BSD does not allow Dual stack due to security concerns). Use the appropriate functions to limit your code to IPv4, IPv6 or be completely IPv agnostic if your system allows it. If there is no dual stack support in your system, it will automatically select whichever IPv is supported in your system out of the box. All you have to do is use the appropriate set ipv function.
- **Send and Recv functions for both TCP and UDP:** Functions that allow the user to send and receive data, as well as retrieve the address from which the data was sent.
- **Crossplatform Operating System support:** The library works on POSIX compliant systems, as well as Windows (minimum version required is Windows Vista, because the Windows support makes use of Winsock2).
- **Poll support**: operating system agnostic polling functions that work using the OS provided polling mechanisms under the hood. Offers a simple crossplatform interface for polling.
- **Byte order changing functions:** A set of functions that allow the users to change the byte order of variable size data structures and buffers, so that the byte order of any object can be changed to network and host byte orders without much effort.

## Usage
Compile the C source files.
Include the header "dra_net.h" in your project.
Link the created object files.
Link against operating system specific dynamically linked libraries (example: winsock2 in Windows) 
```gcc my_program.c *.o -lws2_32 -o program.exe```

## Planned Features
- **Simplified build steps**
- **Endpoint data structure:** The last update decoupled the logic of a socket and an address storage. Now, a planned feature will be an optional Endpoint structure that contains both the address and socket of a connection.
- **Simplified interface**
- **C++ wrappers**
  
