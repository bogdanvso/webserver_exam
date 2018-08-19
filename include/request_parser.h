#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include<tuple>
#include "tribool.h"
#include "asio.hpp"

namespace server
{
    struct Request;
    class Request_parser
    {
    private:
        /// The current state of the parser.
        enum state
        {
            method_start,
            method,
            uri,
            http_version_h,
            http_version_t_1,
            http_version_t_2,
            http_version_p,
            http_version_slash,
            http_version_major_start,
            http_version_major,
            http_version_minor_start,
            http_version_minor,
            expecting_newline_1,
            header_line_start,
            header_lws,
            header_name,
            space_before_header_value,
            header_value,
            expecting_newline_2,
            expecting_newline_3
        } m_state;
    public:
        Request_parser() : m_state(method_start) {}
        void reset() { m_state = method_start; }

        /// Parse some data. The tribool return value is true when a complete request
        /// has been parsed, false if the data is invalid, indeterminate when more
        /// data is required. The InputIterator return value indicates how much of the
        /// input has been consumed.
        template <typename InputIterator>
        std::tuple<logic::tribool, InputIterator> parse(Request& req,
            InputIterator begin, InputIterator end)
        {
            while (begin != end)
            {
                logic::tribool result = consume(req, *begin++);
                if (result || !result)
                    return std::make_tuple(result, begin);
            }
            logic::tribool result = logic::tribool::INDETERMINATE;
            return std::make_tuple(result, begin);
        }
    private:
        //handle a next character of input
        logic::tribool consume(Request& req, char input);

         /// Check if a byte is an HTTP character.
        static bool is_char(int c);
        /// Check if a byte is an HTTP control character.
        static bool is_ctl(int c);
        /// Check if a byte is defined as an HTTP tspecial character.
        static bool is_tspecial(int c);
        /// Check if a byte is a digit.
        static bool is_digit(int c);
    };
}

#endif // !REQUEST_PARSER_H
