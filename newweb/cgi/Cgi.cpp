/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahajji <ahajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:48:53 by ahajji            #+#    #+#             */
/*   Updated: 2024/03/31 02:51:10 by ahajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

std::string Cgi::getLastPart(const std::string& path) {
    size_t pos = path.find_last_of("/");
    if (pos != std::string::npos) {
        return path.substr(pos + 1);
    }
    return "";
}

std::string Cgi::CallCgi(std::string path, Request& request)
{
    std::string outputFile = "/Users/ahajji/Desktop/newweb/tmp/file.txt";  
    std::string inputFile;
    
    std::string SERVER_PROTOCOL = "SERVER_PROTOCOL=" + request.getHttpVersion();
    std::string CONTENT_TYPE = "CONTENT_TYPE=application/x-www-form-urlencoded";
    std::stringstream leng;
    leng << request.getBody().length();
    std::string CONTENT_LENGTH = "CONTENT_LENGTH=" + leng.str();
    std::string Location = "Location=http://www.example.com";
    std::string Status = "Status=200 OK";
    std::string REDIRECT_STATUS = "REDIRECT_STATUS=200";
    std::string REQUEST_METHOD = "REQUEST_METHOD="+request.getRequestMethod();
    std::string DOCUMENT_ROOT = "DOCUMENT_ROOT=/Users/ahajji/Desktop/newweb/image/all";
    std::string SCRIPT_FILENAME = "SCRIPT_FILENAME=/Users/ahajji/Desktop/newweb/image/all/index.php";
    
    std::string headers[] = {
    SERVER_PROTOCOL,
    CONTENT_TYPE,
    CONTENT_LENGTH,
    Location,
    Status,
    REDIRECT_STATUS,
    REQUEST_METHOD,
    DOCUMENT_ROOT,
    SCRIPT_FILENAME
};

int numHeaders = 10;
char** arr = new char*[numHeaders * 2 + 1];
size_t i = 0;
for (i = 0; i < sizeof(headers)/sizeof(headers[0]); ++i) {
    arr[i] = new char[headers[i].size() + 1];
    std::strcpy(arr[i], headers[i].c_str());
}

std::string query;
if(request.getBody() == "GET")
{  
    query = "name=John&email=Doe";
    std::string queryString = "QUERY_STRING=" + query;
    arr[i] = new char[queryString.size() + 1];
    std::strcpy(arr[i], queryString.c_str());
    ++i;
}

arr[i] = NULL;
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

int out;

out = dup(STDOUT_FILENO);

    if (pid == 0) {
if (request.getRequestMethod() == "POST") {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Write the POST data to the write end of the pipe
    std::string postdata = request.getBody(); // replace this with your actual POST data
    write(pipefd[1], postdata.c_str(), postdata.size());

    // Close the write end of th e pipe
    close(pipefd[1]);

    // Replace the standard input with the read end of the pipe
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]); // close the read end of the pipe, it's not needed anymore
    }
        int fd = open(outputFile.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        if (fd == -1)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);  
        const char* argv[] = {"php-cgi" ,path.c_str(), NULL};        
        execve("php-cgi", (char* const*)argv, arr);
        
       
        perror("execve");
        exit(EXIT_FAILURE);
    } else {
   
        waitpid(pid, NULL, 0);
        dup2(STDOUT_FILENO, out);
        // dup2(STDIN_FILENO,in);
        std::ifstream file(outputFile.c_str(), std::ios::binary);
        std::string result((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        std::string body;
        remove(outputFile.c_str());
        std::cout << result << std::endl;
        remove(inputFile.c_str());
        std::map<std::string, std::string> header_s;
        size_t separator = result.find("\r\n\r\n");
        std::cout << separator << "\n";
        if (separator != std::string::npos) {
            body = result.substr(separator + 4);
            std::string headers = result.substr(0, separator);
            size_t pos = 0;
            while((pos =  headers.find('\n')) != std::string::npos)
            {
                std::string line = headers.substr(0, pos);
                size_t doublePoint = line.find(": ");
                if(doublePoint != std::string::npos)
                {
                    std::string key = line.substr(0, doublePoint);
                    std::string value = line.substr(doublePoint + 2);
                    header_s[key] = value;
                }
                headers.erase(0, pos + 1);
            }
        }
        else
        {
            std::cout << "ana d5olt\n";  
            body = result;
        }
        std::cout << "hahahahaahha    " <<body << "\n";
        return body;
    }
}