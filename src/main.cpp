#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include <cstring>
#include <unistd.h>
#include <thread>
#include "server.h"


static const char *OPTSTR = "h:p:d:";

int main(int argc, char ** argv)
{
    std::string sHost, sPort, sPath;
    int opt = getopt(argc, argv, OPTSTR);
    while(opt != -1)
    {
        switch(opt)
        {
            case 'h':
            sHost = optarg;
            break;
            case 'p':
            sPort = optarg;
            break;
            case 'd':
            sPath = optarg;
            break;
        }
        opt = getopt(argc, argv, OPTSTR);    
    }

    //may return 0 when not able to detect
    unsigned concurentThreads = std::thread::hardware_concurrency();
    

    pid_t procId = 0, sessionId = 0;
    
    procId = fork();
    
    if(procId < 0)
        return 1;
    else if(procId > 0)
    {
        std::cout << "Daemon pid = " << procId << std::endl;
        return 0;
    }

    //unmask the file mode
    umask(0);
    //set new session
    sessionId = setsid();

    if(sessionId < 0)
        return 1;


    std::ofstream fout("./log");

    //change the current working directory to root.
    chdir("/");

    //close stdin. stdout and stderr
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);


    //server::Server srv("127.0.0.1", "12345", "/home/usr/site", 2);
    try
    {
        fout << "Running server..." << std::endl;
        server::Server srv(sHost, sPort, sPath, concurentThreads > 0 ? concurentThreads : 2);
        srv.run();
        fout << "Stopping server..." << std::endl;
    }
    catch(std::exception& e)
    {
        fout << "Error:\n" << e.what() << std::endl;
        fout.close();
        throw;
    }
    catch(...)
    {
        fout << "Critical error\n";
        fout.close();
        throw;
    }
    fout.close();

    return 0;
}