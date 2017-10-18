#include "modisg.h"

/* Структура для регистрации функции перехватчика входящих ip пакетов */
struct nf_hook_ops prerouting_bundle, postrouting_bundle;

unsigned int hook_prerouting(
  const struct nf_hook_ops *ops,
  struct sk_buff *skb,
  const struct net_device *in,
  const struct net_device *out,
  int (*okfn)(struct sk_buff *));
unsigned int hook_postrouting(
  const struct nf_hook_ops *ops,
  struct sk_buff *skb,
  const struct net_device *in,
  const struct net_device *out,
  int (*okfn)(struct sk_buff *));

void modisg_hook_init(void)
{
	/* регистрируем функцию обработки прероутинга */
	prerouting_bundle.hook = hook_prerouting;
	prerouting_bundle.owner = THIS_MODULE;
	prerouting_bundle.pf = PF_INET;
	prerouting_bundle.hooknum = NF_INET_PRE_ROUTING;
	prerouting_bundle.priority = NF_IP_PRI_FIRST;
	nf_register_hook(&prerouting_bundle);

	/* регистрируем функцию обработки построутинга */
	postrouting_bundle.hook = hook_postrouting;
	postrouting_bundle.owner = THIS_MODULE;
	postrouting_bundle.pf = PF_INET;
	postrouting_bundle.hooknum = NF_INET_POST_ROUTING;
	postrouting_bundle.priority = NF_IP_PRI_LAST;
	nf_register_hook(&postrouting_bundle);
}

void modisg_hook_clean(void)
{
  /* Удаляем из цепочки hook функцию */
	nf_unregister_hook(&prerouting_bundle);
	nf_unregister_hook(&postrouting_bundle);
}

unsigned int hook_prerouting(
  const struct nf_hook_ops *ops,
  struct sk_buff *skb,
  const struct net_device *in,
  const struct net_device *out,
  int (*okfn)(struct sk_buff *))
{
  return (modisg_profile_req_oper(skb, NF_INET_PRE_ROUTING));
}

unsigned int hook_postrouting(
  const struct nf_hook_ops *ops,
  struct sk_buff *skb,
  const struct net_device *in,
  const struct net_device *out,
  int (*okfn)(struct sk_buff *))
{
  return (modisg_profile_req_oper(skb, NF_INET_POST_ROUTING));
}
