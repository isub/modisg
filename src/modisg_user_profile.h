#ifndef _MODISG_USER_PROFILE_H_
#define _MODISG_USER_PROFILE_H_

#include "modisg.h"

/* процедуры и функции для работы с профилями пользователей */
void modisg_user_profile_init( void );
unsigned short modisg_user_profile_get( __be32 p_uiUserAddr );
void modisg_user_profile_set( unsigned int p_uiUserKey, unsigned short p_usUserProfile );

#endif /* _MODISG_USER_PROFILE_H_ */
