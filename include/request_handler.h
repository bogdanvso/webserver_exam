#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <string>
#include <sstream>

namespace server
{
    struct Reply;
    struct Request;

    //handler for all incoming requests
    class Request_handler
    {
    public:
        Request_handler(const Request_handler&) = delete;
        Request_handler& operator=(const Request_handler&) = delete;
        Request_handler(Request_handler&&) = delete;
        Request_handler& operator=(Request_handler&&) = delete;

        explicit Request_handler(const std::string& doc_root)
            : m_doc_root(doc_root) {}

        void handle_request(const Request& req, Reply& rep);

    private:
        std::string m_doc_root;

        static bool url_decode(const std::string& in, std::string& out)
        {
            out.clear();
            out.reserve(in.size());

            for(size_t i = 0; i < in.size(); ++i)
            {
                if(in[i] == '%')
                {
                    if(i + 3 <= in.size())
                    {
                        int value = 0;
                        std::istringstream is(in.substr(i + 1, 2));
                        if(is >> std::hex >> value)
                        {
                            out += static_cast<char>(value);
                            i += 2;
                        }
                        else
                            return false;    
                    }
                    else
                        return false;
                }
                else if(in[i] == '+')
                    out += ' ';
                else
                    out += in[i];
            }
            return true;
        }
    };
}

#endif // !REQUEST_HANDLER_H
