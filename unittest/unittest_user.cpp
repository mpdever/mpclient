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

static unsigned long long tagid;

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

TEST(User, CreateTags){
	CreateTagsReq req;
	CreateTagsResp resp;
	req.set_access_token(g_token);
	req.mutable_tag()->set_name("newtag");
	int ret = MpClient().CreateTags(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_LT(0,resp.tag().id());
	tagid = resp.tag().id();
}

TEST(User, GetTags){
	GetTagsReq req;
	GetTagsResp resp;
	req.set_access_token(g_token);
	int ret = MpClient().GetTags(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(User, UpdateTags){
	UpdateTagsReq req;
	UpdateTagsResp resp;
	req.set_access_token(g_token);
	req.mutable_tag()->set_id(tagid);
	req.mutable_tag()->set_name("newtag2");
	int ret = MpClient().UpdateTags(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(User, BatchTaggingMembers){
	BatchTaggingMembersReq req;
	BatchTaggingMembersResp resp;
	req.set_access_token(g_token);
	req.set_tagid(tagid);
	req.add_openid_list()->assign(g_openid);
	int ret = MpClient().BatchTaggingMembers(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(User, GetTagUser){
	GetTagUserReq req;
	GetTagUserResp resp;
	req.set_access_token(g_token);
	req.set_tagid(tagid);
	int ret = MpClient().GetTagUser(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(User, BatchUnTaggingMembers){
	BatchUnTaggingMembersReq req;
	BatchUnTaggingMembersResp resp;
	req.set_access_token(g_token);
	req.set_tagid(tagid);
	req.add_openid_list()->assign(g_openid);
	int ret = MpClient().BatchUnTaggingMembers(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(User, GetTagsIdList){
	GetTagsIdListReq req;
	GetTagsIdListResp resp;
	req.set_access_token(g_token);
	req.set_openid(g_openid);
	int ret = MpClient().GetTagsIdList(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_LT(0,resp.tagid_list_size());
}

TEST(User, DeleteTags){
	DeleteTagsReq req;
	DeleteTagsResp resp;
	req.set_access_token(g_token);
	req.mutable_tag()->set_id(tagid);
	int ret = MpClient().DeleteTags(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(User, UpdateUserInfoRemark){
	UpdateUserInfoRemarkReq req;
	UpdateUserInfoRemarkResp resp;
	req.set_access_token(g_token);
	req.set_openid(g_openid);
	req.set_remark("ll_666");
	int ret = MpClient().UpdateUserInfoRemark(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(User, UserInfo){
	UserInfoReq req;
	UserInfoResp resp;
	req.set_access_token(g_token);
	req.set_openid(g_openid);
	req.set_lang("zh_CN");
	int ret = MpClient().UserInfo(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(User, BatchGetUserInfo){
	BatchGetUserInfoReq req;
	BatchGetUserInfoResp resp;
	req.set_access_token(g_token);
	BatchGetUserItem item;
	item.set_openid(g_openid);
	item.set_lang("zh_CN");
	req.add_user_list()->Swap(&item);
	int ret = MpClient().BatchGetUserInfo(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(User, GetUser){
	GetUserReq req;
	GetUserResp resp;
	req.set_access_token(g_token);
	req.set_next_openid(g_openid);
	int ret = MpClient().GetUser(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}
