/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DataConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahajji <ahajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 12:11:43 by ahajji            #+#    #+#             */
/*   Updated: 2024/02/19 19:32:21 by ahajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DataConfig.hpp"

DataConfig::DataConfig()
{
    this->autoIndex = 0;
}

void    DataConfig::errorData()
{
    std::cout << "error in parse data" << std::endl;
}

void    DataConfig::setListen(std::string listen)
{
    this->listen.push_back(listen);
}
std::vector<std::string>    DataConfig::getListen()
{
    return this->listen;
}

std::vector<std::string>    DataConfig::getServerName()
{
    return this->serverName;
}
void    DataConfig::setServerName(std::string serverName)
{
    this->serverName.push_back(serverName);
}

std::string    DataConfig::getRoot()
{
    return this->root;
}

void    DataConfig::setRoot(std::string root)
{
    this->root = root;
}

std::string    DataConfig::getIndex()
{
    return this->index;
}

void    DataConfig::setIndex(std::string index)
{
    this->index = index;
}

std::vector<ErrorPage> DataConfig::getErrorPage()
{
    return this->errorPage;
}

void    DataConfig::setErrorPage(std::string error, std::string page)
{
    ErrorPage newErrorPage;
    newErrorPage.error = error;
    newErrorPage.page = page;
    this->errorPage.push_back(newErrorPage);
}

std::vector<Location>&    DataConfig::getLocation()
{
    return this->vectorLocation;
}

std::vector<Location>::iterator DataConfig::getSpecificLocation(std::string location) {
    for (std::vector<Location>::iterator it = vectorLocation.begin(); it != vectorLocation.end(); it++) {
        if (it->location.compare(location) == 0) {
            return (it);
        }
    }
    return (vectorLocation.end());
}

void    DataConfig::setLocation(std::string _location)
{
    Location location;

    location.location = _location;
    this->vectorLocation.push_back(location);
}
void    DataConfig::setLocationRoot(std::string root)
{
    this->vectorLocation.back().root = root;
}

void    DataConfig::setLocationAlias(std::string alias)
{
    this->vectorLocation.back().alias = alias;
}

void    DataConfig::setLocationIndex(std::string index)
{
    this->vectorLocation.back().index = index;
}

std::string    DataConfig::getHost()
{
    return this->host;
}
void    DataConfig::setAutoIndex(int autoIndex)
{
    this->autoIndex = autoIndex;
}
int    DataConfig::getAutoIndex()
{
    return this->autoIndex;
}
void    DataConfig::setHost(std::string host)
{
    this->host = host;
}
void    DataConfig::setLocationMethods(std::string method1, std::string method2, std::string method3)
{
    if(!method1.empty() && method1 == "GET")
        this->vectorLocation.back().methods.get = 1;
    else if(!method1.empty() && method1 == "POST")
        this->vectorLocation.back().methods.post = 1;
    else if(!method1.empty() && method1 == "DELETE")
        this->vectorLocation.back().methods._delete = 1;
    else if(!method1.empty())
        errorData();
    if(!method2.empty() && method2 == "GET")
        this->vectorLocation.back().methods.get = 1;
    else if(!method2.empty() && method2 == "POST")
        this->vectorLocation.back().methods.post = 1;
    else if(!method2.empty() && method2 == "DELETE")
        this->vectorLocation.back().methods._delete = 1;
    else if(!method2.empty())
        errorData();
    if(!method3.empty() && method3 == "GET")
        this->vectorLocation.back().methods.get = 1;
    else if(!method3.empty() && method3 == "POST")
        this->vectorLocation.back().methods.post = 1;
    else if(!method3.empty() && method3 == "DELETE")
        this->vectorLocation.back().methods._delete = 1;
    else if(!method3.empty())
        errorData();
}

void    DataConfig::setLocationAutoIndex(std::string autoIndex)
{
    if(autoIndex == "off")
        this->vectorLocation.back().autoIndex = 0;
    else if(autoIndex == "on")
        this->vectorLocation.back().autoIndex = 1;
    else
        errorData();
}

void    DataConfig::setLocationCgiExtention(std::string cgiExtention)
{
    this->vectorLocation.back().cgiExtension = cgiExtention;
}

void    DataConfig::setLocationCgiBin(std::string cgiBin)
{
    this->vectorLocation.back().cgiBin = cgiBin;
}

void    DataConfig::setLocationReturn(std::string returnStatus, std::string returnPath)
{
    this->vectorLocation.back()._return.status = returnStatus;
    this->vectorLocation.back()._return.path = returnPath;
}
