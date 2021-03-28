#pragma once
#ifndef JSONVALUE_H
#define JSONVALUE_H

#include <string>
#include <vector>
#include <utility>
#include "json.h"

namespace M {
	namespace json {
        typedef std::vector<std::pair<std::string, Value>> objValue;
		class Value {
        public:
            void parse(const std::string& s);
            void stringify(std::string& res) const;

            int get_type() const;
            void set_type(type t);

            double get_number() const;
            void set_number(double d);

            const std::string& get_string() const;
            void set_string(const std::string& str);

            size_t          get_array_size() const;
            const Value&    get_array_element(size_t index) const;
            void            set_array(const std::vector<Value>& arr) ;
            void            push_back(const Value& val);
            void            pop_back();
            void            insert(const Value& val, size_t index);
            void            erase(size_t index, size_t count);
            void            clear_arr();

            size_t get_object_size() const;
            const std::string get_object_key(size_t index) const;
            size_t get_object_key_length(size_t index) const;
            const Value& get_object_value(size_t index) const;
            const Value& get_object_value(const std::string& s) const;
            void set_object_value(std::string key, const Value& val);
            void set_object(const objValue& obj);
            size_t get_object_index(const std::string key) const;
            void remove_object_value(size_t index);
            void clear_obj();


            Value(): num_(0) {}
            Value(const Value& rhs)  { init(rhs); }
            Value& operator=(const Value& rhs) ;
            ~Value();

        private:
            void init(const Value& rhs) ;
            void free() ;

            json::type type_ = json::TYPE_NULL;
            //union {
            //    double num_; //4B
            //    std::string str_; //28B
            //    std::vector<Value> arr_; //3 * 8B = 24B
            //    objValue obj_; //24B
            //};
            union {
                double num_; //8B
                std::string* str_; //8B
                std::vector<Value>* arr_; // 8B
                objValue* obj_; //8b
            };

            friend bool operator==(const Value& lhs, const Value& rhs) ;

		};
        bool operator==(const Value& lhs, const Value& rhs) ;
        bool operator!=(const Value& lhs, const Value& rhs) ;
	}
}

#endif

