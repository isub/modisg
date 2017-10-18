#include <linux/types.h> /* ssize_t */
#include <linux/fs.h> /* struct file */
#include <linux/ip.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

#include "modisg_list.h"
#include "modisg_dev.h"
#include "modisg_dev.h"
#include "modisg_user_profile.h"

#define MAXUSER 0x000000FF
#define USER_KEY(a) (unsigned int)(ntohl(a)&(MAXUSER))

#define INET_ADDR(a,b,c,d) ((__be32)((((__be32)a)&0x000000FF) | ((((__be32)b)&0x000000FF)<<8) | ((((__be32)c)&0x000000FF)<<16) | ((((__be32)d)&0x000000FF)<<24)))
#define IS_SUBNET(a,b) (((a)&0x00FFFFFF)==((b)&0x00FFFFFF))
#define L4R_ADDR (INET_ADDR(10,1,2,2))
#define INNER_NET (INET_ADDR(10,1,3,0))
