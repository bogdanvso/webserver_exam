#include "request_handler.h"
#include <fstream>
#include <string>
#include "mime_types.h"
#include "reply.h"
#include "request.h"

//tmp!
//#include<iostream>

namespace server
{
    void Request_handler::handle_request(const Request& req, Reply& rep)
    {
//tmp
        /*std::cout << "m_doc: " << this->m_doc_root << std::endl 
            << "req.method = " << req.method << std::endl
            << "req.uri = " << req.uri << std::endl
            << "http ver = " << req.http_version_major << ' ' 
            << req.http_version_minor << std::endl;*/

        //url to path
        std::string req_path;
        if(!url_decode(req.uri, req_path))
        {
            //std::cout << "decode err!\n" << req_path << std::endl;

            rep = Reply::stock_reply(Reply::STATUS_TYPE::BAD_REQUEST);
            return;
        }

        //request path must be absolute and not contain ".."
        if(req_path.empty() || req_path[0] != '/'
            || req_path.find("..") != std::string::npos)
        {
            //std::cout << "another err!\n" << req_path << std::endl;

            rep = Reply::stock_reply(Reply::STATUS_TYPE::BAD_REQUEST);
            return;
        }

        //if path ends in slash then add "index.html"
        if(req_path[req_path.size() - 1] == '/')
            req_path += "index.html";
        
        //determine the file extension
        size_t last_slash_pos = req_path.find_last_of("/");
        size_t last_dot_pos = req_path.find_last_of(".");
        std::string extension;
        if(last_slash_pos != std::string::npos && last_dot_pos > last_slash_pos)
            extension = req_path.substr(last_dot_pos + 1);

        //open file to send back
        std::string full_path = m_doc_root + req_path;
        std::ifstream fin(full_path.c_str(), std::fstream::in | std::fstream::binary);
        if(!fin)
        {
            rep = Reply::stock_reply(Reply::STATUS_TYPE::NOT_FOUND);
            return;
        }

        //fill out the reply to be sent to the client
        rep.status = Reply::STATUS_TYPE::OK;
        char buf[512];
        while(fin.read(buf, sizeof(buf)).gcount() > 0)
            rep.content.append(buf, fin.gcount());
        rep.headers.resize(2);
        rep.headers[0].name = "Content-Lenght";
        rep.headers[0].value = std::to_string(rep.content.size());
        rep.headers[1].name = "Content-Type";
        rep.headers[1].value = mime_types::extension_to_type(extension);
    }
}