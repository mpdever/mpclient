#pragma once

#ifndef _MP_CLIENT_MP_CLIENT_H_
#define _MP_CLIENT_MP_CLIENT_H_

#include "mpmsg.pb.h"
#include "mpcomm.h"
#include "simplelogger.h"

#include <string>

namespace google{
	namespace protobuf{
		class Message;
	}
}

class JsonHelper;
class SimpleLogger;
class HttpHelper;
struct UploadFile;

namespace mpcomm {
	struct Service;

	class IMutex{
		MP_DISALLOW_COPY_AND_ASSIGN(IMutex);
		public:
		IMutex();
		virtual ~IMutex();
		static int GetLockNum();
		virtual unsigned long int GetThreadId() = 0;
		virtual int Lock( int id) = 0;
		virtual int UnLock( int id) = 0;
	};
}

class MpClient {
		MP_DISALLOW_COPY_AND_ASSIGN(MpClient);
	public:

		MpClient();
		virtual ~MpClient();

		static int GlobalInit( mpcomm::IMutex * mutex = NULL);
		static int ShutDown();

		int SetLogConfig( mpcomm::LogLevel level, int loglimit, 
				const char * dir );
		int SetHost( const char * host );
		int SetTimeOut( int connTimeout, int timeout );
		int CheckNetwork();

		int Token( const mpcomm::TokenReq & req, 
				mpcomm::TokenResp * pResp );
		int GetCallbackIP( const mpcomm::GetCallbackIPReq & req, 
				mpcomm::GetCallbackIPResp * pResp );

		int CreateMenu( const mpcomm::CreateMenuReq & req, 
				mpcomm::CreateMenuResp * pResp );
		int GetMenu( const mpcomm::GetMenuReq & req, 
				mpcomm::GetMenuResp * pResp );
		int DeleteMenu( const mpcomm::DeleteMenuReq & req, 
				mpcomm::DeleteMenuResp * pResp );
		int AddConditionalMenu( const mpcomm::AddConditionalMenuReq & req, 
				mpcomm::AddConditionalMenuResp * pResp );
		int DelConditionalMenu( const mpcomm::DelConditionalMenuReq & req, 
				mpcomm::DelConditionalMenuResp * pResp );
		int TryMatchMenu( const mpcomm::TryMatchMenuReq & req, 
				mpcomm::TryMatchMenuResp * pResp );
		int GetCurrentSelfMenuInfo( const mpcomm::GetCurrentSelfMenuInfoReq & req, 
				mpcomm::GetCurrentSelfMenuInfoResp * pResp );

		int SendCustomMessage( const mpcomm::SendCustomMessageReq & req, 
				mpcomm::SendCustomMessageResp * pResp );
		int UploadImg( const mpcomm::UploadImgReq & req, 
				mpcomm::UploadImgResp * pResp );
		int UploadNews( const mpcomm::UploadNewsReq & req, 
				mpcomm::UploadNewsResp * pResp );
		int MassSendAll( const mpcomm::MassSendAllReq & req, 
				mpcomm::MassSendAllResp * pResp );
		int MassSend( const mpcomm::MassSendReq & req, 
				mpcomm::MassSendResp * pResp );
		int MassDelete( const mpcomm::MassDeleteReq & req, 
				mpcomm::MassDeleteResp * pResp );
		int MassPreview( const mpcomm::MassPreviewReq & req, 
				mpcomm::MassPreviewResp * pResp );
		int MassGet( const mpcomm::MassGetReq & req, 
				mpcomm::MassGetResp * pResp );

		int SetIndustry( const mpcomm::SetIndustryReq & req, 
				mpcomm::SetIndustryResp * pResp );
		int GetIndustry( const mpcomm::GetIndustryReq & req, 
				mpcomm::GetIndustryResp * pResp );
		int AddTemplate( const mpcomm::AddTemplateReq & req, 
				mpcomm::AddTemplateResp * pResp );
		int GetAllPrivateTemplate( const mpcomm::GetAllPrivateTemplateReq & req, 
				mpcomm::GetAllPrivateTemplateResp * pResp );
		int DelPrivateTemplate( const mpcomm::DelPrivateTemplateReq & req, 
				mpcomm::DelPrivateTemplateResp * pResp );
		int SendTemplateMessage( const mpcomm::SendTemplateMessageReq & req, 
				mpcomm::SendTemplateMessageResp * pResp );
		int GetCurrentAutoReplyInfo( const mpcomm::GetCurrentAutoReplyInfoReq & req, 
				mpcomm::GetCurrentAutoReplyInfoResp * pResp );

		int UploadMedia( const mpcomm::UploadMediaReq & req, 
				mpcomm::UploadMediaResp * pResp );
		int GetMedia( const mpcomm::GetMediaReq & req, 
				mpcomm::GetMediaResp * pResp );
		int AddNewsMaterial( const mpcomm::AddNewsMaterialReq & req, 
				mpcomm::AddNewsMaterialResp * pResp );
		int AddMaterial( const mpcomm::AddMaterialReq & req, 
				mpcomm::AddMaterialResp * pResp );
		int GetMaterial( const mpcomm::GetMaterialReq & req, 
				mpcomm::GetMaterialResp * pResp );
		int DelMaterial( const mpcomm::DelMaterialReq & req, 
				mpcomm::DelMaterialResp * pResp );
		int UpdateNews( const mpcomm::UpdateNewsReq & req, 
				mpcomm::UpdateNewsResp * pResp );
		int GetMaterialCount( const mpcomm::GetMaterialCountReq & req, 
				mpcomm::GetMaterialCountResp * pResp );
		int BatchGetMaterial( const mpcomm::BatchGetMaterialReq & req, 
				mpcomm::BatchGetMaterialResp * pResp );

		int CreateTags( const mpcomm::CreateTagsReq & req, 
				mpcomm::CreateTagsResp * pResp );
		int GetTags( const mpcomm::GetTagsReq & req, 
				mpcomm::GetTagsResp * pResp );
		int UpdateTags( const mpcomm::UpdateTagsReq & req, 
				mpcomm::UpdateTagsResp * pResp );
		int DeleteTags( const mpcomm::DeleteTagsReq & req, 
				mpcomm::DeleteTagsResp * pResp );
		int GetTagUser( const mpcomm::GetTagUserReq & req, 
				mpcomm::GetTagUserResp * pResp );
		int BatchTaggingMembers( const mpcomm::BatchTaggingMembersReq &
				req, mpcomm::BatchTaggingMembersResp * pResp );
		int BatchUnTaggingMembers( const mpcomm::BatchUnTaggingMembersReq &
				req, mpcomm::BatchUnTaggingMembersResp * pResp );
		int GetTagsIdList( const mpcomm::GetTagsIdListReq & req,
				mpcomm::GetTagsIdListResp * pResp );
		int UpdateUserInfoRemark( const mpcomm::UpdateUserInfoRemarkReq & req, 
				mpcomm::UpdateUserInfoRemarkResp * pResp );
		int UserInfo( const mpcomm::UserInfoReq & req, 
				mpcomm::UserInfoResp * pResp );
		int BatchGetUserInfo( const mpcomm::BatchGetUserInfoReq & req, 
				mpcomm::BatchGetUserInfoResp * pResp );
		int GetUser( const mpcomm::GetUserReq & req, 
				mpcomm::GetUserResp * pResp );

		int GeneralGet( const std::string & url, std::string * pResp,
				bool * pIsRespJson, std::string * pFilename = NULL );

		int GeneralPostJson( const std::string & url, 
				const std::string & postdata, 
				bool checkInputJson, std::string * pResp, bool *
				pIsRespJson,std::string * pFilename = NULL );

		int GeneralMultiPartPost( const std::string & url, const
				std::vector<UploadFile> & fileList,
				std::string * pResp, bool * pIsRespJson, 
				std::string * pFilename = NULL );

	public:
		static mpcomm::IMutex * g_mutex;

	protected:
		int HandleRequest( const mpcomm::Service * pService, 
				const google::protobuf::Message & req, 
				google::protobuf::Message * pResp, 
				mpcomm::MediaFile * downloadMedia = NULL );

		int HandleGeneralGet( const std::string & url, std::string * pResp,
				bool * pIsRespJson, std::string * pFilename = NULL );

		int HandleGeneralPostJson( const std::string & url, 
				const std::string & postdata, 
				bool checkInputJson, std::string * pResp, bool *
				pIsRespJson,std::string * pFilename = NULL );

		int HandleMultiPartPost( const std::string & url, const
				std::vector<UploadFile> & fileList,
				std::string * pResp, bool * pIsRespJson, 
				std::string * pFilename = NULL );

		bool IsResponseJson( const std::string & contentType );
		bool GetFileName( const std::string & httpHeader, 
				std::string * pFilename);
		bool GetHint( const std::string & httpHeader );

		long long GetRespErrcode( const google::protobuf::Message & resp );

	private:
		std::string m_host;
		JsonHelper * m_pJsonHelper;
		SimpleLogger * m_pLogger;
		HttpHelper * m_pHttpHelper;
		int m_iTimeout;
		int m_iConnTimeout;
};

#endif
////_MP_CLIENT_MP_CLIENT_H_
