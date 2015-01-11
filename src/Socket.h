/*
 * Socket.h
 *
 *  Created on: 07 янв. 2015 г.
 *      Author: tarasov
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <netinet/in.h>
#include <cerrno>
#include <string>
#include <map>

#include "Noncopyable.h"
#include "SharedPtr.h"
#include "Exception.h"



typedef socklen_t       SocketLen;
typedef unsigned short  Port;
typedef int SocketFD;



class Socket;

class SocketAddr : private Noncopyable
{
public:
    SocketAddr(Port portNumber);

private:
    struct sockaddr_in m_addr;
    friend Socket;
};

class Socket : private Noncopyable
{
public:
    Socket();
    Socket(SocketFD sockfd);
    virtual ~Socket();
    void close();
    void bind(const SocketAddr & address);
    void listen(int backlog = 64);
    Socket* acceptConnection();

    int sendBytes(const void* buffer, int length, int flags = 0);
    int receiveBytes(void* buffer, int length, int flags = 0);

private:


    void init(const SocketAddr & address);
    void setOption(int level, int option, int value);
    void setRawOption(int level, int option, const void* value, SocketLen length);
    static void doThrow(int code);

    static std::map<int,std::string> createSocketErrorMap();
    static const std::map<int,std::string> socketErrorMap;

    SocketFD m_sockfd;
};




#endif /* SOCKET_H_ */
