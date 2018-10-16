#include "../include/handler.h"
#include "../include/database_header.h"

const vector<string> FIELDS = {
        "user_name",
        "eth_amount"
};

handler::handler()
{
    mongocxx::uri uri{"mongodb://localhost:27017/?minPoolSize=2&maxPoolSize=4"};
    std::shared_ptr<mongocxx::instance> mongo_instance = std::shared_ptr<mongocxx::instance>(new mongocxx::instance{});
    this->mongo_pool = std::shared_ptr<mongocxx::pool>(new mongocxx::pool{uri});
    //ctor
}
handler::handler(utility::string_t url):m_listener(url)
{
    m_listener.support(methods::GET, std::bind(&handler::handle_get, this, std::placeholders::_1));
    m_listener.support(methods::PUT, std::bind(&handler::handle_put, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&handler::handle_post, this, std::placeholders::_1));
    m_listener.support(methods::DEL, std::bind(&handler::handle_delete, this, std::placeholders::_1));
    mongocxx::uri uri{"mongodb://localhost:27017/?minPoolSize=2&maxPoolSize=4"};
    std::shared_ptr<mongocxx::instance> mongo_instance = std::shared_ptr<mongocxx::instance>(new mongocxx::instance{});
    this->mongo_pool = std::shared_ptr<mongocxx::pool>(new mongocxx::pool{uri});

}
handler::~handler()
{
    //dtor
}

void handler::handle_error(pplx::task<void>& t)
{
    try
    {
        t.get();
    }
    catch(...)
    {
        // Ignore the error, Log it if a logger is available
    }
}


//
// Get Request 
//
void handler::handle_get(http_request message)
{
    ucout  <<  message.to_string() << endl;
   
    auto client = mongo_pool->acquire();
    auto collection = (*client)["testdb"]["testcollection"];

    //auto paths = http::uri::split_path(http::uri::decode(message.relative_uri().path()));
    auto user_id =  message.relative_uri().path();
    try{
       auto user_info = getUser(collection, user_id);
       if(user_info != ""){
            message.reply(status_codes::OK, user_info,  U("application/json"))
                .then([](pplx::task<void> t)
                {
                        try{
                                t.get();
                        }
                        catch(...){
                                //
                        }
        });
       }
       else {
             message.reply(status_codes::NotFound, U("Not Found\\n"))
                .then([](pplx::task<void> t)
                {
                        try{
                                t.get();
                        }
                        catch(...){
                                //
                        }
        });
       }
    }
    catch(...){
       message.reply(status_codes::InternalError,U("INTERNAL ERROR "));
    }


    return;

};

//
// A POST request
//
void handler::handle_post(http_request message)
{
    ucout <<  message.to_string() << endl;
    
    auto client = mongo_pool->acquire();
    auto collection = (*client)["testdb"]["testcollection"];
    
    pplx::task<utility::string_t> body_json = message.extract_string();
    std::string jsonstr=    utility::conversions::to_utf8string(body_json.get());
    cout << jsonstr << endl;

    try{
       auto parsed_doc = bsoncxx::from_json(jsonstr);
       bsoncxx::v_noabi::document::view view = parsed_doc.view();
   
       if(!check_fields_need_all(view, FIELDS)){
           cout  << "Not all fields specified\n";
           message.reply(status_codes::BadRequest, U("Not all fields specified\\n"))
                .then([](pplx::task<void> t)
                {
                        try{
                                t.get();
                        }
                        catch(...){
                                //
                        }
           });
           return;
       }

       if(!check_fields_only_registered(view, FIELDS)){
            cout  << "Found not registered fields in POST json\n";
            message.reply(status_codes::BadRequest, U("Found not registered fields in POST json\\n"))
                .then([](pplx::task<void> t)
                {
                        try{
                                t.get();
                        }
                        catch(...){
                                //
                        }
        });
           return;
       }

       std::string user = view["user_name"].get_utf8().value.to_string();
       double eth_am = view["eth_amount"].get_double().value;
       std::string  newId = addUser(collection, user, eth_am);
       auto doc_value = get_result_json(newId,  document{} << "user_name" << user << "eth_amount" << eth_am << finalize);
       auto doc_str = bsoncxx::to_json(doc_value);
       std::cout  << doc_str << std::endl;
       
       if(true){
            message.reply(status_codes::OK, doc_str, U("application/json"))
                .then([](pplx::task<void> t)
                {
                        try{
                                t.get();
                        }
                        catch(...){
                                //
                        }
        });
       }
       else {
             message.reply(status_codes::NotFound, U("Not Found\\n"))
                .then([](pplx::task<void> t)
                {
                        try{
                                t.get();
                        }
                        catch(...){
                                //
                        }
        });
       }
    }
    catch(...){
       message.reply(status_codes::InternalError,U("INTERNAL ERROR"));
    }

    return ;
};

//
// A DELETE request
//
void handler::handle_delete(http_request message)
{
    ucout <<  message.to_string() << endl;
    
    auto client = mongo_pool->acquire();
    auto collection = (*client)["testdb"]["testcollection"];
    
    auto user_id =  message.relative_uri().path();
    cout << user_id << endl;

    try{
       deleteUser(collection, user_id);
       if(true){
            message.reply(status_codes::OK, U("Deleted\\n"))
                .then([](pplx::task<void> t)
                {
                        try{
                                t.get();
                        }
                        catch(...){
                                //
                        }
        });
       }
       else {
             message.reply(status_codes::NotFound, U("Not Found\\n"))
                .then([](pplx::task<void> t)
                {
                        try{
                                t.get();
                        }
                        catch(...){
                                //
                        }
        });
       }
    }
    catch(...){
       message.reply(status_codes::InternalError,U("INTERNAL ERROR"));
    }
    
    return;
};


//
// A PUT request 
//
void handler::handle_put(http_request message)
{
    ucout <<  message.to_string() << endl;

    auto client = mongo_pool->acquire();
    auto collection = (*client)["testdb"]["testcollection"];

    auto user_id =  message.relative_uri().path();
    cout << user_id << endl;
    
    pplx::task<utility::string_t> body_json = message.extract_string();
    std::string jsonstr=    utility::conversions::to_utf8string(body_json.get());
    cout << jsonstr << endl;

    try{
       auto parsed_doc1 = bsoncxx::from_json(jsonstr);
       bsoncxx::v_noabi::document::view view1 = parsed_doc1.view();
       if(!check_fields_need_all(view1, {"eth_amount"})){
           cout  << "Not all fields specified\n";
           message.reply(status_codes::BadRequest, U("Not all fields specified\\n"))
                .then([](pplx::task<void> t)
                {
                        try{
                                t.get();
                        }
                        catch(...){
                                //
                        }
        });
           return;
        }

       if(!check_fields_only_registered(view1, {"eth_amount"})){
            cout  << "Found not registered fields in PUT json\n";
            message.reply(status_codes::BadRequest, U("Found not regsistered fields in PUT json\\n"))
                .then([](pplx::task<void> t)
                {
                        try{
                                t.get();
                        }
                        catch(...){
                                //
                        }
            });
            return;
        }
        auto result =  updateUser(collection, user_id, view1["eth_amount"].get_double().value);
        if(!result || result->modified_count() == 0) {
             message.reply(status_codes::NotFound, U("Not Found\\n"))
                .then([](pplx::task<void> t)
                {
                        try{
                                t.get();
                        }
                        catch(...){
                                //
                        }
             });
             return;
        }
        else{
            message.reply(status_codes::OK, U("OK\\n"))
                .then([](pplx::task<void> t)
                {
                        try{
                                t.get();
                        }
                        catch(...){
                                //
                        }
        });
        }
    }
    catch(...){
       message.reply(status_codes::InternalError,U("INTERNAL ERROR"));
    }
    
    return;
};
