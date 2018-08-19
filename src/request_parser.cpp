#include "request_parser.h"
#include "request.h"

namespace server
{
    logic::tribool Request_parser::consume(Request& req, char input)
    {
    switch (m_state)
    {
    case method_start:
        if (!is_char(input) || is_ctl(input) || is_tspecial(input))
            return false;
        else
        {
            m_state = method;
            req.method.push_back(input);
            return logic::tribool::INDETERMINATE;
        }
    case method:
        if (input == ' ')
        {
            m_state = uri;
            return logic::tribool::INDETERMINATE;
        }
        else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
            return false;
        else
        {
            req.method.push_back(input);
            return logic::tribool::INDETERMINATE;
        }
    case uri:
        if (input == ' ')
        {
            m_state = http_version_h;
            return logic::tribool::INDETERMINATE;
        }
        else if (is_ctl(input))
            return false;
        else
        {
            req.uri.push_back(input);
            return logic::tribool::INDETERMINATE;
        }
    case http_version_h:
        if (input == 'H')
        {
            m_state = http_version_t_1;
            return logic::tribool::INDETERMINATE;
        }
        else
            return false;
    case http_version_t_1:
        if (input == 'T')
        {
            m_state = http_version_t_2;
            return logic::tribool::INDETERMINATE;
        }
        else
            return false;
    case http_version_t_2:
        if (input == 'T')
        {
            m_state = http_version_p;
            return logic::tribool::INDETERMINATE;
        }
        else
            return false;
    case http_version_p:
        if (input == 'P')
        {
            m_state = http_version_slash;
            return logic::tribool::INDETERMINATE;
        }
        else
            return false;
    case http_version_slash:
        if (input == '/')
        {
            req.http_version_major = 0;
            req.http_version_minor = 0;
            m_state = http_version_major_start;
            return logic::tribool::INDETERMINATE;
        }
        else
            return false;
    case http_version_major_start:
        if (is_digit(input))
        {
            req.http_version_major = req.http_version_major * 10 + input - '0';
            m_state = http_version_major;
            return logic::tribool::INDETERMINATE;
        }
        else
            return false;
    case http_version_major:
        if (input == '.')
        {
            m_state = http_version_minor_start;
            return logic::tribool::INDETERMINATE;
        }
        else if (is_digit(input))
        {
            req.http_version_major = req.http_version_major * 10 + input - '0';
            return logic::tribool::INDETERMINATE;
        }
        else
            return false;
    case http_version_minor_start:
        if (is_digit(input))
        {
            req.http_version_minor = req.http_version_minor * 10 + input - '0';
            m_state = http_version_minor;
            return logic::tribool::INDETERMINATE;
        }
        else
            return false;
    case http_version_minor:
        if (input == '\r')
        {
            m_state = expecting_newline_1;
            return logic::tribool::INDETERMINATE;
        }
        else if (is_digit(input))
        {
            req.http_version_minor = req.http_version_minor * 10 + input - '0';
            return logic::tribool::INDETERMINATE;
        }
        else
            return false;
    case expecting_newline_1:
        if (input == '\n')
        {
            m_state = header_line_start;
            return logic::tribool::INDETERMINATE;
        }
        else
            return false;
    case header_line_start:
        if (input == '\r')
        {
            m_state = expecting_newline_3;
            return logic::tribool::INDETERMINATE;
        }
        else if (!req.headers.empty() && (input == ' ' || input == '\t'))
        {
            m_state = header_lws;
            return logic::tribool::INDETERMINATE;
        }
        else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
            return false;
        else
        {
            req.headers.push_back(Header());
            req.headers.back().name.push_back(input);
            m_state = header_name;
            return logic::tribool::INDETERMINATE;
        }
    case header_lws:
        if (input == '\r')
        {
            m_state = expecting_newline_2;
            return logic::tribool::INDETERMINATE;
        }
        else if (input == ' ' || input == '\t')
            return logic::tribool::INDETERMINATE;
        else if (is_ctl(input))
            return false;
        else
        {
            m_state = header_value;
            req.headers.back().value.push_back(input);
            return logic::tribool::INDETERMINATE;
        }
    case header_name:
        if (input == ':')
        {
            m_state = space_before_header_value;
            return logic::tribool::INDETERMINATE;
        }
        else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
            return false;
        else
        {
            req.headers.back().name.push_back(input);
            return logic::tribool::INDETERMINATE;
        }
    case space_before_header_value:
        if (input == ' ')
        {
            m_state = header_value;
            return logic::tribool::INDETERMINATE;
        }
        else
            return false;
    case header_value:
        if (input == '\r')
        {
            m_state = expecting_newline_2;
            return logic::tribool::INDETERMINATE;
        }
        else if (is_ctl(input))
            return false;
        else
        {
            req.headers.back().value.push_back(input);
            return logic::tribool::INDETERMINATE;
        }
    case expecting_newline_2:
        if (input == '\n')
        {
            m_state = header_line_start;
            return logic::tribool::INDETERMINATE;
        }
        else
            return false;
    case expecting_newline_3:
        return (input == '\n');
    default:
        return false;
    }
    }

    bool Request_parser::is_char(int c)
    {
        return c >= 0 && c <= 127;
    }

    bool Request_parser::is_ctl(int c)
    {
        return (c >= 0 && c <= 31) || (c == 127);
    }

    bool Request_parser::is_tspecial(int c)
    {
        switch (c)
        {
        case '(': case ')': case '<': case '>': case '@':
        case ',': case ';': case ':': case '\\': case '"':
        case '/': case '[': case ']': case '?': case '=':
        case '{': case '}': case ' ': case '\t':
            return true;
        default:
            return false;
        }
    }

    bool Request_parser::is_digit(int c)
    {
        return c >= '0' && c <= '9';
    }
}