#ifndef REQUEST_H
#define REQUEST_H

#include<string>
#include<vector>
#include"header.h"

namespace server
{
    struct Request
    {
        std::string method;
        std::string uri;
        int http_version_major;
        int http_version_minor;
        std::vector<Header> headers;
    };
}

#endif