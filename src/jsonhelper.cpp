#include "jsonhelper.h"
#include "simplelogger.h"

#include <iostream>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"

using namespace mpcomm;

////////////////////

JsonHelper::JsonHelper( SimpleLogger * pLogger ):m_pLogger(pLogger){
}

JsonHelper::~JsonHelper(){
}

void JsonHelper::pb2json( const google::protobuf::Message & msg, std::string *
		pJson, const std::set<std::string> * pSkippedField ){
	rapidjson::Value::AllocatorType allocator;
	rapidjson::Value* json = msg2rapidjson(msg, allocator, pSkippedField);
	if ( json ){
		rapidjson2string(*json, pJson);
		delete json;
	}else{
		if (m_pLogger) 
			SIMPLELOGERR(*m_pLogger,"ERR json is NULL");
	}
}

void JsonHelper::rapidjson2string( const rapidjson::Value & json, 
		std::string * jsonstring ) {
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	json.Accept(writer);
	jsonstring->append(buffer.GetString(), buffer.GetSize());
}

rapidjson::Value * JsonHelper::msg2rapidjson( const google::protobuf::Message & msg,
		rapidjson::Value::AllocatorType & allocator,
		const std::set<std::string> * pSkippedField ){
	const google::protobuf::Descriptor * pDescriptor = msg.GetDescriptor();
	if ( !pDescriptor ) {
		if (m_pLogger) SIMPLELOGERR(*m_pLogger,"[PROTOBUF] GetDescriptor fail");
		return NULL;
	}
	const google::protobuf::Reflection * pReflection = msg.GetReflection();
	if ( !pReflection ) {
		if (m_pLogger) SIMPLELOGERR(*m_pLogger,"[PROTOBUF] GetReflection fail");
		return NULL;
	}
	if ( msg.GetTypeName() == "mpcomm.Json" ){
		if (m_pLogger) SIMPLELOGDEBUG(*m_pLogger,"Special Field: Json Field");
		const google::protobuf::FieldDescriptor * field =
			pDescriptor->FindFieldByName("content");
		if ( !field ) {
			if (m_pLogger) SIMPLELOGERR(*m_pLogger,
					"[PROTOBUF] FindFieldByName[Json.content] fail");
			return NULL;
		}
		std::string tmp;
		const std::string & content = 
			pReflection->GetStringReference( msg, field, &tmp);
		rapidjson::Document * pDoc = new rapidjson::Document(&allocator);
		pDoc->Parse<0>(content.c_str());
		if (pDoc->HasParseError()) {
			if (m_pLogger) SIMPLELOGERR(*m_pLogger, 
					"[JSON] invalid json string, parse fail %s", 
					GetParseError_En(pDoc->GetParseError()));
			delete pDoc, pDoc = NULL;
		}
		return pDoc;
	}
	rapidjson::Value * root = 
		new rapidjson::Value(rapidjson::kObjectType);
	if ( !root ) return NULL;
	size_t count = pDescriptor->field_count();
	for ( size_t i = 0; i < count; ++i ){
		const google::protobuf::FieldDescriptor * field = pDescriptor->field(i);
		if (!field) {
			if (m_pLogger) SIMPLELOGERR(*m_pLogger,"[PROTOBUF] get field fail");
			delete root, root = NULL;
			return NULL;
		}
		if (pSkippedField && pSkippedField->count( field->name() ) ){
			if (m_pLogger) SIMPLELOGINFO(*m_pLogger,"Skip Field[%s]", field->name().c_str() );
			continue;
		}
		if (field->is_optional() && !pReflection->HasField(msg, field)) {
			if (m_pLogger) SIMPLELOGINFO(*m_pLogger,"Empty Field[%s]", field->name().c_str() );
		}
		else {
			if (m_pLogger) SIMPLELOGDEBUG(*m_pLogger,"Process field[%s]", field->name().c_str() );
			rapidjson::Value* field_json = field2rapidjson(msg, field, allocator);
			rapidjson::Value field_name(field->name().c_str(), field->name().size());
			if ( field_json ){
				root->AddMember(field_name, *field_json, allocator);
				delete field_json;
			}
			else{
				if (m_pLogger) SIMPLELOGERR(*m_pLogger,
						"Process field[%s] fail", field->name().c_str() );
			}
		}
	}
	return root;

}

rapidjson::Value * JsonHelper::field2rapidjson( const google::protobuf::Message & msg,
		const google::protobuf::FieldDescriptor * field,
		rapidjson::Value::AllocatorType & allocator ){
	const google::protobuf::Reflection * ref = msg.GetReflection();
	if ( !ref ){
		if (m_pLogger) SIMPLELOGERR(*m_pLogger,"[PROTOBUF] GetReflection fail");
	}
	const bool repeated = field->is_repeated();
	size_t array_size = 0;
	if (repeated) {
		array_size = ref->FieldSize(msg, field);
	}
	rapidjson::Value* json = NULL;
	if (repeated) {
		json = new rapidjson::Value(rapidjson::kArrayType);
	}
	if (m_pLogger) SIMPLELOGDEBUG(*m_pLogger,
			"Field type[%d] is_repeated[%d]", field->cpp_type(),
			(int)repeated);
	switch (field->cpp_type()) {
		case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
			if (repeated) {
				for (size_t i = 0; i != array_size; ++i) {
					double value = ref->GetRepeatedDouble(msg, field, i);
					rapidjson::Value v(value);
					json->PushBack(v, allocator);
				}
			} else {
				json = new rapidjson::Value(ref->GetDouble(msg, field));
			}
			break;
		case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
			if (repeated) {
				for (size_t i = 0; i != array_size; ++i) {
					float value = ref->GetRepeatedFloat(msg, field, i);
					rapidjson::Value v(value);
					json->PushBack(v, allocator);
				}
			} else {
				json = new rapidjson::Value(ref->GetFloat(msg, field));
			}
			break;
		case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
			if (repeated) {
				for (size_t i = 0; i != array_size; ++i) {
					int64_t value = ref->GetRepeatedInt64(msg, field, i);
					rapidjson::Value v(value);
					json->PushBack(v, allocator);
				}
			} else {
				json = new rapidjson::Value(ref->GetInt64(msg, field));
			}
			break;
		case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
			if (repeated) {
				for (size_t i = 0; i != array_size; ++i) {
					uint64_t value = ref->GetRepeatedUInt64(msg, field, i);
					rapidjson::Value v(value);
					json->PushBack(v, allocator);
				}
			} else {
				json = new rapidjson::Value(ref->GetUInt64(msg, field));
			}
			break;
		case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
			if (repeated) {
				for (size_t i = 0; i != array_size; ++i) {
					int32_t value = ref->GetRepeatedInt32(msg, field, i);
					rapidjson::Value v(value);
					json->PushBack(v, allocator);
				}
			} else {
				json = new rapidjson::Value(ref->GetInt32(msg, field));
			}
			break;
		case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
			if (repeated) {
				for (size_t i = 0; i != array_size; ++i) {
					uint32_t value = ref->GetRepeatedUInt32(msg, field, i);
					rapidjson::Value v(value);
					json->PushBack(v, allocator);
				}
			} else {
				json = new rapidjson::Value(ref->GetUInt32(msg, field));
			}
			break;
		case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
			if (repeated) {
				for (size_t i = 0; i != array_size; ++i) {
					bool value = ref->GetRepeatedBool(msg, field, i);
					rapidjson::Value v(value);
					json->PushBack(v, allocator);
				}
			} else {
				json = new rapidjson::Value(ref->GetBool(msg, field));
			}
			break;
		case google::protobuf::FieldDescriptor::CPPTYPE_STRING: {
				if (repeated) {
					for (size_t i = 0; i != array_size; ++i) {
						std::string value = ref->GetRepeatedString(msg, field, i);
						rapidjson::Value v(value.c_str(), value.size());
						json->PushBack(v, allocator);
					}
				} else {
					std::string value = ref->GetString(msg, field);
					json = new rapidjson::Value(value.c_str(), value.size(), allocator);
				}
				break;
			}
		case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
			if (repeated) {
				for (size_t i = 0; i != array_size; ++i) {
					const google::protobuf::Message & value = 
						ref->GetRepeatedMessage(msg, field, i);
					rapidjson::Value * v = msg2rapidjson(value, allocator);
					if (v){
						json->PushBack(*v, allocator);
						delete v;
					}
				}
			}
			else {
				const google::protobuf::Message & value = 
					ref->GetMessage(msg, field);
				json = msg2rapidjson(value, allocator);
			}
			break;
		case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
			if (repeated) {
				for (size_t i = 0; i != array_size; ++i) {
					const google::protobuf::EnumValueDescriptor* value 
						= ref->GetRepeatedEnum(msg, field, i);
					rapidjson::Value v(value->number());
					json->PushBack(v, allocator);
				}
			} else {
				json = new rapidjson::Value(ref->GetEnum(msg, field)->number());
			}
			break;
		default:
			if (m_pLogger) SIMPLELOGERR(*m_pLogger,
					"[PROTOBUF] Unknown field type[%d]", field->cpp_type());
			break;
	}
	return json;
}

bool JsonHelper::validateJson( const std::string & json, 
		std::string * pErrmsg ) {
	rapidjson::Document doc;
	doc.Parse<0>(json.c_str());
	if (doc.HasParseError()){
		pErrmsg->append(GetParseError_En(doc.GetParseError()));
		return false;
	}
	return true;
}

int JsonHelper::json2pb( const std::string & json, 
		google::protobuf::Message * pMsg ){
	rapidjson::Document doc;
	doc.Parse<0>(json.c_str());
	if (doc.HasParseError()){
		if (m_pLogger) SIMPLELOGERR(*m_pLogger,"Parse json fail, errmsg[%s]", 
				GetParseError_En(doc.GetParseError()));
		return ErrInvalidJsonStr;
	}
	int ret = rapidjson2msg(doc, pMsg);
	return ret;
}

int JsonHelper::rapidjson2msg( const rapidjson::Value & json,
		google::protobuf::Message * pMsg ) {
	if (json.GetType() != rapidjson::kObjectType) {
		if (m_pLogger) SIMPLELOGERR(*m_pLogger,
				"Param type mismatch[%d], not a object", json.GetType() );
		return ErrJsonPbMismatch;
	}
	const google::protobuf::Descriptor * pDescriptor = 
		pMsg->GetDescriptor();
	if ( !pDescriptor ){
		if (m_pLogger) SIMPLELOGERR(*m_pLogger,"[PROTOBUF] GetDescriptor fail");
		return ErrProtobuf;
	}
	const google::protobuf::Reflection * ref = pMsg->GetReflection();
	if ( !ref ){
		if (m_pLogger) SIMPLELOGERR(*m_pLogger,"[PROTOBUF] GetReflection fail");
		return ErrProtobuf;
	}
	for ( rapidjson::Value::ConstMemberIterator it = json.MemberBegin(); 
			it != json.MemberEnd(); ++it ){
		const char * name = it->name.GetString();
		const google::protobuf::FieldDescriptor * field = 
			pDescriptor->FindFieldByName(name);
		if (!field){
			if (m_pLogger) SIMPLELOGERR(*m_pLogger,"FindFieldByName[%s] fail", name );
			field = ref->FindKnownExtensionByName(name);
		}
		if (!field){
			if (m_pLogger) SIMPLELOGERR(*m_pLogger,
					"FindKnownExtensionByName[%s] fail", name );
			continue; 
		}
			//TODO add to unknown field;
		if (m_pLogger) SIMPLELOGDEBUG(*m_pLogger,
				"Process field[%s] is_repeated[%d]", name,
				(int)field->is_repeated() );
		if (it->value.GetType() == rapidjson::kNullType) {
			ref->ClearField(pMsg, field);
			continue;
		}
		if (field->is_repeated()) {
			if (it->value.GetType() != rapidjson::kArrayType){
				if (m_pLogger) SIMPLELOGERR(*m_pLogger,
						"Field[%s] mismatch, not a array", name);
				return ErrJsonPbMismatch;
			}
			for ( rapidjson::Value::ConstValueIterator inner_it = it->value.Begin(); 
					inner_it != it->value.End(); ++inner_it ) {
				int ret = rapidjson2msgField(*inner_it, pMsg, field);
				if (ret != 0) {
					if (m_pLogger) SIMPLELOGERR(*m_pLogger,
							"Field[%s] rapidjson2msgField array", name);
					return ret;
				}
			}
		}
		else {
			int ret = rapidjson2msgField(it->value, pMsg, field);
			if (ret != 0) {
				if (m_pLogger) SIMPLELOGERR(*m_pLogger,
						"Field[%s] rapidjson2msgField array", name);
				return ret;
			}
		}
	}
	return 0;
}

int JsonHelper::rapidjson2msgField(const rapidjson::Value & json,
		google::protobuf::Message * pMsg, 
		const google::protobuf::FieldDescriptor * field ){
	const google::protobuf::Reflection * ref = pMsg->GetReflection();
	const bool repeated = field->is_repeated();
	switch (field->cpp_type()) {
		case google::protobuf::FieldDescriptor::CPPTYPE_INT32: 
			{
				if (json.GetType() != rapidjson::kNumberType) {
					if (m_pLogger) SIMPLELOGERR(*m_pLogger,
							"field mismatch: not a number");
					return ErrJsonPbMismatch;
				}
				if (repeated) {
					ref->AddInt32(pMsg, field, (int32_t)json.GetInt());
				}
				else {
					ref->SetInt32(pMsg, field, (int32_t)json.GetInt());
				}
				break;
			}
		case google::protobuf::FieldDescriptor::CPPTYPE_UINT32: 
			{
				if (json.GetType() != rapidjson::kNumberType) {
					if (m_pLogger) SIMPLELOGERR(*m_pLogger,
							"field mismatch: not a number");
					return ErrJsonPbMismatch;
				}
				if (repeated) {
					ref->AddUInt32(pMsg, field, json.GetUint());
				}
				else {
					ref->SetUInt32(pMsg, field, json.GetUint());
				}
				break;
			}
		case google::protobuf::FieldDescriptor::CPPTYPE_INT64: 
			{
				if (json.GetType() != rapidjson::kNumberType) {
					if (m_pLogger) SIMPLELOGERR(*m_pLogger,
							"field mismatch: not a number");
					return ErrJsonPbMismatch;
				}
				if (repeated) {
					ref->AddInt64(pMsg, field, json.GetInt64());
				}
				else {
					ref->SetInt64(pMsg, field, json.GetInt64());
				}
				break;
			}
		case google::protobuf::FieldDescriptor::CPPTYPE_UINT64: 
			{
				if (json.GetType() != rapidjson::kNumberType) {
					if (m_pLogger) SIMPLELOGERR(*m_pLogger,
							"field mismatch: not a number");
					return ErrJsonPbMismatch;
				}
				if (repeated) {
					ref->AddUInt64(pMsg, field, json.GetUint64());
				}
				else {
					ref->SetUInt64(pMsg, field, json.GetUint64());
				}
				break;
			}
		case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE: 
			{
				if (json.GetType() != rapidjson::kNumberType) {
					if (m_pLogger) SIMPLELOGERR(*m_pLogger,
							"field mismatch: not a number");
					return ErrJsonPbMismatch;
				}
				if (repeated) {
					ref->AddDouble(pMsg, field, json.GetDouble());
				}
				else {
					ref->SetDouble(pMsg, field, json.GetDouble());
				}
				break;
			}
		case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT: 
			{
				if (json.GetType() != rapidjson::kNumberType) {
					if (m_pLogger) SIMPLELOGERR(*m_pLogger,
							"field mismatch: not a number");
					return ErrJsonPbMismatch;
				}
				if (repeated) {
					ref->AddFloat(pMsg, field, json.GetDouble());
				}
				else {
					ref->SetFloat(pMsg, field, json.GetDouble());
				}
				break;
			}
		case google::protobuf::FieldDescriptor::CPPTYPE_BOOL: 
			{
				if (json.GetType() != rapidjson::kTrueType && 
						json.GetType() != rapidjson::kFalseType) {
					if (m_pLogger) SIMPLELOGERR(*m_pLogger,
							"field mismatch: not a boolean");
					return ErrJsonPbMismatch;
				}
				bool v = json.GetBool();
				if (repeated) {
					ref->AddBool(pMsg, field, v);
				}
				else {
					ref->SetBool(pMsg, field, v);
				}
				break;
			}
		case google::protobuf::FieldDescriptor::CPPTYPE_STRING: 
			{
				if (json.GetType() != rapidjson::kStringType) {
					if (m_pLogger) SIMPLELOGERR(*m_pLogger,
							"field mismatch: not a string");
					return ErrJsonPbMismatch;
				}
				const char* value = json.GetString();
				uint32_t str_size = json.GetStringLength();
				std::string str_value(value, str_size);
				if (repeated) {
					ref->AddString(pMsg, field, str_value);
				} else {
					ref->SetString(pMsg, field, str_value);
				}
				break;
			}
		case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE: 
			{
				google::protobuf::Message * pInnerMsg = 
					(repeated) ? ref->AddMessage(pMsg, field) 
					: ref->MutableMessage(pMsg, field);
				return rapidjson2msg(json, pInnerMsg);
			}
		case google::protobuf::FieldDescriptor::CPPTYPE_ENUM: 
			{
				const google::protobuf::EnumDescriptor *ed = field->enum_type();
				const google::protobuf::EnumValueDescriptor *ev = 0;
				if (json.GetType() == rapidjson::kNumberType) {
					ev = ed->FindValueByNumber(json.GetInt());
				}
				else if (json.GetType() == rapidjson::kStringType) {
					ev = ed->FindValueByName(json.GetString());
				}
				else{
					if (m_pLogger) SIMPLELOGERR(*m_pLogger,
							"field mismatch: not a string/number");
					return ErrJsonPbMismatch;
				}
				if (!ev){
					if (m_pLogger) SIMPLELOGERR(*m_pLogger,
							"field mismatch: invalid enum value");
					return ErrJsonPbMismatch;
				}
				if (repeated) {
					ref->AddEnum(pMsg, field, ev);
				}
				else {
					ref->SetEnum(pMsg, field, ev);
				}
				break;
			}
		default:
			{
				if (m_pLogger) SIMPLELOGERR(*m_pLogger,
						"[PROTOBUF] unknown field type[%d]", field->cpp_type() );
				return ErrProtobuf;
			}
	}
	return 0;
}

