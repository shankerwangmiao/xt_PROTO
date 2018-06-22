/*
 * Protocol modification target for IP tables
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <net/checksum.h>

#include <linux/netfilter/x_tables.h>
#include "xt_PROTO.h"

MODULE_AUTHOR("Shanker Wang <i@innull.com>");
MODULE_DESCRIPTION("Xtables: Protocol field modification target");
MODULE_LICENSE("GPL");

static unsigned int
proto_tg(struct sk_buff *skb, const struct xt_action_param *par)
{
	struct iphdr *iph;
	const struct xt_PROTO_info *info = par->targinfo;
	int new_proto;

	if (!skb_make_writable(skb, skb->len))
		return NF_DROP;

	iph = ip_hdr(skb);

	switch (info->mode) {
	case XT_PROTO_SET:
		new_proto = info->proto;
		break;
	default:
		new_proto = iph->protocol;
		break;
	}

	if (new_proto != iph->protocol) {
		csum_replace2(&iph->check, htons(iph->protocol & 0xff),
					   htons(new_proto & 0xff));
		iph->protocol = new_proto;
	}

	return XT_CONTINUE;
}

static unsigned int
proto_tg6(struct sk_buff *skb, const struct xt_action_param *par)
{
	struct ipv6hdr *ip6h;
	const struct xt_PROTO_info *info = par->targinfo;
	int new_proto;

	if (!skb_make_writable(skb, skb->len))
		return NF_DROP;

	ip6h = ipv6_hdr(skb);

	switch (info->mode) {
	case XT_PROTO_SET:
		new_proto = info->proto;
		break;
	default:
		new_proto = ip6h->nexthdr;
		break;
	}

	ip6h->nexthdr = new_proto;

	return XT_CONTINUE;
}

static int proto_tg_check(const struct xt_tgchk_param *par)
{
	const struct xt_PROTO_info *info = par->targinfo;

	if (info->mode > XT_PROTO_MAXMODE)
		return -EINVAL;
	return 0;
}

static struct xt_target proto_tg_reg[] __read_mostly = {
	{
		.name       = "PROTO",
		.revision   = 0,
		.family     = NFPROTO_IPV4,
		.target     = proto_tg,
		.targetsize = sizeof(struct xt_PROTO_info),
		.table      = "mangle",
		.checkentry = proto_tg_check,
		.me         = THIS_MODULE,
	},
	{
		.name       = "PROTO",
		.revision   = 0,
		.family     = NFPROTO_IPV6,
		.target     = proto_tg6,
		.targetsize = sizeof(struct xt_PROTO_info),
		.table      = "mangle",
		.checkentry = proto_tg_check,
		.me         = THIS_MODULE,
	},
};

static int __init proto_tg_init(void)
{
	return xt_register_targets(proto_tg_reg, ARRAY_SIZE(proto_tg_reg));
}

static void __exit proto_tg_exit(void)
{
	xt_unregister_targets(proto_tg_reg, ARRAY_SIZE(proto_tg_reg));
}

module_init(proto_tg_init);
module_exit(proto_tg_exit);
MODULE_ALIAS("ipt_PROTO");
MODULE_ALIAS("ip6t_PROTO");

