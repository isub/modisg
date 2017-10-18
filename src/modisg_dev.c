#include "modisg.h"

#include <uapi/asm-generic/errno-base.h> /* EINVAL */
#include <linux/miscdevice.h> /* MISC_DYNAMIC_MINOR */

struct file_operations g_soFileOps = {
	.owner = THIS_MODULE,
	.read = device_read,
	.write = device_write,
};

static struct miscdevice mod_isg_dev = {
	MISC_DYNAMIC_MINOR,
	"mod_isg",
	&g_soFileOps
};

int modisg_dev_init(void)
{
  return misc_register(&mod_isg_dev);
}

void modisg_dev_clean(void)
{
	misc_deregister(&mod_isg_dev);
}

ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
  return modisg_profile_set(buff, len);
}

ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t * offset)
{
	ssize_t stBitesWritten = length;

	modisg_list_read(buffer, &stBitesWritten);

	return stBitesWritten;
}
