#include "Request.hpp"
#include "RequestMethods.hpp"
#include "Response.hpp"
#include <string>
#include <vector>
#include <sys/stat.h>

//  ******** CONSTRUCTORS ********

Request::Request(std::string buffer) {
    requestEntity = buffer;
    parseRequest(buffer, "\r\n");
    path = "";
    location = "";
}

Request::~Request() {

}

//  ******** GETTERS ********

std::string Request::getRequestMethod() const {
    return (this->requestMethod);
}

std::string Request::getRequestRessource() const {
    return (this->requestRessource);
}

std::string Request::getHttpVersion() const {
    return (this->httpVersion);
}

std::string Request::getHeader(std::string key) const {
    std::map<std::string, std::string>::const_iterator it = headers.find(key);
    if (it != headers.end())
        return (it->second);
    else
        return "";
}

std::string Request::getBody() const {
    return (this->body);
}

std::string Request::getPath() const {
    return (this->path);
}

std::string Request::getLocation() const {
    return (this->location);
}

//  ******** PARSING METHODS ********

std::string& Request::trimSpaces(std::string& val) {
    std::string whiteSpaces = "\t ";
    val.erase(val.find_last_not_of(whiteSpaces) + 1);
    val.erase(0, val.find_first_not_of(whiteSpaces));
    if (val[val.size() - 1] == '?')
        val = val.substr(0, val.size() - 1);
    return (val);
}

void Request::parseRequestLine(std::string buffer) {
    std::stringstream requestLine(buffer);
    std::vector<std::string> tokens;
    std::string token;

    while (std::getline(requestLine, token, ' ')) {
        tokens.push_back(token);
    }
    this->requestMethod = trimSpaces(tokens[0]);
    this->requestRessource = trimSpaces(tokens[1]);
    this->httpVersion = trimSpaces(tokens[2]);
}

void Request::parseHeaders(std::string buffer) {
    std::stringstream requestLine(buffer);
    std::string key;
    std::string value;

    std::getline(requestLine, key, ':');
    std::getline(requestLine, value, ':');
    this->headers[trimSpaces(key)] = trimSpaces(value);
}

int Request::checkAllowedChars(std::string value) {
    std::string allowedChars = "-._~:/?#[]@!$&&\'()*+;=%}";
    for (size_t i = 0; i < value.size(); i++) {
        if (!std::isalpha(value[i]) && !std::isdigit(value[i]) && allowedChars.find(value[i]) == std::string::npos) {
            // change the hardcoded value into a macro
            return (400);
        }
    }
    return (0);
}

void Request::parseRequest(std::string buffer, std::string delim) {
    std::vector<std::string> values;
    std::string line;
    size_t pos = 0;

    while ((pos = buffer.find(delim))!= std::string::npos) {
        if (buffer.compare(0, 4, delim, 0, 4) == 0) {
            values.push_back(delim);
        }
        line = buffer.substr(0, pos);
        values.push_back(line);
        buffer.erase(0, pos + delim.size());
    }
    if (!buffer.empty()) {
        body = buffer;
    }
    for (size_t i = 0; i < values.size() && values[i].compare(delim) != 0; i++) {
        if (i == 0) {
            parseRequestLine(values[i]);
        } else {
            parseHeaders(values[i]);
        }
    }
}

std::string getLocationPath(DataConfig &config, std::vector<Location>::iterator &location, std::string requestRessource) {
    if (location->root.empty() && location->alias.empty()) {
        return (config.getRoot() + requestRessource.substr(1));
    } else if (!location->root.empty()) {
        return (location->root);
    } else {
        return (config.getRoot() + location->alias.substr(1));
    }
}

void Request::buildPath(DataConfig &config) {
    std::vector<Location> locations = config.getLocation();
    std::vector<Location>::iterator it = locations.begin();
    std::string tempRequest = requestRessource.back() != '/' ? requestRessource + "/" : requestRessource;
    size_t pos = 0;
    while (it != locations.end()) {
        pos = tempRequest.find(it->location.substr(0, it->location.size()));
        if (pos != std::string::npos && it->location.compare("/") != 0) {
            location = tempRequest.substr(0, pos + it->location.size());
            break ;
        } else if (pos != std::string::npos && requestRessource.compare("/") == 0) {
            location = "/";
        }
        it++;
    }
    if (it != locations.end()) {
        if (location.size() < requestRessource.size()) {
            path = getLocationPath(config, it, location) + requestRessource.substr(location.size());
        } else {
            path = getLocationPath(config, it, location);
        }
    } else {
        path = config.getRoot() + requestRessource.substr(1);
    }
    // std::cout << "location built is \"" << location << "\"" << std::endl;
    // std::cout << "path built is \"" << path << "\"" << std::endl;
}

int Request::validateUri(DataConfig &config) {
    int hasSlash = 0;
    if (requestRessource.back() != '/') {
        hasSlash = 1;
    }

    buildPath(config);
    // std::cout << "path built is " << path << std::endl;
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) != 0) {
        return (NOT_FOUND);
    } else if (S_ISDIR(statbuf.st_mode) && hasSlash) {
        if (requestMethod.compare("GET") == 0)
            return (PERMANENTLY_MOVED);
        else if (requestMethod.compare("POST") == 0 || requestMethod.compare("DELETE") == 0) {
            return (TEMPORARY_REDIRECT);
        }
    } else {
        if (requestMethod.compare("GET") == 0 && !(statbuf.st_mode & S_IRUSR)) {
            return (FORBIDDEN);
        } else if (requestMethod.compare("POST") == 0 && !(statbuf.st_mode & S_IWUSR)) {
            return (FORBIDDEN);
        }
    }
    return (0);
}

int Request::validRequest(DataConfig config) {
    config.getRoot();
    if (headers.find("Transfer-Encoding") != headers.end()) {
        std::cout << "transfer encoding found\n";
        if (headers["Transfer-Encoding"] != "chunked") {
            return (501);
        }
    }
    if (requestMethod == "POST" && (headers.find("Content-Length") == headers.end()))
        return (BAD_REQUEST);
    if (checkAllowedChars(requestRessource) == 400) {
        std::cout << "request uri : Bad request\n";
        return (BAD_REQUEST);
    }
    if (requestRessource.size() > 2048)
    {
        std::cout << "request uri : exceeded 2048\n";
        return (REQUEST_URI_EXCEEDED);
    }
    // if client request body is larger than maximum body allowed in config file (change 8000 value to config file value
    if (requestEntity.size() > 8000) {
        std::cout << "request entity too large\n";
        return (ENTITY_LENGTH_EXCEEDED);
    }
    if (requestMethod.compare("GET") != 0 && requestMethod.compare("POST") != 0 && requestMethod.compare("DELETE") != 0) {
        return (NOT_IMPLEMENTED);
    }
    return (validateUri(config));
}

//  ******** HANDLER ********

int Request::methodAllowed(DataConfig config) {
    std::vector<Location>::iterator locationData = config.getSpecificLocation(location);
    if (locationData != config.getLocation().end()) {
        if (requestMethod.compare("GET") == 0) {
            if (locationData->methods.get == 0)
                return (0);
        } else if (requestMethod.compare("POST") == 0) {
            if (locationData->methods.post == 0)
                return (0);
        } else if (requestMethod.compare("DELETE") == 0) {
            if (locationData->methods._delete == 0)
                return (0);
        }
    } else if (requestRessource == "/") {
        if (requestMethod.compare("GET") == 0)
            return (0);
    }
    return (1);
}

Response Request::runHttpMethod(DataConfig config) {
    Response response;
    if (requestMethod.compare("GET") == 0)
        response = RequestMethod::GET(*this, config);
    else if (requestMethod.compare("POST") == 0)
        response = RequestMethod::POST(*this, config);
    // else if (requestMethod.compare("DELETE") == 0) {
    //     response = RequestMethod::DELETE(*this, config);
    // }
    return (response);
}

Response Request::handleRequest(DataConfig config) {
    Response response;
    int errorCode = validRequest(config);
    if (errorCode != 0) {
        std::cout << "errorCode "<<errorCode << "\n";
        if (errorCode >= 300 && errorCode <= 308) {
            response.setHeader("Location:", requestRessource + "/");
        }
        response.buildResponse(errorCode);
        return (response);
    }
    // check if there is a redirection
    std::vector<Location>::iterator it = config.getSpecificLocation(location);
    if (it != config.getLocation().end()) {
        if (!it->_return.path.empty() && !it->_return.status.empty()) {
            response.setHeader("Location:", it->_return.path);
            response.buildResponse(atoi(it->_return.status.c_str()));
            return (response);
        }
    }
    // check if the method is allowed on the requested ressource
    if (!methodAllowed(config)) {
        response.buildResponse(METHOD_NOT_ALLOWED);
        return (response);
    }
    response = runHttpMethod(config);
    return response;
}