
	REQ_TYPE REQ_NAME;
	boost::shared_ptr<TTransport> REQ_NAME_itrans(new TSimpleFileTransport("REQ_PATH", true, true));
	boost::shared_ptr<TProtocol> REQ_NAME_iprot(new TJSONProtocol(REQ_NAME_itrans));
	REQ_NAME.read(REQ_NAME_iprot.get());
	log_info("SERVICE_NAME::METHOD||entry||REQ_TYPE=%s", to_string(REQ_NAME).c_str());	
