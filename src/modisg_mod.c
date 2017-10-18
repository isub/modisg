#include "modisg.h"

#include <linux/module.h>
#include <linux/kernel.h>

MODULE_AUTHOR("isub /* subbeer@gmail.com */");
MODULE_DESCRIPTION("simple dpi module");
MODULE_LICENSE("GPL");


int Init(void)
{
  int iRetVal = 0;
  printk(KERN_INFO "start module mod_isg\n");

  local_irq_disable();

  iRetVal = modisg_dev_init();
  if(0 > iRetVal) {
    return iRetVal;
  }
  modisg_nat_init();
  modisg_user_profile_init();
  modisg_list_init();
  modisg_hook_init();

  return iRetVal;
}

void Exit(void)
{
  modisg_hook_clean();
  modisg_dev_clean();
  modisg_list_clean();
  printk(KERN_INFO "end module mod_isg\n");
}

module_init(Init);
module_exit(Exit);
