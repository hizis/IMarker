#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>
#include <vector>
#include <queue>

#include "Thread.h"
#include "Socket.h"
#include "ConnectionHandler.h"
#include "ThreadPool.h"
#include "Exception.h"

using namespace std;

int main(int argc, char* argv[])
{
    try
    {
        if (argc < 2)
        {
            throw RuntimeException("Invalid command options. Try \"./HTTPServer_server <port>\" ");
        }

        int portNo = atoi(argv[1]);

        if ((portNo > 65535) || (portNo < 2000))
        {
            throw RuntimeException("Invalid port number");
        }

        SocketAddr svrAdd(portNo);
        Socket socket;
        socket.bind(svrAdd);
        socket.listen(5);

        SharedPtr<ThreadPool> tp(new ThreadPool(10)); // ThreadPool *tp = new ThreadPool(10);

        for (;;)
        {
            try
            {
                SharedPtr<Socket> pSocket(socket.acceptConnection());
                tp->addTask(new ConnectionHandler(pSocket));

            }
            catch (...)
            {
                std::cerr << "catch (...)" << std::endl;
                //$$$%%%ErrorHandler::handle();
            }
        }
    }
    catch (const SocketException & e)
    {
        std::cerr << "SocketException with message:" << e.message() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const RuntimeException & e)
    {
        std::cerr << "RuntimeException with message:" << e.message() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "Unknown error" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;

}
