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
	string my_template_id_short = "TM00001";
	string my_openid;
	string templateid;

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

	//Sample: SetIndustry
	{
		SetIndustryReq req;
		SetIndustryResp resp;
		req.set_access_token(my_access_token);
		req.set_industry_id1("1");
		req.set_industry_id2("2");
		int ret = pMpClient->SetIndustry(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: GetIndustry
	{
		GetIndustryReq req;
		GetIndustryResp resp;
		req.set_access_token(my_access_token);
		int ret = pMpClient->GetIndustry(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: AddTemplate
	{
		AddTemplateReq req;
		AddTemplateResp resp;
		req.set_access_token(my_access_token);
		req.set_template_id_short( my_template_id_short );
		int ret = pMpClient->AddTemplate( req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
		cout<<resp.template_id()<<endl;
	}

	//Sample: DelPrivateTemplate
	{
		DelPrivateTemplateReq req;
		DelPrivateTemplateResp resp;
		req.set_access_token(my_access_token);
		req.set_template_id( templateid );
		int ret = pMpClient->DelPrivateTemplate(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: GetAllPrivateTemplate
	{
		GetAllPrivateTemplateReq req;
		GetAllPrivateTemplateResp resp;
		req.set_access_token(my_access_token);
		int ret = pMpClient->GetAllPrivateTemplate(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: SendTemplateMessage
	{
		SendTemplateMessageReq req;
		SendTemplateMessageResp resp;
		req.set_access_token(my_access_token);
		req.set_touser(my_openid);
		req.set_template_id(templateid);
		req.set_url("http://mp.weixin.qq.com");
		req.mutable_data()->set_content( 
				"{ \"name\": { \"value\":\"item1\" ,\"color\":\"#173177\"}, "
				"\"remark\": { \"value\":\"oooo\" ,\"color\":\"#173177\"} }" );
		int ret = pMpClient->SendTemplateMessage(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	delete pMpClient;
	//global shutdown once at exit
	MpClient::ShutDown();
}
