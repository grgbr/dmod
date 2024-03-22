/******************************************************************************
 * SPDX-License-Identifier: LGPL-3.0-only
 *
 * This file is part of DMod.
 * Copyright (C) 2024 Grégor Boirie <gregor.boirie@free.fr>
 ******************************************************************************/

#include "common.h"

#if defined(CONFIG_DMOD_XACT_NULL)

#define dmod_xact_assert_null(_xact) \
	dmod_xact_assert_intern(_xact)

static int __dmod_nonull(1)
dmod_xact_begin_null(struct dmod_xact * xact __unused,
                     struct dmod_xact * parent __unused)
{
	dmod_xact_assert_null(xact);
#if defined(DMOD_ASSERT_INTERN)
	if (parent)
		dmod_xact_assert_intern(parent);
#endif /* defined(DMOD_ASSERT_INTERN) */

	return 0;
}

static int __dmod_nonull(1) __warn_result
dmod_xact_assess_null(struct dmod_xact * xact __unused, int status)
{
	dmod_xact_assert_null(xact);

	return status;
}

static struct dmod_xact * __dmod_nothrow __attribute_malloc__ __warn_result
dmod_xact_alloc_null(void)
{
	return malloc(sizeof(struct dmod_xact));
}

static const struct dmod_xact_ops dmod_xact_null_ops = {
	.begin     = dmod_xact_begin_null,
	.end       = dmod_xact_assess_null,
	.abort     = dmod_xact_assess_null,
	.destroy   = dmod_xact_free
};

struct dmod_xact *
dmod_xact_create_null(void)
{
	struct dmod_xact * xact;

	xact = dmod_xact_alloc_null();
	if (!xact)
		return NULL;

	dmod_xact_init(xact, &dmod_xact_null_ops);

	return xact;
}

#endif /* defined(CONFIG_DMOD_XACT_NULL) */
