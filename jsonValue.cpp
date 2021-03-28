#ifdef _WINDOWS
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <assert.h>
#include <string>
#include <vector>
#include "jsonValue.h"
#include "jsonParser.h"
#include <iostream>
#include "jsonGenerator.h"

namespace M {
	namespace json {
        Value& Value::operator=(const Value& rhs) {
            free();
            init(rhs);
            return *this;
        }

        Value::~Value() noexcept
        {
            free();
        }

        void Value::init(const Value& rhs) {
            type_ = rhs.type_;
            num_ = 0;
     
            switch (type_) {
            case json::TYPE_NUMBER: num_ = rhs.num_;
                break;
            case json::TYPE_STRING:
                if (str_ != nullptr) delete str_;
                str_ = new std::string(*rhs.str_);
                break;
            case json::TYPE_ARRAY:
                if (arr_ != nullptr) delete arr_;
                arr_ = new std::vector<Value>(*rhs.arr_);
                //new(&arr_) std::vector<Value>(rhs.arr_);
                break;
            case json::TYPE_OBJECT:
                if (obj_ != nullptr) delete obj_;
                obj_ = new json::objValue(*rhs.obj_);
                //new(&obj_) json::objValue(rhs.obj_);
                break;
            }
            //std::cout << sizeof(*this) << std::endl;
            
        }

        void Value::free() {
            using std::string;
            switch (type_) {
            case json::TYPE_STRING:
                //str_.~string();
                if (str_ != nullptr) delete str_;
                break;
            case json::TYPE_ARRAY:
                if (arr_ != nullptr) delete arr_;
                //arr_.~vector<Value>();
                break;
            case json::TYPE_OBJECT:
                if (obj_ != nullptr) delete obj_;
                // obj_.~vector<std::pair<std::string, Value>>();
                break;
            }
        }

        int Value::get_type() const {
            return type_;
        }

        void Value::set_type(type t) {
            free();
            type_ = t;
        }

        double Value::get_number() const{
            assert(type_ == json::TYPE_NUMBER);
            return num_;
        }

        void Value::set_number(double d) {
            free();
            type_ = json::TYPE_NUMBER;
            num_ = d;
        }

        const std::string& Value::get_string() const{
            assert(type_ == json::TYPE_STRING);
            return *str_;
        }

        void Value::set_string(const std::string& str) {
            free();
            type_ = json::TYPE_STRING;
            str_ = new std::string(str);
            /*if (type_ == json::TYPE_STRING)
                str_ = str;
            else {
                free();
                type_ = json::TYPE_STRING;
                new(&str_) std::string(str);
            }*/
        }

        void Value::parse(const std::string& s){
            Parser(*this, s);
        }

        bool operator==(const Value& lhs, const Value& rhs){
            if (lhs.type_ != rhs.type_)
                return false;
            switch (lhs.type_) {
                case json::TYPE_STRING: return *lhs.str_ == *rhs.str_;
                case json::TYPE_NUMBER: return lhs.num_ == rhs.num_;
                case json::TYPE_ARRAY: 
                    if (lhs.get_array_size() != rhs.get_array_size()) return false;
                    for (int i = 0; i < lhs.get_array_size(); ++i) {
                        if (lhs.get_array_element(i) != rhs.get_array_element(i)) return false;
                    }
                    return true;
                case json::TYPE_OBJECT:
                    if (lhs.get_object_size() != rhs.get_object_size()) return false;
                    for (int i = 0; i < lhs.get_object_size(); ++i) {
                        if (rhs.get_object_value(lhs.get_object_key(i)) != lhs.get_object_value(i)) return false;
                    }
                    return true;
            }
            return true;
        }
        bool operator!=(const Value& lhs, const Value& rhs){
            return !(lhs == rhs);
        }

        size_t Value::get_array_size() const {
            assert(type_ == json::TYPE_ARRAY);
            return arr_->size();
        }

        const Value& Value::get_array_element(size_t index) const {
            assert(type_ == json::TYPE_ARRAY);
            assert(index >= 0 && index <= arr_->size());
            return (*arr_)[index];
        }
        
        void Value::set_array(const std::vector<Value>& arr) {
            free();
            type_ = json::TYPE_ARRAY;
            arr_ = new std::vector<Value>(arr);
            /*if (type_ == json::TYPE_ARRAY) {
                arr_ = arr;
            }
            else {
                free();
                type_ = json::TYPE_ARRAY;
                new(&arr_) std::vector<Value>(arr);
            }*/
        }

        void Value::push_back(const Value& val) {
            assert(type_ == json::TYPE_ARRAY);
            arr_->push_back(val);
        }

        void Value::pop_back() {
            assert(type_ == json::TYPE_ARRAY);
            arr_->pop_back();
        }
        
        void Value::insert(const Value& val, size_t index) {
            assert(type_ == json::TYPE_ARRAY);
            arr_->insert(arr_->begin() + index, val);
        }

        void Value::erase(size_t index, size_t count) {
            assert(type_ == json::TYPE_ARRAY);
            arr_->erase(arr_->begin() + index, arr_->begin() + index + count);
        }
        
        void Value::clear_arr() {
            assert(type_ == json::TYPE_ARRAY);
            arr_->clear();
        }

        size_t Value::get_object_size() const {
            assert(type_ == json::TYPE_OBJECT);
            return obj_->size();
        }

        const std::string Value::get_object_key(size_t index) const {
            assert(type_ == json::TYPE_OBJECT);
            return (*obj_)[index].first;
        }

        size_t Value::get_object_key_length(size_t index) const {
            assert(type_ == json::TYPE_OBJECT);
            return (*obj_)[index].first.size();
        }

        const Value& Value::get_object_value(size_t index) const {
            assert(type_ == json::TYPE_OBJECT);
            return (*obj_)[index].second;
        }

        const Value& Value::get_object_value(const std::string& s) const {
            assert(type_ == json::TYPE_OBJECT);
            if (get_object_index(s) == get_object_size()) {
                Value e;
                e.set_type(json::TYPE_NULL);
                return e;
            }
            else {
                return (*obj_)[get_object_index(s)].second;
            }
        }


        void Value::set_object_value(std::string key, const Value& val) {
            assert(type_ == json::TYPE_OBJECT);
            auto idx = get_object_index(key);
            if (idx < get_object_size()) (*obj_)[idx].second = val;
            else obj_->push_back(make_pair(key, val));
        }

        void Value::set_object(const objValue& obj) {
            free();
            type_ = json::TYPE_OBJECT;
            obj_ = new objValue(obj);
            /*if (type_ == json::TYPE_OBJECT) {miao
                obj_ = obj;
            }
            else {
                free();
                type_ = json::TYPE_OBJECT;
                new(&obj_) objValue(obj);
            }*/
        }

        size_t Value::get_object_index(const std::string key) const {
            assert(type_ == json::TYPE_OBJECT);
            for (size_t i = 0; i < obj_->size(); ++i) {
                if ((*obj_)[i].first == key) return i;
            }
            return get_object_size();
        }

        void Value::remove_object_value(size_t index) {
            assert(type_ == json::TYPE_OBJECT);
            obj_->erase(obj_->begin() + index, obj_->begin() + index + 1);
        }

        void Value::clear_obj() {
            assert(type_ == json::TYPE_OBJECT);
            obj_->clear();
        }

        void Value::stringify(std::string& res) const {
            Generator(*this, res);
        }


	}
}
