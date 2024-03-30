/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfigFile.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nachab <nachab@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 20:41:14 by ahajji            #+#    #+#             */
/*   Updated: 2024/02/23 20:39:50 by nachab           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE
#define PARSE

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
// #include <cstring>
// #include <cstdlib>
#include "DataConfig.hpp"

class ParseConfigFile
{
    private:
        int findBraciteRight;
        int findBraciteLeft;
        int findBraciteRightLocation;
        int findBraciteLeftLocation;
        std::vector<DataConfig> data;
        std::vector<std::string>     split(std::string str);
        void    checkValidServer(std::vector<std::string> splitVector);
        void    checkValidListen(std::vector<std::string> splitVector);
        void    checkValidServerName(std::vector<std::string> splitVector);
        void    checkValidRoot(std::vector<std::string> splitVector);
        void    checkValidIndex(std::vector<std::string> splitVector);
        void    checkValidErrorPage(std::vector<std::string> splitVector);
        void    checkValidLocation(std::vector<std::string> splitVector);
        void    checkValidLocationRoot(std::vector<std::string> splitVector);
        void    checkValidLocationAlias(std::vector<std::string> splitVector);
        void    checkValidLocationIndex(std::vector<std::string> splitVector);
        void    checkValidLocationMethods(std::vector<std::string> splitVector);
        void    checkValidLocationAutoIndex(std::vector<std::string> splitVector);
        void    checkValidLocationCgiExtention(std::vector<std::string> splitVector);
        void    checkValidLocationCgiBin(std::vector<std::string> splitVector);
        void    checkValidLocationReturn(std::vector<std::string> splitVector);
        void    checkValidLocationLeftBrackite(std::vector<std::string> splitVector);
        void    checkValidServerLeftBrackite(std::vector<std::string> splitVector);
        void    checkValidHost(std::vector<std::string> splitVector);
        void    checkValidAutoIndex(std::vector<std::string> splitVector);
        void    errorParse();
    public:
        ParseConfigFile();
        void    parser(std::string nameFile);
        std::vector<DataConfig> getData();
};

#endif