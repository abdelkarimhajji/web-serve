/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahajji <ahajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:39:36 by ahajji            #+#    #+#             */
/*   Updated: 2024/04/01 01:19:48 by ahajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI
#define CGI
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <map>
#include "../httpstuff/Request.hpp"
#include "../httpstuff/Response.hpp"
#include "CgiOutput.hpp"


class Request;
class Response;
class CgiOutput;

class Cgi
{
    private:
        static std::string getLastPart(const std::string& path);
        static std::string getPath(const std::string& path);
    public:
        static  CgiOutput    CallCgi(std::string script, Request& request, std::string check);
};

#endif