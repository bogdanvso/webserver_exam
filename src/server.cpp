#include <thread>
#include <functional>
#include <memory>
#include <vector>
#include "server.h"

namespace server
{
    Server::Server(const std::string& address, const std::string& port,
                const std::string& doc_root, size_t thread_pool_size)
                : m_thread_pool_size(thread_pool_size),
                m_signals(m_io_context),
                m_acceptor(m_io_context),
                m_new_connection(),
                m_request_handler(doc_root)
    {
        // Register to handle the signals that indicate when the server should exit.
        // It is safe to register for the same signal multiple times in a program,
        // provided all registration for the specified signal is made through Asio.
        m_signals.add(SIGINT);
        m_signals.add(SIGTERM);
    #if defined(SIGQUIT)
        m_signals.add(SIGQUIT);
    #endif 
        m_signals.async_wait(std::bind(&Server::handle_stop, this));

        // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
        asio::ip::tcp::resolver resolver(m_io_context);
        asio::ip::tcp::endpoint endpoint = *resolver.resolve(address, port).begin();
        m_acceptor.open(endpoint.protocol());
        m_acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
        m_acceptor.bind(endpoint);
        m_acceptor.listen();

        start_accept();
    }

    void Server::run()
    {
        
        std::vector<std::shared_ptr<std::thread>> threads;
        for(size_t i = 0; i < m_thread_pool_size; ++i)
        {
            asio::io_context::count_type (asio::io_context::*fn)(void) = &asio::io_context::run;
            std::shared_ptr<std::thread> thread(new std::thread(
                std::bind(fn , &m_io_context)));
            threads.push_back(thread);
        }

        for(const auto& currTh : threads)
            currTh->join();
    }

    void Server::start_accept()
    {
        //???
        //m_new_connection.reset();
        m_new_connection = Connection::create(m_io_context, m_request_handler);
        m_acceptor.async_accept(m_new_connection->socket(),
        std::bind(&Server::handle_accept, this, std::placeholders::_1));
    }

    void Server::handle_accept(const asio::error_code& e)
    {
        if(!e)
        {
            m_new_connection->start();
        }
        start_accept();
    }

    void Server::handle_stop()
    {
        m_io_context.stop();
    }
}