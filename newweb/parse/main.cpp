/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdenguir <mdenguir@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 15:10:08 by ahajji            #+#    #+#             */
/*   Updated: 2024/02/22 16:30:23 by mdenguir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseConfigeFile.hpp"



int main(void)
{
    ParseConfigeFile parse;

    parse.parser("configfile.txt");

    
    return 0;
}