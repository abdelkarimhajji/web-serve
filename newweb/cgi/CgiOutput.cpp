/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiOutput.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahajji <ahajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 22:36:04 by ahajji            #+#    #+#             */
/*   Updated: 2024/03/31 23:03:26 by ahajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiOutput.hpp"

CgiOutput::CgiOutput()
{
    this->body = "";
    this->location = "";   
}

std::string CgiOutput::getBody()
{
    return this->body;
}

std::string CgiOutput::getLocation()
{
    return this->location;
}
void CgiOutput::setBody(std::string body)
{
    this->body = body;
}

void CgiOutput::setLocation(std::string location)
{
    this->location = location;
}