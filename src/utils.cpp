#include "utils.hpp"

cocos2d::ccColor3B Utils::intToColor(int col) {
    int r = col >> 16 & 0xFF;
    int g = col >> 8 & 0xFF;
    int b = col & 0xFF;
    return cocos2d::ccc3(r, g, b);
}

int Utils::colorToInt(cocos2d::ccColor3B col) {
    return (col.r << 16) | (col.g << 8) | col.b;
}
