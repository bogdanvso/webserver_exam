#ifndef CONNECTION_H
#define CONNECTION_H

#include <array>
#include <memory>
#include "request.h"
#include "request_handler.h"
#include "request_parser.h"
#include "reply.h"
#include "asio.hpp"

namespace server
{
    class Connection;
    using connection_ptr_t = std::shared_ptr<Connection>;
    class Connection : public std::enable_shared_from_this<Connection>
    {
        // Strand to ensure the connection's handlers are not called concurrently.
        asio::io_context::strand m_strand;
        //Socket for conn
        asio::ip::tcp::socket m_socket;
        //The handler used to process the incoming request
        Request_handler& m_request_handler;
        //Buffer for incoming data
        std::array<char, 8192> m_buffer;
        //The incoming request
        Request m_request;
        //The parser for the incoming request
        Request_parser m_request_parser;
        //The reply to be sent back to the client
        Reply m_reply;

    public:
        asio::ip::tcp::socket& socket() { return m_socket; }
        void start();

        static connection_ptr_t create(asio::io_context& io_context, Request_handler& handler)
            { return connection_ptr_t(new Connection(io_context, handler)); }
        Connection(const Connection&) = delete;
        Connection& operator=(const Connection&) = delete;
        Connection(Connection&&) = delete;
        Connection& operator=(Connection&&) = delete;

    private:
        Connection(asio::io_context& io_context, Request_handler& handler)
            : m_strand(io_context), m_socket(io_context), m_request_handler(handler) {}

        void handle_read(const asio::error_code& e, size_t bytes_transferred);
        void handle_write(const asio::error_code& e);
    };

    
}

#endif // !CONNECTION_H