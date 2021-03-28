#pragma once
#ifndef JSON_H
#define JSON_H

#include <memory>
#include <string>
#include <vector>

namespace M {
	namespace json {
		enum type : int {TYPE_NULL, TYPE_TRUE, TYPE_FALSE, TYPE_NUMBER, TYPE_STRING, TYPE_ARRAY, TYPE_OBJECT};
		class Value;
	}

	class Json {
	public:
		void parse(const std::string& s, std::string& status);
		void parse(const std::string& s);

		Json();
		~Json();
		Json(const Json& rhs);
		Json& operator=(const Json& rhs);
		Json(Json&& rhs);
		Json& operator=(Json&& rhs);
		void swap(Json& rhs);

		int get_type() const;
		void set_null();
		void set_boolean(bool b);
		Json& operator=(bool b) { set_boolean(b); return *this; }

		double get_number() const;
		void set_number(double d);
		Json& operator=(double d) { set_number(d); return *this; }

		const std::string get_string() const;
		void set_string(const std::string& s);
		Json& operator=(const std::string& str) { set_string(str); return *this; }

		size_t get_array_size() const;
		Json get_array_element(size_t index) const;
		void set_array();
		void push_back(const Json& val);
		void pop_back();
		void insert(const Json& val, size_t index);
		void erase(size_t index, size_t count);
		void clear_arr();

		void set_object() ;
		size_t get_object_size() const ;
		const std::string get_object_key(size_t index) const ;
		size_t get_object_key_length(size_t index) const ;
		Json get_object_value(size_t index) const ;
		void set_object_value(const std::string& key, const Json& val) ;
		size_t get_object_index(const std::string& key) const ;
		void remove_object_value(size_t index);
		void clear_object() ;

		void stringify(std::string& content) const;

	private:
		std::unique_ptr<json::Value> v;

		friend bool operator==(const Json& lhs, const Json& rhs);
		friend bool operator!=(const Json& lhs, const Json& rhs);

	};

	bool operator==(const Json& lhs, const Json& rhs) ;
	bool operator!=(const Json& lhs, const Json& rhs) ;
	void swap(Json& lhs, Json& rhs) ;
}


#endif


