#include "Server.hpp"
#include "parse/DataConfig.hpp"

void Server::createSocket(DataConfig config)
{
    std::vector<std::string> ports = config.getListen();
    size_t i;
    for (i = 0; i < ports.size(); i++)
    {
        int socketFd = socket(AF_INET, SOCK_STREAM, 0);
        if (socketFd == -1)
        {
            std::cout << "Failed to create socket. Exiting..." << std::endl;
            exit(1);
        }
        int enable = 1;
        if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
            std::cerr << "setsockopt(SO_REUSEADDR) failed";
        setServerSocket(socketFd);
        servers[socketFd] = config;
    }
    std::cout << "[INFO] " << i << " Sockets successfully created" << std::endl;
}

void Server::createServer(std::vector<DataConfig> config)
{
    for (size_t i = 0; i < config.size(); i++)
    {
        std::vector<std::string> ports = config[i].getListen();
        for (size_t j = 0; j < ports.size(); j++)
        {
            sockaddr_in address;
            std::memset(&address, 0, sizeof(address));
            address.sin_family = AF_INET; // address family : ipv4
            // address.sin_addr.s_addr = INADDR_ANY;
            // address.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // specify the IP address to which the server will bind
            // std::cout << "host :" << config[i].getHost().c_str() << "\n";
            // address.sin_addr.s_addr = inet_addr(config[i].getHost().c_str()); // specify the IP address to which the server will bind
            if (!strcmp(config[i].getHost().c_str(), "localhost"))
            {
                address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
            }
            else
            {
                // Resolve host to its IP address
                if (inet_pton(AF_INET, config[i].getHost().c_str(), &address.sin_addr) <= 0) {
                    std::cerr << "Invalid address or address not supported\n";
                    return ;
                }
            }
            address.sin_port = htons(atoi(ports[j].c_str()));
            serverAddress.push_back(address);
        }
    }
 
    for (size_t i = 0; i < serverSockets.size(); i++)
    {
        int bindResult = bind(this->getServerSocket(i), reinterpret_cast<sockaddr*>(&(this->serverAddress[i])), sizeof(this->serverAddress[i]));
        if (bindResult == -1)
        {
            std::cout << "Failed to bind the socket.to port " << ntohs(serverAddress[i].sin_port) << ". Exiting..." << std::endl;
            close(this->getServerSocket(i));
            exit(1);
        }
        std::cout << "[INFO] Server created : socket successfully binded with 127.0.0.1:" << ntohs(serverAddress[i].sin_port) << std::endl;
    }
}

void Server::putServerOnListening()
{
    for (size_t i = 0; i < serverSockets.size(); i++)
    {
        int listenResult = listen(getServerSocket(i), SOMAXCONN);
        if (listenResult == -1) {
            std::cerr << "Failed to listen on socket for port " << ntohs(serverAddress[i].sin_port) << ". Exiting..." << std::endl;
            close(getServerSocket(i));
            exit(1);
        }
        std::cout << "[INFO] Server listening on port " << ntohs(serverAddress[i].sin_port) << std::endl;
    }
}

const std::vector<int>& Server::getServerSockets()
{
    return serverSockets;
}
const std::vector<sockaddr_in>& Server::getServerAddress()
{
    return serverAddress;
}

const int& Server::getServerSocket(int index)
{
    return (this->serverSockets[index]);
}

void Server::setServerSocket(int socket)
{
    this->serverSockets.push_back(socket);
}

std::map<int, DataConfig>& Server::getServers()
{
    return (this->servers);
}

void Server::setServer(int socketFd, DataConfig config)
{
    this->servers[socketFd] = config;
}