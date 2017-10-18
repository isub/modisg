#ifndef _MODISG_NAT_H_
#define _MODISG_NAT_H_

#include "modisg.h"

/* процедуры и функции для работы с DNAT */
void modisg_nat_init( void );
void modisg_dnat_oper( struct sk_buff *p_psoSKB, struct iphdr *p_psoIP );
void modisg_snat_oper( struct sk_buff *p_psoSKB, struct iphdr *p_psoIP );

#endif /* _MODISG_NAT_H_ */
