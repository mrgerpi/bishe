#include "INTER_SERVICE_NAME.h"

#include "simple_log.h"
#include <boost/shared_ptr.hpp>
#include <stdio.h>

#include <thrift/transport/TSocket.h>
#include <thrift/transport/TSimpleFileTransport.h>
#include <thrift/transport/TTransport.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/TToString.h>

using namespace std;
using namespace NAME_SPACE;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

