#pragma once

#ifndef _MP_CLIENT_MP_COMM_H_
#define _MP_CLIENT_MP_COMM_H_

#define MP_DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&);               \
	void operator=(const TypeName&)


namespace mpcomm{

	enum LogLevel{
		DEBUG 			= 0,
		INFO 			= 1,
		WARN			= 2,
		ERR 			= 3,
		MAX_LOG_LEVEL 	= 4,
	};

	enum Errcode{
		ErrInvalidMataData  = -5000,

		ErrSimpleLogger 	= -4000,
		ErrLoggerDirLen		= -3999,
		ErrLogArgsInvalid	= -3998,
		ErrLoggerOpenFile	= -3997,
		ErrLogLimited		= -3996,

		ErrProtobuf			= -3000,

		ErrRapidJson 		= -2000,
		ErrInvalidJsonStr	= -1999,
		ErrJsonPbMismatch	= -1998,

		ErrLibcurl 			= -1000,
		ErrCurlInit			= -999,
		ErrCurlTimeout		= -998,
		ErrCurlPartialFile  = -997,
		ErrCurlFailConn		= -996,
		ErrCurlAccessDenied = -995,

		ErrInvalidArg		= -2,
		ErrInternal 		= -1,

		//[0,600) for http
		ErrOk				= 0,
	};

	const char * errmsg( int code );

};

#endif
////_MP_CLIENT_MP_COMM_H_
