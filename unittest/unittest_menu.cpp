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

static unsigned long long cond_menuid;

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

TEST(Menu, CreateMenu){
	CreateMenuReq req;
	CreateMenuResp resp;
	req.set_access_token( g_token );
	MenuButton button,subbtn;
	button.set_name("menu1");

	subbtn.set_name("11-click");
	subbtn.set_type("click");
	subbtn.set_key("keyOf11");
	button.add_sub_button()->Swap(&subbtn);
	subbtn.Clear();

	subbtn.set_name("12-view");
	subbtn.set_type("view");
	subbtn.set_url("http://www.qq.com");
	button.add_sub_button()->Swap(&subbtn);
	subbtn.Clear();

	subbtn.set_name("13-scanpush");
	subbtn.set_type("scancode_push");
	subbtn.set_key("keyOf13");
	button.add_sub_button()->Swap(&subbtn);
	subbtn.Clear();

	subbtn.set_name("14-scanwait");
	subbtn.set_type("scancode_waitmsg");
	subbtn.set_key("keyOf14");
	button.add_sub_button()->Swap(&subbtn);
	subbtn.Clear();

	subbtn.set_name("15-pic_sysphoto");
	subbtn.set_type("pic_sysphoto");
	subbtn.set_key("keyOf15");
	button.add_sub_button()->Swap(&subbtn);
	subbtn.Clear();

	req.add_button()->Swap( &button );
	button.Clear();
	button.set_name("menu2");

	subbtn.set_name("21-pic_photo_or_album");
	subbtn.set_type("pic_photo_or_album");
	subbtn.set_key("keyOf21");
	button.add_sub_button()->Swap(&subbtn);
	subbtn.Clear();

	subbtn.set_name("22-pic_weixin");
	subbtn.set_type("pic_weixin");
	subbtn.set_key("keyOf22");
	button.add_sub_button()->Swap(&subbtn);
	subbtn.Clear();

	subbtn.set_name("23-location_select");
	subbtn.set_type("location_select");
	subbtn.set_key("keyOf23");
	button.add_sub_button()->Swap(&subbtn);
	subbtn.Clear();

	req.add_button()->Swap( &button );
	button.Clear();
	button.set_name("menu3");
	button.set_type("click");
	button.set_key("keyof3");
	req.add_button()->Swap( &button );

	int ret = MpClient().CreateMenu(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(Menu, GetMenu){
	GetMenuReq req;
	GetMenuResp resp;
	req.set_access_token(g_token);
	int ret = MpClient().GetMenu(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.menu().DebugString().size(), 0);
}

TEST(Menu, AddConditionalMenu){
	AddConditionalMenuReq req;
	AddConditionalMenuResp resp;
	req.set_access_token( g_token );
	MenuButton button,subbtn;
	button.set_name("menu1");
	subbtn.set_name("11-click");
	subbtn.set_type("click");
	subbtn.set_key("keyOf11");
	button.add_sub_button()->Swap(&subbtn);
	subbtn.Clear();
	subbtn.set_name("12-view");
	subbtn.set_type("view");
	subbtn.set_url("http://www.qq.com");
	button.add_sub_button()->Swap(&subbtn);

	req.add_button()->Swap( &button );
	button.Clear();
	button.set_name("menu3");
	button.set_type("click");
	button.set_key("keyof3");
	req.add_button()->Swap( &button );

	MenuMatchRule rule;
	rule.set_tag_id("2");
	rule.set_sex("1");
	rule.set_client_platform_type("2");
	rule.set_country( "China");
	rule.set_province("Guangdong");
	rule.set_city( "Guangzhou");
	rule.set_language("zh_En");
	req.mutable_matchrule()->Swap(&rule);

	int ret = MpClient().AddConditionalMenu(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.menuid(),0);
	cond_menuid = resp.menuid();
}

TEST(Menu, DelConditionalMenu){
	DelConditionalMenuReq req;
	DelConditionalMenuResp resp;
	req.set_access_token(g_token);
	req.set_menuid(cond_menuid);
	int ret = MpClient().DelConditionalMenu(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}


TEST(Menu, TryMatchMenu){
	TryMatchMenuReq req;
	TryMatchMenuResp resp;
	req.set_access_token(g_token);
	req.set_user_id( g_openid );
	int ret = MpClient().TryMatchMenu(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(Menu, DeleteMenu){
	DeleteMenuReq req;
	DeleteMenuResp resp;
	req.set_access_token(g_token);
	int ret = MpClient().DeleteMenu(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}
