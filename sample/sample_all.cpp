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
	string my_template_id_short = "TM00001";
	unsigned long long  my_tagid = 103;
	unsigned long long  my_conditional_menuid;
	unsigned long long  my_mass_msg_id;
	string my_tmp_image_media_id;
	string my_tmp_voice_media_id;
	string my_tmp_video_media_id;
	string my_tmp_thumb_media_id;
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

	//Sample: UploadMediaImage
	{
		UploadMediaReq req;
		UploadMediaResp resp;
		req.set_access_token(my_access_token);
		req.set_type("image");
		//open image file
		ifstream is("test.jpg", ios::in|ios::binary);
		if ( is ){
			is.seekg (0, is.end);
			int length = is.tellg();                                                                                      
			is.seekg (0, is.beg);                                                                                         
			req.mutable_media()->mutable_buffer()->resize(length);                                                        
			//we read the file to req.media().buffer()
			//or
			//req.mutable_media()->set_buffer(src_buffer);
			is.read( &(req.mutable_media()->mutable_buffer()->at(0)),
					length);                                            
			is.close();
			req.mutable_media()->set_file_name( "test.jpg");
		}
		int ret = pMpClient->UploadMedia(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
		cout<<resp.media_id()<<endl;
	}

	//Sample: UploadMediaVoice
	{
		UploadMediaReq req;
		UploadMediaResp resp;
		req.set_access_token(my_access_token);
		req.set_type("voice");
		ifstream is("test.mp3", ios::in|ios::binary);
		if ( is ){
			is.seekg (0, is.end);
			int length = is.tellg();                                                                                      
			is.seekg (0, is.beg);                                                                                         
			req.mutable_media()->mutable_buffer()->resize(length);                                                        
			is.read( &(req.mutable_media()->mutable_buffer()->at(0)),
					length);                                            
			is.close();
			req.mutable_media()->set_file_name( "test.mp3");
		}
		int ret = pMpClient->UploadMedia(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
		cout<<resp.media_id()<<endl;
	}

	//Sample: UploadMediaVideo
	{
		UploadMediaReq req;
		UploadMediaResp resp;
		req.set_access_token(my_access_token);
		req.set_type("video");
		ifstream is("test.mp4", ios::in|ios::binary);
		if ( is ){
			is.seekg (0, is.end);
			int length = is.tellg();                                                                                      
			is.seekg (0, is.beg);                                                                                         
			req.mutable_media()->mutable_buffer()->resize(length);                                                        
			is.read( &(req.mutable_media()->mutable_buffer()->at(0)),
					length);                                            
			is.close();
			req.mutable_media()->set_file_name( "test.mp4");
		}
		int ret = pMpClient->UploadMedia(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
		cout<<resp.media_id()<<endl;
	}

	//Sample: UploadMediaThumb
	{
		UploadMediaReq req;
		UploadMediaResp resp;
		req.set_access_token(my_access_token);
		req.set_type("thumb");
		ifstream is("thumb.jpg", ios::in|ios::binary);
		if ( is ){
			is.seekg (0, is.end);
			int length = is.tellg();                                                                                      
			is.seekg (0, is.beg);                                                                                         
			req.mutable_media()->mutable_buffer()->resize(length);                                                        
			is.read( &(req.mutable_media()->mutable_buffer()->at(0)),
					length);                                            
			is.close();
			req.mutable_media()->set_file_name( "thumb.jpg");
		}
		int ret = pMpClient->UploadMedia(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
		cout<<resp.media_id()<<endl;
	}

	//Sample: GetMediaImage
	{
		GetMediaReq req;
		GetMediaResp resp;
		req.set_access_token(my_access_token);
		req.set_media_id( my_tmp_image_media_id );
		int ret = pMpClient->GetMedia(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
		//store in file
		std::ofstream outfile(resp.media().file_name().c_str(), std::ofstream::binary);
		outfile.write(resp.media().buffer().c_str(), resp.media().buffer().size());
		outfile.close();
	}

	//Sample: GetMediaVideo
	{
		GetMediaReq req;
		GetMediaResp resp;
		req.set_access_token(my_access_token);
		req.set_media_id( my_tmp_video_media_id );
		int ret = pMpClient->GetMedia(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}
	//Sample: GetMediaVoice
	{
		GetMediaReq req;
		GetMediaResp resp;
		req.set_access_token(my_access_token);
		req.set_media_id( my_tmp_voice_media_id );
		int ret = pMpClient->GetMedia(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
		std::ofstream outfile(resp.media().file_name().c_str(), std::ofstream::binary);
		outfile.write(resp.media().buffer().c_str(), resp.media().buffer().size());
		outfile.close();
	}
	//Sample: GetMediaThumb
	{
		GetMediaReq req;
		GetMediaResp resp;
		req.set_access_token(my_access_token);
		req.set_media_id( my_tmp_thumb_media_id );
		int ret = pMpClient->GetMedia(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
		std::ofstream outfile(resp.media().file_name().c_str(), std::ofstream::binary);
		outfile.write(resp.media().buffer().c_str(), resp.media().buffer().size());
		outfile.close();
	}

	//Sample: UploadImg
	{
		UploadImgReq req;
		UploadImgResp resp;
		req.set_access_token(my_access_token);
		req.mutable_media()->set_file_name( "test.jpg");
		ifstream is("test.jpg", ios::in|ios::binary);
		if ( is ){
			is.seekg (0, is.end);
			int length = is.tellg();
			is.seekg (0, is.beg);
			req.mutable_media()->mutable_buffer()->resize(length);
			is.read( &(req.mutable_media()->mutable_buffer()->at(0)),length );
			is.close();
		}
		int ret = pMpClient->UploadImg(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: UploadNews
	{
		UploadNewsReq req;
		UploadNewsResp resp;
		NewsItem item;
		item.set_thumb_media_id(my_tmp_thumb_media_id);
		item.set_title("1st title");
		item.set_digest("digest 1");
		item.set_content("content 1");
		item.set_content_source_url("http://www.qq.com");
		item.set_author("hi");
		item.set_show_cover_pic(1);
		req.add_articles()->Swap(&item);
		item.set_thumb_media_id(my_tmp_thumb_media_id);
		item.set_title("title2");
		item.set_digest("this is digest 2");
		item.set_content("content 2");
		item.set_content_source_url("http://www.qq.com/v2");
		item.set_author("ng");
		item.set_show_cover_pic(0);
		req.add_articles()->Swap(&item);
		req.set_access_token(my_access_token);
		int ret = pMpClient->UploadNews(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: AddMaterialImage
	{
		AddMaterialReq req;
		AddMaterialResp resp;
		req.set_access_token(my_access_token);
		string filename("test.jpg");
		req.set_type("image");
		req.mutable_media()->set_file_name( filename );
		ifstream is(filename.c_str(), ios::in|ios::binary);
		if ( is ){
			is.seekg (0, is.end);
			int length = is.tellg();
			is.seekg (0, is.beg);
			req.mutable_media()->mutable_buffer()->resize(length);
			is.read( &(req.mutable_media()->mutable_buffer()->at(0)),length );
			is.close();
		}
		int ret = pMpClient->AddMaterial(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: AddMaterialVoice
	{
		AddMaterialReq req;
		AddMaterialResp resp;
		req.set_access_token(my_access_token);
		string filename("test.mp3");
		req.set_type("voice");
		req.mutable_media()->set_file_name( filename );
		ifstream is(filename.c_str(), ios::in|ios::binary);
		if ( is ){
			is.seekg (0, is.end);
			int length = is.tellg();
			is.seekg (0, is.beg);
			req.mutable_media()->mutable_buffer()->resize(length);
			is.read( &(req.mutable_media()->mutable_buffer()->at(0)),length );
			is.close();
		}
		int ret = pMpClient->AddMaterial(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: AddMaterialVideo
	{
		AddMaterialReq req;
		AddMaterialResp resp;
		req.set_access_token(my_access_token);
		string filename("test.mp4");
		req.set_type("video");
		req.mutable_media()->set_file_name( filename );
		req.mutable_description()->set_title("test title");
		req.mutable_description()->set_introduction("test introduce");
		ifstream is(filename.c_str(), ios::in|ios::binary);
		if ( is ){
			is.seekg (0, is.end);
			int length = is.tellg();
			is.seekg (0, is.beg);
			req.mutable_media()->mutable_buffer()->resize(length);
			is.read( &(req.mutable_media()->mutable_buffer()->at(0)),length );
			is.close();
		}
		int ret = pMpClient->AddMaterial(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: AddMaterialThumb
	{
		AddMaterialReq req;
		AddMaterialResp resp;
		req.set_access_token(my_access_token);
		string filename("thumb.jpg");
		req.set_type("thumb");
		req.mutable_media()->set_file_name( filename );
		ifstream is(filename.c_str(), ios::in|ios::binary);
		if ( is ){
			is.seekg (0, is.end);
			int length = is.tellg();
			is.seekg (0, is.beg);
			req.mutable_media()->mutable_buffer()->resize(length);
			is.read( &(req.mutable_media()->mutable_buffer()->at(0)),length );
			is.close();
		}
		int ret = pMpClient->AddMaterial(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}


	//Sample: AddNewsMaterial
	{
		AddNewsMaterialReq req;
		AddNewsMaterialResp resp;
		NewsItem item;
		item.set_thumb_media_id(thumb_media_id);
		item.set_title("title1");
		item.set_digest("digest1");
		item.set_content("content1");
		item.set_content_source_url("http://www.qq.com");
		item.set_author("im");
		item.set_show_cover_pic(1);
		req.add_articles()->Swap(&item);
		item.set_thumb_media_id(thumb_media_id);
		item.set_title("second");
		item.set_digest("digest2");
		item.set_content("content2");
		item.set_content_source_url("http://www.qq.com/v2");
		item.set_author("ng");
		item.set_show_cover_pic(0);
		req.add_articles()->Swap(&item);
		req.set_access_token(my_access_token);
		int ret = pMpClient->AddNewsMaterial(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: GetMaterialNews
	{

		GetMaterialReq req;
		GetMaterialResp resp;
		req.set_access_token(my_access_token);
		req.set_media_id(news_media_id);
		int ret = pMpClient->GetMaterial(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: GetMaterialVideo
	{

		GetMaterialReq req;
		GetMaterialResp resp;
		req.set_access_token(my_access_token);
		req.set_media_id(video_media_id);
		int ret = pMpClient->GetMaterial(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: GetMaterialImage
	{

		GetMaterialReq req;
		GetMaterialResp resp;
		req.set_access_token(my_access_token);
		req.set_media_id(image_media_id);
		int ret = pMpClient->GetMaterial(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
		std::ofstream outfile(resp.media().file_name().c_str(), std::ofstream::binary);
		outfile.write(resp.media().buffer().c_str(), resp.media().buffer().size());
		outfile.close();
	}

	//Sample: GetMaterialVoice
	{

		GetMaterialReq req;
		GetMaterialResp resp;
		req.set_access_token(my_access_token);
		req.set_media_id(voice_media_id);
		int ret = pMpClient->GetMaterial(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
		std::ofstream outfile(resp.media().file_name().c_str(), std::ofstream::binary);
		outfile.write(resp.media().buffer().c_str(), resp.media().buffer().size());
		outfile.close();
	}

	//Sample: UpdateNews
	{
		UpdateNewsReq req;
		UpdateNewsResp resp;
		req.set_access_token(my_access_token);
		req.set_media_id(news_media_id);
		req.set_index(0);

		NewsItem item;
		item.set_thumb_media_id(thumb_media_id);
		item.set_title("the title");
		item.set_digest("the digest");
		item.set_content("the content");
		item.set_content_source_url("http://www.qq.com/");
		item.set_author("m3");
		item.set_show_cover_pic(1);
		req.mutable_articles()->Swap(&item);
		int ret = pMpClient->UpdateNews(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: GetMaterialCount
	{
		GetMaterialCountReq req;
		GetMaterialCountResp resp;
		req.set_access_token(my_access_token);

		int ret = pMpClient->GetMaterialCount(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
		cout<<resp.image_count()<<endl;
	}

	//Sample: BatchGetMaterialNews
	{
		BatchGetMaterialReq req;
		BatchGetMaterialResp resp;
		req.set_access_token(my_access_token);
		req.set_type("news");
		req.set_offset(0);
		req.set_count(2);
		int ret = pMpClient->BatchGetMaterial(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//Sample: BatchGetMaterialImage 
	{
		BatchGetMaterialReq req;
		BatchGetMaterialResp resp;
		req.set_access_token(my_access_token);
		req.set_type("image");
		req.set_offset(0);
		req.set_count(2);

		int ret = pMpClient->BatchGetMaterial(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
		cout<<resp.item_count()<<endl;
	}

	//Sample: BatchGetMaterialVideo
	{
		BatchGetMaterialReq req;
		BatchGetMaterialResp resp;
		req.set_access_token(my_access_token);
		req.set_type("video");
		req.set_offset(0);
		req.set_count(2);

		int ret = pMpClient->BatchGetMaterial(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
		cout<<resp.item_count()<<endl;
	}

	//Sample: BatchGetMaterialVoice
	{
		BatchGetMaterialReq req;
		BatchGetMaterialResp resp;
		req.set_access_token(my_access_token);
		req.set_type("voice");
		req.set_offset(0);
		req.set_count(2);

		int ret = pMpClient->BatchGetMaterial(req,&resp);
		if ( ret || resp.errcode() ){
			//error
		}
		cout<<resp.item_count()<<endl;
	}

	//Sample: DelMaterial
	{
		DelMaterialReq req;
		DelMaterialResp resp;
		req.set_access_token(my_access_token);
		req.set_media_id( voice_media_id );
		int ret = pMpClient->DelMaterial(req, &resp);
		if ( ret || resp.errcode() ){
			//error
		}
	}

	//===message

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

	//===userinfo


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


	//============

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
