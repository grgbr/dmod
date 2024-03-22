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

#include <kvstore/repo.h>

struct dmod_xact_kvs {
	struct dmod_xact         base;
	struct kvs_xact          kvs;
	const struct kvs_repo *  repo;
};

#define dmod_xact_assert_kvs(_xact) \
	dmod_xact_assert_intern(_xact); \
	dmod_assert_intern(((struct dmod_xact_kvs *)_xact)->repo)

struct kvs_xact *
dmod_xact_get_kvs(struct dmod_xact * xact)
{
	dmod_xact_assert_kvs(xact);

	return &((struct dmod_xact_kvs *)xact)->kvs;
}

static int __dmod_nonull(1)
dmod_xact_begin_kvs(struct dmod_xact * xact, struct dmod_xact * parent)
{
	dmod_xact_assert_kvs(xact);
#if defined(DMOD_ASSERT_INTERN)
	if (parent)
		dmod_xact_assert_intern(parent);
#endif /* defined(DMOD_ASSERT_INTERN) */

	struct dmod_xact_kvs * kxact = (struct dmod_xact_kvs *)xact;

	return kvs_repo_begin_xact(
		kxact->repo,
		!parent ? NULL : &((struct dmod_xact_kvs *)parent)->kvs,
		&kxact->kvs,
		0);
}

static int __dmod_nonull(1) __warn_result
dmod_xact_end_kvs(struct dmod_xact * xact, int status)
{
	dmod_xact_assert_kvs(xact);

	return kvs_complete_xact(&((struct dmod_xact_kvs *)xact)->kvs, status);
}

static int __dmod_nonull(1) __warn_result
dmod_xact_abort_kvs(struct dmod_xact * xact, int status)
{
	dmod_xact_assert_kvs(xact);

	return kvs_abort_xact(&((struct dmod_xact_kvs *)xact)->kvs, status);
}

static struct dmod_xact_kvs * __dmod_nothrow __attribute_malloc__ __warn_result
dmod_xact_alloc_kvs(void)
{
	return malloc(sizeof(struct dmod_xact_kvs));
}

static const struct dmod_xact_ops dmod_xact_kvs_ops = {
	.begin     = dmod_xact_begin_kvs,
	.end       = dmod_xact_end_kvs,
	.abort     = dmod_xact_abort_kvs,
	.destroy   = dmod_xact_free
};

struct dmod_xact_kvs *
dmod_xact_create_kvs(const struct kvs_repo * __restrict repo)
{
	dmod_assert_api(repo);

	struct dmod_xact_kvs * kxact;

	kxact = dmod_xact_alloc_kvs();
	if (!kxact)
		return NULL;

	dmod_xact_init(&kxact->base, &dmod_xact_kvs_ops);
	kxact->repo = repo;

	return kxact;
}
