#include "mpclient.h"

#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "metadata.h"
#include "jsonhelper.h"
#include "httphelper.h"
#include "simplelogger.h"

#include <google/protobuf/descriptor.h>
#include "curl/curl.h"

#define USE_OPENSSL

#ifdef USE_OPENSSL
#include <openssl/crypto.h> 
#endif

#if (defined _WIN32) && (defined GetMessage)
//define GetMessage GetMessageA in win32
#undef GetMessage
#endif

using namespace mpcomm;

IMutex * MpClient::g_mutex = NULL;

IMutex::IMutex(){}
IMutex::~IMutex(){}

int IMutex::GetLockNum(){
#if (defined USE_OPENSSL) && (defined OPENSSL_THREADS)
	return CRYPTO_num_locks();
#endif
	return 0;
}

MpClient::MpClient():m_host(DEFUALT_HOST),m_iTimeout(0),m_iConnTimeout(0){
	m_pLogger = new SimpleLogger();
	m_pJsonHelper = new JsonHelper(m_pLogger);
	m_pHttpHelper = new HttpHelper(m_pLogger);
}

MpClient::~MpClient(){
	if ( m_pHttpHelper ){
		delete m_pHttpHelper;
		m_pHttpHelper = NULL;
	}
	if ( m_pJsonHelper ){
		delete m_pJsonHelper;
		m_pJsonHelper = NULL;
	}
	if ( m_pLogger ){
		delete m_pLogger;
		m_pLogger = NULL;
	}
}

int MpClient::SetLogConfig( LogLevel level, 
		int loglimit, const char * dir ){
	m_pLogger->SetBehaviour(__func__);
	int ret = m_pLogger->SetLogDir(dir);
	if ( ret ) {
		return ret;
	}
	ret = m_pLogger->SetLogLimit( loglimit );
	if ( ret ) {
		return ret;
	}
	ret = m_pLogger->SetLevel(level);
	if ( ret ) {
		return ret;
	}
	return 0;
}

int MpClient::SetHost( const char * host ){
	m_pLogger->SetBehaviour(__func__);
	if ( NULL == host ){
		return ErrInvalidArg;
	}
	m_host.assign(host);
	return 0;
}

int MpClient::SetTimeOut( int connTimeout, int timeout ){
	m_pLogger->SetBehaviour(__func__);
	m_iConnTimeout = connTimeout;
	m_iTimeout = timeout;
	return 0;
}

int MpClient::CheckNetwork(){
	std::string url = std::string(HTTP) + m_host + TEST_DIR;
	std::string resp;
	std::string header;
	std::string contentType;
	int ret = m_pHttpHelper->HttpGet( url, &resp, &contentType, &header, 
			m_iConnTimeout, m_iTimeout); 
	if ( ret ){
		SIMPLELOGERR(*m_pLogger,"httpGet ret[%d]", ret);
		return ret;
	}
	return 0;
}

int MpClient::Token( const mpcomm::TokenReq & req, 
		mpcomm::TokenResp * pResp ){
	return HandleRequest( TokenService, req, pResp );
}

int MpClient::GetCallbackIP( const mpcomm::GetCallbackIPReq & req, 
		mpcomm::GetCallbackIPResp * pResp ){
	return HandleRequest( GetCallbackIPService, req, pResp );
}

int MpClient::CreateMenu( const mpcomm::CreateMenuReq & req, 
		mpcomm::CreateMenuResp * pResp ){
	return HandleRequest( CreateMenuService, req, pResp );
}

int MpClient::GetMenu( const mpcomm::GetMenuReq & req, 
		mpcomm::GetMenuResp * pResp ){
	return HandleRequest( GetMenuService, req, pResp );
}

int MpClient::DeleteMenu( const mpcomm::DeleteMenuReq & req, 
		mpcomm::DeleteMenuResp * pResp ){
	return HandleRequest( DeleteMenuService, req, pResp );
}

int MpClient::AddConditionalMenu( 
		const mpcomm::AddConditionalMenuReq & req, 
		mpcomm::AddConditionalMenuResp * pResp ){
	return HandleRequest( AddConditionalMenuService, req, pResp );
}

int MpClient::DelConditionalMenu( 
		const mpcomm::DelConditionalMenuReq & req, 
		mpcomm::DelConditionalMenuResp * pResp ){
	return HandleRequest( DelConditionalMenuService, req, pResp );
}

int MpClient::TryMatchMenu( const mpcomm::TryMatchMenuReq & req, 
		mpcomm::TryMatchMenuResp * pResp ){
	return HandleRequest( TryMatchMenuService, req, pResp );
}

int MpClient::GetCurrentSelfMenuInfo( 
		const mpcomm::GetCurrentSelfMenuInfoReq & req, 
		mpcomm::GetCurrentSelfMenuInfoResp * pResp ){
	return HandleRequest( GetCurrentSelfMenuInfoService, req, pResp );
}

int MpClient::SendCustomMessage( 
		const mpcomm::SendCustomMessageReq & req, 
		mpcomm::SendCustomMessageResp * pResp ){
	return HandleRequest( SendCustomMessageService, req, pResp );
}

int MpClient::UploadImg( const mpcomm::UploadImgReq & req, 
		mpcomm::UploadImgResp * pResp ){
	return HandleRequest( UploadImgService, req, pResp );
}

int MpClient::UploadNews( const mpcomm::UploadNewsReq & req, 
		mpcomm::UploadNewsResp * pResp ){
	return HandleRequest( UploadNewsService, req, pResp );
}

int MpClient::MassSendAll( const mpcomm::MassSendAllReq & req, 
		mpcomm::MassSendAllResp * pResp ){
	return HandleRequest( MassSendAllService, req, pResp );
}

int MpClient::MassSend( const mpcomm::MassSendReq & req, 
		mpcomm::MassSendResp * pResp ){
	return HandleRequest( MassSendService, req, pResp );
}

int MpClient::MassDelete( const mpcomm::MassDeleteReq & req, 
		mpcomm::MassDeleteResp * pResp ){
	return HandleRequest( MassDeleteService, req, pResp );
}

int MpClient::MassPreview( const mpcomm::MassPreviewReq & req, 
		mpcomm::MassPreviewResp * pResp ){
	return HandleRequest( MassPreviewService, req, pResp );
}

int MpClient::MassGet( const mpcomm::MassGetReq & req, 
		mpcomm::MassGetResp * pResp ){
	return HandleRequest( MassGetService, req, pResp );
}

int MpClient::SetIndustry( const mpcomm::SetIndustryReq & req, 
		mpcomm::SetIndustryResp * pResp ){
	return HandleRequest( SetIndustryService, req, pResp );
}

int MpClient::GetIndustry( const mpcomm::GetIndustryReq & req, 
		mpcomm::GetIndustryResp * pResp ){
	return HandleRequest( GetIndustryService, req, pResp );
}

int MpClient::AddTemplate( const mpcomm::AddTemplateReq & req, 
		mpcomm::AddTemplateResp * pResp ){
	return HandleRequest( AddTemplateService, req, pResp );
}

int MpClient::GetAllPrivateTemplate( 
		const mpcomm::GetAllPrivateTemplateReq & req, 
		mpcomm::GetAllPrivateTemplateResp * pResp ){
	return HandleRequest( GetAllPrivateTemplateService, req, pResp );
}

int MpClient::DelPrivateTemplate( 
		const mpcomm::DelPrivateTemplateReq & req, 
		mpcomm::DelPrivateTemplateResp * pResp ){
	return HandleRequest( DelPrivateTemplateService, req, pResp );
}

int MpClient::SendTemplateMessage( 
		const mpcomm::SendTemplateMessageReq & req, 
		mpcomm::SendTemplateMessageResp * pResp ){
	return HandleRequest( SendTemplateMessageService, req, pResp );
}

int MpClient::GetCurrentAutoReplyInfo( 
		const mpcomm::GetCurrentAutoReplyInfoReq & req, 
		mpcomm::GetCurrentAutoReplyInfoResp * pResp ){
	return HandleRequest( GetCurrentAutoReplyInfoService, req, pResp );
}

int MpClient::UploadMedia( const mpcomm::UploadMediaReq & req, 
		mpcomm::UploadMediaResp * pResp ){
	return HandleRequest( UploadMediaService, req, pResp );
}

int MpClient::GetMedia( const mpcomm::GetMediaReq & req, 
		mpcomm::GetMediaResp * pResp ){
	return HandleRequest( GetMediaService, req, pResp, 
			pResp->mutable_media() );
}

int MpClient::AddNewsMaterial( const mpcomm::AddNewsMaterialReq & req, 
		mpcomm::AddNewsMaterialResp * pResp ){
	return HandleRequest( AddNewsMaterialService, req, pResp );
}

int MpClient::AddMaterial( const mpcomm::AddMaterialReq & req, 
		mpcomm::AddMaterialResp * pResp ){
	return HandleRequest( AddMaterialService, req, pResp);
}

int MpClient::GetMaterial( const mpcomm::GetMaterialReq & req, 
		mpcomm::GetMaterialResp * pResp ){
	return HandleRequest( GetMaterialService, req, pResp, 
			pResp->mutable_media());
}

int MpClient::DelMaterial( const mpcomm::DelMaterialReq & req, 
		mpcomm::DelMaterialResp * pResp ){
	return HandleRequest( DelMaterialService, req, pResp );
}

int MpClient::UpdateNews( const mpcomm::UpdateNewsReq & req, 
		mpcomm::UpdateNewsResp * pResp ){
	return HandleRequest( UpdateNewsService, req, pResp );
}

int MpClient::GetMaterialCount( const mpcomm::GetMaterialCountReq & req, 
		mpcomm::GetMaterialCountResp * pResp ){
	return HandleRequest( GetMaterialCountService, req, pResp );
}

int MpClient::BatchGetMaterial( const mpcomm::BatchGetMaterialReq & req, 
		mpcomm::BatchGetMaterialResp * pResp ){
	return HandleRequest( BatchGetMaterialService, req, pResp );
}

int MpClient::CreateTags( const mpcomm::CreateTagsReq & req, 
		mpcomm::CreateTagsResp * pResp ){
	return HandleRequest( CreateTagsService, req, pResp );
}

int MpClient::GetTags( const mpcomm::GetTagsReq & req, 
		mpcomm::GetTagsResp * pResp ){
	return HandleRequest( GetTagsService, req, pResp );
}

int MpClient::UpdateTags( const mpcomm::UpdateTagsReq & req, 
		mpcomm::UpdateTagsResp * pResp ){
	return HandleRequest( UpdateTagsService, req, pResp );
}

int MpClient::DeleteTags( const mpcomm::DeleteTagsReq & req, 
		mpcomm::DeleteTagsResp * pResp ){
	return HandleRequest( DeleteTagsService, req, pResp );
}

int MpClient::GetTagUser( const mpcomm::GetTagUserReq & req, 
		mpcomm::GetTagUserResp * pResp ){
	return HandleRequest( GetTagUserService, req, pResp );
}

int MpClient::BatchTaggingMembers( 
		const mpcomm::BatchTaggingMembersReq & req, 
		mpcomm::BatchTaggingMembersResp * pResp ){
	return HandleRequest( BatchTaggingMembersService, req, pResp );
}

int MpClient::BatchUnTaggingMembers( 
		const mpcomm::BatchUnTaggingMembersReq & req, 
		mpcomm::BatchUnTaggingMembersResp * pResp ){
	return HandleRequest( BatchUnTaggingMembersService, req, pResp );
}

int MpClient::GetTagsIdList( const mpcomm::GetTagsIdListReq & req, 
		mpcomm::GetTagsIdListResp * pResp ){
	return HandleRequest( GetTagsIdListService, req, pResp );
}

int MpClient::UpdateUserInfoRemark( 
		const mpcomm::UpdateUserInfoRemarkReq & req, 
		mpcomm::UpdateUserInfoRemarkResp * pResp ){
	return HandleRequest( UpdateUserInfoRemarkService, req, pResp );
}

int MpClient::UserInfo( const mpcomm::UserInfoReq & req, 
		mpcomm::UserInfoResp * pResp ){
	return HandleRequest( UserInfoService, req, pResp );
}

int MpClient::BatchGetUserInfo( const mpcomm::BatchGetUserInfoReq & req, 
		mpcomm::BatchGetUserInfoResp * pResp ){
	return HandleRequest( BatchGetUserInfoService, req, pResp );
}

int MpClient::GetUser( const mpcomm::GetUserReq & req, 
		mpcomm::GetUserResp * pResp ){
	return HandleRequest( GetUserService, req, pResp );
}

int MpClient::GeneralGet( const std::string & url, std::string * pResp,
				bool * pIsRespJson, std::string * pFilename ){
	m_pLogger->SetBehaviour(__func__);
	return HandleGeneralGet(url,
			pResp,pIsRespJson,pFilename);
}

int MpClient::GeneralPostJson( const std::string & url, 
		const std::string & postdata, 
		bool checkInputJson, std::string * pResp, bool *
		pIsRespJson,std::string * pFilename ){
	m_pLogger->SetBehaviour(__func__);
	return HandleGeneralPostJson(url,postdata,checkInputJson,
			pResp, pIsRespJson, pFilename );
}

int MpClient::GeneralMultiPartPost( const std::string & url, const
		std::vector<UploadFile> & fileList,
		std::string * pResp, bool * pIsRespJson, 
		std::string * pFilename ){
	m_pLogger->SetBehaviour(__func__);
	return HandleMultiPartPost(url,fileList, 
			pResp, pIsRespJson, pFilename );
}

int MpClient::HandleRequest( const Service * pService, 
		const google::protobuf::Message & req, 
		google::protobuf::Message * pResp, 
		mpcomm::MediaFile * downloadMedia ){
	m_pLogger->SetBehaviour(pService->name);
	SIMPLELOGDEBUG(*m_pLogger,
			"Process api[%s] dir[%s] http_type[%d] ", 
			pService->name, pService->directory, pService->method);
	std::string url;
	std::string postdata;
	std::string response;
	std::string filename;
	std::set<std::string> fieldToSkip;
	std::vector<UploadFile> fileList;
	std::vector<std::string> jsonContainer;
	const google::protobuf::Descriptor * pDescriptor = req.GetDescriptor();
	const google::protobuf::Reflection * pReflection = req.GetReflection();
	if ( !pDescriptor ){
		 SIMPLELOGERR(*m_pLogger,
				"[PROTOBUF] Message.GetDescriptor fail");
		 return ErrProtobuf;
	}
	url.append("?sdkversion=").append(VERSION);
	for ( const ParamItem * item = pService->params; item->name; ++item ) {
		 SIMPLELOGDEBUG(*m_pLogger,
				"Process message field[%s] type[%d] position[%d}", 
				item->name, item->type, item->position);
		const google::protobuf::FieldDescriptor * pFieldDescriptor 
			= pDescriptor->FindFieldByName( item->name );
		if ( !pFieldDescriptor ){
			 SIMPLELOGERR(*m_pLogger,
					"Deescriptor.FindFieldByName[%s] fail", item->name);
			 return ErrProtobuf;
		}
		if ( AT_QUERY == item->position ) {
			std::string param;
			std::stringstream stream;
			switch( item->type ) {
				case PARAM_TYPE_UINT:
					stream << pReflection->GetUInt64( req, pFieldDescriptor );
					stream >> param;
					break;

				case PARAM_TYPE_INT:
					stream << pReflection->GetInt64( req, pFieldDescriptor );
					stream >> param;
					break;

				case PARAM_TYPE_STRING:
					param = pReflection->GetString( req, pFieldDescriptor );
					break;

				default:
					 SIMPLELOGERR(*m_pLogger,
							"Process field[%s] is unsupported type", item->name);
					return ErrInvalidMataData;
			}
			if ( url.empty() ){
				url.append("?").append(item->name).append("=").append(param);
			}else{
				url.append("&").append(item->name).append("=").append(param);
			}
			
		}
		if ( AT_POST_JSON != item->position ){
			fieldToSkip.insert(item->name);
		}
		if ( HTTP_POST_MULTIPART == pService->method ){
			if ( AT_POST_JSON == item->position && 
					PARAM_TYPE_MESSAGE == item->type ){
				SIMPLELOGDEBUG(*m_pLogger, "Process field[%s]" 
						"is a message at form, parse to json", item->name);
				std::string json;
				m_pJsonHelper->pb2json
					( pReflection->GetMessage(req, pFieldDescriptor), &json );
				jsonContainer.push_back(std::string());
				jsonContainer.back().swap(json);
				UploadFile file;
				file.formId = item->name;
				file.pBuffer = &(jsonContainer.back());
				fileList.push_back( file );
			}
			else if ( AT_POST == item->position && 
					PARAM_TYPE_MESSAGE == item->type ){
				const google::protobuf::Message & subMessage = 
					pReflection->GetMessage(req, pFieldDescriptor);
				if ( subMessage.GetTypeName() == "mpcomm.MediaFile"){
					SIMPLELOGDEBUG(*m_pLogger,"Process field[%s] is a MediaFile",
							item->name);
					UploadFile file;
					file.formId = item->name;
					const google::protobuf::Descriptor * subDescriptor = 
						subMessage.GetDescriptor();
					const google::protobuf::FieldDescriptor * subField =
						subDescriptor->FindFieldByName("file_name");
					if ( !subField ) {
						SIMPLELOGERR(*m_pLogger,"Process field[%s]" 
								"MediaFile.file_name not found", item->name );
						continue;
					}
					const google::protobuf::Reflection * pSubReflection
						= subMessage.GetReflection();
					std::string scratch;
					const std::string & fileName = 
						pSubReflection->GetStringReference( subMessage,
								subField, &scratch );
					file.fileName = fileName;
					subField = subDescriptor->FindFieldByName("buffer");
					if ( !subField ) {
						SIMPLELOGERR(*m_pLogger,"Process field[%s]"
								"MediaFile.buffer not found", item->name );
						continue;
					}
					const std::string & buffer =
						pSubReflection->GetStringReference( subMessage,
								subField, &scratch );
					file.pBuffer = & buffer;
					fileList.push_back( file );
					SIMPLELOGDEBUG(*m_pLogger,"Process field[%s] find file",
							item->name);
				}

			}
		}
	}
	url = std::string(HTTPS) + m_host + pService->directory + url;
	int ret = 0;
	bool isJson = true;
	switch (pService->method){
		case HTTP_GET:
			ret = HandleGeneralGet( url, &response, &isJson, &filename );
			break;
		case HTTP_POST:
			m_pJsonHelper->pb2json(req, &postdata, &fieldToSkip);
			ret = HandleGeneralPostJson( url, postdata, false, 
					&response, &isJson, &filename );
			break;
		case HTTP_POST_MULTIPART:
			ret = HandleMultiPartPost( url, fileList, &response, &isJson, 
					&filename );
			break;
		default:
			SIMPLELOGERR(*m_pLogger,"UNKNOWN http method");
			return ErrInvalidMataData;
	}
	if ( ret ){
		SIMPLELOGERR(*m_pLogger,"MPClient Request url[%s] postdata[%s] ret[%d]", 
				url.c_str(), postdata.c_str(), ret );
		return ret;
	}
	if ( !response.empty() ){
		if ( isJson ){
			SIMPLELOGDEBUG(*m_pLogger,"response is json data, parse to pb");
			ret = m_pJsonHelper->json2pb( response, pResp );
			if ( ret ){
				SIMPLELOGERR(*m_pLogger,"response is json data," 
						"parse to pb fail ret[%d]", ret );
				return ret;
			}
			long long errcode = GetRespErrcode( *pResp );
			if ( errcode ){
				SIMPLELOGWARN(*m_pLogger,"resp[%s] errcode is not 0",
						response.c_str() );
			}
		}else{
			if ( downloadMedia ){
				SIMPLELOGDEBUG(*m_pLogger,"response is binary data");
				downloadMedia->mutable_buffer()->swap( response );
				downloadMedia->mutable_file_name()->swap( filename );
			}else{
				SIMPLELOGERR(*m_pLogger,"response is binary data, need to save");
			}
		}
	}else{
		SIMPLELOGERR(*m_pLogger, "response is empty" ); 
	}
	return 0;
}

int MpClient::HandleGeneralGet( const std::string & url, 
		std::string * pResp, bool * pIsRespJson,
		std::string * pFilename ){
	SIMPLELOGDEBUG(*m_pLogger,"url[%s]", url.c_str() );
	std::string contentType;
	std::string header;
	int ret = m_pHttpHelper->HttpGet( url, pResp, &contentType, &header,
			m_iConnTimeout, m_iTimeout );
	if ( ret ){
		SIMPLELOGERR(*m_pLogger,"HttpGet ret[%d]", ret );
		return ret;
	}
	*pIsRespJson = IsResponseJson( contentType );
	if (pFilename) GetFileName(header,pFilename);
	SIMPLELOGDEBUG(*m_pLogger,"resp:[%s]", pResp->c_str() );
	SIMPLELOGDEBUG(*m_pLogger,"json[%d] content-type[%s]", 
			(int)(*pIsRespJson), contentType.c_str() );
	SIMPLELOGDEBUG(*m_pLogger,"header[%s]", header.c_str() );
	return 0;
}


int MpClient::HandleGeneralPostJson( const std::string & url, 
		const std::string & postdata, bool checkInputJson, 
		std::string * pResp, bool * pIsRespJson,
		std::string * pFilename){
	SIMPLELOGDEBUG(*m_pLogger,"url[%s]", url.c_str() );
	SIMPLELOGDEBUG(*m_pLogger,"postdata[%s]", postdata.c_str() );
	std::string contentType;
	std::string header;
	if ( checkInputJson ){
		std::string errmsg;
		bool res = m_pJsonHelper->validateJson(postdata, &errmsg);
		if (!res){
			SIMPLELOGERR(*m_pLogger,"invalid json[%s] errmsg[%s]",
					postdata.c_str(), errmsg.c_str() );
			return ErrInvalidJsonStr;
		}
	}
	int ret = m_pHttpHelper->HttpPost( url, postdata, pResp,
			&contentType, &header, m_iConnTimeout, m_iTimeout );
	if ( ret ){
		SIMPLELOGERR(*m_pLogger,"HttpPost ret[%d]", ret );
		return ret;
	}
	*pIsRespJson = IsResponseJson( contentType );
	if (pFilename) GetFileName(header,pFilename);
	SIMPLELOGDEBUG(*m_pLogger,"resp[%s]", pResp->c_str() );
	SIMPLELOGDEBUG(*m_pLogger,"json[%d] content-type[%s]", 
			(int)(*pIsRespJson), contentType.c_str() );
	SIMPLELOGDEBUG(*m_pLogger,"header[%s]", header.c_str() );
	return 0;
}

int MpClient::HandleMultiPartPost( const std::string & url, const
		std::vector<UploadFile> & fileList,
		std::string * pResp, bool * pIsRespJson,
		std::string * pFilename ){
	SIMPLELOGDEBUG(*m_pLogger,"url[%s]", url.c_str() );
	std::string contentType;
	std::string header;
	int ret = m_pHttpHelper->HttpPostMultiPart( url, fileList, pResp,
			&contentType, &header, m_iConnTimeout, m_iTimeout );
	if ( ret ){
		SIMPLELOGERR(*m_pLogger,"HttpPostMultiPart ret[%d]", ret );
		return ret;
	}
	*pIsRespJson = IsResponseJson( contentType );
	if (pFilename) GetFileName(header,pFilename);
	SIMPLELOGDEBUG(*m_pLogger,"resp[%s]", pResp->c_str() );
	SIMPLELOGDEBUG(*m_pLogger,"json[%d] content-type[%s]", 
			(int)(*pIsRespJson), contentType.c_str() );
	SIMPLELOGDEBUG(*m_pLogger,"header[%s]", header.c_str() );
	return 0;
}

bool MpClient::IsResponseJson( const std::string & contentType ){
	std::string lower;;
	lower.resize(contentType.size());
	std::transform( contentType.begin(), contentType.end(), 
			lower.begin(), ::tolower);
	if ( lower.find("json") != std::string::npos || 
			lower.find("text") != std::string::npos ){
		return true;
	}
	else{
		return false;
	}
}


bool MpClient::GetFileName( const std::string & httpHeader, 
		std::string * pFilename){
	std::string lower;;
	lower.resize(httpHeader.size());
	std::transform( httpHeader.begin(), httpHeader.end(), 
			lower.begin(), ::tolower );
	size_t pos = lower.find("filename");
	pos += strlen("filename");
	if ( std::string::npos == pos ){
		return false;
	}
	size_t quo1 = 0;
	size_t quo2 = 0;
	//filename="${filename}"
	while ( pos < lower.size() && lower[pos] ==' ') pos++;
	if ( pos >= lower.size() || '=' != lower[pos] ){
		return false;
	}
	while ( ++pos < lower.size() && lower[pos] ==' '){}
	if ( pos >= lower.size() || '"' != lower[pos] ){
		return false;
	}
	quo1 = pos;
	while ( ++pos < lower.size() && lower[pos] != '"'){}
	if ( pos >= lower.size() || '"' != lower[pos] ){
		return false;
	}
	quo2 = pos;
	if ( quo2 - quo1 - 1 > 0 ){
		*pFilename = httpHeader.substr(quo1+1, quo2-quo1-1);
		return true;
	}
	return false;
}

#if (defined USE_OPENSSL) && (defined OPENSSL_THREADS)

void LockCallBack(int mode, int type, const char * file, int line){
	if ( !MpClient::g_mutex ) return;
	if ( mode& CRYPTO_LOCK ){
		MpClient::g_mutex->Lock(type);
	}else{
		MpClient::g_mutex->UnLock(type);
	} 
}
#define OPENSSL_LOCK_CALLBACK CRYPTO_set_locking_callback

#if OPENSSL_VERSION_NUMBER > OPENSSL_VERSION_1_0_0

void GetTid(CRYPTO_THREADID * id){
	if ( MpClient::g_mutex ) 
		id->val = MpClient::g_mutex->GetThreadId();
	else id->val = 0;
}
#define OPENSSL_TID_CALLBACK CRYPTO_THREADID_set_callback

////end: version > 1.0.0
#else

unsigned long GetTid(){
	if ( g_mutex )
		return g_mutex->GetThreadId();
	return 0;
}
#define OPENSSL_TID_CALLBACK CRYPTO_set_id_callback

#endif ////end: version < 1.0.0

#endif ////openssl && threads


int MpClient::GlobalInit(IMutex * mutex){
	if (mutex){
		g_mutex = mutex;
		OPENSSL_TID_CALLBACK(GetTid);
		OPENSSL_LOCK_CALLBACK(LockCallBack);
	}
	curl_global_init(CURL_GLOBAL_ALL);
	return 0;
}

int MpClient::ShutDown(){
	curl_global_cleanup();
	google::protobuf::ShutdownProtobufLibrary();
	return 0;
}

long long MpClient::GetRespErrcode( 
		const google::protobuf::Message & resp ){
	const google::protobuf::Descriptor * pDescriptor = 
		resp.GetDescriptor();
	const google::protobuf::Reflection * pReflection = 
		resp.GetReflection();
	if ( !pDescriptor ){
		if ( m_pLogger ) SIMPLELOGERR( *m_pLogger, 
				"Message.GetDescriptor fail" );
		return ErrProtobuf;
	}
	if ( !pReflection ){
		if ( m_pLogger ) SIMPLELOGERR( *m_pLogger, 
				"Message.GetReflection fail" );
		return ErrProtobuf;
	}
	const google::protobuf::FieldDescriptor * field = 
		pDescriptor->FindFieldByName("errcode");
	if ( !field ){
		if ( m_pLogger ) SIMPLELOGERR( *m_pLogger, 
				"Descriptor.FindFieldByName[errcode] fail");
		return ErrProtobuf;
	}
	return pReflection->GetInt64(resp, field);
}

