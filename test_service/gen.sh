mkdir -p ./thrift_gen/test_service
thrift -r -gen cpp -out ./thrift_gen/test_service idl/test_service.thrift
mkdir -p ./thrift_gen/calculate
thrift -r -gen cpp -out ./thrift_gen/calculate idl/calculate.thrift
