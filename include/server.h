#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <vector>
#include <memory>
#include "connection.h"
#include "request_handler.h"
#include <asio.hpp>

namespace server
{
    class Server
    {
    public:
        Server(const Server&) = delete;
        Server& operator=(const Server&) = delete;
        Server(Server&&) = delete;
        Server& operator=(Server&&) = delete;

        explicit Server(const std::string& address, const std::string& port,
            const std::string& doc_root, size_t thread_pool_size);
        void run();

    private:
        //Init async accept operation
        void start_accept();
        //Handle complection of an async accept operation
        void handle_accept(const asio::error_code& e);
        //Handle a request to stop the server
        void handle_stop();
        
        //Num of threads that will call context::run()
        size_t m_thread_pool_size;
        //Context uded to perform async operations
        asio::io_context m_io_context;
        //The signal_set is used to register process termination notification
        asio::signal_set m_signals;
        //Acceptor used to listen for incoming connections
        asio::ip::tcp::acceptor m_acceptor;
        //The next connection to be accepted
        connection_ptr_t m_new_connection;
        //The handler for all incoming requests
        Request_handler m_request_handler;
        
    };
}
#endif