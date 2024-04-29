#pragma once
#include "structs.hpp"

class State {
    public:
        // store profiles for int, ProfileInfo
        std::map<int, ProfileData> profiles;
        static State* sharedInstance();
};
