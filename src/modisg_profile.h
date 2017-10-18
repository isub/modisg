#ifndef _MODISG_PROFILE_H_
#define _MODISG_PROFILE_H_

#include "modisg.h"

/* функции для работы с профилями */
int modisg_profile_req_oper( struct sk_buff *p_psoSKB, int p_iHook );
ssize_t modisg_profile_set( const char *buff, size_t len );

#endif /* _MODISG_PROFILE_H_ */
