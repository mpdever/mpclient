#pragma once

#ifndef _MP_CLIENT_JSON_HELPER_H_
#define _MP_CLIENT_JSON_HELPER_H_

#include "mpcomm.h"

#include <string>
#include <set>

#include "rapidjson/document.h"


namespace google{
	namespace protobuf{
		class Message;
		class FieldDescriptor;
	}
}

class SimpleLogger;

class JsonHelper{
	MP_DISALLOW_COPY_AND_ASSIGN(JsonHelper);
	public:
		JsonHelper( SimpleLogger * pLogger = NULL );
		virtual ~JsonHelper();
		void pb2json( const google::protobuf::Message & msg, std::string 
				* pJson, const std::set<std::string> * pSkippedField = NULL );
		int json2pb( const std::string & json, google::protobuf::Message * pMsg );
		bool validateJson( const std::string & json, std::string * pErrmsg );
	private:
		rapidjson::Value * msg2rapidjson( const google::protobuf::Message & msg,
				rapidjson::Value::AllocatorType & allocator,
				const std::set<std::string> * pSkippedField = NULL );
		rapidjson::Value * field2rapidjson( const google::protobuf::Message & msg,
				const google::protobuf::FieldDescriptor * field,
				rapidjson::Value::AllocatorType & allocator );
		void rapidjson2string( const rapidjson::Value & json, 
				std::string * jsonstring );

		int rapidjson2msg( const rapidjson::Value & json, 
				google::protobuf::Message * pMsg ); 
		int rapidjson2msgField(const rapidjson::Value & json,
				google::protobuf::Message * pMsg, 
				const google::protobuf::FieldDescriptor * field );
	private:
		SimpleLogger * m_pLogger;
};

#endif
////_MP_CLIENT_JSON_HELPER_H_
