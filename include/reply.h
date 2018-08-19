#ifndef REPLY_H
#define REPLY_H

#include <string>
#include <vector>
#include <asio.hpp>
#include "header.h"

namespace server
{
    struct Reply
    {
        enum class STATUS_TYPE
        {
            OK = 200,
            BAD_REQUEST = 400,
            NOT_FOUND = 404,
            INTERNAL_SERVER_ERROR = 500
        } status;

        //to be included in the reply
        std::vector<Header> headers;
        //to be sent in the reply
        std::string content;
        //the reply object must remain valid and
        //not be changed until the write operation has completed!
        std::vector<asio::const_buffer> to_buffers();
        //get a stock reply
        static Reply stock_reply(Reply::STATUS_TYPE);
    };
}

#endif