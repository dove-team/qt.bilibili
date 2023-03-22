#pragma once
#include <string>
#include "httplib.h"
#include "../enums/requestType.h"
using namespace std;

class request {
	request(string url, string path, requestType type) {
		this->url = url;
		this->type = type;
		this->path = path;
	}
	request(string url, string path, requestType type, map<string, string> paras) {
		this->url = url;
		this->type = type;
		this->path = path;
		for (auto i = paras.begin(); i != paras.end(); i++)
			this->params.insert(i->first, i->second);
	}
public:
	string url;
	string path;
	requestType type;
	httplib::Params params;
	httplib::Headers headers;
	void setHeader(map<string, string> headers) {
		for (auto i = headers.begin(); i != headers.end(); i++)
		{
			this->headers.insert(i->first, i->second);
		}
	}
};