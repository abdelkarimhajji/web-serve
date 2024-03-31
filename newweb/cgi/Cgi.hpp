/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahajji <ahajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:39:36 by ahajji            #+#    #+#             */
/*   Updated: 2024/03/30 23:02:30 by ahajji           ###   ########.fr       */
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


class Request;
class Response;

class Cgi
{
    private:
        static std::string getLastPart(const std::string& path);
    public:
        static std::string    CallCgi(std::string script, Request& request);
};

#endif