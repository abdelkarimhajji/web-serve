#pragma once


#include "./parse/DataConfig.hpp"
#include "./parse/ParseConfigFile.hpp"

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <dirent.h>
#include <string>
#include <cstring>

#include <vector>
#include <map>

enum statusCodes {
    // GET REQUEST SUCCESSFUL
    OK = 200,
    // CLIENT SHOULD NOT REPEAT THE REQUEST WITHOUT MODFYING THE REQUEST
    PERMANENTLY_MOVED = 301,
    Found = 302,
    TEMPORARY_REDIRECT = 307,
    BAD_REQUEST = 400,
    // CLIENT AUTHENTIFICATION FAILED, MUST SEND WWW-AUTHENTICATE HEADER IN SOME CASES
    UNAUTHORIZED = 401,
    // ??
    PAYMENT_REQUIRED = 402,
    // EITHER SEND THE REASON THE SERVER DIDN'T TREAT THE REQUEST OR SEND INSTEAD 404
    FORBIDDEN = 403,
    // USE THIS EITHER WHEN THE SERVER DOESN'T WANT TO EXPLAIN THE REFUSAL OF THE REQUEST, OR WHEN THE REQUESTED RESSOURCE ISN'T AVAILABLE
    NOT_FOUND = 404,
    // METHOD IS NOT ALLOWED FOR THE REQUESTED RESSOURCE, MUST SEND IN THE RESPONSE THE ALLOWED METHODS FOR THIS SPECIFIC RESSOURCE
    METHOD_NOT_ALLOWED = 405,
    // THE REQUESTED RESSOURCE IS IN A FORMAT NOT ACCEPTED BY THE CLIENT (ACCEPT HEADER) / CAN ALSO SEND THE RESSOURCE, THE CLIENT SHOULD HANDLE IT ON IT'S OWN.
    NOT_ACCEPTABLE = 406,
    PROXY_AUTHENTIFICATION_REQUIRED = 407,
    // CLIENT DID NOT PRODUCE A REQUEST WITHIN THE ALLOWED TIME BY THE SERVER
    REQUEST_TIMEOUT = 408,
    // ??
    CONFLICT = 409,
    // RESSOURCE IS NO LONGER AVAILABLE TO THE SERVER (PROMOTIONAL STUFF) AND THE CLIENT SHOULD DELETE THE URI ACCESSING THIS RESSOURCE / SEND 404 IF THE RESSOURCE GONE ISN'T PERMANENT
    GONE = 410,
    // CONTENT LENGTH ISN'T PROVIDED FOR THE BODY
    LENGTH_REQUIRED = 411,
    // ??
    PRECONDITION_FAILED = 412,
    // REQUEST ENTITY EXCEEDS LENGTH SUPPORTED BY SERVER
    ENTITY_LENGTH_EXCEEDED = 413,
    REQUEST_URI_EXCEEDED = 414,
    // SERVER DOESN'T SUPPORT MEDIA TYPE OF REQUEST ENTITY
    UNSUPPORTED_MEDIA_TYPE = 415,
    RANGE_NOT_SATISFIABLE = 416,
    EXPECTATION_FAILED = 417,
    NOT_IMPLEMENTED = 501
};

class Server
{
    private:
        std::vector<int> serverSockets;
        std::map<int, DataConfig> servers;
        std::vector<sockaddr_in> serverAddress;

    public :
        void createSocket(DataConfig config);
        void createServer(std::vector<DataConfig> config);
        void putServerOnListening();

        const std::vector<int>& getServerSockets();
        const int& getServerSocket(int index);
        void setServerSocket(int socket);
        const std::vector<sockaddr_in>& getServerAddress();

        std::map<int, DataConfig>& getServers();
        void setServer(int socketFd, DataConfig config);
};