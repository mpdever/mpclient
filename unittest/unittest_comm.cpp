#pragma once
#include <string>

#include "gtest/gtest.h"

using std::string;

static string g_appid="";
static string g_secret="";
static string g_token;

//for send msg
static string g_image_media_id="";
static string g_voice_media_id="";
static string g_video_media_id="";
static string g_thumb_media_id="";
static string g_news_media_id="";
static string g_picurl="";
static string g_newsurl="";

//test user
static string g_openid = "";
static string g_wxid = "";

//test user tag
static unsigned long long g_tagid = 0;

//test mass msg id
static unsigned long long g_mass_msg_id = 0;

//test template shor id
static string g_template_id_short = "TM00001";

class GlobalEnv : public testing::Environment {
	public:
		virtual void SetUp() {
			MpClient::GlobalInit();
		}
		virtual void TearDown() {
			MpClient::ShutDown();
		}
};

int main(int argc, char* argv[]) {
	printf("Running main() from unittest_comm.cpp\n");
	testing::AddGlobalTestEnvironment(new GlobalEnv);
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
