#include "HotspotService.h"

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
using namespace ::hotspot::service;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;


extern "C" int hotspot_api_GetRecommendStationList_9999(string ip, int port)
{
	boost::shared_ptr<TSocket> socket(new TSocket(ip, port));
	boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

//	socket->setConnTimeout(500);
//	socket->setRecvTimeout(300);
//	socket->setSendTimeout(300);
	shared_ptr<HotspotServiceClient> client(new HotspotServiceClient(protocol));
	using apache::thrift::to_string;

	HotspotResponse rsp;

	HotspotRequest req0;
	boost::shared_ptr<TTransport> req0_itrans(new TSimpleFileTransport("/home/Shit/thrift-test/src/../data/hotspot_api/GetRecommendStationList/req0.json", true, true));
	boost::shared_ptr<TProtocol> req0_iprot(new TJSONProtocol(req0_itrans));
	req0.read(req0_iprot.get());
	log_info("hotspot_api::GetRecommendStationList||entry||HotspotRequest=%s", to_string(req0).c_str());	

	Trace req1;
	boost::shared_ptr<TTransport> req1_itrans(new TSimpleFileTransport("/home/Shit/thrift-test/src/../data/hotspot_api/GetRecommendStationList/req1.json", true, true));
	boost::shared_ptr<TProtocol> req1_iprot(new TJSONProtocol(req1_itrans));
	req1.read(req1_iprot.get());
	log_info("hotspot_api::GetRecommendStationList||entry||Trace=%s", to_string(req1).c_str());	

	transport->open();
	try {
		client->GetRecommendStationList(rsp, req0, req1);	
	} catch (TException& tx) {
		log_error("hotspot_api::GetRecommendStationList Exception||message=%s", tx.what());
		transport->close();
		return -1;
	}
	transport->close();
	log_info("hotspot_api::GetRecommendStationList ||rpc||rsp=%s", to_string(rsp).c_str());


	boost::shared_ptr<TTransport> otrans(new TSimpleFileTransport("/home/Shit/thrift-test/src/../data/hotspot_api/GetRecommendStationList/rsp.json", true, true));
	boost::shared_ptr<TProtocol> oprot(new TJSONProtocol(otrans));
	FILE* fp = fopen("/home/Shit/thrift-test/src/../data/hotspot_api/GetRecommendStationList/rsp.json", "w");
	fclose(fp);
	rsp.write(oprot.get());
	log_info("hotspot_api::GetRecommendStationList ||exit||HotspotResponse=%s", to_string(rsp).c_str());	
	return 0;
}

extern "C" int hotspot_api_ForecastDepartureTime_9999(string ip, int port)
{
	boost::shared_ptr<TSocket> socket(new TSocket(ip, port));
	boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

//	socket->setConnTimeout(500);
//	socket->setRecvTimeout(300);
//	socket->setSendTimeout(300);
	shared_ptr<HotspotServiceClient> client(new HotspotServiceClient(protocol));
	using apache::thrift::to_string;

	ForecastDepartureTimeResponse rsp;

	ForecastDepartureTimeRequest req0;
	boost::shared_ptr<TTransport> req0_itrans(new TSimpleFileTransport("/home/Shit/thrift-test/src/../data/hotspot_api/ForecastDepartureTime/req0.json", true, true));
	boost::shared_ptr<TProtocol> req0_iprot(new TJSONProtocol(req0_itrans));
	req0.read(req0_iprot.get());
	log_info("hotspot_api::ForecastDepartureTime||entry||ForecastDepartureTimeRequest=%s", to_string(req0).c_str());	

	transport->open();
	try {
		client->ForecastDepartureTime(rsp, req0);	
	} catch (TException& tx) {
		log_error("hotspot_api::ForecastDepartureTime Exception||message=%s", tx.what());
		transport->close();
		return -1;
	}
	transport->close();
	log_info("hotspot_api::ForecastDepartureTime ||rpc||rsp=%s", to_string(rsp).c_str());


	boost::shared_ptr<TTransport> otrans(new TSimpleFileTransport("/home/Shit/thrift-test/src/../data/hotspot_api/ForecastDepartureTime/rsp.json", true, true));
	boost::shared_ptr<TProtocol> oprot(new TJSONProtocol(otrans));
	FILE* fp = fopen("/home/Shit/thrift-test/src/../data/hotspot_api/ForecastDepartureTime/rsp.json", "w");
	fclose(fp);
	rsp.write(oprot.get());
	log_info("hotspot_api::ForecastDepartureTime ||exit||ForecastDepartureTimeResponse=%s", to_string(rsp).c_str());	
	return 0;
}

extern "C" int hotspot_api_getMatchDetail_9999(string ip, int port)
{
	boost::shared_ptr<TSocket> socket(new TSocket(ip, port));
	boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

//	socket->setConnTimeout(500);
//	socket->setRecvTimeout(300);
//	socket->setSendTimeout(300);
	shared_ptr<HotspotServiceClient> client(new HotspotServiceClient(protocol));
	using apache::thrift::to_string;

	MatchDetailResponse rsp;

	MatchDetailRequest req0;
	boost::shared_ptr<TTransport> req0_itrans(new TSimpleFileTransport("/home/Shit/thrift-test/src/../data/hotspot_api/getMatchDetail/req0.json", true, true));
	boost::shared_ptr<TProtocol> req0_iprot(new TJSONProtocol(req0_itrans));
	req0.read(req0_iprot.get());
	log_info("hotspot_api::getMatchDetail||entry||MatchDetailRequest=%s", to_string(req0).c_str());	

	transport->open();
	try {
		client->getMatchDetail(rsp, req0);	
	} catch (TException& tx) {
		log_error("hotspot_api::getMatchDetail Exception||message=%s", tx.what());
		transport->close();
		return -1;
	}
	transport->close();
	log_info("hotspot_api::getMatchDetail ||rpc||rsp=%s", to_string(rsp).c_str());


	boost::shared_ptr<TTransport> otrans(new TSimpleFileTransport("/home/Shit/thrift-test/src/../data/hotspot_api/getMatchDetail/rsp.json", true, true));
	boost::shared_ptr<TProtocol> oprot(new TJSONProtocol(otrans));
	FILE* fp = fopen("/home/Shit/thrift-test/src/../data/hotspot_api/getMatchDetail/rsp.json", "w");
	fclose(fp);
	rsp.write(oprot.get());
	log_info("hotspot_api::getMatchDetail ||exit||MatchDetailResponse=%s", to_string(rsp).c_str());	
	return 0;
}

extern "C" int hotspot_api_GetForecastFeature_9999(string ip, int port)
{
	boost::shared_ptr<TSocket> socket(new TSocket(ip, port));
	boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

//	socket->setConnTimeout(500);
//	socket->setRecvTimeout(300);
//	socket->setSendTimeout(300);
	shared_ptr<HotspotServiceClient> client(new HotspotServiceClient(protocol));
	using apache::thrift::to_string;

	GetForecastFeatureResponse rsp;

	GetForecastFeatureRequest req0;
	boost::shared_ptr<TTransport> req0_itrans(new TSimpleFileTransport("/home/Shit/thrift-test/src/../data/hotspot_api/GetForecastFeature/req0.json", true, true));
	boost::shared_ptr<TProtocol> req0_iprot(new TJSONProtocol(req0_itrans));
	req0.read(req0_iprot.get());
	log_info("hotspot_api::GetForecastFeature||entry||GetForecastFeatureRequest=%s", to_string(req0).c_str());	

	transport->open();
	try {
		client->GetForecastFeature(rsp, req0);	
	} catch (TException& tx) {
		log_error("hotspot_api::GetForecastFeature Exception||message=%s", tx.what());
		transport->close();
		return -1;
	}
	transport->close();
	log_info("hotspot_api::GetForecastFeature ||rpc||rsp=%s", to_string(rsp).c_str());


	boost::shared_ptr<TTransport> otrans(new TSimpleFileTransport("/home/Shit/thrift-test/src/../data/hotspot_api/GetForecastFeature/rsp.json", true, true));
	boost::shared_ptr<TProtocol> oprot(new TJSONProtocol(otrans));
	FILE* fp = fopen("/home/Shit/thrift-test/src/../data/hotspot_api/GetForecastFeature/rsp.json", "w");
	fclose(fp);
	rsp.write(oprot.get());
	log_info("hotspot_api::GetForecastFeature ||exit||GetForecastFeatureResponse=%s", to_string(rsp).c_str());	
	return 0;
}

extern "C" int hotspot_api_getPrematchhHoldInfo_9999(string ip, int port)
{
	boost::shared_ptr<TSocket> socket(new TSocket(ip, port));
	boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

//	socket->setConnTimeout(500);
//	socket->setRecvTimeout(300);
//	socket->setSendTimeout(300);
	shared_ptr<HotspotServiceClient> client(new HotspotServiceClient(protocol));
	using apache::thrift::to_string;

	PrematchhHoldInfoResponse rsp;

	PrematchhHoldInfoRequest req0;
	boost::shared_ptr<TTransport> req0_itrans(new TSimpleFileTransport("/home/Shit/thrift-test/src/../data/hotspot_api/getPrematchhHoldInfo/req0.json", true, true));
	boost::shared_ptr<TProtocol> req0_iprot(new TJSONProtocol(req0_itrans));
	req0.read(req0_iprot.get());
	log_info("hotspot_api::getPrematchhHoldInfo||entry||PrematchhHoldInfoRequest=%s", to_string(req0).c_str());	

	transport->open();
	try {
		client->getPrematchhHoldInfo(rsp, req0);	
	} catch (TException& tx) {
		log_error("hotspot_api::getPrematchhHoldInfo Exception||message=%s", tx.what());
		transport->close();
		return -1;
	}
	transport->close();
	log_info("hotspot_api::getPrematchhHoldInfo ||rpc||rsp=%s", to_string(rsp).c_str());


	boost::shared_ptr<TTransport> otrans(new TSimpleFileTransport("/home/Shit/thrift-test/src/../data/hotspot_api/getPrematchhHoldInfo/rsp.json", true, true));
	boost::shared_ptr<TProtocol> oprot(new TJSONProtocol(otrans));
	FILE* fp = fopen("/home/Shit/thrift-test/src/../data/hotspot_api/getPrematchhHoldInfo/rsp.json", "w");
	fclose(fp);
	rsp.write(oprot.get());
	log_info("hotspot_api::getPrematchhHoldInfo ||exit||PrematchhHoldInfoResponse=%s", to_string(rsp).c_str());	
	return 0;
}

extern "C" int hotspot_api_GetPrematchRecommendInfo_9999(string ip, int port)
{
	boost::shared_ptr<TSocket> socket(new TSocket(ip, port));
	boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

//	socket->setConnTimeout(500);
//	socket->setRecvTimeout(300);
//	socket->setSendTimeout(300);
	shared_ptr<HotspotServiceClient> client(new HotspotServiceClient(protocol));
	using apache::thrift::to_string;

	PrematchRecommendResponse rsp;

	PrematchRecommendRequest req0;
	boost::shared_ptr<TTransport> req0_itrans(new TSimpleFileTransport("/home/Shit/thrift-test/src/../data/hotspot_api/GetPrematchRecommendInfo/req0.json", true, true));
	boost::shared_ptr<TProtocol> req0_iprot(new TJSONProtocol(req0_itrans));
	req0.read(req0_iprot.get());
	log_info("hotspot_api::GetPrematchRecommendInfo||entry||PrematchRecommendRequest=%s", to_string(req0).c_str());	

	transport->open();
	try {
		client->GetPrematchRecommendInfo(rsp, req0);	
	} catch (TException& tx) {
		log_error("hotspot_api::GetPrematchRecommendInfo Exception||message=%s", tx.what());
		transport->close();
		return -1;
	}
	transport->close();
	log_info("hotspot_api::GetPrematchRecommendInfo ||rpc||rsp=%s", to_string(rsp).c_str());


	boost::shared_ptr<TTransport> otrans(new TSimpleFileTransport("/home/Shit/thrift-test/src/../data/hotspot_api/GetPrematchRecommendInfo/rsp.json", true, true));
	boost::shared_ptr<TProtocol> oprot(new TJSONProtocol(otrans));
	FILE* fp = fopen("/home/Shit/thrift-test/src/../data/hotspot_api/GetPrematchRecommendInfo/rsp.json", "w");
	fclose(fp);
	rsp.write(oprot.get());
	log_info("hotspot_api::GetPrematchRecommendInfo ||exit||PrematchRecommendResponse=%s", to_string(rsp).c_str());	
	return 0;
}

extern "C" int hotspot_api_GetEtdInfo_9999(string ip, int port)
{
	boost::shared_ptr<TSocket> socket(new TSocket(ip, port));
	boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

//	socket->setConnTimeout(500);
//	socket->setRecvTimeout(300);
//	socket->setSendTimeout(300);
	shared_ptr<HotspotServiceClient> client(new HotspotServiceClient(protocol));
	using apache::thrift::to_string;

	CarpoolEtdResponse rsp;

	CarpoolEtdRequest req0;
	boost::shared_ptr<TTransport> req0_itrans(new TSimpleFileTransport("/home/Shit/thrift-test/src/../data/hotspot_api/GetEtdInfo/req0.json", true, true));
	boost::shared_ptr<TProtocol> req0_iprot(new TJSONProtocol(req0_itrans));
	req0.read(req0_iprot.get());
	log_info("hotspot_api::GetEtdInfo||entry||CarpoolEtdRequest=%s", to_string(req0).c_str());	

	transport->open();
	try {
		client->GetEtdInfo(rsp, req0);	
	} catch (TException& tx) {
		log_error("hotspot_api::GetEtdInfo Exception||message=%s", tx.what());
		transport->close();
		return -1;
	}
	transport->close();
	log_info("hotspot_api::GetEtdInfo ||rpc||rsp=%s", to_string(rsp).c_str());


	boost::shared_ptr<TTransport> otrans(new TSimpleFileTransport("/home/Shit/thrift-test/src/../data/hotspot_api/GetEtdInfo/rsp.json", true, true));
	boost::shared_ptr<TProtocol> oprot(new TJSONProtocol(otrans));
	FILE* fp = fopen("/home/Shit/thrift-test/src/../data/hotspot_api/GetEtdInfo/rsp.json", "w");
	fclose(fp);
	rsp.write(oprot.get());
	log_info("hotspot_api::GetEtdInfo ||exit||CarpoolEtdResponse=%s", to_string(rsp).c_str());	
	return 0;
}
