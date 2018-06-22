/*
 * PROTO Target module
 * This program is distributed under the terms of GNU GPL
 */
#include <stdio.h>
#include <xtables.h>
#include "xt_PROTO.h"

enum {
	O_PROTO_SET = 0,
	F_PROTO_SET = 1 << O_PROTO_SET,
	F_ANY    = F_PROTO_SET,
};

#define s struct xt_PROTO_info
static const struct xt_option_entry PROTO_opts[] = {
	{.name = "proto-set", .type = XTTYPE_UINT8, .id = O_PROTO_SET,
	 .excl = F_ANY, .flags = XTOPT_PUT, XTOPT_POINTER(s, proto)},
	XTOPT_TABLEEND,
};
#undef s

static void PROTO_help(void)
{
	printf(
"PROTO target options\n"
"  --proto-set value		Set protocol to <value 0-255>\n"
	);
}

static void PROTO_parse(struct xt_option_call *cb)
{
	struct xt_PROTO_info *info = cb->data;

	xtables_option_parse(cb);
	switch (cb->entry->id) {
	case O_PROTO_SET:
		info->mode = XT_PROTO_SET;
		break;
	}
}

static void PROTO_check(struct xt_fcheck_call *cb)
{
	if (!(cb->xflags & F_ANY))
		xtables_error(PARAMETER_PROBLEM,
				"PROTO: You must specify an action");
}

static void PROTO_save(const void *ip, const struct xt_entry_target *target)
{
	const struct xt_PROTO_info *info = 
		(struct xt_PROTO_info *) target->data;

	switch (info->mode) {
		case XT_PROTO_SET:
			printf(" --proto-set");
			break;
	}
	printf(" %u", info->proto);
}

static void PROTO_print(const void *ip, const struct xt_entry_target *target,
                     int numeric)
{
	const struct xt_PROTO_info *info =
		(struct xt_PROTO_info *) target->data;

	printf(" PROTO ");
	switch (info->mode) {
		case XT_PROTO_SET:
			printf("set to");
			break;
	}
	printf(" %u", info->proto);
}

static struct xtables_target proto_tg_reg = {
	.name 		= "PROTO",
	.version	= XTABLES_VERSION,
	.family		= NFPROTO_UNSPEC,
	.size		= XT_ALIGN(sizeof(struct xt_PROTO_info)),
	.userspacesize	= XT_ALIGN(sizeof(struct xt_PROTO_info)),
	.help		= PROTO_help,
	.print		= PROTO_print,
	.save		= PROTO_save,
	.x6_parse	= PROTO_parse,
	.x6_fcheck	= PROTO_check,
	.x6_options	= PROTO_opts,
};

static __attribute__((constructor)) void _init(void)
{
	xtables_register_target(&proto_tg_reg);

}
