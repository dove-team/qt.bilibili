#ifndef QT_BILIBILI_LIBS_GLOBAL_H
#define QT_BILIBILI_LIBS_GLOBAL_H

#include "qt.bilibili.libs_global.h"
#include "logger.h"
#include "json/json.h"
#include <iostream>
#include <regex>
#define OUT
using namespace std;

class untils {
private :
    unsigned char ToHex(unsigned char x) {
        return  x > 9 ? x + 55 : x + 48;
    }
    unsigned char FromHex(unsigned char x) {
        unsigned char y;
        if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
        else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
        else if (x >= '0' && x <= '9') y = x - '0';
        else return 0;
        return y;
    }
public:
    string UrlEncode(const string& str, string& dst) {
        dst = "";
        size_t length = str.length();
        for (size_t i = 0; i < length; i++) {
            if (isalnum((unsigned char)str[i]) ||
                (str[i] == '-') ||
                (str[i] == '_') ||
                (str[i] == '.') ||
                (str[i] == '~')) {
                dst += str[i];
            }
            else if (str[i] == ' ') {
                dst += "+";
            }
            else {
                dst += '%';
                dst += ToHex((unsigned char)str[i] >> 4);
                dst += ToHex((unsigned char)str[i] % 16);
            }
        }
        return dst;
    }
    string UrlDecode(const string& str, string& dst) {
        dst = "";
        size_t length = str.length();
        for (size_t i = 0; i < length; i++) {
            if (str[i] == '+') {
                dst += ' ';
            }
            else if (i + 2 < length && str[i] == '%') {
                unsigned char high = FromHex((unsigned char)str[++i]);
                unsigned char low = FromHex((unsigned char)str[++i]);
                dst += high * 16 + low;
            }
            else {
                dst += str[i];
            }
        }
        return dst;
    }
    bool parse(string rawJson, Json::Value OUT result) {
        logger log;
        try {
            Json::Value root;
            JSONCPP_STRING err;
            Json::CharReaderBuilder builder;
            auto rawJsonLength = static_cast<int>(rawJson.length());
            const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
            if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root, &err)) {
                log.logError(err);
                result = root;
                return EXIT_FAILURE;
            }
        }
        catch (exception ex) {
            log.logError("parse", ex);
        }
        return EXIT_SUCCESS;
    }
    bool stringify(Json::Value root, OUT string *json_str) {
        logger log;
        try {
            Json::StreamWriterBuilder builder;
            *json_str = Json::writeString(builder, root);
            return EXIT_SUCCESS;
        }
        catch (exception ex) {
            log.logError("stringify", ex);
            return EXIT_FAILURE;
        }
    }
    string replace(string resource_str, string sub_str, string new_str) {
        string dst_str = resource_str;
        string::size_type pos = 0;
        while ((pos = dst_str.find(sub_str)) != string::npos)
            dst_str.replace(pos, sub_str.length(), new_str);
        return dst_str;
    }
    string replace(string resource_str, map<string,string> sub_str) {
        string dst_str = resource_str;
        for (auto i = sub_str.begin(); i != sub_str.end(); i++)
            dst_str = replace(dst_str, i->first, i->second);
        return dst_str;
    }
    bool regexMatch(string input, string pattern, OUT string *result) {
        try
        {
            regex reg(pattern);
            smatch match;
            bool ret = regex_match(input, match, reg);
            if (ret) {
                *result = match[0];
                return false;
            }
        }
        catch (const std::exception&) {}
        return false;
    }
};

#endif