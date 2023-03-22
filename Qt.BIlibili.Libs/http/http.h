#pragma once
#include "../logger.h"
#include "request.h"
#include "httplib.h"
using namespace std;
template <typename T>

class http {
public:
    void sendRequest(request r) {
        switch (r.type) {
        case  requestGet:
            get(r);
            break;
        case requestPost:
            post(r);
            break;
        case requestPut:
            put(r);
            break;
        case requestDelete:
            del(r);
            break;
        }
    }
private:
    void get<T>(request r) {
        logger log;
        httplib::Client cli(r.url);
        if (auto res = cli.Get(r.path, r.headers)) {
            if (res->status == 200) {
                auto result = res->body;
            }
        }
        else {
            auto err = res.error();
            log.logInfo("HTTP error: " + httplib::to_string(err));
        }
    }
    void post(request r) {
        logger log;
        httplib::Client cli(r.url);
        if (auto res = cli.Post(r.path, r.headers, r.params)) {
            if (res->status == 200) {
                auto result = res->body;
            }
        }
        else {
            auto err = res.error();
            log.logInfo("HTTP error: " + httplib::to_string(err));
        }
    }
    void put(request r) {
        logger log;
        httplib::Client cli(r.url);
        if (auto res = cli.Put(r.path, r.headers, r.params)) {
            if (res->status == 200) {
                auto result = res->body;

            }
        }
        else {
            auto err = res.error();
            log.logInfo("HTTP error: " + httplib::to_string(err));
        }
    }
    void del(request r) {
        logger log;
        httplib::Client cli(r.url);
        if (auto res = cli.Delete(r.path, r.headers)) {
            if (res->status == 200) {
                auto result = res->body;

            }
        }
        else {
            auto err = res.error();
            log.logInfo("HTTP error: " + httplib::to_string(err));
        }
    }
};