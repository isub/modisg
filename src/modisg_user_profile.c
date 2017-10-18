#include "modisg.h"

/* учет профилей пользователей */
unsigned short g_musUserProfile[MAXUSER];

void modisg_user_profile_init(void)
{
  memset(g_musUserProfile, 0, sizeof(g_musUserProfile));
}

unsigned short modisg_user_profile_get(__be32 p_uiUserAddr)
{
  unsigned int uiUserKey;

  uiUserKey = USER_KEY(p_uiUserAddr);
  return (g_musUserProfile[uiUserKey]);
}

void modisg_user_profile_set(unsigned int p_uiUserKey, unsigned short p_usUserProfile)
{
  if(MAXUSER < p_uiUserKey)
    return;

  g_musUserProfile[p_uiUserKey] = p_usUserProfile;
}
