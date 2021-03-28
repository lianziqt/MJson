#include "jsonGenerator.h"
namespace M {
	namespace json {
		Generator::Generator(const json::Value& val, std::string& result): res_(result){
			res_.clear();
			stringify_value(val);
		}

		void Generator::stringify_value(const json::Value& val) {
			switch (val.get_type()) {
				case json::TYPE_NULL: res_ += "null"; break;
				case json::TYPE_TRUE: res_ += "true"; break;
				case json::TYPE_FALSE: res_ += "false"; break;
				case json::TYPE_NUMBER: {
						char buffer[32] = { 0 };
						sprintf_s(buffer, "%.17g", val.get_number());
						res_ += buffer;
					}
									  break;
				case json::TYPE_STRING: stringify_string(val.get_string()); break;
				case json::TYPE_ARRAY:
					res_ += '[';
					for (size_t i = 0; i < val.get_array_size(); i++) {
						if (i > 0) res_ += ',';
						stringify_value(val.get_array_element(i));
					}
					res_ += ']';
					break;
				case json::TYPE_OBJECT:
					res_ += '{';
					for (size_t i = 0; i < val.get_object_size(); i++) {
						if (i > 0) res_ += ',';
						stringify_string(val.get_object_key(i));
						res_ += ':';
						stringify_value(val.get_object_value(i));
					}
					res_ += '}';
					break;
			}
		}

		void Generator::stringify_string(const std::string& s) {
			res_ += '\"';
			for(auto it = s.begin(); it != s.end(); ++it){
				unsigned char ch = *it;
				switch (ch) {
				case '\"': res_ += "\\\""; break;
				case '\\': res_ += "\\\\"; break;
				case '\b': res_ += "\\b";  break;
				case '\f': res_ += "\\f";  break;
				case '\n': res_ += "\\n";  break;
				case '\r': res_ += "\\r";  break;
				case '\t': res_ += "\\t";  break;
				default:
					if (ch < 0x20) {
						char buffer[7] = { 0 };
						sprintf_s(buffer, "\\u%04X", ch);
						res_ += buffer;
					}
					else
						res_ += *it;
				}
			}
			res_ += '\"';
		}
	}
}