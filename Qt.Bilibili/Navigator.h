#pragma once

#include <string>
#include <iostream>
#include "../Qt.BIlibili.Libs/enums/playMode.h"
#include "../Qt.BIlibili.Libs/untils.h"
#include "h/DynamicInfoWindow.h"
#include "h/UserCenterWindow.h"
#include "h/VideoWindow.h"

class Navigator
{
public:
	bool navigateTo(std::string url) {
		untils obj;
		string result;
#pragma region video
		if (obj.regexMatch(obj.replace(url, { { "aid", "av"},{"/", ""},{"=", "" } }), "av(\d+)", &result)) {
			navigateToPlayer(result, video);
			return true;
		}
		if (obj.regexMatch(url, "bilibili://video/(\d+)", &result)) {
			navigateToPlayer(result, video);
			return true;
		}
		if (obj.regexMatch(url, "avid=(\d+)", &result)) {
			navigateToPlayer(result, video);
			return true;
		}
		if (obj.regexMatch(url, "[Bb][Vv]([a-zA-Z0-9]{5,})", &result)) {
			navigateToPlayer(result, video);
			return true;
		}
#pragma endregion
#pragma region bangmi
		if (obj.regexMatch(obj.replace(url, { { "movie", "ss"},{"anime", "ss"},{"season", "ss"},{"/", "" } }), "ss(\d+)", &result)) {
			navigateToBang(result);
			return true;
		}
		if (obj.regexMatch(url, "ep(\d+)", &result)) {
			auto id = bangumiEpidToSid(result);
			navigateToBang(id);
			return true;
		}
#pragma endregion
		if (obj.regexMatch(obj.replace(url, { { "h5", "live"},{"live.bilibili.com", "live"},{"/", ""} }), "live(\d+)", &result)) {
			navigateToPlayer(result, live);
			return true;
		}
		if (obj.regexMatch(obj.replace(url, { { "read/mobile/", "article"},{"read/cv", "article"},{"/", "" } }), "article(\d+)", &result)) {
			navigateToWeb(result,true);
			return true;
		}
		if (obj.regexMatch(obj.replace(url, { {"music/detail/", "au"},{"/", "" } }), "au(\d+)", &result)) {
			navigateToMusic(result, false);
			return true;
		}
		if (obj.regexMatch(obj.replace(url, { { "menu/detail/", "am"},{"/", "" } }), "am(\d+)", &result)) {
			navigateToMusic(result, true);
			return true;
		}
		if (obj.regexMatch(obj.replace(url, { { "bilibili://following/detail/", "album" }, { "h.bilibili.com/ywh/h5/", "album" }, { "h.bilibili.com", "album" },
			{"t.bilibili.com", "album" }, { "/", "" } }), "am(\d+)", &result)) {
			auto win = new DynamicInfoWindow(result);
			win->show();
			return true;
		}
		if (obj.regexMatch(obj.replace(url, { {"space.bilibili.com", "space"},{"author", "space"},{"/", "" } }), "space(\d+)", &result)) {
			auto win = new UserCenterWindow(result);
			win->show();
			return true;
		}
		if (obj.regexMatch(url, "tag/(.*?)/feed", &result)) {
			navigateToTopic(0, result);
			return true;
		}
		if (obj.regexMatch(url + "/", "tag/.*?/\?name=(.*?)/", &result)) {
			string id;
			obj.UrlDecode(result, id);
			navigateToTopic(1, id);
			return true;
		}
		if (url.find("/add_archive") != string::npos) {
			navigateToWeb("https://member.bilibili.com/v2#/upload/video/frame",false);
			return true;
		}

	}
	void navigateToTopic(int type, string id) {
		switch (type) {
		case 0:

			break;
		case 1:

			break;
		}
	}
	void navigateToPlayer(string id, playMode mode) {
		auto win = new VideoWindow(id, mode);
		win->show();
	}
	void navigateToBang(string id) {

	}
	void navigateToWeb(string url, bool article) {

	}
	void navigateToMusic(string id, bool menu) {

	}
	string bangumiEpidToSid(string url) {

	}
};