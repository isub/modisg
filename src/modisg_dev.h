#ifndef _MODISG_DEV_H_
#define _MODISG_DEV_H_

#include "modisg.h"

/* процедуры и функции для работы с устройством */
int modisg_dev_init( void );
void modisg_dev_clean( void );
ssize_t device_write( struct file *filp, const char *buff, size_t len, loff_t * off );
ssize_t device_read( struct file *filp, char *buffer, size_t length, loff_t * offset );

#endif /* _MODISG_DEV_H_ */
