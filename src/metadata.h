#pragma once

#ifndef _MP_CLIENT_META_DATA_H_
#define _MP_CLIENT_META_DATA_H_

namespace mpcomm
{
	enum HTTP_TYPE {
		HTTP_GET    		= 1,
		HTTP_POST   		= 2,
		HTTP_POST_MULTIPART	= 3,
	};

	enum PARAM_TYPE {
		PARAM_TYPE_INT      = 1,
		PARAM_TYPE_UINT		= 2,
		PARAM_TYPE_STRING   = 3,
		PARAM_TYPE_MESSAGE	= 4,
		PARAM_TYPE_MAX      = 5,
	};

	enum PARAM_POSITION {
		AT_QUERY 		= 1,
		AT_POST_JSON	= 2,
		AT_POST			= 3,
		POSITION_MAX	= 4,
	};

	struct ParamItem {
		const char * name;
		PARAM_TYPE type;
		PARAM_POSITION position;
	};

	struct Service {
		const char * name;
		HTTP_TYPE method;
		const char * directory;
		const ParamItem * params;
	};

	extern const char * HTTP;
	extern const char * HTTPS;
	extern const char * DEFUALT_HOST;
	extern const char * TEST_DIR;
	extern const char * VERSION;

	extern Service * TokenService;
	extern Service * GetCallbackIPService;
	extern Service * CreateMenuService;
	extern Service * GetMenuService;
	extern Service * DeleteMenuService;
	extern Service * AddConditionalMenuService;
	extern Service * DelConditionalMenuService;
	extern Service * TryMatchMenuService;
	extern Service * GetCurrentSelfMenuInfoService;
	extern Service * SendCustomMessageService;
	extern Service * UploadImgService;
	extern Service * UploadNewsService;
	extern Service * MassSendAllService;
	extern Service * MassSendService;
	extern Service * MassDeleteService;
	extern Service * MassPreviewService;
	extern Service * MassGetService;
	extern Service * SetIndustryService;
	extern Service * GetIndustryService;
	extern Service * AddTemplateService;
	extern Service * GetAllPrivateTemplateService;
	extern Service * DelPrivateTemplateService;
	extern Service * SendTemplateMessageService;
	extern Service * GetCurrentAutoReplyInfoService;
	extern Service * UploadMediaService;
	extern Service * GetMediaService;
	extern Service * AddNewsMaterialService;
	extern Service * AddMaterialService;
	extern Service * GetMaterialService;
	extern Service * DelMaterialService;
	extern Service * UpdateNewsService;
	extern Service * GetMaterialCountService;
	extern Service * BatchGetMaterialService;
	extern Service * CreateTagsService;
	extern Service * GetTagsService;
	extern Service * UpdateTagsService;
	extern Service * DeleteTagsService;
	extern Service * GetTagUserService;
	extern Service * BatchTaggingMembersService;
	extern Service * BatchUnTaggingMembersService;
	extern Service * GetTagsIdListService;
	extern Service * UpdateUserInfoRemarkService;
	extern Service * UserInfoService;
	extern Service * BatchGetUserInfoService;
	extern Service * GetUserService;
};

#endif
////_MP_CLIENT_META_DATA_H_
