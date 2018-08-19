#include <string>
#include "reply.h"

namespace server
{
    namespace STATUS_STRS
    {
        const std::string OK = "HTTP/1.0 200 OK\r\n";
        const std::string NOT_FOUND = "HTTP/1.0 404 Not Found\r\n";
        const std::string BAD_REQUEST = "HTTP/1.0 400 Bad Request\r\n";
        const std::string INTERNAL_SERVER_ERROR = "HTTP/1.0 500 Internal Server Error\r\n";
        
        asio::const_buffer to_buffer(Reply::STATUS_TYPE status)
        {
            switch(status)
            {
            case Reply::STATUS_TYPE::OK:
                return asio::buffer(OK);
            case Reply::STATUS_TYPE::NOT_FOUND:
                return asio::buffer(NOT_FOUND);
            case Reply::STATUS_TYPE::BAD_REQUEST:
                return asio::buffer(BAD_REQUEST);
            default://other msgs not supported by task
                return asio::buffer(INTERNAL_SERVER_ERROR); 
            }
        }
    }

    namespace MISK_STRINGS
    {
        const char NAME_VALUE_SEPARATOR[] = { ':', ' ' };
        const char CRLF[] = { '\r', '\n' };    
    }

    std::vector<asio::const_buffer> Reply::to_buffers()
    {
        std::vector<asio::const_buffer> buffers;
        buffers.reserve( headers.size() * 4 + 3 );
        buffers.push_back(STATUS_STRS::to_buffer(status));
        for(const auto& currH : headers)
        {
            buffers.push_back(asio::buffer(currH.name));
            buffers.push_back(asio::buffer(MISK_STRINGS::NAME_VALUE_SEPARATOR));
            buffers.push_back(asio::buffer(currH.value));
            buffers.push_back(asio::buffer(MISK_STRINGS::CRLF));
        }
        buffers.push_back(asio::buffer(MISK_STRINGS::CRLF));
        buffers.push_back(asio::buffer(content));
        return buffers;
    }

    namespace STOCK_REPLIES
    {
        const char OK[] = "";
        const char NOT_FOUND[] = 
        "<html>"
        "<head><title>Not Found</title></head>"
        "<body><h1>404 Not Found</h1></body>"
        "</html>";
        const char BAD_REQUEST[] =
        "<html>"
        "<head><title>Bad Request</title></head>"
        "<body><h1>400 Bad Request</h1></body>"
        "</html>";
        const char INTERNAL_SERVER_ERROR[] =
        "<html>"
        "<head><title>Internal Server Error</title></head>"
        "<body><h1>500 Internal Server Error</h1></body>"
        "</html>";
        std::string to_string(Reply::STATUS_TYPE status)
        {
            switch(status)
            {
            case Reply::STATUS_TYPE::OK:
                return OK;
            case Reply::STATUS_TYPE::NOT_FOUND:
                return NOT_FOUND;
            case Reply::STATUS_TYPE::BAD_REQUEST:
                return BAD_REQUEST;
            default:
                return INTERNAL_SERVER_ERROR;
            }
        }
    }

    Reply Reply::stock_reply(Reply::STATUS_TYPE status)
    {
        Reply rep;
        rep.status = status;
        rep.content = STOCK_REPLIES::to_string(status);
        rep.headers.resize(2);
        rep.headers[0].name = "Content-Lenght";
        rep.headers[0].value = std::to_string(rep.content.size());
        rep.headers[1].name = "Content-Type";
        rep.headers[1].value = "text/html";
        return rep;
    }
}