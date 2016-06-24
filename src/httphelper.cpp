#include "httphelper.h"
#include "simplelogger.h"
#include "curl/curl.h"

using namespace std;
using namespace mpcomm;


HttpHelper::HttpHelper( SimpleLogger * pLogger ):m_pLogger(pLogger){
}

HttpHelper::~HttpHelper(){
}

size_t HttpHelper::BufferAppend(void *ptr, size_t size, 
		size_t nmemb, void *stream){
	size_t len = size * nmemb;
	((string *)stream)->append( (const char *)ptr, len );
	return len;
}

mpcomm::Errcode HttpHelper::GetErrcode( CURLcode code ){
	switch (code){
		case CURLE_FAILED_INIT:
			return ErrCurlInit;
		case CURLE_COULDNT_CONNECT:
			return ErrCurlFailConn;
		case CURLE_REMOTE_ACCESS_DENIED:
			return ErrCurlAccessDenied;
		case CURLE_PARTIAL_FILE:
			return ErrCurlPartialFile;
		case CURLE_OPERATION_TIMEDOUT:
			return ErrCurlTimeout;
		default:
			return ErrLibcurl;
	}
}

int HttpHelper::HttpGet( const std::string & url, std::string *
		pResp, std::string * pContentType, std::string * pHeader, 
		int iConnTimeOut, int iTimeOut ){
	CURL *curl;
	CURLcode res;
	long httpCode = 0;
	curl = curl_easy_init();
	if(!curl) {
		if (m_pLogger) SIMPLELOGERR(*m_pLogger,
				"curl_easy_init failed");
		return ErrLibcurl;
	}
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
#if 0
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 3);
	if ( iConnTimeOut ){
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, iConnTimeOut);
	}
	if ( iTimeOut ){
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, iTimeOut);
	}
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, BufferAppend);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, pResp);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, pHeader );
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		if (m_pLogger) SIMPLELOGERR(*m_pLogger,
				"curl_easy_perform[%s]", curl_easy_strerror(res));
		curl_easy_cleanup(curl);
		return GetErrcode(res);
	}
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
	char * contentType = NULL;
	res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &contentType);
	if ( res != CURLE_OK ){
		if (m_pLogger) SIMPLELOGERR(*m_pLogger,
				"curl_easy_perform[%s]", curl_easy_strerror(res));
		curl_easy_cleanup(curl);
		return GetErrcode(res);
	}
	if ( NULL != contentType ) pContentType->assign(contentType);
	curl_easy_cleanup(curl);
	if ( 200==httpCode ) return 0;
	if (m_pLogger) SIMPLELOGERR(*m_pLogger,
			"http code[%ld]", httpCode );
	return httpCode;
}

int HttpHelper::HttpPost( const std::string & url, const
		std::string data, std::string * pResp, std::string * pContentType,
		std::string * pHeader, int iConnTimeOut, int iTimeOut ){
	CURL *curl;
	CURLcode res;
	long httpCode = 0;
	curl = curl_easy_init();
	if(!curl) {
		if (m_pLogger) SIMPLELOGERR(*m_pLogger,
				"curl_easy_init failed");
		return ErrLibcurl;
	}
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
#if 0
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
	if ( iConnTimeOut ){
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, iConnTimeOut);
	}
	if ( iTimeOut ){
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, iTimeOut);
	}
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, BufferAppend);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, pResp);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, pHeader );
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		if (m_pLogger) SIMPLELOGERR(*m_pLogger,
				"curl_easy_perform[%s]", curl_easy_strerror(res));
		curl_easy_cleanup(curl);
		return GetErrcode(res);
	}
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
	char * contentType = NULL;
	res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &contentType);
	if ( res != CURLE_OK ){
		if (m_pLogger) SIMPLELOGERR(*m_pLogger,
				"curl_easy_perform[%s]", curl_easy_strerror(res));
		curl_easy_cleanup(curl);
		return GetErrcode(res);
	}
	if ( NULL != contentType ) pContentType->assign(contentType);
	curl_easy_cleanup(curl);
	if ( 200==httpCode ) return 0;
	if (m_pLogger) SIMPLELOGERR(*m_pLogger,
			"http code[%ld]", httpCode );
	return httpCode;
}

int HttpHelper::HttpPostMultiPart( const std::string & url, 
		const std::vector<UploadFile> & fileList, 
		std::string * pResp, std::string * pContentType,
		std::string * pHeader,int iConnTimeOut, int iTimeOut){
	CURL *curl;
	CURLcode res;
	long httpCode = 0;
	curl = curl_easy_init();
	if(!curl) {
		if (m_pLogger) SIMPLELOGERR(*m_pLogger,
				"curl_easy_init failed");
		return ErrLibcurl;
	}
	curl_httppost *post=NULL;
	curl_httppost *last=NULL;
	for ( size_t i = 0; i < fileList.size(); ++ i ){
		curl_formadd( &post,&last,
				CURLFORM_COPYNAME, fileList[i].formId.c_str(),
				CURLFORM_BUFFER,fileList[i].fileName.c_str(),
				CURLFORM_BUFFERPTR,fileList[i].pBuffer->c_str(),
				CURLFORM_BUFFERLENGTH,fileList[i].pBuffer->size(),
				CURLFORM_END );
	}
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
#if 0
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
	if ( iConnTimeOut){
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, iConnTimeOut);
	}
	if ( iTimeOut ){
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, iTimeOut);
	}
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, post );
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, BufferAppend);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, pResp);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, pHeader );
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	res = curl_easy_perform(curl);
	curl_formfree(post);
	if (res != CURLE_OK) {
		if (m_pLogger) SIMPLELOGERR(*m_pLogger,
				"curl_easy_perform[%s]", curl_easy_strerror(res));
		curl_easy_cleanup(curl);
		return GetErrcode(res);
	}
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
	char * contentType = NULL;
	res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &contentType);
	if ( res != CURLE_OK ){
		if (m_pLogger) SIMPLELOGERR(*m_pLogger,
				"curl_easy_perform[%s]", curl_easy_strerror(res));
		curl_easy_cleanup(curl);
		return GetErrcode(res);
	}
	if ( NULL != contentType ) pContentType->assign(contentType);
	curl_easy_cleanup(curl);
	if ( 200==httpCode ) return 0;
	if (m_pLogger) SIMPLELOGERR(*m_pLogger,
			"http code[%ld]", httpCode );
	return httpCode;
}

