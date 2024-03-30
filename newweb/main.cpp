#include "Server.hpp"
#include <fstream>
#include <vector>
#include <list>
#include "httpstuff/Request.hpp"
#include "httpstuff/Response.hpp"
#include "parse/DataConfig.hpp"
#include <cstring>
// cretae a socket
// bind the socket to IP / port
// mark the socket for listening
// accept a call
// close the listening socket
// 
// close socket

int main()
{
    Server server;
    ParseConfigFile config;
    std::list<Response> chunkedResponses;
    config.parser("./parse/configfile.txt");
    for (size_t i = 0; i < config.getData().size(); i++)
    {
        server.createSocket(config.getData()[i]);
    }
    
    server.createServer(config.getData());
    server.putServerOnListening();

    std::vector<int> activeConnections;
    fd_set setOfFds, readSet, writeSet;
    FD_ZERO(&setOfFds);
    int max_fd = -1;
    for (size_t i = 0; i < server.getServerSockets().size(); i++)
    {
        int fd = server.getServerSocket(i);
        FD_SET(fd, &setOfFds);
        if (fd > max_fd)
            max_fd = fd;
    }
 
    while (true) 
    {
        readSet = setOfFds;
        writeSet = setOfFds;
        int selectResult = select(max_fd + 1, &readSet, &writeSet, NULL, NULL);
        if (selectResult == -1)
        {
            std::cerr << "Error in select\n";
            return 1;
        }
        for (size_t i = 0; i < server.getServerSockets().size(); i++)
        {
            if (FD_ISSET(server.getServerSocket(i), &readSet))
            {
                int clientSocket = accept(server.getServerSocket(i), NULL, NULL);
                if (clientSocket == -1)
                {
                    std::cerr << "Error accepting client connection\n";
                }
                else
                {
                    int flags = fcntl(clientSocket, F_GETFL, 0);
                    if (flags == -1) {
                        std::cerr << "Error getting flags for socket\n";
                        return 1;
                    }
                    // std::cout << "*********************************>flags : " << flags << "\n";
                    flags |= O_NONBLOCK;
                    if (fcntl(clientSocket, F_SETFL, flags) == -1) {
                        std::cerr << "Error setting socket to non-blocking\n";
                        return 1;
                    }
                    
                    std::cout << "New client connected, socket: " << clientSocket << "\n";
                    if (clientSocket > max_fd)
                        max_fd = clientSocket;
                    // std::cout << "max = " << max_fd << "\n";
                    activeConnections.push_back(clientSocket);
                    
                    FD_SET(clientSocket, &setOfFds);
                    server.setServer(clientSocket, server.getServers()[server.getServerSocket(i)]);
                }
            }
        }
            for(std::vector<int>::iterator it = activeConnections.begin(); it != activeConnections.end(); ++it)
            {
                if (FD_ISSET(*it, &readSet))
                {
                    char buffer[4096];
                    memset(buffer, 0, 4096);
                    ssize_t bytesRead = recv(*it, buffer, 4096 - 1, 0); //receive request
                    if (bytesRead > 0)
                    {
                        DataConfig config = server.getServers()[*it];
                        
                        Request req(buffer);
                        Response response = req.handleRequest(config);
                        response.sendResponse(*it);
                    } 
                    else if (bytesRead == 0)
                    {
                        // Connection closed by the client
                        std::cout << "Client disconnected, socket: " << *it << "\n";
                        close(*it);
                        FD_CLR(*it, &setOfFds);
                        it = activeConnections.erase(it) - 1; // Decrement iterator to avoid skipping the next element
                    }
                    else
                    {
                        // Error in recv
                        std::cerr << "Error receiving data from client, socket: " << *it << "\n";
                        close(*it);
                        FD_CLR(*it, &setOfFds);
                        it = activeConnections.erase(it) - 1;
                    }
                }
            }
    }
}