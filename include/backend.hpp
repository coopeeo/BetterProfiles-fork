#if __has_include("./../endpoints.hpp")
#pragma message("!! USING CUSTOM ENDPOINT PREFIX !!")
#include "./../endpoints.hpp"
#else
#define BACKEND_PREFIX "https://gd-backend.foxgirl.wtf"
#endif
