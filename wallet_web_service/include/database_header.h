#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/pool.hpp>


using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::finalize;
using namespace std;

bsoncxx::document::value get_result_json(const string& uid, const bsoncxx::v_noabi::document::view& doc_view);

std::string  addUser(mongocxx::collection collection, std::string user_name, float eth_amount);

std::string getUser(mongocxx::collection collection, std::string user_id);

bsoncxx::stdx::optional<mongocxx::result::update> updateUser(mongocxx::collection collection, std::string user_id, float new_amount);

void deleteUser(mongocxx::collection collection, std::string user_id);

bool check_fields_need_all(const bsoncxx::v_noabi::document::view& view, vector<string> fields);

bool check_fields_only_registered(const bsoncxx::v_noabi::document::view& view, vector<string> fields);
