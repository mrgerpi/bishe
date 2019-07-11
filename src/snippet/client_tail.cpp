
	transport->open();
	try {
		client->METHOD(rspALL_REQ);	
	} catch (TException& tx) {
		log_error("SERVICE_NAME::METHOD Exception||message=%s", tx.what());
		transport->close();
		return -1;
	}
	transport->close();
	log_info("SERVICE_NAME::METHOD ||rpc||rsp=%s", to_string(rsp).c_str());


	boost::shared_ptr<TTransport> otrans(new TSimpleFileTransport("RSP_PATH", true, true));
	boost::shared_ptr<TProtocol> oprot(new TJSONProtocol(otrans));
	FILE* fp = fopen("RSP_PATH", "w");
	fclose(fp);
	rsp.write(oprot.get());
	log_info("SERVICE_NAME::METHOD ||exit||RSP_TYPE=%s", to_string(rsp).c_str());	
	return 0;
}
