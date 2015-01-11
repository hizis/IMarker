/*
 * ConnectionHandler.cpp
 *
 *  Created on: 09 янв. 2015 г.
 *      Author: tarasov
 */

#include <ConnectionHandler.h>
#include <sstream>
#include <iostream>


ConnectionHandler::ConnectionHandler(SharedPtr<Socket> socket) :
        m_socket(socket)
{
    std::cerr << pthread_self() << ":HTTPClient::HTTPClient()" << std::endl;
}

ConnectionHandler::~ConnectionHandler()
{
    std::cerr << pthread_self() << ":HTTPClient::~HTTPClient()" << std::endl;
}

void ConnectionHandler::run()
{
    char input[1024];
    m_socket->receiveBytes(input, sizeof(input) - 1);

    std::string str(input);
    std::cerr << str << std::endl;

    std::ostringstream headerOut;
    std::ostringstream bodyOut;
    std::ostringstream commonOut;

    headerOut << "HTTP/1.1 200 OK" << std::endl;
//    headerOut << "Date: Wed, 11 Feb 2009 11:20:59 GMT" << std::endl;
//    headerOut << "Server: Apache" << std::endl;
//    headerOut << "X-Powered-By: PHP/5.2.4-2ubuntu5wm1" << std::endl;
//    headerOut << "Last-Modified: Wed, 11 Feb 2009 11:20:59 GMT" << std::endl;
//    headerOut << "Content-Language: ru" << std::endl;
//    headerOut << "Content-Type: text/html; charset=utf-8" << std::endl;
//    headerOut << "Content-Length: 1234" << std::endl;
    headerOut << "Connection: close" << std::endl;
    headerOut << std::endl;

//    "14fd6"
    bodyOut << "<!DOCTYPE html>" << std::endl;
    bodyOut << "<html>" << std::endl;
    bodyOut << "<head>" << std::endl;
    bodyOut << "<title>Hello world</title>" << std::endl;
    bodyOut << "</head>" << std::endl;
    bodyOut << "<body>" << std::endl;
    bodyOut << "<h1>Hello world</h1>" << std::endl;

    bodyOut << "Thread No: " << pthread_self() << std::endl;

    bodyOut << "</body>" << std::endl;
    bodyOut << std::endl;
    bodyOut << std::endl;



    commonOut << headerOut.str() << bodyOut.str();

    m_socket->sendBytes(commonOut.str().c_str(), commonOut.str().length());
    m_socket->close(); // TODO XXX
    std::cerr << pthread_self() << ":Out HTTPClient::run" << std::endl;

}


