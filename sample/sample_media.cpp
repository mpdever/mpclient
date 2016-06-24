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
	string my_tmp_image_media_id;
	string my_tmp_voice_media_id;
	string my_tmp_video_media_id;
	string my_tmp_thumb_media_id;
	string image_media_id;
	string voice_media_id;
	string video_media_id;
	string thumb_media_id;
	string news_media_id;

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

	delete pMpClient;
	//global shutdown once at exit
	MpClient::ShutDown();
}
