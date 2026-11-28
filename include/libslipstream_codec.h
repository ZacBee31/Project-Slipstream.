//
// Created by Zac B on 11/13/26. 4:24PM
//

#ifndef SLIPSTREAM_LIBSLIPSTREAM_H
#define SLIPSTREAM_LIBSLIPSTREAM_H

#endif //SLIPSTREAM_LIBSLIPSTREAM_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <functional>
#include <string>
#include <iostream>

/*
 * -Milestone 1 is to build the GCMD/1 and GCOE/1 serialisation layer,
 * and the two clients that feed it.
 * -These are the parts of the server that the clients connect to.
 * -Connects over TCP (UDP can be used for MD client).
 */

class SocketWrapper
{
public: //remember to change int to MdMessage later.
    virtual ~SocketWrapper() = default;
    virtual void start(std::function<void(const int&)> on_message) = 0;
    virtual void stop() = 0;

};

class TCPSocketWrapper : public SocketWrapper
{



};


void Parser();
void Receiver();






 

