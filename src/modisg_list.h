#ifndef _MODISG_LIST_H_
#define _MODISG_LIST_H_

#include "modisg.h"

/* ��������� ��� ������ �� ������� ������������ ������� */
/* ��������� ������������� ������ ������� */
void modisg_list_init( void );
/* ��������� ���������� ������ � ������ */
void modisg_list_add( void *p_pData, unsigned int p_uiDataLen );
/* ��������� ������ ������ �� ������ */
void modisg_list_read( unsigned char *p_pucBuf, ssize_t *p_pstBufSize );
/* ��������� ������� ������ */
void modisg_list_clean( void );

#endif /* _MODISG_LIST_H_ */
