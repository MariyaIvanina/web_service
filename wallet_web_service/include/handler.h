#ifndef HANDLER_H
#define HANDLER_H
#include <iostream>
#include "stdafx.h"
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/pool.hpp>

//#include "../dbms/include/Dbms.h"

using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;


class handler
{
    public:
        handler();
        handler(utility::string_t url);
        virtual ~handler();

        pplx::task<void>open(){return m_listener.open();}
        pplx::task<void>close(){return m_listener.close();}

    protected:

    private:
        void handle_get(http_request message);
        void handle_put(http_request message);
        void handle_post(http_request message);
        void handle_delete(http_request message);
        void handle_error(pplx::task<void>& t);
        http_listener m_listener;
        std::shared_ptr<mongocxx::pool> mongo_pool;
};

#endif // HANDLER_H


