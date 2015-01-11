/*
 * ConnectionHandler.h
 *
 *  Created on: 09 янв. 2015 г.
 *      Author: tarasov
 */

#ifndef CONNECTIONHANDLER_H_
#define CONNECTIONHANDLER_H_

#include "Runnable.h"
#include "Socket.h"


class ConnectionHandler : public Runnable
{
public:
    ConnectionHandler(SharedPtr<Socket> socket);
    virtual ~ConnectionHandler();
    virtual void run();
private:
    SharedPtr<Socket> m_socket;
};


#endif /* CONNECTIONHANDLER_H_ */
