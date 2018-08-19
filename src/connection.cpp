#include <vector>
#include <functional>
#include "request_handler.h"
#include "connection.h"


namespace server
{
    void Connection::start()
    {
        m_socket.async_read_some(asio::buffer(m_buffer), 
            asio::bind_executor(m_strand,
                std::bind(&Connection::handle_read, shared_from_this(),
                std::placeholders::_1,
                std::placeholders::_2)));
       
    }

    void Connection::handle_read(const asio::error_code&e, size_t bytes_transferred)
    {
        if(!e)
        {
            logic::tribool result = std::get<0>(m_request_parser.parse(
                m_request, m_buffer.data(), m_buffer.data() + bytes_transferred
            ));

            if(result)
            {
                m_request_handler.handle_request(m_request, m_reply);
                asio::async_write(m_socket, m_reply.to_buffers(),
                    asio::bind_executor(m_strand,
                        std::bind(&Connection::handle_write, shared_from_this(),
                            std::placeholders::_1)));
            }
            else if(!result)
            {
                //bad request not supported by task, then send not found
                m_reply = Reply::stock_reply(Reply::STATUS_TYPE::NOT_FOUND);
                asio::async_write(m_socket, m_reply.to_buffers(),
                    asio::bind_executor(m_strand,
                        std::bind(&Connection::handle_write, shared_from_this(),
                            std::placeholders::_1)));
            }
            else
            {
                m_socket.async_read_some(asio::buffer(m_buffer), 
                    asio::bind_executor(m_strand,
                        std::bind(&Connection::handle_read, shared_from_this(),
                        std::placeholders::_1,
                        std::placeholders::_2)));    
            }
            // If an error occurs then no new asynchronous operations are started. This
            // means that all shared_ptr references to the connection object will
            // disappear and the object will be destroyed automatically after this
            // handler returns. The connection class's destructor closes the socket.
        }
    }
    void Connection::handle_write(const asio::error_code& e)
    {
        if(!e)
        {
            //Initiate graceful connection closure
            asio::error_code ignored_err;
            m_socket.shutdown(asio::ip::tcp::socket::shutdown_both, ignored_err);
        }
        // No new asynchronous operations are started. This means that all shared_ptr
        // references to the connection object will disappear and the object will be
        // destroyed automatically after this handler returns. The connection class's
        // destructor closes the socket.    
    }
}
