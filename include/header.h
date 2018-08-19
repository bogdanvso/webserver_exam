#ifndef HEADER_H
#define HEADER_H

#include<string>

namespace server
{
    struct Header
    {
        std::string name;
        std::string value;
    };
}

#endif