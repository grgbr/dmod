/******************************************************************************
 * SPDX-License-Identifier: LGPL-3.0-only
 *
 * This file is part of DMod.
 * Copyright (C) 2024 Grégor Boirie <gregor.boirie@free.fr>
 ******************************************************************************/

#include "common.h"

/******************************************************************************
 * Transaction processing
 ******************************************************************************/

#include <kvstore/store.h>

struct dmod_xact_kvs {
	struct dmod_xact         base;
	struct kvs_xact          kvs;
	const struct kvs_depot * depot;
};

#define dmod_xact_assert_kvs(_xact) \
	dmod_xact_assert_intern(_xact); \
	dmod_assert_intern(((struct dmod_xact_kvs *)_xact)->depot)

static int __dmod_nonull(1)
dmod_xact_begin_kvs(struct dmod_xact * xact, struct dmod_xact * parent)
{
	dmod_xact_assert_kvs(xact);
#if defined(DMOD_ASSERT_INTERN)
	if (parent)
		dmod_xact_assert_intern(parent);
#endif /* defined(DMOD_ASSERT_INTERN) */

	struct dmod_xact_kvs * kxact = (struct dmod_xact_kvs *)xact;

	return kvs_begin_xact(kxact->depot,
	                      !parent ? NULL :
	                                &((struct dmod_xact_kvs *)parent)->kvs,
	                      &kxact->kvs,
	                      0);
}

static int __dmod_nonull(1)
dmod_xact_end_kvs(struct dmod_xact * xact, int status)
{
	dmod_xact_assert_kvs(xact);

	return kvs_end_xact(&((struct dmod_xact_kvs *)xact)->kvs, status);
}

static int __dmod_nonull(1)
dmod_xact_commit_kvs(struct dmod_xact * xact)
{
	dmod_xact_assert_kvs(xact);

	return kvs_commit_xact(&((struct dmod_xact_kvs *)xact)->kvs);
}

static int __dmod_nonull(1)
dmod_xact_rollback_kvs(struct dmod_xact * xact)
{
	dmod_xact_assert_kvs(xact);

	return kvs_rollback_xact(&((struct dmod_xact_kvs *)xact)->kvs);
}

static const char *
dmod_xact_kvs_errstr(int error)
{
	dmod_assert_api(error <= 0);

	return kvs_strerror(error);
}

static struct dmod_xact_kvs * __dmod_nothrow __attribute_malloc__ __warn_result
dmod_xact_alloc_kvs(void)
{
	return malloc(sizeof(struct dmod_xact_kvs));
}

static const struct dmod_xact_ops dmod_xact_kvs_ops = {
	.begin     = dmod_xact_begin_kvs,
	.end       = dmod_xact_end_kvs,
	.commit    = dmod_xact_commit_kvs,
	.rollback  = dmod_xact_rollback_kvs,
	.errstr    = dmod_xact_kvs_errstr,
	.destroy   = dmod_xact_free
};

struct dmod_xact_kvs *
dmod_xact_create_kvs(const struct kvs_depot * __restrict depot)
{
	dmod_assert_api(depot);

	struct dmod_xact_kvs * kxact;

	kxact = dmod_xact_alloc_kvs();
	if (!kxact)
		return NULL;

	dmod_xact_init(&kxact->base, &dmod_xact_kvs_ops);
	kxact->depot = depot;

	return kxact;
}