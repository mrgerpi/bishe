	using apache::thrift::to_string;
	using namespace std;
	log_info("CLASS::METHODREQ_LOG"LOG_PARA);

	string path = "RESPONSE_PATH";
	boost::shared_ptr<TTransport> transport(new TSimpleFileTransport(path, true, true));
	boost::shared_ptr<TProtocol> protocol(new TJSONProtocol(transport));
	_return.read(protocol.get());

	log_info("CLASS::METHOD||rsp=%s", to_string(_return).c_str());
