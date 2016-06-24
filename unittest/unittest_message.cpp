#include <limits.h>
#include "gtest/gtest.h"

#include "mpclient.h"
#include <string>
#include "unittest_comm.cpp"

using std::string;
using namespace mpcomm;

extern string g_appid;
extern string g_secret;
extern string g_token;
extern string g_image_media_id;
extern string g_voice_media_id;
extern string g_video_media_id;
extern string g_thumb_media_id;
extern string g_news_media_id;
extern string g_picurl;
extern string g_newsurl;
extern string g_openid;
extern string g_wxid;
extern unsigned long long g_tagid;
extern unsigned long long g_mass_msg_id;

TEST(Basic, Token){
	TokenReq req;
	TokenResp resp;
	req.set_appid(g_appid);
	req.set_secret( g_secret );
	int ret = MpClient().Token(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.access_token().size(), 0);
	g_token = resp.access_token();
}

TEST(Message, SendCustomMessageText){
	SendCustomMessageReq req;
	SendCustomMessageResp resp;
	req.set_access_token(g_token);
	req.set_touser(g_openid);
	req.set_msgtype("text");
	req.mutable_text()->set_content("HELLO this is me");
	int ret = MpClient().SendCustomMessage(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(Message, SendCustomMessageImage){
	SendCustomMessageReq req;
	SendCustomMessageResp resp;
	req.set_access_token(g_token);
	req.set_touser(g_openid);
	req.set_msgtype("image");
	req.mutable_image()->set_media_id(g_image_media_id);
	int ret = MpClient().SendCustomMessage(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(Message, SendCustomMessageVoice){
	SendCustomMessageReq req;
	SendCustomMessageResp resp;
	req.set_access_token(g_token);
	req.set_touser(g_openid);
	req.set_msgtype("voice");
	req.mutable_voice()->set_media_id(g_voice_media_id);
	int ret = MpClient().SendCustomMessage(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(Message, SendCustomMessageVideo){
	SendCustomMessageReq req;
	SendCustomMessageResp resp;
	req.set_access_token(g_token);
	req.set_touser(g_openid);
	req.set_msgtype("video");
	req.mutable_video()->set_media_id(g_video_media_id);
	req.mutable_video()->set_thumb_media_id(g_thumb_media_id);
	req.mutable_video()->set_title("a title");
	req.mutable_video()->set_description("a description");
	int ret = MpClient().SendCustomMessage(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(Message, SendCustomMessageMpNews){
	SendCustomMessageReq req;
	SendCustomMessageResp resp;
	req.set_access_token(g_token);
	req.set_touser(g_openid);
	req.set_msgtype("mpnews");
	req.mutable_mpnews()->set_media_id(g_news_media_id);
	int ret = MpClient().SendCustomMessage(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(Message, SendCustomMessageNews){
	SendCustomMessageReq req;
	SendCustomMessageResp resp;
	req.set_access_token(g_token);
	req.set_touser(g_openid);
	req.set_msgtype("news");
	ArticlesItem item;
	item.set_title("today's test");
	item.set_description("this is description");
	item.set_picurl(g_picurl);
	item.set_url(g_newsurl);
	req.mutable_news()->add_articles()->Swap(&item);
	req.mutable_news()->set_thumb_media_id( g_thumb_media_id );
	req.mutable_mpnews()->set_media_id(g_news_media_id);
	int ret = MpClient().SendCustomMessage(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

/////send all

TEST(Message, MassSendAllMpNews){
	MassSendAllReq req;
	MassSendAllResp resp;
	req.set_access_token(g_token);
	req.mutable_filter()->set_is_to_all(false);
	req.mutable_filter()->set_tag_id(g_tagid);
	req.mutable_mpnews()->set_media_id( g_news_media_id );
	req.set_msgtype("mpnews");
	int ret = MpClient().MassSendAll(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_LT(0,resp.msg_id());
	ASSERT_LT(0,resp.msg_data_id());
}

TEST(Message, MassSendAllText){
	MassSendAllReq req;
	MassSendAllResp resp;
	req.set_access_token(g_token);
	req.mutable_filter()->set_is_to_all(false);
	req.mutable_filter()->set_tag_id(g_tagid);
	req.mutable_text()->set_content( "this is a test for mass send" );
	req.set_msgtype("text");
	int ret = MpClient().MassSendAll(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_LT(0,resp.msg_id());
}

TEST(Message, MassSendAllVoice){
	MassSendAllReq req;
	MassSendAllResp resp;
	req.set_access_token(g_token);
	req.mutable_filter()->set_is_to_all(false);
	req.mutable_filter()->set_tag_id(g_tagid);
	req.mutable_voice()->set_media_id( g_voice_media_id );
	req.set_msgtype("voice");
	int ret = MpClient().MassSendAll(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_LT(0,resp.msg_id());
}

TEST(Message, MassSendAllImage){
	MassSendAllReq req;
	MassSendAllResp resp;
	req.set_access_token(g_token);
	req.mutable_filter()->set_is_to_all(false);
	req.mutable_filter()->set_tag_id(g_tagid);
	req.mutable_image()->set_media_id( g_image_media_id );
	req.set_msgtype("image");
	int ret = MpClient().MassSendAll(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_LT(0,resp.msg_id());
}


TEST(Message, MassSendAllMpVideo){
	MassSendAllReq req;
	MassSendAllResp resp;
	req.set_access_token(g_token);
	req.mutable_filter()->set_is_to_all(false);
	req.mutable_filter()->set_tag_id(g_tagid);
	req.mutable_mpvideo()->set_media_id( g_video_media_id );
	req.set_msgtype("mpvideo");
	int ret = MpClient().MassSendAll(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_LT(0,resp.msg_id());
}

/////mass send

TEST(Message, MassSendImage){
	MassSendReq req;
	MassSendResp resp;
	req.set_access_token(g_token);
	req.add_touser()->assign(g_openid);
	req.add_touser()->assign(g_openid);
	req.mutable_image()->set_media_id( g_image_media_id );
	req.set_msgtype("image");
	int ret = MpClient().MassSend(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_LT(0,resp.msg_id());
}

TEST(Message, MassSendMpVideo){
	MassSendReq req;
	MassSendResp resp;
	req.set_access_token(g_token);
	req.add_touser()->assign(g_openid);
	req.add_touser()->assign(g_openid);
	req.mutable_mpvideo()->set_media_id( g_video_media_id );
	req.set_msgtype("mpvideo");
	int ret = MpClient().MassSend(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_LT(0,resp.msg_id());
}


TEST(Message, MassSendMpNews){
	MassSendReq req;
	MassSendResp resp;
	req.set_access_token(g_token);
	req.add_touser()->assign(g_openid);
	req.add_touser()->assign(g_openid);
	req.mutable_mpnews()->set_media_id( g_news_media_id );
	req.set_msgtype("mpnews");
	int ret = MpClient().MassSend(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_LT(0,resp.msg_id());
	ASSERT_LT(0,resp.msg_data_id());
}

TEST(Message, MassSendText){
	MassSendReq req;
	MassSendResp resp;
	req.set_access_token(g_token);
	req.add_touser()->assign(g_openid);
	req.add_touser()->assign(g_openid);
	req.mutable_text()->set_content( "test for text" );
	req.set_msgtype("text");
	int ret = MpClient().MassSend(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_LT(0,resp.msg_id());
}

TEST(Message, MassSendVoice){
	MassSendReq req;
	MassSendResp resp;
	req.set_access_token(g_token);
	req.add_touser()->assign(g_openid);
	req.add_touser()->assign(g_openid);
	req.mutable_voice()->set_media_id( g_voice_media_id );
	req.set_msgtype("voice");
	int ret = MpClient().MassSend(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_LT(0,resp.msg_id());
}

/////mass send preview

TEST(Message, MassPreviewImage){
	MassPreviewReq req;
	MassPreviewResp resp;
	req.set_access_token(g_token);
	req.set_touser(g_openid);
	req.mutable_image()->set_media_id( g_image_media_id );
	req.set_msgtype("image");
	int ret = MpClient().MassPreview(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(Message, MassPreviewMpVideo){
	MassPreviewReq req;
	MassPreviewResp resp;
	req.set_access_token(g_token);
	req.set_touser(g_openid);
	req.mutable_mpvideo()->set_media_id( g_video_media_id );
	req.set_msgtype("mpvideo");
	int ret = MpClient().MassPreview(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}


TEST(Message, MassPreviewMpNews){
	MassPreviewReq req;
	MassPreviewResp resp;
	req.set_access_token(g_token);
	req.set_touser(g_openid);
	req.mutable_mpnews()->set_media_id( g_news_media_id );
	req.set_msgtype("mpnews");
	int ret = MpClient().MassPreview(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(Message, MassPreviewText){
	MassPreviewReq req;
	MassPreviewResp resp;
	req.set_access_token(g_token);
	req.set_touser(g_openid);
	req.mutable_text()->set_content( "test for text" );
	req.set_msgtype("text");
	int ret = MpClient().MassPreview(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(Message, MassPreviewVoice){
	MassPreviewReq req;
	MassPreviewResp resp;
	req.set_access_token(g_token);
	req.set_towxname(g_wxid);
	req.mutable_voice()->set_media_id( g_voice_media_id );
	req.set_msgtype("voice");
	int ret = MpClient().MassPreview(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

///get && del

TEST(Message, MassGet){
	MassGetReq req;
	MassGetResp resp;
	req.set_access_token(g_token);
	req.set_msg_id( g_mass_msg_id );
	int ret = MpClient().MassGet(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(Message, MassDelete){
	MassDeleteReq req;
	MassDeleteResp resp;
	req.set_access_token(g_token);
	req.set_msg_id( g_mass_msg_id);
	int ret = MpClient().MassDelete(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}
