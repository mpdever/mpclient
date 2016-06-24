#include "metadata.h"
#include <cstdlib>

using namespace mpcomm;

#define PARAM(name,type,position) {name,type,position}
#define DEFINE_SERVICE(identifier,name,method,directory,...) \
	ParamItem identifier##Params [] = { __VA_ARGS__ };\
	Service identifier##Obj = {name,method,directory,identifier##Params};\
	Service * identifier##Service = &identifier##Obj;

#define PARAM_END PARAM(NULL,PARAM_TYPE_MAX,POSITION_MAX)
#define PARAM_ACCESS_TOKEN PARAM("access_token",PARAM_TYPE_STRING,AT_QUERY)
namespace mpcomm{

	const char * HTTP = "http://";
	const char * HTTPS = "https://";
	const char * DEFUALT_HOST = "api.weixin.qq.com";
	const char * TEST_DIR = "/cgi-bin/";
	const char * VERSION = "apisdkver=cppv0.1";

DEFINE_SERVICE(Token,"Token",
		HTTP_GET,"/cgi-bin/token", 
		PARAM("grant_type",	PARAM_TYPE_STRING,AT_QUERY),
		PARAM("appid",		PARAM_TYPE_STRING,AT_QUERY),
		PARAM("secret",		PARAM_TYPE_STRING,AT_QUERY),
		PARAM_END)

DEFINE_SERVICE(GetCallbackIP,"GetCallbackIP",
		HTTP_GET,"/cgi-bin/getcallbackip",
		PARAM_ACCESS_TOKEN,
		PARAM_END)

DEFINE_SERVICE(CreateMenu,"CreateMenu",
		HTTP_POST,"/cgi-bin/menu/create",
		PARAM_ACCESS_TOKEN,
		PARAM("button",		PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(GetMenu,"GetMenu",
		HTTP_GET,"/cgi-bin/menu/get",
		PARAM_ACCESS_TOKEN,
		PARAM_END)

DEFINE_SERVICE(DeleteMenu,"DeleteMenu",
		HTTP_GET,"/cgi-bin/menu/delete",
		PARAM_ACCESS_TOKEN,
		PARAM_END)

DEFINE_SERVICE(AddConditionalMenu,"AddConditionalMenu",
		HTTP_POST,"/cgi-bin/menu/addconditional",
		PARAM_ACCESS_TOKEN,
		PARAM("button",		PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("matchrule",	PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(DelConditionalMenu,"DelConditionalMenu",
		HTTP_POST,"/cgi-bin/menu/delconditional",
		PARAM_ACCESS_TOKEN,
		PARAM("menuid",		PARAM_TYPE_UINT,AT_POST_JSON), 
		PARAM_END)

DEFINE_SERVICE(TryMatchMenu,"TryMatchMenu",
		HTTP_POST,"/cgi-bin/menu/trymatch",
		PARAM_ACCESS_TOKEN,
		PARAM("user_id",	PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(GetCurrentSelfMenuInfo,"GetCurrentSelfMenuInfo",
		HTTP_GET,"/cgi-bin/get_current_selfmenu_info",
		PARAM_ACCESS_TOKEN,
		PARAM_END)

DEFINE_SERVICE(SendCustomMessage,"SendCustomMessage",
		HTTP_POST,"/cgi-bin/message/custom/send",
		PARAM_ACCESS_TOKEN,
		PARAM("touser",			PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM("msgtype",		PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM("text",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("image",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("voice",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("video",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("music",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("news",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("mpnews",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("wxcard",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("customservice",	PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("mpvideo",		PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(UploadImg,"UploadImg",
		HTTP_POST_MULTIPART,"/cgi-bin/media/uploadimg",
		PARAM_ACCESS_TOKEN,
		PARAM("media",	PARAM_TYPE_MESSAGE, AT_POST),
		PARAM_END)

DEFINE_SERVICE(UploadNews,"UploadNews",
		HTTP_POST,"/cgi-bin/media/uploadnews",
		PARAM_ACCESS_TOKEN,
		PARAM("articles", 		PARAM_TYPE_MESSAGE, AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(MassSendAll,"MassSendAll",
		HTTP_POST,"/cgi-bin/message/mass/sendall",
		PARAM_ACCESS_TOKEN,
		PARAM("filter",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("msgtype",		PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM("text",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("image",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("voice",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("mpvideo",		PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("mpnews",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("wxcard",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(MassSend,"MassSend",
		HTTP_POST,"/cgi-bin/message/mass/send",
		PARAM_ACCESS_TOKEN,
		PARAM("touser",			PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM("msgtype",        PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM("text",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("image",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("voice",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("mpnews",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("wxcard",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("mpvideo",		PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(MassDelete,"MassDelete",
		HTTP_POST,"/cgi-bin/message/mass/delete",
		PARAM_ACCESS_TOKEN,
		PARAM("msg_id",			PARAM_TYPE_UINT, AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(MassPreview,"MassPreview",
		HTTP_POST,"/cgi-bin/message/mass/preview",
		PARAM_ACCESS_TOKEN,
		PARAM("touser",			PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM("towxname",		PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM("msgtype",        PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM("text",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("image",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("voice",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("mpnews",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("wxcard",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM("mpvideo",		PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM_END) 

DEFINE_SERVICE(MassGet,"MassGet",
		HTTP_POST,"/cgi-bin/message/mass/get",
		PARAM_ACCESS_TOKEN,
		PARAM("msg_id",         PARAM_TYPE_UINT, AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(SetIndustry,"SetIndustry",
		HTTP_POST,"/cgi-bin/template/api_set_industry",
		PARAM_ACCESS_TOKEN,
		PARAM("industry_id1",	PARAM_TYPE_STRING,AT_POST_JSON), 
		PARAM("industry_id2",	PARAM_TYPE_STRING,AT_POST_JSON), 
		PARAM_END)

DEFINE_SERVICE(GetIndustry,"GetIndustry",
		HTTP_GET,"/cgi-bin/template/get_industry",
		PARAM_ACCESS_TOKEN,
		PARAM_END)

DEFINE_SERVICE(AddTemplate,"AddTemplate",
		HTTP_POST,"/cgi-bin/template/api_add_template",
		PARAM_ACCESS_TOKEN,
		PARAM("template_id_short",	PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(GetAllPrivateTemplate,"GetAllPrivateTemplate",
		HTTP_GET,"/cgi-bin/template/get_all_private_template",
		PARAM_ACCESS_TOKEN,
		PARAM_END)

DEFINE_SERVICE(DelPrivateTemplate,"DelPrivateTemplate",
		HTTP_POST,"/cgi-bin/template/del_private_template",
		PARAM_ACCESS_TOKEN,
		PARAM("template_id",		PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(SendTemplateMessage,"SendTemplateMessage",
		HTTP_POST,"/cgi-bin/message/template/send",
		PARAM_ACCESS_TOKEN,
		PARAM("touser",         PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM("template_id",	PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM("url",			PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM("data",			PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(GetCurrentAutoReplyInfo,"GetCurrentAutoReplyInfo",
		HTTP_GET,"/cgi-bin/get_current_autoreply_info",
		PARAM_ACCESS_TOKEN,
		PARAM_END)

DEFINE_SERVICE(UploadMedia,"UploadMedia",
		HTTP_POST_MULTIPART,"/cgi-bin/media/upload",
		PARAM_ACCESS_TOKEN,
		PARAM("type",         	PARAM_TYPE_STRING,AT_QUERY),
		PARAM("media",			PARAM_TYPE_MESSAGE,AT_POST),
		PARAM_END)

DEFINE_SERVICE(GetMedia,"GetMedia",
		HTTP_GET,"/cgi-bin/media/get",
		PARAM_ACCESS_TOKEN,
		PARAM("media_id",		PARAM_TYPE_STRING,AT_QUERY),
		PARAM_END)

DEFINE_SERVICE(AddNewsMaterial,"AddNewsMaterial",
		HTTP_POST,"/cgi-bin/material/add_news",
		PARAM_ACCESS_TOKEN,
		PARAM("articles",		PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(AddMaterial,"AddMaterial",
		HTTP_POST_MULTIPART,"/cgi-bin/material/add_material",
		PARAM_ACCESS_TOKEN,
		PARAM("type",			PARAM_TYPE_STRING,AT_QUERY),
		PARAM("media",			PARAM_TYPE_MESSAGE,AT_POST),
		PARAM("description",	PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(GetMaterial,"GetMaterial",
		HTTP_POST,"/cgi-bin/material/get_material",
		PARAM_ACCESS_TOKEN,
		PARAM("media_id",		PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(DelMaterial,"DelMaterial",
		HTTP_POST,"/cgi-bin/material/del_material",
		PARAM_ACCESS_TOKEN,
		PARAM("media_id",       PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(UpdateNews,"UpdateNews",
		HTTP_POST,"/cgi-bin/material/update_news",
		PARAM_ACCESS_TOKEN,
		PARAM("media_id",		PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM("index",			PARAM_TYPE_UINT,AT_POST_JSON),
		PARAM("articles",		PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM_END)
		
DEFINE_SERVICE(GetMaterialCount,"GetMaterialCount",
		HTTP_GET,"/cgi-bin/material/get_materialcount",
		PARAM_ACCESS_TOKEN,
		PARAM_END)

DEFINE_SERVICE(BatchGetMaterial,"BatchGetMaterial",
		HTTP_POST,"/cgi-bin/material/batchget_material",
		PARAM_ACCESS_TOKEN,
		PARAM("type",       	PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM("offset",       	PARAM_TYPE_UINT,AT_POST_JSON),
		PARAM("count",       	PARAM_TYPE_UINT,AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(CreateTags,"CreateTags",
		HTTP_POST,"/cgi-bin/tags/create",	
		PARAM_ACCESS_TOKEN,
		PARAM("tag",           	PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(GetTags,"GetTags",
		HTTP_GET,"/cgi-bin/tags/get",
		PARAM_ACCESS_TOKEN,
		PARAM_END)

DEFINE_SERVICE(UpdateTags,"UpdateTags",
		HTTP_POST,"/cgi-bin/tags/update",
		PARAM_ACCESS_TOKEN,
		PARAM("tag",			PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(DeleteTags,"DeleteTags",
		HTTP_POST,"/cgi-bin/tags/delete",
		PARAM_ACCESS_TOKEN,
		PARAM("tag",          PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(GetTagUser,"GetTagUser",
		HTTP_POST,"/cgi-bin/user/tag/get",
		PARAM_ACCESS_TOKEN,
		PARAM("tagid",			PARAM_TYPE_UINT,AT_POST_JSON),
		PARAM("next_openid",	PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(BatchTaggingMembers,"BatchTaggingMembers",
		HTTP_POST,"/cgi-bin/tags/members/batchtagging",
		PARAM_ACCESS_TOKEN,
		PARAM("openid_list", 	PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM("tagid",			PARAM_TYPE_UINT, AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(BatchUnTaggingMembers,"BatchUnTaggingMembers",
		HTTP_POST,"/cgi-bin/tags/members/batchuntagging",
		PARAM_ACCESS_TOKEN,
		PARAM("openid_list", 	PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM("tagid",			PARAM_TYPE_UINT, AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(GetTagsIdList,"GetTagsIdList",
		HTTP_POST,"/cgi-bin/tags/getidlist",
		PARAM_ACCESS_TOKEN,
		PARAM("openid",			PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(UpdateUserInfoRemark,"UpdateUserInfoRemark",
		HTTP_POST,"/cgi-bin/user/info/updateremark",
		PARAM_ACCESS_TOKEN,
		PARAM("openid",			PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM("remark",			PARAM_TYPE_STRING,AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(UserInfo,"UserInfo",
		HTTP_GET,"/cgi-bin/user/info",
		PARAM_ACCESS_TOKEN,
		PARAM("openid",			PARAM_TYPE_STRING,AT_QUERY),
		PARAM("lang",			PARAM_TYPE_STRING,AT_QUERY),
		PARAM_END)

DEFINE_SERVICE(BatchGetUserInfo,"BatchGetUserInfo",
		HTTP_POST,"/cgi-bin/user/info/batchget",
		PARAM_ACCESS_TOKEN,
		PARAM("user_list",          PARAM_TYPE_MESSAGE,AT_POST_JSON),
		PARAM_END)

DEFINE_SERVICE(GetUser,"GetUser",
		HTTP_GET,"/cgi-bin/user/get",
		PARAM_ACCESS_TOKEN,
		PARAM("next_openid",	PARAM_TYPE_STRING,AT_QUERY),
		PARAM_END)
};

#undef PARAM_ACCESS_TOKEN
#undef PARAM_END
#undef DEFINE_SERVICE
#undef PARAM
