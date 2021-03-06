/*
*  @file     preloadList.h
*  @brief    需要预加载的资源的文件名（带路径）
*  @author   王亮
*/

#ifndef __PRELOAD_LIST_H__
#define __PRELOAD_LIST_H__

#include <array>

namespace Preload
{
	//图集对应的plist文件名字符串数组
	const std::array<std::string, 6> plists = {
		"Animation/explosion.plist",
		"Animation/wind.plist",
		"GameItem/Unit/unit1.plist",
		"GameItem/Unit/unit2.plist",
		"GameItem/Unit/unit3.plist",
		"GameItem/Unit/BaseCar.plist"
    };

	//图片文件名字符串数组
	const std::array<std::string, 0> image = {

	};

	//背景音乐文件名字符串数组
	const std::array<std::string, 1> bgm = {
		"Sound/bgm/WelcomeScene.mp3",
	};

	//音效文件名字符串数组
	const std::array<std::string, 4> effect = {
		"Sound/button.wav",
		"Sound/bell.mp3",
		"Sound/move.mp3",
		"Sound/Explosion.mp3"
	};
}

#endif // !__PRELOAD_LIST_H__

