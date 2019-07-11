namespace cpp calculator

enum OPType{
<<<<<<< HEAD
 ADD = 0;
       SUB = 1;
       MUL = 2;
       DIV = 3;
}

struct CalculateRequest {
  1: required i32 left;
  2: required i32 right;
 3: required OPType op;
}

struct CalculateResponse {
        1: required i32 errorCode;
     2: required string errorMsg;
3: required i32 result;
}

service CalculatorService {
 CalculateResponse Calculate(1:CalculateRequest req);
}
=======
	ADD = 0;
	SUB = 1;
	MUL = 2;
	DIV = 3;
}

struct CalculateRequest {
	1: required i32 left;
	2: required i32 right;
	3: required OPType op;
}

struct CalculateResponse {
	1: required i32 errorCode;
	2: required string errorMsg;
	3: required i32 result;
}

service CalculatorService {
	CalculateResponse Calculate(1:CalculateRequest req);
}
>>>>>>> 592b5ecec0f6379a6b24bb7739e919bb54686fd1
