#include <Geode/Geode.hpp>
#include <dashauth.hpp>

#include "ui/edit_pronouns.hpp"
#include "structs.hpp"

class ExtendedProfilePage : public geode::Popup<ProfileData* const&, GJUserScore* const&> {
protected:
    bool setup(ProfileData* const& profile_data, GJUserScore* const& user_score) override;

    // members
    ProfileData* m_profile_data;
    GJUserScore* m_user_score;
public:
    static ExtendedProfilePage* create(ProfileData* const& profile_data, GJUserScore* const& user_score);
};
