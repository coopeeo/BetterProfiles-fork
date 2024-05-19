#pragma once
#if CMAKE_SYSTEM_NAME == "iOS"
#pragma message("iOS Includes: The current platform is iOS, so there will be std included and other files so iOS works.")
#include <string>
#include <optional>
#include <vector>
#include <Geode/Geode.hpp>
#include <Geode/cocos/include/ccTypes.h>
#else
#pragma message("iOS Includes: not iOS, so nothing will be included")
#endif
// ? This is here for importing classes on iOS because iOS doesn't like when string isn't imported!