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
	string my_appid;
	string my_secret;

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

	//Sample: Get AccessToken
	{
		TokenReq req;
		TokenResp resp;
		req.set_appid(my_appid);
		req.set_secret(my_secret);
		int ret = pMpClient->Token(req, &resp);
		if ( ret || resp.errcode() ){
			//error 
		}
		my_access_token = resp.access_token();
	}

	//Sample: GetCallbackIp
	{
		GetCallbackIPReq req;
		GetCallbackIPResp resp;
		req.set_access_token( my_access_token );
		int ret = pMpClient->GetCallbackIP(req,&resp);
		if ( ret || resp.errcode() ){
			//error 
		}
		for ( size_t i = 0; i < resp.ip_list_size(); ++i ){
			cout<<resp.ip_list(i)<<endl;
		}
	}

	//Sample: GetCurrentAutoReplyInfo
	{
		GetCurrentAutoReplyInfoReq req;
		GetCurrentAutoReplyInfoResp resp;
		req.set_access_token(my_access_token);
		int ret = pMpClient->GetCurrentAutoReplyInfo(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	delete pMpClient;
	//global shutdown once at exit
	MpClient::ShutDown();
}
