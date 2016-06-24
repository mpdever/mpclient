#pragma once

#ifndef _MP_CLIENT_SIMPLE_LOGGER_H_
#define _MP_CLIENT_SIMPLE_LOGGER_H_

#include <cstdarg>
#include <ctime>
#include <cstdio>
#include <string>

#include "mpcomm.h"

#ifdef _WIN32
#define __func__ __FUNCTION__
#endif

#define SIMPLELOGERR(logger,fmt, ... ) (logger).Log( mpcomm::ERR, \
		"%s %s:%d "fmt"\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__ )
#define SIMPLELOGWARN(logger,fmt, ... ) (logger).Log( mpcomm::WARN, \
		"%s %s:%d "fmt"\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__ )
#define SIMPLELOGINFO(logger,fmt, ... ) (logger).Log( mpcomm::INFO, \
		"%s %s:%d "fmt"\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__ )
#define SIMPLELOGDEBUG(logger,fmt, ... ) (logger).Log( mpcomm::DEBUG, \
		"%s %s:%d "fmt"\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__ )

#define LOG_FILE_FORMAT "_yyyyMMddHH.log"
#define LOG_TIME_FORMAT "yyyy-MM-dd HH:mm:ss"

class SimpleLogger{
	MP_DISALLOW_COPY_AND_ASSIGN(SimpleLogger);

	public:
		static const unsigned int MAX_FILE_DIR = 1024;
		static const unsigned int MAX_FILE_PATH = 
			MAX_FILE_DIR + sizeof(LOG_FILE_FORMAT);
		static const unsigned int LOG_FORMAT_LIMIT = 1024;
		static const unsigned int LOG_ONE_LIMIT = 4096;//4KB
		static const unsigned int LOG_LIMIT_DEFAULT = 1<<30;//1GB

	public:

		SimpleLogger();
		SimpleLogger( mpcomm::LogLevel level, int logLimit, const char * dir );
		virtual ~SimpleLogger();

		int SetLevel( mpcomm::LogLevel level );
		int SetLogDir( const char * dir );
		int SetLogLimit( int limit );
		int Log( mpcomm::LogLevel level, const char * format, ... );

		void SetBehaviour( const std::string & behaviour );

	private:

		const char * GetLogPrefix( mpcomm::LogLevel level );
		int GetLogFormat( char * buf, size_t size, const char * headerFormat, ... );
		int ChangeLogFile( time_t now );
		int GetLogFileId( time_t now );
		int GetLogTimeFormat( time_t now, char * buf, size_t size );

	private:

		std::string m_behaviour;

		mpcomm::LogLevel m_level;

		int m_iLimit; 
		unsigned int m_iLoggerId;

		char m_fileDir[MAX_FILE_DIR];
		char m_filePath[MAX_FILE_PATH];

		int m_iLogFileId;
		FILE * m_logFile;

};

#endif
////_MP_CLIENT_SIMPLE_LOGGER_H_
