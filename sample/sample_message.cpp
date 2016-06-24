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
	unsigned long long my_mass_msg_id;
	string image_media_id;
	string voice_media_id;
	string video_media_id;
	string thumb_media_id;
	string news_media_id;
	string my_picurl;
	string my_news_url;
	string my_openid;
	string my_openid2;
	string my_wxid;

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

	//Sample: SendCustomMessageText
	{
		SendCustomMessageReq req;
		SendCustomMessageResp resp;
		req.set_access_token(my_access_token);
		req.set_touser(my_openid);
		req.set_msgtype("text");
		req.mutable_text()->set_content("HELLO this is kim");
		int ret = pMpClient->SendCustomMessage(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: SendCustomMessageImage
	{
		SendCustomMessageReq req;
		SendCustomMessageResp resp;
		req.set_access_token(my_access_token);
		req.set_touser(my_openid);
		req.set_msgtype("image");
		req.mutable_image()->set_media_id(image_media_id);
		int ret = pMpClient->SendCustomMessage(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: SendCustomMessageVoice
	{
		SendCustomMessageReq req;
		SendCustomMessageResp resp;
		req.set_access_token(my_access_token);
		req.set_touser(my_openid);
		req.set_msgtype("voice");
		req.mutable_voice()->set_media_id(voice_media_id);
		int ret = pMpClient->SendCustomMessage(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: SendCustomMessageVideo
	{
		SendCustomMessageReq req;
		SendCustomMessageResp resp;
		req.set_access_token(my_access_token);
		req.set_touser(my_openid);
		req.set_msgtype("video");
		req.mutable_video()->set_media_id(video_media_id);
		req.mutable_video()->set_thumb_media_id(thumb_media_id);
		req.mutable_video()->set_title("ttt");
		req.mutable_video()->set_description("ddd");
		int ret = pMpClient->SendCustomMessage(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}


	//Sample: SendCustomMessageMpNews
	{
		SendCustomMessageReq req;
		SendCustomMessageResp resp;
		req.set_access_token(my_access_token);
		req.set_touser(my_openid);
		req.set_msgtype("mpnews");
		req.mutable_mpnews()->set_media_id(news_media_id);
		int ret = pMpClient->SendCustomMessage(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: SendCustomMessageNews
	{
		SendCustomMessageReq req;
		SendCustomMessageResp resp;
		req.set_access_token(my_access_token);
		req.set_touser(my_openid);
		req.set_msgtype("news");

		ArticlesItem item;
		item.set_title("title");
		item.set_description("description");
		item.set_picurl(my_picurl);
		item.set_url(my_news_url);
		req.mutable_news()->add_articles()->Swap(&item);
		req.mutable_news()->set_thumb_media_id( thumb_media_id );
		req.mutable_mpnews()->set_media_id(news_media_id);
		int ret = pMpClient->SendCustomMessage(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//===Mass

	//Sample: MassSendAllMpNews
	{
		MassSendAllReq req;
		MassSendAllResp resp;
		req.set_access_token(my_access_token);
		req.mutable_filter()->set_is_to_all(false);
		req.mutable_filter()->set_tag_id(my_tagid);
		req.mutable_mpnews()->set_media_id( news_media_id );
		req.set_msgtype("mpnews");
		int ret = pMpClient->MassSendAll(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
		cout<<resp.msg_id()<<endl;
	}

	//Sample: MassSendAllText
	{
		MassSendAllReq req;
		MassSendAllResp resp;
		req.set_access_token(my_access_token);
		req.mutable_filter()->set_is_to_all(false);
		req.mutable_filter()->set_tag_id(my_tagid);
		req.mutable_text()->set_content( "this is a test for mass send" );
		req.set_msgtype("text");
		int ret = pMpClient->MassSendAll(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
		cout<<resp.msg_id()<<endl;
	}

	//Sample: MassSendAllVoice
	{
		MassSendAllReq req;
		MassSendAllResp resp;
		req.set_access_token(my_access_token);
		req.mutable_filter()->set_is_to_all(false);
		req.mutable_filter()->set_tag_id(my_tagid);
		req.mutable_voice()->set_media_id( voice_media_id );
		req.set_msgtype("voice");
		int ret = pMpClient->MassSendAll(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
		cout<<resp.msg_id()<<endl;
	}

	//Sample: MassSendAllImage
	{
		MassSendAllReq req;
		MassSendAllResp resp;
		req.set_access_token(my_access_token);
		req.mutable_filter()->set_is_to_all(false);
		req.mutable_filter()->set_tag_id(my_tagid);
		req.mutable_image()->set_media_id( image_media_id );
		req.set_msgtype("image");
		int ret = pMpClient->MassSendAll(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
		cout<<resp.msg_id()<<endl;
	}

	//Sample: MassSendAllMpVideo
	{
		MassSendAllReq req;
		MassSendAllResp resp;
		req.set_access_token(my_access_token);
		req.mutable_filter()->set_is_to_all(false);
		req.mutable_filter()->set_tag_id(my_tagid);
		req.mutable_mpvideo()->set_media_id( video_media_id );
		req.set_msgtype("mpvideo");
		int ret = pMpClient->MassSendAll(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
		cout<<resp.msg_id()<<endl;
	}

	//Sample: MassSendImage
	{
		MassSendReq req;
		MassSendResp resp;
		req.set_access_token(my_access_token);
		req.add_touser()->assign(my_openid);
		req.add_touser()->assign(my_openid2);
		req.mutable_image()->set_media_id( image_media_id );
		req.set_msgtype("image");
		int ret = pMpClient->MassSend(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
		cout<<resp.msg_id()<<endl;
	}

	//Sample: MassSendMpVideo
	{
		MassSendReq req;
		MassSendResp resp;
		req.set_access_token(my_access_token);
		req.add_touser()->assign(my_openid);
		req.add_touser()->assign(my_openid2);
		req.mutable_mpvideo()->set_media_id( video_media_id );
		req.set_msgtype("mpvideo");
		int ret = pMpClient->MassSend(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
		cout<<resp.msg_id()<<endl;
	}


	//Sample: MassSendMpNews
	{
		MassSendReq req;
		MassSendResp resp;
		req.set_access_token(my_access_token);
		req.add_touser()->assign(my_openid);
		req.add_touser()->assign(my_openid2);
		req.mutable_mpnews()->set_media_id( news_media_id );
		req.set_msgtype("mpnews");
		int ret = pMpClient->MassSend(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
		cout<<resp.msg_id()<<endl;
	}

	//Sample: MassSendText
	{
		MassSendReq req;
		MassSendResp resp;
		req.set_access_token(my_access_token);
		req.add_touser()->assign(my_openid);
		req.add_touser()->assign(my_openid2);
		req.mutable_text()->set_content( "test for text" );
		req.set_msgtype("text");
		int ret = pMpClient->MassSend(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
		cout<<resp.msg_id()<<endl;
	}

	//Sample: MassSendVoice
	{
		MassSendReq req;
		MassSendResp resp;
		req.set_access_token(my_access_token);
		req.add_touser()->assign(my_openid);
		req.add_touser()->assign(my_openid2);
		req.mutable_voice()->set_media_id( voice_media_id );
		req.set_msgtype("voice");
		int ret = pMpClient->MassSend(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
		cout<<resp.msg_id()<<endl;
	}


	//Sample: MassPreviewImage
	{
		MassPreviewReq req;
		MassPreviewResp resp;
		req.set_access_token(my_access_token);
		req.set_touser(my_openid);
		req.mutable_image()->set_media_id( image_media_id );
		req.set_msgtype("image");
		int ret = pMpClient->MassPreview(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: MassPreviewMpVideo
	{
		MassPreviewReq req;
		MassPreviewResp resp;
		req.set_access_token(my_access_token);
		req.set_touser(my_openid);
		req.mutable_mpvideo()->set_media_id( video_media_id );
		req.set_msgtype("mpvideo");
		int ret = pMpClient->MassPreview(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}


	//Sample: MassPreviewMpNews
	{
		MassPreviewReq req;
		MassPreviewResp resp;
		req.set_access_token(my_access_token);
		req.set_touser(my_openid);
		req.mutable_mpnews()->set_media_id( news_media_id );
		req.set_msgtype("mpnews");
		int ret = pMpClient->MassPreview(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: MassPreviewText
	{
		MassPreviewReq req;
		MassPreviewResp resp;
		req.set_access_token(my_access_token);
		req.set_touser(my_openid);
		req.mutable_text()->set_content( "test for text" );
		req.set_msgtype("text");
		int ret = pMpClient->MassPreview(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: MassPreviewVoice
	{
		MassPreviewReq req;
		MassPreviewResp resp;
		req.set_access_token(my_access_token);
		req.set_towxname(my_wxid);
		req.mutable_voice()->set_media_id( voice_media_id );
		req.set_msgtype("voice");
		int ret = pMpClient->MassPreview(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: MassGet
	{
		MassGetReq req;
		MassGetResp resp;
		req.set_access_token(my_access_token);
		req.set_msg_id(my_mass_msg_id);
		int ret = pMpClient->MassGet(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: MassDelete
	{
		MassDeleteReq req;
		MassDeleteResp resp;
		req.set_access_token(my_access_token);
		req.set_msg_id(my_mass_msg_id);
		int ret = pMpClient->MassDelete(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	delete pMpClient;
	//global shutdown once at exit
	MpClient::ShutDown();
}
