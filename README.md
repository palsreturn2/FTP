# Command line tool for File Transfer Protocol 

# Synopsis

This is a command line tool for transfering files from one machine to another. One of the machines serves as a Server and the others serve as Clients. The client program has a command line interface and can be used for file transfer between itself and the server machine. Following commands are allowed.

1) ls: list all the files in the current directory in the client.
2) lls: list all the files in the current directory of the server.
3) get: Download a particular file from the server.
4) put: Upload a particular file to the server.
5) mget: Download multiple files from the server.
6) mput: Upload multiple files to the server.

The program has been tested on various kind of files like .FLV, .PDF, .TXT, .MP4, .MP3.

# Motivation

This was a lab assignment in Computer Networks lab at my University to learn how to use sockets to transfer information from one machine to another. 
