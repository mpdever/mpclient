#include <limits.h>
#include "gtest/gtest.h"

#include "mpclient.h"
#include <string>
#include "unittest_comm.cpp"

using std::string;
using namespace mpcomm;

extern string g_secret;
extern string g_appid;
extern string g_token;

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

TEST(Basic, GetCallbackIP){
	GetCallbackIPReq req;
	GetCallbackIPResp resp;
	req.set_access_token( g_token );
	int ret = MpClient().GetCallbackIP(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.ip_list_size(), 0);
	ASSERT_GT(resp.ip_list(0).size(), 0);
}

TEST(Basic, GetCurrentAutoReplyInfo){
	GetCurrentAutoReplyInfoReq req;
	GetCurrentAutoReplyInfoResp resp;
	req.set_access_token(g_token);
	int ret = MpClient().GetCurrentAutoReplyInfo(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(Basic, GetCurrentSelfMenuInfo){
	GetCurrentAutoReplyInfoReq req;
	GetCurrentAutoReplyInfoResp resp;
	req.set_access_token(g_token);
	int ret = MpClient().GetCurrentAutoReplyInfo(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}
