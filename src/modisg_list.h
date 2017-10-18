#ifndef _MODISG_LIST_H_
#define _MODISG_LIST_H_

#include "modisg.h"

/* процедуры для работы со списком кэшированных пакетов */
/* процедура инициализации списка пакетов */
void modisg_list_init( void );
/* процедура добавления пакета в список */
void modisg_list_add( void *p_pData, unsigned int p_uiDataLen );
/* процедура чтения пакета из списка */
void modisg_list_read( unsigned char *p_pucBuf, ssize_t *p_pstBufSize );
/* процедура очистки списка */
void modisg_list_clean( void );

#endif /* _MODISG_LIST_H_ */
