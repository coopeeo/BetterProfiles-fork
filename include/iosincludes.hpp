#pragma once
#ifdef GEODE_IS_IOS
#pragma message(STATUS "iOS Includes: The current platform is iOS, so there will be std included and other files so iOS works.")
#include <string>
#include <optional>
#include <vector>
#include <Geode/Geode.hpp>
#include <Geode/cocos/include/ccTypes.h>
#else
#pragma message(STATUS "iOS Includes: not iOS, so nothing will be included")
#endif
// ? This is here for importing classes on iOS because iOS doesn't like when string isn't imported!