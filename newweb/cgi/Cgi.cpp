/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahajji <ahajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:48:53 by ahajji            #+#    #+#             */
/*   Updated: 2024/03/30 02:00:08 by ahajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

std::string Cgi::CallCgi(std::string path, Request& request)
{
    std::string outputFile = "/Users/ahajji/Desktop/newweb/tmp/file.txt";  
    std::string inputFile;
    // int fd_in = -1;
    // if (request.getRequestMethod() == "POST") {
    //     inputFile = "/Users/ahajji/Desktop/newweb/tmp/input.txt";
    //     int fd = open(inputFile.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    //     if (fd == -1)
    //     {
    //         perror("open");
    //         exit(EXIT_FAILURE);
    //     }
    //     write(fd, request.getBody().c_str(), request.getBody().size());
    //     close(fd);
    // }
    
    std::map<std::string, std::string> headers;
    headers["SERVER_PROTOCOL"] = request.getHttpVersion();
    headers["CONTENT_TYPE"] = "application/x-www-form-urlencoded";
    headers["CONTENT_LENGTH"] = "23";
    headers["Location"] = "http://www.example.com";
    headers["Status"] = "200 OK";
    headers["REDIRECT_STATUS"] = "200";
    headers["REQUEST_METHOD"] ="POST";
    headers["DOCUMENT_ROOT"] = "/Users/ahajji/Desktop/newweb/image/all";
    headers["SCRIPT_FILENAME"] = "/Users/ahajji/Desktop/newweb/image/all/index.php";
// Calculate the total number of headers plus one for the QUERY_STRING
int numHeaders = headers.size() + 1;

// Allocate memory for the array of pointers, plus one for the null pointer
char** arr = new char*[numHeaders * 2 + 1];

int i = 0;
for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
    // Combine the key and value into a single string in the format key=value
    std::string keyValue = it->first + "=" + it->second;

    // Allocate memory for the string and copy it
    arr[i] = new char[keyValue.size() + 1];
    std::strcpy(arr[i], keyValue.c_str());
    ++i;
}

// Add the null pointer to the end of the array
arr[i] = NULL;

// Add the QUERY_STRING
std::string query;
if(request.getBody() == "GET")
{
    query = "name=John&email=Doe";  
    std::string queryString = "QUERY_STRING=" + query;
    arr[i] = new char[queryString.size() + 1];
    std::strcpy(arr[i], queryString.c_str());
    ++i;
}

// Set the last element to null
arr[i] = NULL;
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    // char** envpArray = envp.data();
int out;
// int in;
out = dup(STDOUT_FILENO);
// in = dup(STDIN_FILENO);
    if (pid == 0) {
if (request.getRequestMethod() == "POST") {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Write the POST data to the write end of the pipe
    std::string postdata = "key1=hajji&key2=karim"; // replace this with your actual POST data
    write(pipefd[1], postdata.c_str(), postdata.size());

    // Close the write end of the pipe
    close(pipefd[1]);

    // Replace the standard input with the read end of the pipe
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]); // close the read end of the pipe, it's not needed anymore
}
        int fd = open(outputFile.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);  
        const char* argv[] = {"php-cgi" ,path.c_str(), (request.getRequestMethod() == "GET") ? query.c_str() : NULL, NULL};        
        execve("php-cgi", (char* const*)argv, arr);
        
       
        perror("execve");
        exit(EXIT_FAILURE);
    } else {
   
        waitpid(pid, NULL, 0);
        dup2(STDOUT_FILENO, out);
        // dup2(STDIN_FILENO,in);
        std::ifstream file(outputFile.c_str(), std::ios::binary);
        std::string result((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

      
        remove(outputFile.c_str());
        // remove(inputFile.c_str());
        return result;
    }
}