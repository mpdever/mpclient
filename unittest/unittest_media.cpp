#include <limits.h>
#include "gtest/gtest.h"

#include "mpclient.h"
#include <string>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include "unittest_comm.cpp"

using std::string;
using namespace std;
using namespace mpcomm;

extern string g_appid;
extern string g_secret;
extern string g_token;

static string tmp_image_id;
static string tmp_voice_id;
static string tmp_video_id;
static string tmp_thumb_id;
static string image_media_id;
static string voice_media_id;
static string video_media_id;
static string thumb_media_id;
static string news_media_id;
static string picurl;
static string newsurl;

int ReadMediaFile(const char * filename, MediaFile * file ){
	ifstream is(filename, ios::in|ios::binary);
	if ( is ){
		is.seekg (0, is.end);
		int length = is.tellg();
		is.seekg (0, is.beg);
		file->mutable_buffer()->resize(length);
		is.read( &(file->mutable_buffer()->at(0)),
				length);
		if ( !is ) return -1;
		is.close();
		file->set_file_name( filename);
		return 0;
	}
	return -1;
}

int WriteMediaFile(MediaFile *file ){
	FILE * out = fopen( file->file_name().c_str(),"wb");
	if ( out ){
		int writesize = fwrite( file->buffer().c_str(), sizeof(char),
				file->buffer().size(), out );
		fclose(out);
		if ( writesize == file->buffer().size() ) return 0;
	}
	return -1;
}


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

TEST(Media, UploadMediaImage){
	UploadMediaReq req;
	UploadMediaResp resp;
	req.set_access_token(g_token);
	req.set_type("image");
	ASSERT_EQ(0,ReadMediaFile("test.jpg",req.mutable_media()));
	int ret = MpClient().UploadMedia(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.media_id().size(), 0);
	tmp_image_id = resp.media_id();
}

TEST(Media, UploadMediaVoice){
	UploadMediaReq req;
	UploadMediaResp resp;
	req.set_access_token(g_token);
	req.set_type("voice");
	ASSERT_EQ(0,ReadMediaFile("test.mp3",req.mutable_media()));
	int ret = MpClient().UploadMedia(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.media_id().size(), 0);
	tmp_voice_id = resp.media_id();
}

TEST(Media, UploadMediaVideo){
	UploadMediaReq req;
	UploadMediaResp resp;
	req.set_access_token(g_token);
	req.set_type("video");
	ASSERT_EQ(0,ReadMediaFile("test.mp4",req.mutable_media()));
	int ret = MpClient().UploadMedia(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.media_id().size(), 0);
	tmp_video_id = resp.media_id();
}

TEST(Media, UploadMediaThumb){
	UploadMediaReq req;
	UploadMediaResp resp;
	req.set_access_token(g_token);
	req.set_type("thumb");
	ASSERT_EQ(0,ReadMediaFile("thumb.jpg",req.mutable_media()));
	int ret = MpClient().UploadMedia(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.thumb_media_id().size(), 0);
	tmp_thumb_id = resp.thumb_media_id();
}

TEST(Media, GetMediaImage){
	GetMediaReq req;
	GetMediaResp resp;
	req.set_access_token(g_token);
	req.set_media_id( tmp_image_id );
	int ret = MpClient().GetMedia(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_LT(0,resp.media().buffer().size());
	ASSERT_LT(0,resp.media().file_name().size());
	ASSERT_EQ(0,WriteMediaFile(resp.mutable_media()));
}

TEST(Media, GetMediaVideo){
	GetMediaReq req;
	GetMediaResp resp;
	req.set_access_token(g_token);
	req.set_media_id( tmp_video_id );
	int ret = MpClient().GetMedia(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	//	ASSERT_GT(0,resp.media().buffer().size());
	//	ASSERT_GT(0,resp.media().file_name().size());
	//	ASSERT_EQ(0,WriteMediaFile(resp.mutable_media()));
}

TEST(Media, GetMediaVoice){
	GetMediaReq req;
	GetMediaResp resp;
	req.set_access_token(g_token);
	req.set_media_id( tmp_voice_id );
	int ret = MpClient().GetMedia(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_LT(0,resp.media().buffer().size());
	ASSERT_LT(0,resp.media().file_name().size());
	ASSERT_EQ(0,WriteMediaFile(resp.mutable_media()));
}

TEST(Media, GetMediaThumb){
	GetMediaReq req;
	GetMediaResp resp;
	req.set_access_token(g_token);
	req.set_media_id( tmp_thumb_id );
	int ret = MpClient().GetMedia(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_LT(0,resp.media().buffer().size());
	ASSERT_LT(0,resp.media().file_name().size());
	ASSERT_EQ(0,WriteMediaFile(resp.mutable_media()));
}

TEST(Media, UploadImg){
	UploadImgReq req;
	UploadImgResp resp;
	req.set_access_token(g_token);
	ASSERT_EQ(0,ReadMediaFile("test.jpg",req.mutable_media()));
	int ret = MpClient().UploadImg(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.url().size(), 0);
}

TEST(Media, UploadNews){
	UploadNewsReq req;
	UploadNewsResp resp;
	NewsItem item;
	item.set_thumb_media_id(tmp_thumb_id);
	item.set_title("1st");
	item.set_digest("this is digest");
	item.set_content("this is content");
	item.set_content_source_url("http://www.qq.com");
	item.set_author("xiaomin");
	item.set_show_cover_pic(1);
	req.add_articles()->Swap(&item);
	item.set_thumb_media_id(tmp_thumb_id);
	item.set_title("2rd");
	item.set_digest("this is digest2");
	item.set_content("this is content2");
	item.set_content_source_url("http://www.qq.com/v2");
	item.set_author("xiaoli");
	item.set_show_cover_pic(0);
	req.add_articles()->Swap(&item);
	req.set_access_token(g_token);
	int ret = MpClient().UploadNews(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.media_id().size(),0);
	news_media_id = resp.media_id();
}

TEST(Media, AddMaterialImage){
	AddMaterialReq req;
	AddMaterialResp resp;
	req.set_access_token(g_token);
	req.set_type("image");
	ASSERT_EQ(0,ReadMediaFile("test.jpg",req.mutable_media()));
	int ret = MpClient().AddMaterial(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.media_id().size(),0);
	image_media_id = resp.media_id();
}

TEST(Media, AddMaterialVoice){
	AddMaterialReq req;
	AddMaterialResp resp;
	req.set_access_token(g_token);
	req.set_type("voice");
	ASSERT_EQ(0,ReadMediaFile("test.mp3",req.mutable_media()));
	int ret = MpClient().AddMaterial(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.media_id().size(),0);
	voice_media_id = resp.media_id();
}


TEST(Media, AddMaterialVideo){
	AddMaterialReq req;
	AddMaterialResp resp;
	req.set_access_token(g_token);
	req.set_type("video");
	req.mutable_description()->set_title("video title");
	req.mutable_description()->set_introduction("video introduce");
	ASSERT_EQ(0,ReadMediaFile("test.mp4",req.mutable_media()));
	int ret = MpClient().AddMaterial(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.media_id().size(),0);
	video_media_id = resp.media_id();
}

TEST(Media, AddMaterialThumb){
	AddMaterialReq req;
	AddMaterialResp resp;
	req.set_access_token(g_token);
	req.set_type("thumb");
	ASSERT_EQ(0,ReadMediaFile("thumb.jpg",req.mutable_media()));
	int ret = MpClient().AddMaterial(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.media_id().size(),0);
	thumb_media_id = resp.media_id();
}

TEST(Media, AddNewsMaterial){
	AddNewsMaterialReq req;
	AddNewsMaterialResp resp;
	NewsItem item;
	item.set_thumb_media_id(thumb_media_id);
	item.set_title("1st");
	item.set_digest("digest1");
	item.set_content("content1");
	item.set_content_source_url("http://www.qq.com");
	item.set_author("lili");
	item.set_show_cover_pic(1);
	req.add_articles()->Swap(&item);
	item.set_thumb_media_id(thumb_media_id);
	item.set_title("2rd");
	item.set_digest("digest2");
	item.set_content("content2");
	item.set_content_source_url("http://www.qq.com/v2");
	item.set_author("leilei");
	item.set_show_cover_pic(0);
	req.add_articles()->Swap(&item);
	req.set_access_token(g_token);
	int ret = MpClient().AddNewsMaterial(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.media_id().size(),0);
	news_media_id = resp.media_id();
}

TEST(Media, GetMaterialNews){
	GetMaterialReq req;
	GetMaterialResp resp;
	req.set_access_token(g_token);
	req.set_media_id(news_media_id);
	int ret = MpClient().GetMaterial(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.news_item_size(), 0);
}



TEST(Media, GetMaterialVideo){
	GetMaterialReq req;
	GetMaterialResp resp;
	req.set_access_token(g_token);
	req.set_media_id(video_media_id);
	int ret = MpClient().GetMaterial(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.title().size(),0);
	ASSERT_GT(resp.description().size(),0);
	ASSERT_GT(resp.down_url().size(),0);
}

TEST(Media, GetMaterialImage){
	GetMaterialReq req;
	GetMaterialResp resp;
	req.set_access_token(g_token);
	req.set_media_id(image_media_id);
	int ret = MpClient().GetMaterial(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_LT(0,resp.media().buffer().size());
	ASSERT_LT(0,resp.media().file_name().size());
	ASSERT_EQ(0,WriteMediaFile(resp.mutable_media()));
}

TEST(Media, GetMaterialVoice){
	GetMaterialReq req;
	GetMaterialResp resp;
	req.set_access_token(g_token);
	req.set_media_id(voice_media_id);
	int ret = MpClient().GetMaterial(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_LT(0,resp.media().buffer().size());
	ASSERT_LT(0,resp.media().file_name().size());
	ASSERT_EQ(0,WriteMediaFile(resp.mutable_media()));
}

TEST(Media, GetMaterialThumb){
	GetMaterialReq req;
	GetMaterialResp resp;
	req.set_access_token(g_token);
	req.set_media_id(thumb_media_id);
	int ret = MpClient().GetMaterial(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_LT(0,resp.media().buffer().size());
	ASSERT_LT(0,resp.media().file_name().size());
	ASSERT_EQ(0,WriteMediaFile(resp.mutable_media()));
}

TEST(Media, UpdateNews){
	UpdateNewsReq req;
	UpdateNewsResp resp;
	req.set_access_token(g_token);
	req.set_media_id(news_media_id);
	req.set_index(0);
	NewsItem item;
	item.set_thumb_media_id(thumb_media_id);
	item.set_title("3");
	item.set_digest("digest new 3");
	item.set_content("content 3");
	item.set_content_source_url("http://www.qq.com/3");
	item.set_author("j");
	item.set_show_cover_pic(1);
	req.mutable_articles()->Swap(&item);
	int ret = MpClient().UpdateNews(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}

TEST(Media, GetMaterialCount){
	GetMaterialCountReq req;
	GetMaterialCountResp resp;
	req.set_access_token(g_token);
	int ret = MpClient().GetMaterialCount(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.voice_count(), 0);
	ASSERT_GT(resp.video_count(), 0);
	ASSERT_GT(resp.image_count(), 0);
	ASSERT_GT(resp.news_count(), 0);
}

TEST(Media, BatchGetMaterialNews){
	BatchGetMaterialReq req;
	BatchGetMaterialResp resp;
	req.set_access_token(g_token);
	req.set_type("news");
	req.set_offset(0);
	req.set_count(2);
	int ret = MpClient().BatchGetMaterial(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.total_count(), 0);
	ASSERT_GT(resp.item_count(), 0);
	ASSERT_GT(resp.item_size(), 0);

}

TEST(Media, BatchGetMaterialImage){
	BatchGetMaterialReq req;
	BatchGetMaterialResp resp;
	req.set_access_token(g_token);
	req.set_type("image");
	req.set_offset(0);
	req.set_count(2);
	int ret = MpClient().BatchGetMaterial(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.total_count(), 0);
	ASSERT_GT(resp.item_count(), 0);
	ASSERT_GT(resp.item_size(), 0);
}

TEST(Media, BatchGetMaterialVideo){
	BatchGetMaterialReq req;
	BatchGetMaterialResp resp;
	req.set_access_token(g_token);
	req.set_type("video");
	req.set_offset(0);
	req.set_count(2);
	int ret = MpClient().BatchGetMaterial(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.total_count(), 0);
	ASSERT_GT(resp.item_count(), 0);
	ASSERT_GT(resp.item_size(), 0);
}

TEST(Media, BatchGetMaterialVoice){
	BatchGetMaterialReq req;
	BatchGetMaterialResp resp;
	req.set_access_token(g_token);
	req.set_type("voice");
	req.set_offset(0);
	req.set_count(2);
	int ret = MpClient().BatchGetMaterial(req,&resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
	ASSERT_GT(resp.total_count(), 0);
	ASSERT_GT(resp.item_count(), 0);
	ASSERT_GT(resp.item_size(), 0);
}

TEST(Media, DelMaterial){
	DelMaterialReq req;
	DelMaterialResp resp;
	req.set_access_token(g_token);
	req.set_media_id( voice_media_id );
	int ret = MpClient().DelMaterial(req, &resp);
	ASSERT_EQ(0,ret);
	ASSERT_EQ(0,resp.errcode());
}
