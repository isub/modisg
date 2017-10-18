#include "modisg.h"

#include <linux/ip.h>
#include <linux/netdevice.h>

#define MAXPROFOP 16
#define MAXPROF   128

int modisg_profile_req_oper(struct sk_buff *p_psoSKB, int p_iHook)
{
  int iRetVal = NF_ACCEPT;
  struct iphdr *psoIP = ip_hdr(p_psoSKB);
  unsigned short uiUserProfile;

  /* определяем идентификатор профиля абонента */
  /* внутренний интерфейс */
  if(0 == strcmp("enp0s8", p_psoSKB->dev->name)) {
    if(IS_SUBNET(INNER_NET, psoIP->saddr)) {
      uiUserProfile = modisg_user_profile_get(psoIP->saddr);
    } else if(IS_SUBNET(INNER_NET, psoIP->daddr)) {
      uiUserProfile = modisg_user_profile_get(psoIP->daddr);
    } else {
      return iRetVal;
    }
  } else {
    return iRetVal;
  }

  /* если пользователь авторизован то ничего не меняем */
  if(uiUserProfile) {
    return iRetVal;
  }

  /* обрабатываем только TCP */
  if(6 != psoIP->protocol) {
    return iRetVal;
  }

  /* NAT */
  switch(p_iHook) {
    case NF_INET_PRE_ROUTING:
      modisg_dnat_oper(p_psoSKB, psoIP);
      break;
    case NF_INET_POST_ROUTING:
      modisg_snat_oper(p_psoSKB, psoIP);
      break;
  }

  return iRetVal;
}

ssize_t modisg_profile_set(const char *buff, size_t len)
{
  ssize_t stRetVal = 0;
  __be32 uiAddr = *(__be32*)buff;
  __be32 uiProfile = *(__be32*)(&buff[4]);
  unsigned int uiUserKey = USER_KEY(uiAddr);

  if(len != 8) {
    return -EINVAL;
  } else {
    modisg_user_profile_set(uiUserKey, uiProfile);
    stRetVal = len;
  }

  return stRetVal;
}
