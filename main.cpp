#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <chrono>

#include <restclient-cpp/connection.h>
#include <restclient-cpp/restclient.h>

#include "properties_file_reader.h"
#include "json/json.h"

enum HttpMethod{
    GET,POST,PUT,DELETE,HEAD
};

/** @brief rest_client
*/
struct rest_client {

    //ctor
    rest_client(std::string &&properties_file_name)
        : cfg_(std::move(properties_file_name)) {}

    int init(){

        //we create authentication related connection object
        
        auth_conn_.reset(new RestClient::Connection(cfg_("auth_server")[0]));
        if(auth_conn_){
            std::cout << "auth_conn_ object created" << std::endl;
        }else{
            std::cerr << "failed creating auth_conn_ object" << std::endl;
            return -1;
        }

        //we setup authentication related connection object
        auth_conn_->SetTimeout(30);
        RestClient::HeaderFields authHeaderFields;
        authHeaderFields["Content-Type"] = std::string("application/x-www-form-urlencoded");
        auth_conn_->SetHeaders(authHeaderFields);


        //we create resource related connection object
        resource_conn_.reset(new RestClient::Connection(cfg_("rest_api_url")[0]));
        if(resource_conn_){
            std::cout << "resource_conn_ object created" << std::endl;
        }else{
            std::cerr << "failed creating resource_conn_ object" << std::endl;
            return -1;
        }

        //we setup resource related connection object
        resource_conn_->SetTimeout(30);

        //all ok
        return 0;
    }

    int refresh_auth_token_if_expired();

    RestClient::Response do_request(const std::string &url,
                                    HttpMethod http_method,
                                    const std::string &query_string,
                                    const std::string &opt_data);

    utils::PropertiesFileReader cfg_;

    std::unique_ptr<RestClient::Connection> auth_conn_;
    std::unique_ptr<RestClient::Connection> resource_conn_;
    std::string auth_token_;        //authentication token
    std::string auth_token_type_;   //authentication token type (Bearer)
    std::chrono::system_clock::time_point auth_token_expire_tp_; //auth-token expire time-point
};

int main() {

    rest_client rc("rc.properties");
    int res = 0;

    if((res = rc.init())){
        std::cerr << "error init rest_client, exiting..." << std::endl;
        return res;
    }    

    return 0;
}