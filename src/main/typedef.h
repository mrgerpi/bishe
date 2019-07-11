#ifndef _type_def_h_
#define _type_def_h_

#include <string>
#include <thrift/server/TSimpleServer.h>

typedef int (*Client_Entry)(std::string ip, int port);

typedef apache::thrift::server::TSimpleServer* (*Server_Entry)(int port);

#endif
