#include "simplelogger.h"

#include <cstring>
#include <cstdlib>
#include <iostream>

#ifdef _WIN32
#define rand_r(seed) (((int)seed)^rand())
#define snprintf _snprintf
#define localtime_r(time,localtime) localtime_s(localtime,time)
#endif

#define LOGSTDERR (std::cerr<<__FILE__<<":"<<__func__<<":"<<__LINE__<<" [ERROR]:")

using namespace mpcomm;

SimpleLogger::SimpleLogger(){
	m_level = ERR;
	m_iLimit = LOG_LIMIT_DEFAULT;
	unsigned int now = time(0);
	unsigned int seed = now + (unsigned long)(&now);
	m_iLoggerId = rand_r(&seed);
	memset(m_fileDir, 0, sizeof(m_fileDir));
	memset(m_filePath, 0, sizeof(m_filePath));
	m_iLogFileId = 0xFFFFFFFF;
	m_logFile = NULL;
}

SimpleLogger::SimpleLogger( LogLevel level, 
		int logLimit, const char * dir ){
	m_level = level;
	m_iLimit = logLimit;
	unsigned int now = time(0);
	unsigned int seed = now + (unsigned long)(&now);
	m_iLoggerId = rand_r(&seed);
	m_logFile = NULL;
	memset(m_fileDir, 0, sizeof(m_fileDir));
	memset(m_filePath, 0, sizeof(m_filePath));
	m_iLogFileId = 0xFFFFFFFF;
	if ( strnlen(dir, MAX_FILE_DIR) < MAX_FILE_DIR ){
		strncpy(m_fileDir, dir, MAX_FILE_DIR);
		ChangeLogFile(now);
	}else{
		LOGSTDERR<<"ERROR FILE DIR TOO LONG"; 
	}
}

SimpleLogger::~SimpleLogger(){
	if ( m_logFile ){
		fclose(m_logFile);
		m_logFile = NULL;
	}
}

int SimpleLogger::SetLevel( LogLevel level ){
	m_level = level;
	return 0;
}

int SimpleLogger::SetLogLimit( int limit ){
	m_iLimit = limit;
	return 0;
}

int SimpleLogger::SetLogDir( const char * dir ){
	if ( strnlen(dir, MAX_FILE_DIR) < MAX_FILE_DIR ){
		strncpy(m_fileDir, dir, MAX_FILE_DIR);
		time_t now = time(0);
		ChangeLogFile(now);
		return 0;
	}else{
		LOGSTDERR<<"ERROR FILE DIR TOO LONG"<<std::endl; 
		return ErrLoggerDirLen;
	}
}

void SimpleLogger::SetBehaviour( const std::string & behaviour ){
	m_behaviour = behaviour;
}

const char * SimpleLogger::GetLogPrefix( LogLevel level ){
	switch (level){
		case ERR:
			return "[ERROR]";
		case WARN:
			return "[WARN]";
		case INFO:
			return "[INFO]";
		case DEBUG:
			return "[DEBUG]";
		default:
			LOGSTDERR<<"ERROR: UNKNOWN LOG LEVEL"<<std::endl;
			return "[UNKNOWN LEVEL]";
	}
}

int SimpleLogger::GetLogFormat(char * buf, size_t size, 
		const char * format, ... ){
	va_list args;
	va_start(args,format);
	int ret = vsnprintf(buf, size, format,args);
	buf[size-1]='\0';//for windows
	va_end(args);
	if ( ret < 0 || ret > size ){
		LOGSTDERR<<"vsnprintf ret "<<ret<<" size "<<size<<std::endl;
		return ErrLogArgsInvalid;
	}
	return 0;
}

int SimpleLogger::ChangeLogFile( time_t now ){
	if ( strnlen(m_fileDir, MAX_FILE_DIR) >= MAX_FILE_DIR ){
		LOGSTDERR<<"ERROR: filePath invalid"<<std::endl;
		return ErrLoggerDirLen;
	}
	const char * del = "";
	char endOfDir = m_fileDir[strnlen(m_fileDir, MAX_FILE_DIR)];
#ifdef _WIN32
	if ( 0 != m_fileDir[0] && '\\' != endOfDir ) del ="\\";
#else
	if ( 0 != m_fileDir[0] && '/' !=  endOfDir ) del = "/";
#endif
	struct tm localtime;
	localtime_r(&now, &localtime);
	snprintf(m_filePath, MAX_FILE_PATH, "%s%s%04d%02d%02d%02d.log",
			m_fileDir, del, localtime.tm_year + 1900, localtime.tm_mon + 1,
			localtime.tm_mday, localtime.tm_hour);
	m_filePath[MAX_FILE_PATH-1]='\0';//for windows;
	m_iLogFileId = GetLogFileId(now);
	if ( m_logFile ){
		fclose(m_logFile);
		m_logFile = NULL;
	}
	return 0;
}

int SimpleLogger::GetLogTimeFormat( time_t now, char * buf, size_t size ){
	//yyyy-MM-dd HH:mm:ss
	struct tm localtime;
	localtime_r(&now, &localtime);
	int year = 1900 + localtime.tm_year;
	int month = localtime.tm_mon + 1;
	int day = localtime.tm_mday;
	int hour = localtime.tm_hour;
	int minute = localtime.tm_min;
	int second = localtime.tm_sec;
	int ret = snprintf(buf, size, "%04d-%02d-%02d %02d:%02d:%02d",
			year,month,day,hour,minute,second);
	buf[size-1]='\0';
	if ( ret < 0 || ret > size ){
		LOGSTDERR<<"vsnprintf ret "<<ret<<" size "<<size<<std::endl;
		return ErrLogArgsInvalid;
	}
	return 0;
}

int SimpleLogger::GetLogFileId( time_t now ){
	struct tm localtime;
	localtime_r(&now, &localtime);
	return localtime.tm_yday*24 + localtime.tm_hour;
}

int SimpleLogger::Log( LogLevel level, const char * format, ... ){
	if ( level < m_level ){
		return 0;
	}
	if ( 0 == m_filePath[0] ){
		return 0;
	}
	int ret = 0;
	time_t now = time(0);
	int curLogFileId = GetLogFileId(now);
	if ( curLogFileId != m_iLogFileId ){
		ret = ChangeLogFile(now);
		if ( ret ) return ret;
	}
	char logTime[sizeof(LOG_TIME_FORMAT)];
	char header[LOG_FORMAT_LIMIT];
	ret = GetLogTimeFormat(now, logTime, sizeof(logTime));
	if ( ret ){
		LOGSTDERR<<"GetLogTimeFormat ret "<<ret<<std::endl;
		return ret;
	}
	if ( m_behaviour.empty() ) m_behaviour = "UNKNOWN";
	ret = GetLogFormat(header,sizeof(header),"%s [%s] [%s] [%u] %s",
			GetLogPrefix(level), logTime, m_behaviour.c_str(), 
			m_iLoggerId, format);
	if ( ret ){
		LOGSTDERR<<"GetLogFormat ret "<<ret<<std::endl;
		return ret;
	}
	if ( !m_logFile ){
		m_logFile = fopen(m_filePath,"a+");
		if ( !m_logFile ){
			LOGSTDERR<<"fopen "<<m_filePath<<" fail!"<<ret<<std::endl;
			return ErrLoggerOpenFile;
		}
	}
	long len = 0;
	len = ftell(m_logFile);
	if ( m_iLimit < len ){
		return ErrLogLimited;
	}
	char logbuf[LOG_ONE_LIMIT];
	va_list args;
	va_start(args, format);
	ret = vsnprintf(logbuf, sizeof(logbuf), header, args);
	va_end(args);
	logbuf[sizeof(logbuf)-1] = '\0';//for windows;
	if ( ret < 0 ){
		LOGSTDERR<<"vsnprintf ret "<<ret<<std::endl;
		return ErrLogArgsInvalid;
	}
	fprintf(m_logFile,"%s",logbuf);
	return 0;
}

#undef LOGSTDERR

#ifdef _WIN32
#undef rand_r
#undef snprintf
#undef localtime_r
#endif

