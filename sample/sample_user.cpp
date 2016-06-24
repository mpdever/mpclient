#include "mpclient.h"

#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>

using namespace std;
using namespace mpcomm;



int main()
{
	string my_access_token;
	unsigned long long my_tagid = 103;
	string my_openid;

	//init the client once
	MpClient::GlobalInit();

	MpClient * pMpClient = new MpClient();

	//set log config
	//loglevel : ERROR
	//loglimit : 1GB
	//log directory : /home/log, log file is /home/log/yyyyMMddHH.log
	int ret = pMpClient->SetLogConfig( ERR, 1<<30, "/home/log" );
	if ( ret ){
		//error
	}

	//set connection timeout and process timeout ( in seconds )
	//set to 0 : no timeout
	ret = pMpClient->SetTimeOut( 0, 0 );
	if ( ret ){
		//error
	}

	//Sample: CreateTags
	{
		CreateTagsReq req;
		CreateTagsResp resp;
		req.set_access_token(my_access_token);
		req.mutable_tag()->set_name("new tag");
		int ret = pMpClient->CreateTags(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
		cout<<resp.tag().id()<<endl;
	}

	//Sample: GetTags
	{
		GetTagsReq req;
		GetTagsResp resp;
		req.set_access_token(my_access_token);
		int ret = pMpClient->GetTags(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: UpdateTags
	{
		UpdateTagsReq req;
		UpdateTagsResp resp;
		req.set_access_token(my_access_token);
		req.mutable_tag()->set_id(my_tagid);
		req.mutable_tag()->set_name("update tag name");
		int ret = pMpClient->UpdateTags(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: DeleteTags
	{
		DeleteTagsReq req;
		DeleteTagsResp resp;
		req.set_access_token(my_access_token);
		req.mutable_tag()->set_id(my_tagid);
		int ret = pMpClient->DeleteTags(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}              
	}

	//Sample: GetTagUser
	{
		GetTagUserReq req;       
		GetTagUserResp resp;
		req.set_access_token(my_access_token);
		req.set_tagid(my_tagid);
		int ret = pMpClient->GetTagUser(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}              
	}

	//Sample: BatchTaggingMembers
	{
		BatchTaggingMembersReq req;
		BatchTaggingMembersResp resp;
		req.set_access_token(my_access_token);
		req.set_tagid(my_tagid);
		req.add_openid_list()->assign(my_openid);
		int ret = pMpClient->BatchTaggingMembers(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}              
	}

	//Sample: BatchUnTaggingMembers
	{
		BatchUnTaggingMembersReq req;
		BatchUnTaggingMembersResp resp;
		req.set_access_token(my_access_token);
		req.set_tagid(my_tagid);
		req.add_openid_list()->assign(my_openid);
		int ret = pMpClient->BatchUnTaggingMembers(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}              
	}

	//Sample: GetTagsIdList
	{
		GetTagsIdListReq req;
		GetTagsIdListResp resp;
		req.set_access_token(my_access_token);
		req.set_openid(my_openid);
		int ret = pMpClient->GetTagsIdList(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}              
	}

	//Sample: UpdateUserInfoRemark
	{
		UpdateUserInfoRemarkReq req;
		UpdateUserInfoRemarkResp resp;
		req.set_access_token(my_access_token);
		req.set_openid(my_openid);
		req.set_remark("remark0");
		int ret = pMpClient->UpdateUserInfoRemark(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}              
	}

	//Sample: UserInfo
	{
		UserInfoReq req;
		UserInfoResp resp;
		req.set_access_token(my_access_token);
		req.set_openid(my_openid);
		req.set_lang("zh_CN");
		int ret = pMpClient->UserInfo(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}    
	}

	//Sample: BatchGetUserInfo
	{
		BatchGetUserInfoReq req;
		BatchGetUserInfoResp resp;
		req.set_access_token(my_access_token);
		BatchGetUserItem item;
		item.set_openid(my_openid); 
		item.set_lang("zh_CN");
		req.add_user_list()->Swap(&item);
		int ret = pMpClient->BatchGetUserInfo(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}    
	}

	//Sample: GetUser
	{
		GetUserReq req;
		GetUserResp resp;
		req.set_access_token(my_access_token);
		req.set_next_openid(my_openid);
		int ret = pMpClient->GetUser(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}    
	}

	delete pMpClient;
	//global shutdown once at exit
	MpClient::ShutDown();
}
