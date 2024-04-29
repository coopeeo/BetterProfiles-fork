#include "state.hpp"

State* State::sharedInstance() {
    static auto instance = new State();
    return instance;
}
