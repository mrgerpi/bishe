namespace cpp hotspot.service
namespace py hotspot.service
namespace php hotspot.service
namespace go hotspot.serviece

struct Trace {
  1: required string logId;  //  128bit 16进制打印 协议中是32长度string  0ade0f25579eaafe561903a2252f1102
}

struct StationInfo {
    1:required double lng;
	2:required list<i32> nlist;
	3:required set<i32> nset;
	4:required map<i32, string> nmap;
}

struct HotspotRequest {
    1: required i64 id;
    2: required bool isTrue;
    3: required string product_id;
	4: required StationInfo sinfo; 
}

// 去尾文案类型
enum TextType {
  EMPTY = 0; // 默认不出文案
  COMMON = 1; // 普通站点拼车文案
  SPACIAL = 2; // 特惠拼车文案
}

struct CarpoolEtdInfo{
    1: required i64 etdLeftMargin;  // etd最早到达时间，单位秒
    2: required i64 etdRightMargin; // etd最晚到达时间，单位秒
}

struct HotspotResponse {
    1:required i32 error_code;
    2:required string error_msg;
    3:optional string extends_info; //地图定义的相应扩展信息(duse透传前段)
}

struct ForecastDepartureTimeRequest {
    1:required i32    city_id;    //城市id
    2:required string pid;        //用户id(未登陆0)
    3:required string traceid;     
    4:required string spanid;
    5:required string station_id; //用户选择的站点id；
    6:optional i64    last_ready_departure_time; //上次预估的出发时间，首次访问不填或者填0
    7:optional string lang;       //语言环境
    8:optional map<string,string> extMap; //其它信息都放在extMap（二鹏也可以将其他信息都通过这个map传下来）
    9:optional double cur_lng;    //乘客当前位置，用于做步行eta保护;
    10:optional double cur_lat;
    11:optional string bubble_pid; //冒泡阶段用户唯一标识id(api生成，预匹配阶段也会是传这个唯一id);
    12:optional string bubble_trace_id; //最后一次预估价接口的trace_id，也就是最后一次切换站点的trace_id;(切换站点，会重新调用预估价接口)
    13:optional string appversion;  //版本号
    14:optional string product_id;
    20:optional Trace trace_info;
}

struct ForecastDepartureTimeResponse {
    1:required i32    error_code;
    2:required string error_msg;
    3:optional i64    ready_departure_time; //预估出发时间（绝对时间); 
    4:optional i32    rec_status;           //0:和API(高二鹏)约定，0做mock
                                            //1:无车无人：“附近车辆较少，叫车时间会较长”，发单后跳转到“等待应答”页面
                                            //2:无车有人：“附近车辆较少，叫车时间会较长”，发单后跳转到“等待应答”页面
                                            //3:有车有人:有车且不需要愿等：“10:12在站点［＊＊］发车”，发单后跳转到“等待应答”页面
                                            //4:有车无人:有车且需要愿等：“10:12在站点［＊＊］发车”，发单后跳转到“前往站点”页面
    5:optional i32    time_type;            //0精确时间，1时间区间
    6:optional i64    left_time;            //1496214826-1496214926 unix时间戳 
    7:optional i64    right_time;          
    8:optional i32    left_show_time;       //2-5分钟这样的区间
    9:optional i32    right_show_time;
    10:optional i32   hold_time;            //相对时间,秒
}

struct MatchDetailRequest {
    1:required string bubble_trace_id;      //预匹配trace_id,定时接口的trace_id
    2:required string city_id;              //城市id
    3:required string pid;                  //用户pid
    4:required i64    birth_time            //订单创建时间                  
    5:optional i64    order_id;             //订单id
    6:optional string appversion;           //版本号
    7:optional map<string, string> extMap   //扩展用
    8:optional string traceid;              //请求的traceid
    20:optional Trace trace_info;
}

struct MatchFeature {
    1: required string pid; //用户id
    2: required i32 match_degree;//匹配程度,60
}

struct MatchDetailResponse {
    1:required i32    error_code;
    2:required string error_msg;
    3:optional list<MatchFeature> matchFeatureList;  //匹配特征
    4:optional i32 hold_time;      //相对时间,秒
    5:optional i32 elapsed_time;   //相对等待时间,单位秒
    6:optional i32 rec_status;     //rec_status
}

struct GetForecastFeatureRequest {
    1:required string bubble_trace_id;   //预匹配trace_id, 用来关联
    2:optional i32 city_id;
    3:optional string pid;         //用户id
    4:optional string phone;       //用户手机号
    5:optional map<string, string> extMap   //扩展用
    6:optional string traceid;    //请求的traceid
    7:optional string spanid;
    20:optional Trace trace_info;
}

struct GetForecastFeatureResponse {
    1:required i32    error_code;
    2:required string error_msg;
    3:required map<string, double> forecastFeature;
}

struct ProductType {
    1:optional string product_id;
    2:optional i16 combo_type;
    3:optional string require_level;
}


struct PrematchhHoldInfoRequest {
    1:required string phone;
    2:required string pid;
    3:required i32 city_id; //城市id
    4:required double cur_lng;   //用户当前定位经度
    5:required double cur_lat;   //用户当前定位纬度
    6:required double start_lng; //冒泡起点经度
    7:required double start_lat; //冒泡起点纬度
    8:required string start_name; //起点名称
    9:required double dest_lng; //冒泡终点经度
    10:required double dest_lat; //冒泡终点纬度
    11:required string dest_name; //终点名称
    12:required string traceid; //冒泡trace
    13:optional map<string,string> extMap; //其它信息都放在extMap中
    14:optional list<ProductType> productReq; //批量请求
    20:optional Trace trace_info; //trace_info 压测用
}

struct PrematchHoldRes {
     1:required i32    error_code;
     2:required string error_msg;
     3:optional string require_level;
     4:optional i32 hold_time; //用户不勾选的愿等时间，单位秒
     5:optional i32 hit_hold; //1或者0, 1表示定时策略认为需要hold
     6:optional i32 hold_time_select; //用户勾选的愿等时间，单位秒
     7:optional ProductType product_info; //具体产品信息
}

struct PrematchhHoldInfoResponse {
    1:required i32    error_code;
    2:required string error_msg;
    3:optional i32 hold_time; //用户不勾选的愿等时间，单位秒
    4:optional i32 hit_hold; //1或者0, 1表示定时策略认为需要hold
    5:optional i32 hold_time_select; //用户勾选的愿等时间，单位秒
    6:optional list<PrematchHoldRes> holdResults; //批量返回,顺序与请求一致
}

struct PrematchStationRes {
     1:required i32    error_code;
     2:required string error_msg;
     3:required ProductType product_info;
     4:required list<StationInfo> station_list;
     5:optional map<string,string> extMap; //其它信息都放在extMap中
}

struct PrematchEtdRes {
     1:required i32    error_code;
     2:required string error_msg;
     3:required ProductType product_info;
     4:required CarpoolEtdInfo etd_info;
     5:optional map<string,string> extMap; //其它信息都放在extMap中
}

struct PrematchRecommendRequest {
    1:required string phone;
    2:required string pid;
    3:required i32 city_id; //城市id
    4:required double cur_lng;   //用户当前定位经度
    5:required double cur_lat;   //用户当前定位纬度
    6:required double start_lng; //冒泡起点经度
    7:required double start_lat; //冒泡起点纬度
    8:required string start_name; //起点名称
    9:required double dest_lng; //冒泡终点经度
    10:required double dest_lat; //冒泡终点纬度
    11:required string dest_name; //终点名称
    12:required string traceid; //冒泡trace
    13:optional map<string,string> extMap; //其它信息都放在extMap中
    14:optional list<ProductType> productReq; //批量请求
    20:optional Trace trace_info; //trace_info 压测用
}

struct PrematchRecommendResponse {
    1:required i32    error_code;
    2:required string error_msg;
    3:optional list<PrematchHoldRes> holdResults; //愿等数据
    4:optional list<PrematchStationRes> stationResults; //站点推荐数据
    5:optional list<PrematchEtdRes> etdResults; //ETD(预估到达时间)数据
    6:optional map<string,string> extMap; //其它信息都放在extMap中
}

struct CarpoolEtdRequest {
    1: required Trace trace_info; // trace信息
    2: required string bubble_id; // 冒泡id
    3: required i64 express_etd; // 快车预估到达时间，单位秒
    4: required i32 city_id; // 城市id
    5: required string phone; // 用户手机号
    6: required string pid; // 用户id 目前平滑移动拿不到pid，会传空字符串
    7: optional map<string, string> ext_info; // 拓展kv字段
}

struct CarpoolEtdResponse {
    1: required i32 error_code; // 返回码
    2: required string error_msg; // 错误信息
    3: required CarpoolEtdInfo carpool_etd; // 拼车计算的etd范围
    4: optional map<string, string> ext_info; // 拓展kv字段
}

service HotspotService {
    HotspotResponse GetRecommendStationList(1:HotspotRequest request, 2:Trace trace);
    ForecastDepartureTimeResponse ForecastDepartureTime(1:ForecastDepartureTimeRequest request);
    MatchDetailResponse getMatchDetail(1:MatchDetailRequest request);
    GetForecastFeatureResponse GetForecastFeature(1:GetForecastFeatureRequest request);
    PrematchhHoldInfoResponse getPrematchhHoldInfo(1:PrematchhHoldInfoRequest request);
    PrematchRecommendResponse GetPrematchRecommendInfo(1:PrematchRecommendRequest request);
    CarpoolEtdResponse GetEtdInfo(1:CarpoolEtdRequest req); // 获取ETD接口
}
