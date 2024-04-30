#pragma once

#include <Geode/Geode.hpp>
#include <Geode/cocos/include/ccTypes.h>

class Utils {
    public:
        static cocos2d::ccColor3B intToColor(int col);
        static int colorToInt(cocos2d::ccColor3B col);
};
