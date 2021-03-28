#pragma once
#ifndef JSON_GENERATOR_H
#define JSON_GENERATOR_H

#include "jsonValue.h"

namespace M {
	namespace json {
		class Generator {
		public:
			Generator(const json::Value& val, std::string& result);
		private:
			std::string& res_;

			void stringify_value(const Value& v);
			void stringify_string(const std::string& s);
		};
	}
}

#endif // !JSON_GENERATOR_H



