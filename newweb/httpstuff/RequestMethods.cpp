#include "RequestMethods.hpp"
#include "Response.hpp"
#include "../cgi/Cgi.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <dirent.h>

std::string generateHTML(const char* path) {
    std::ostringstream ss;
    DIR *dir = opendir(path);
    if (!dir) {
        return "";
    }

    ss << "<html><head><title>Directory Listing</title><style>h1 {text-align:center;}</style></head><body><h1>Directory Listing</h1><ul><br>";

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {
            ss << "<li><a href=\"" << entry->d_name << "\">" << entry->d_name << "</a></li>";
        }
    }

    ss << "</ul></body></html>";
    closedir(dir);
    return ss.str();
}

void fillResponse(Response &response, std::ostringstream& ss, std::string filetype) {
    response.setContentType(filetype);
    response.setContentLength(ss.str().size());
    response.setResponseBody(ss.str());
    response.buildResponse(OK);
}

void handleFolder(Response &response, std::vector<Location>::iterator &it, DataConfig &config, std::string path) {
    std::ostringstream ss;
    if (it != config.getLocation().end()) {
        std::ifstream file(path + it->index);
        if (!file.is_open()) {
            if (it->autoIndex) {
                ss << generateHTML(path.c_str());
                fillResponse(response, ss, ".html");
            } else {
                response.buildResponse(FORBIDDEN);
            }
        } else {
            ss << file.rdbuf();
            fillResponse(response, ss, it->index);
        }
    } else if (it == config.getLocation().end()) {
        std::ifstream file(path + config.getIndex());
        if (!file.is_open()) {
            if (config.getAutoIndex()) {
                ss << generateHTML(path.c_str());
                fillResponse(response, ss, ".html");
            } else {
                response.buildResponse(FORBIDDEN);
            }
        } else {
            ss << file.rdbuf();
            fillResponse(response, ss, config.getIndex());
        }
    }
}

void handleFile(Response &response, std::string path) {
    std::ostringstream ss;
    std::ifstream file(path);
    if (!file.is_open()) {
        response.buildResponse(NOT_FOUND);
    } else {
        ss << file.rdbuf();
        fillResponse(response, ss, path);
    }
}

Response buildResponseWithFile(DataConfig config, std::string path, std::string location) {
    std::ostringstream ss;
    Response response;
   
    if (path.back() == '/') {
        std::vector<Location>::iterator locationData = config.getSpecificLocation(location);
        handleFolder(response, locationData, config, path);
    } else {
        handleFile(response, path);
    } 
    return (response);
}


Response RequestMethod::GET(Request& request, DataConfig config) {
    std::string requestedRessource = request.getRequestRessource();
    Response response;
    if (requestedRessource.compare("/") == 0) {
        // request is empty, send index of root
        response = buildResponseWithFile(config, request.getPath() + config.getIndex(), request.getLocation());
    } else if (requestedRessource[requestedRessource.size() - 1] == '/') {
        // if request wants a directory
        std::cout << "entered directory test\n";
        response = buildResponseWithFile(config, request.getPath(), request.getLocation());
    } else {
        // specific ressource is requested instead of default
        response = buildResponseWithFile(config, request.getPath(), request.getLocation());
    }
    return (response);
}

std::string getLastPart(const std::string& path) {
    size_t pos = path.find_last_of("/");
    if (pos != std::string::npos) {
        return path.substr(pos + 1);
    }
    return "";
}

std::string listFils(const std::string& path, Request request)
{
    std::string html = "<html><body><ul>";
    DIR* dirp = opendir(path.c_str());
    if (dirp) {
        struct dirent * dp;
        while ((dp = readdir(dirp)) != NULL) {
            std::string filename(dp->d_name);
            html += "<li><a href=\"" + request.getRequestRessource() + filename + "\">" + filename + "</a></li>";
        }
        closedir(dirp);
    }
    html += "</ul></body></html>";
    std::cout << path << "\n\n\n\n";
    return html;
}

void returnDefaultContentFile(Request& request, DataConfig config, std::string path, Response& response)
{
    std::string line;
    std::string nameFile;
    if((config.getSpecificLocation(request.getLocation())->autoIndex == 1 || config.getAutoIndex() == 1))
    {
        response.setStatus(200);
        response.setHttpVersion(request.getHttpVersion());
        response.setContentType("index.html");
        response.setContentLength(listFils(path, request).size());
        response.setResponseBody(listFils(path, request));
        response.buildResponse(200);
        return ;
    }
    if(config.getSpecificLocation(request.getLocation())->index.empty() == 0)
        nameFile = config.getSpecificLocation(request.getLocation())->index;
    else if(config.getIndex().empty() == 0)
        nameFile = config.getIndex();
    path += nameFile;

    std::string extension = "";
    std::size_t pos = nameFile.find_last_of(".");
  
    if (pos != std::string::npos)
        extension = nameFile.substr(pos);
    std::ifstream file;
    file.open(path.c_str(), std::ios::binary);
    if (!file || !file.is_open()) {
        response.buildResponse(404);
        std::cerr << "Unable to open file\n";
        return;
    }
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    std::string fileContent(buffer.begin(), buffer.end());
    

    if (extension == ".php")
    {
        
        std::cout << Cgi::CallCgi(path, request) << std::endl;
        // exit (0);
    }
    else
    {
        response.setStatus(200);
        response.setHttpVersion(request.getHttpVersion());
        response.setContentType(getLastPart(path));
        response.setContentLength(buffer.size());
        response.setResponseBody(fileContent);
        response.buildResponse(200);
    }
}

void returnSpecificContentFile(std::string path, Response& response, Request& request)
{
    std::ifstream file;
    file.open(path.c_str(), std::ios::binary);

    if (!file || !file.is_open()) {
        response.buildResponse(404);
        std::cerr << "Unable to open file\n";
        return;
    }
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    response.setStatus(200);
    response.setHttpVersion(request.getHttpVersion());
    response.setContentType(getLastPart(path));
    response.setContentLength(buffer.size());
    std::string fileContent(buffer.begin(), buffer.end());
    response.setResponseBody(fileContent);
    response.buildResponse(200);
}
Response RequestMethod::POST(Request& request, DataConfig config)
{   
    std::string path = request.getPath();
    Response response;
    std::string content;
    std::string fileContent;
    if (path[path.size() - 1] == '/')
        returnDefaultContentFile(request, config, path, response);
    else
        returnSpecificContentFile(path, response, request);
    return response;
}