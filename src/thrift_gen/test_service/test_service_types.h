/**
 * Autogenerated by Thrift Compiler (0.9.2)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef test_service_TYPES_H
#define test_service_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/cxxfunctional.h>


namespace test_service {

struct OPType {
  enum type {
    ADD = 0,
    SUB = 1,
    MUL = 2,
    DIV = 3
  };
};

extern const std::map<int, const char*> _OPType_VALUES_TO_NAMES;

class TestServiceRequest;

class TestServiceResponse;


class TestServiceRequest {
 public:

  static const char* ascii_fingerprint; // = "3C6B5910B2C7A8886AEC90D855401773";
  static const uint8_t binary_fingerprint[16]; // = {0x3C,0x6B,0x59,0x10,0xB2,0xC7,0xA8,0x88,0x6A,0xEC,0x90,0xD8,0x55,0x40,0x17,0x73};

  TestServiceRequest(const TestServiceRequest&);
  TestServiceRequest& operator=(const TestServiceRequest&);
  TestServiceRequest() : left(0), right(0), op((OPType::type)0) {
  }

  virtual ~TestServiceRequest() throw();
  int32_t left;
  int32_t right;
  OPType::type op;

  void __set_left(const int32_t val);

  void __set_right(const int32_t val);

  void __set_op(const OPType::type val);

  bool operator == (const TestServiceRequest & rhs) const
  {
    if (!(left == rhs.left))
      return false;
    if (!(right == rhs.right))
      return false;
    if (!(op == rhs.op))
      return false;
    return true;
  }
  bool operator != (const TestServiceRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TestServiceRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const TestServiceRequest& obj);
};

void swap(TestServiceRequest &a, TestServiceRequest &b);


class TestServiceResponse {
 public:

  static const char* ascii_fingerprint; // = "52C6DAB6CF51AF617111F6D3964C6503";
  static const uint8_t binary_fingerprint[16]; // = {0x52,0xC6,0xDA,0xB6,0xCF,0x51,0xAF,0x61,0x71,0x11,0xF6,0xD3,0x96,0x4C,0x65,0x03};

  TestServiceResponse(const TestServiceResponse&);
  TestServiceResponse& operator=(const TestServiceResponse&);
  TestServiceResponse() : errorCode(0), errorMsg(), result(0) {
  }

  virtual ~TestServiceResponse() throw();
  int32_t errorCode;
  std::string errorMsg;
  int32_t result;

  void __set_errorCode(const int32_t val);

  void __set_errorMsg(const std::string& val);

  void __set_result(const int32_t val);

  bool operator == (const TestServiceResponse & rhs) const
  {
    if (!(errorCode == rhs.errorCode))
      return false;
    if (!(errorMsg == rhs.errorMsg))
      return false;
    if (!(result == rhs.result))
      return false;
    return true;
  }
  bool operator != (const TestServiceResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TestServiceResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const TestServiceResponse& obj);
};

void swap(TestServiceResponse &a, TestServiceResponse &b);

} // namespace

#endif