#include "modisg.h"

#include <net/checksum.h>
#include <linux/ip.h>
#include <linux/tcp.h>

#define dnat_key(a,p) (unsigned int)(((ntohl(a)&(MAXUSER))<<16) | (__be32)p)
#define MAXSESSAMOUNT (MAXUSER<<16)

__be32 g_muiDNATTable[MAXSESSAMOUNT];

void modisg_nat_init(void)
{
  memset(g_muiDNATTable, 0, sizeof(g_muiDNATTable));
}

void modisg_dnat_set(__be32 p_uiUserAddr, __be16 p_usUserPort, __be32 p_uiAddr)
{
  unsigned int uiDNATKey;
  uiDNATKey = dnat_key(p_uiUserAddr, p_usUserPort);
  g_muiDNATTable[uiDNATKey] = p_uiAddr;
}

__be32 modisg_dnat_get(__be32 p_uiUserAddr, __be16 p_usUserPort)
{
  unsigned int uiDNATKey;
  uiDNATKey = dnat_key(p_uiUserAddr, p_usUserPort);
  return g_muiDNATTable[uiDNATKey];
}

void modisg_dnat_oper(struct sk_buff *p_psoSKB, struct iphdr *p_psoIP)
{
  struct tcphdr *psoTCP = tcp_hdr(p_psoSKB);

  if(psoTCP == NULL)
    return;

  /* сохраняем значение адреса назначения в таблице */
  modisg_dnat_set(p_psoIP->saddr, psoTCP->source, p_psoIP->daddr);
  /* пересчитываем контрольные суммы */
  csum_replace4(&p_psoIP->check, p_psoIP->daddr, L4R_ADDR);
  inet_proto_csum_replace4(&psoTCP->check, p_psoSKB, p_psoIP->daddr, L4R_ADDR, 1);
  p_psoIP->daddr = L4R_ADDR;
}

void modisg_snat_oper(struct sk_buff *p_psoSKB, struct iphdr *p_psoIP)
{
  struct tcphdr *psoTCP = tcp_hdr(p_psoSKB);
  __be32 uiSrcAddr;

  if(psoTCP == NULL)
    return;

  /* производим подмену адреса отправителя */
  uiSrcAddr = modisg_dnat_get(p_psoIP->daddr, psoTCP->dest);
  csum_replace4(&p_psoIP->check, p_psoIP->saddr, uiSrcAddr);
  inet_proto_csum_replace4(&psoTCP->check, p_psoSKB, p_psoIP->saddr, uiSrcAddr, 1);
  p_psoIP->saddr = uiSrcAddr;
}
