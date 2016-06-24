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
	unsigned long long my_conditional_menuid;
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

	//Sample: CreateMenu
	{
		CreateMenuReq req;
		CreateMenuResp resp;
		req.set_access_token( my_access_token );
		MenuButton button1,button2;
		MenuButton subbtn;

		button1.set_name("menu1");
		//sub button
		subbtn.set_name("11-click");
		subbtn.set_type("click");
		subbtn.set_key("keyOf11");
		button1.add_sub_button()->Swap(&subbtn);

		req.add_button()->Swap( &button1 );

		button2.set_name("menu2");
		button2.set_type("click");
		button2.set_key("keyof3");
		req.add_button()->Swap( &button2 );
		int ret = pMpClient->CreateMenu(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: GetMenu
	{
		GetMenuReq req;
		GetMenuResp resp;
		req.set_access_token(my_access_token);
		int ret = pMpClient->GetMenu(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
		for ( size_t i = 0; i < resp.menu().button_size(); ++i ){
			cout<<resp.menu().button(i).name()<<endl;
		}
	}

	//Sample: DeleteMenu
	{
		DeleteMenuReq req;
		DeleteMenuResp resp;
		req.set_access_token(my_access_token);
		int ret = pMpClient->DeleteMenu(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}


	//Sample: AddConditionalMenu
	{
		AddConditionalMenuReq req;
		AddConditionalMenuResp resp;
		req.set_access_token( my_access_token );
		MenuButton button,subbtn;
		button.set_name("menu1");
		subbtn.set_name("11-click");
		subbtn.set_type("click");
		subbtn.set_key("keyOf11");
		button.add_sub_button()->Swap(&subbtn);
		req.add_button()->Swap( &button );

		MenuMatchRule rule;
		rule.set_tag_id("2");
		rule.set_sex("1");
		rule.set_client_platform_type("2");
		rule.set_country("China");
		rule.set_province("Guangdong");
		rule.set_city("Guangzhou");
		rule.set_language("en");
		req.mutable_matchrule()->Swap(&rule);
		int ret = pMpClient->AddConditionalMenu(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
		cout<<resp.menuid()<<endl;
	}

	//Sample: DelConditionalMenu
	{
		DelConditionalMenuReq req;
		DelConditionalMenuResp resp;
		req.set_access_token(my_access_token);
		req.set_menuid(my_conditional_menuid);
		int ret = pMpClient->DelConditionalMenu(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: TryMatchMenu
	{
		TryMatchMenuReq req;
		TryMatchMenuResp resp;
		req.set_access_token(my_access_token);
		req.set_user_id( my_openid );
		int ret = pMpClient->TryMatchMenu(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: GetCurrentSelfMenuInfo
	{
		GetCurrentSelfMenuInfoReq req;
		GetCurrentSelfMenuInfoResp resp;
		req.set_access_token(my_access_token);
		int ret = pMpClient->GetCurrentSelfMenuInfo(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	delete pMpClient;
	//global shutdown once at exit
	MpClient::ShutDown();
}
