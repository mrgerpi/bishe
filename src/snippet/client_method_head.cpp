
extern "C" int SERVICE_NAME_METHOD_PORT(string ip, int port)
{
	boost::shared_ptr<TSocket> socket(new TSocket(ip, port));
	boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

	socket->setConnTimeout(500);
	socket->setRecvTimeout(300);
	socket->setSendTimeout(300);
<<<<<<< HEAD


=======
>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
	shared_ptr<INTER_SERVICE_NAMEClient> client(new INTER_SERVICE_NAMEClient(protocol));
	using apache::thrift::to_string;

	RESPONSE_TYPE rsp;
