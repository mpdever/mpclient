#pragma once

#ifndef _MP_CLIENT_HTTP_HELPER_H_
#define _MP_CLIENT_HTTP_HELPER_H_

#include "mpcomm.h"

#include <string>
#include <vector>

#include "curl/curl.h"

class SimpleLogger;

struct UploadFile{
	std::string formId;
	std::string fileName;
	const std::string * pBuffer;
};


class HttpHelper{
		MP_DISALLOW_COPY_AND_ASSIGN(HttpHelper);
	public:
		
		HttpHelper( SimpleLogger * pLogger = NULL );
		virtual ~HttpHelper();

		int HttpGet( const std::string & url, std::string * pResp, 
				std::string * pContentType, std::string * pHeader, 
				int iConnTimeOut, int iTimeOut );

		int HttpPost( const std::string & url, const std::string data, 
				std::string * pResp, std::string * pContentType, 
				std::string * pHeader, int iConnTimeOut, int iTimeOut );

		int HttpPostMultiPart( const std::string & url, 
				const std::vector<UploadFile> & fileList, 
				std::string * pResp, std::string * pContentType,
				std::string * pHeader,int iConnTimeOut, int iTimeOut );

	private:
		static size_t BufferAppend(void *ptr, size_t size, size_t nmemb, 
				void *stream);
		static mpcomm::Errcode GetErrcode( CURLcode code );
		SimpleLogger * m_pLogger;

};

#endif 
////_MP_CLIENT_HTTP_HELPER_H_
