#include <iostream>
#include <string>
#include <map>
#include <memory>

#include <restclient-cpp/connection.h>
#include <restclient-cpp/restclient.h>

#include "properties_file_reader.h"

enum HttpMethod{
    GET,POST,PUT,DELETE,HEAD
};

/** @brief rest_client
*/
struct rest_client {

    //ctor.
    rest_client(std::string &&properties_file_name)
        : cfg_(std::move(properties_file_name)) {}

    ~rest_client() = default;

    int init();

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
    uint64_t auth_token_expire_ts_; //auth. token expire timestamp (nanoseconds)


};

int main() {

    rest_client rc("rc.properties");

    return 0;
}