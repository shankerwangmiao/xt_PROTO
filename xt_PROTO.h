/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
/* Protocol modification module for IP tables */

#ifndef _XT_PROTO_H
#define _XT_PROTO_H

#include <linux/types.h>

enum {
	XT_PROTO_SET = 0,
};

#define XT_PROTO_MAXMODE	XT_PROTO_SET

struct xt_PROTO_info {
	__u8	mode;
	__u8	proto;
};

#endif
