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

#include "../include/database_header.h"

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::finalize;
using namespace std;


const vector<string> FIELDS = {
	"user_name",
	"eth_amount"
};

bsoncxx::document::value get_result_json(const string& uid, const bsoncxx::v_noabi::document::view& doc_view){
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
                                         << "links" 
                                         << bsoncxx::builder::stream::open_document 
                                         << "self" << "/user_wallet/" + uid
                                         << bsoncxx::builder::stream::close_document
                                        << "user_wallet"
                                         << bsoncxx::builder::stream::open_document
                                         << "user_name" << doc_view["user_name"].get_utf8().value.to_string()
                                         << "eth_amount" << doc_view["eth_amount"].get_double().value
                                         << bsoncxx::builder::stream::close_document 
                                         << bsoncxx::builder::stream::finalize;
    return doc_value;
}

std::string  addUser(mongocxx::collection collection, std::string user_name, float eth_amount){
  bsoncxx::builder::stream::document document{};
  document << "user_name" << user_name;
  document << "eth_amount" << eth_amount;
  auto  result = collection.insert_one(document.view());
  return result->inserted_id().get_oid().value.to_string();
}

std::vector<std::string> getAllUsers(mongocxx::collection collection){
  auto cursor = collection.find({});
  std::vector<std::string> users;
  for(auto&& doc: cursor){
     users.push_back(bsoncxx::to_json(doc));
  }
  return users;
}

std::string getUser(mongocxx::collection collection, std::string user_id){
  try {
  auto doc = collection.find_one(document{} << "_id" << bsoncxx::oid(user_id)<<finalize);
  if(doc){
     return bsoncxx::to_json(get_result_json(user_id, *doc));
  } 
 }
  catch(...){
    cout << "incorrect request" << endl;
  }
  return "";
}

bsoncxx::stdx::optional<mongocxx::result::update> updateUser(mongocxx::collection collection, std::string user_id, float new_amount){
  return collection.update_one(document{} << "_id" << bsoncxx::oid(user_id) << finalize, document{} << "$set" << open_document << "eth_amount" << new_amount << close_document << finalize);
}

void deleteUser(mongocxx::collection collection, std::string user_id){
   collection.delete_one(document{} << "_id" << bsoncxx::oid(user_id)<< finalize);
}

bool check_fields_need_all(const bsoncxx::v_noabi::document::view& view, vector<string> fields){
    for(auto& target_key : fields){
        auto parsed = view[target_key];
        if(!parsed){
            return false;
        }
    }
    return true;
}


bool check_fields_only_registered(const bsoncxx::v_noabi::document::view& view, vector<string> fields){
    for (auto& element : view) {
        auto field_key{element.key()};
        std::string key_string = field_key.to_string();

        if (std::find(fields.begin(), fields.end(), key_string) == fields.end()){
            return false;
        }
    }
    return true;
}
