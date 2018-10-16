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
/*
int main(int, char**) {
    bsoncxx::builder::stream::document document{};
    mongocxx::uri uri{"mongodb://localhost:27017/?minPoolSize=2&maxPoolSize=4"};
    std::shared_ptr<mongocxx::instance> mongo_instance = std::shared_ptr<mongocxx::instance>(new mongocxx::instance{});
    std::shared_ptr<mongocxx::pool> mongo_pool = std::shared_ptr<mongocxx::pool>(new mongocxx::pool{uri});
    auto client = mongo_pool->acquire();
    auto collection = (*client)["testdb"]["testollection"];
    
    // New user_wallet
    auto parsed_doc = bsoncxx::from_json("{\"user_name\":\"user_3\", \"eth_amount\": 1.5}");
    bsoncxx::v_noabi::document::view view = parsed_doc.view();
   
    if(!check_fields_need_all(view, FIELDS)){
        cout  << "Not all fields specified\n";
        return 1;
    }

    if(!check_fields_only_registered(view, FIELDS)){
        cout  << "Found not registered fields in POST json\n";
        return 1;
    }

    std::string user = view["user_name"].get_utf8().value.to_string();
    double eth_am = view["eth_amount"].get_double().value;
    std::string  newId = addUser(collection, user, eth_am);
    auto doc_value = get_result_json(newId,  document << "user_name" << user << "eth_amount" << eth_am << finalize);
    std::cout  << bsoncxx::to_json(doc_value) << std::endl;
    
    for (auto doc : getAllUsers(collection)) {
        std::cout << doc << std::endl;
    }
    cout << endl;
    // GET user_wallet
    
    std::cout << newId << std::endl;
    std::cout << getUser(collection, newId) << endl;
    std::cout << getUser(collection, "df") << endl;
    
    // Update user_wallet
    auto parsed_doc1 = bsoncxx::from_json("{\"eth_amount\": 2.4}");
    bsoncxx::v_noabi::document::view view1 = parsed_doc1.view();
    if(!check_fields_need_all(view1, {"eth_amount"})){
        cout  << "Not all fields specified\n";
        return 1;
    }

    if(!check_fields_only_registered(view1, {"eth_amount"})){
        cout  << "Found not registered fields in POST json\n";
        return 1;
    }
    auto result =  updateUser(collection, newId, view1["eth_amount"].get_double().value);
    if(!result || result->modified_count() == 0) {
        cout << "NOT FOUND" << endl;
        return 1;
    }
    else{
       cout << "OK" << endl;
    }
    // Delete user
    deleteUser(collection, newId);
    for (auto doc : getAllUsers(collection)) {
        std::cout << doc << std::endl;
    }
}*/
