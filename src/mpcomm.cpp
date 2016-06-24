#include "mpcomm.h"

using namespace mpcomm;

const char * mpcomm::errmsg( int code ){
	switch ( code ) {
		case ErrInvalidMataData:
			return "[FATAL] some error in metadata";
		case ErrSimpleLogger:
			return "[ERROR] some error in logger";
		case ErrLoggerDirLen:
			return "[ERROR] length of directory out of limit";
		case ErrLogArgsInvalid:
			return "[ERROR] log args invalid";
		case ErrLoggerOpenFile:
			return "[ERROR] openfile fail";
		case ErrLogLimited:
			return "[INFO] log file out of limit";
		case ErrProtobuf:
			return "[ERROR] some error in protobuf";
		case ErrRapidJson:
			return "[ERROR] some error in rapidjson";
		case ErrInvalidJsonStr:
			return "[ERROR] json string invalid";
		case ErrJsonPbMismatch:
			return "[ERROR] json mismatch with protobuf define";
		case ErrLibcurl:
			return "[ERROR] some error in libcurl";
		case ErrCurlInit:
			return "[ERROR] some error in curl init";
		case ErrCurlTimeout:
			return "[ERROR] curl operation time out";
		case ErrCurlPartialFile:
			return "[ERROR] file length error";
		case ErrCurlFailConn:
			return "[ERROR] fail to connect host";
		case ErrCurlAccessDenied:
			return "[ERROR] access denied";
		case ErrInvalidArg:
			return "[ERROR] invalid args";
		case ErrInternal:
			return "[ERROR] some error internal";
		case ErrOk:
			return "No Error";
		default:
			if ( code > 0 && code < 600 ){
				return "Http Status Code";
			}
			return "Unknown error";
	}
}

