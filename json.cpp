#ifdef _WINDOWS
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#include "json.h"
#include "jsonValue.h"
#include "jsonException.h"

namespace M {
	void Json::parse(const std::string& s, std::string& status) {
		try {
			parse(s);
			status = "parse ok";
		}
		catch (const Exception& msg) {
			status = msg.what();
		}
		catch (...) {}

	}

	void Json::parse(const std::string& s){
		v->parse(s);
	}
	Json::Json() : v(new json::Value) { }

	Json::~Json() { }

	Json::Json(const Json& rhs) {
		v.reset(new json::Value(*(rhs.v)));
	}

	Json& Json::operator=(const Json& rhs) {
		v.reset(new json::Value(*(rhs.v)));
		return *this;
	}

	Json::Json(Json&& rhs){
		v.reset(rhs.v.release());
	}

	Json& Json::operator=(Json&& rhs)  {
		v.reset(rhs.v.release());
		return *this;
	}

	void Json::swap(Json& rhs) {
		using std::swap;
		swap(v, rhs.v);
	}

	void swap(Json& lhs, Json& rhs) {
		lhs.swap(rhs);
	}

	int Json::get_type() const {
		if (v == nullptr)
			return json::TYPE_NULL;
		return v->get_type();
	}

	void Json::set_null() {
		v->set_type(json::TYPE_NULL);
	}

	void Json::set_boolean(bool b) {
		if (b) v->set_type(json::TYPE_TRUE);
		else v->set_type(json::TYPE_FALSE);
	}

	double Json::get_number() const {
		return v->get_number();
	}

	void Json::set_number(double d) {
		v->set_number(d);
	}

	const std::string Json::get_string() const {
		return v->get_string();
	}

	void Json::set_string(const std::string& str) {
		v->set_string(str);
	}

	bool operator==(const Json& lhs, const Json& rhs) {
		return *lhs.v == *rhs.v;
	}

	bool operator!=(const Json& lhs, const Json& rhs) {
		return *lhs.v != *rhs.v;
	}

	size_t Json::get_array_size() const {
		return v->get_array_size();
	}

	Json Json::get_array_element(size_t index) const {
		Json res;
		res.v.reset(new json::Value(v->get_array_element(index)));
		return res;
	}

	void Json::set_array() {
		v->set_array(std::vector<json::Value>({}));
	}

	void Json::push_back(const Json& e) {
		v->push_back(*e.v);
	}

	void Json::pop_back() {
		v->pop_back();
	}

	void Json::insert(const Json& e, size_t index) {
		v->insert(*e.v, index);
	}

	void Json::erase(size_t index, size_t count) {
		v->erase(index, count);
	}

	void Json::clear_arr() {
		v->clear_arr();
	}

	void Json::set_object() {
		v->set_object(json::objValue({}));
	}
	size_t Json::get_object_size() const {
		return v->get_object_size();
	}

	const std::string Json::get_object_key(size_t index) const {
		return v->get_object_key(index);
	}

	size_t Json::get_object_key_length(size_t index) const {
		return v->get_object_key_length(index);
	}

	Json Json::get_object_value(size_t index) const {
		Json e;
		e.v.reset(new json::Value(v->get_object_value(index)));
		return e;
	}

	void Json::set_object_value(const std::string& key, const Json& val) {
		v->set_object_value(key, *val.v);
	}

	size_t Json::get_object_index(const std::string& key) const {
		return v->get_object_index(key);
	}

	void Json::remove_object_value(size_t index) {
		v->remove_object_value(index);
	}

	void Json::clear_object() {
		v->clear_obj();
	}

	void Json::stringify(std::string& res) const {
		v->stringify(res);
		return;
	}
}