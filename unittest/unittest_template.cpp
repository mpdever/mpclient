#include <limits.h>
#include "gtest/gtest.h"

#include "mpclient.h"
#include <string>
#include "unittest_comm.cpp"

using std::string;
using namespace mpcomm;

extern string g_token;
extern string g_appid;
extern string g_secret;
extern string g_template_id_short;

static string templateid;

TEST(Basic, Token){
	TokenReq req;
	TokenResp resp;
	req.set_appid(g_appid);
	req.set_secret( g_secret);
	int ret = MpClient().Token(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.access_token().size(), 0);
	g_token = resp.access_token();
}

TEST(Template, SetIndustry){
	SetIndustryReq req;
	SetIndustryResp resp;
	req.set_access_token(g_token);
	req.set_industry_id1("1");
	req.set_industry_id2("2");
	int ret = MpClient().SetIndustry(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(Template, GetIndustry){
	GetIndustryReq req;
	GetIndustryResp resp;
	req.set_access_token(g_token);
	int ret = MpClient().GetIndustry(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_LT(0,resp.primary_industry().first_class().size());
}

TEST(Template, AddTemplate){
	AddTemplateReq req;
	AddTemplateResp resp;
	req.set_access_token(g_token);
	req.set_template_id_short( g_template_id_short );
	int ret = MpClient().AddTemplate( req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_LT(0,resp.template_id().size());
	templateid=resp.template_id();
}

TEST(Template, GetAllPrivateTemplate){
	GetAllPrivateTemplateReq req;
	GetAllPrivateTemplateResp resp;
	req.set_access_token(g_token);
	int ret = MpClient().GetAllPrivateTemplate(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(Template, SendTemplateMessage){
	SendTemplateMessageReq req;
	SendTemplateMessageResp resp;
	req.set_access_token(g_token);
	req.set_touser(g_openid);
	req.set_template_id(templateid);
	req.set_url("http://mp.weixin.qq.com");
	req.mutable_data()->set_content( "{ \"name\": { \"value\":\"HAHA\" ,\"color\":\"#173177\"}, \"remark\": { \"value\":\"Hey\" ,\"color\":\"#173177\"} }" );
	int ret = MpClient().SendTemplateMessage(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_LT(0,resp.msgid());
}

TEST(Template, DelPrivateTemplate){
	DelPrivateTemplateReq req;
	DelPrivateTemplateResp resp;
	req.set_access_token(g_token);
	req.set_template_id( templateid );
	int ret = MpClient().DelPrivateTemplate(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}
