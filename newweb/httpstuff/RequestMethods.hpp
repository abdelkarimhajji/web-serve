#pragma once

#include "../parse/DataConfig.hpp"
#include "../Server.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Request;

class RequestMethod {
    public:
        static Response GET(Request& request, DataConfig config);
        static Response POST(Request& request, DataConfig config);
        static Response DELETE(Request& request, DataConfig config);
};