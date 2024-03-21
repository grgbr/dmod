/******************************************************************************
 * SPDX-License-Identifier: LGPL-3.0-only
 *
 * This file is part of DMod.
 * Copyright (C) 2024 Grégor Boirie <gregor.boirie@free.fr>
 ******************************************************************************/

/**
 * @file
 * Data MODel transaction interface
 *
 * @author    Grégor Boirie <gregor.boirie@free.fr>
 * @date      19 Mar 2024
 * @copyright Copyright (C) 2024 Grégor Boirie <gregor.boirie@free.fr>
 * @license   [GNU Lesser General Public License (LGPL) v3]
 *            (https://www.gnu.org/licenses/lgpl+gpl-3.0.txt)
 */
#ifndef _DMOD_XACT_H
#define _DMOD_XACT_H

#include <dmod/priv/cdefs.h>
#include <stdlib.h>

struct dmod_xact;

typedef int
        (dmod_xact_begin_fn)(struct dmod_xact *, struct dmod_xact *);

typedef int
        (dmod_xact_end_fn)(struct dmod_xact *, int);

typedef int
        (dmod_xact_assess_fn)(struct dmod_xact *);

typedef const char *
        (dmod_xact_errstr_fn)(int);

typedef void
        (dmod_xact_destroy_fn)(struct dmod_xact *);

struct dmod_xact_ops {
	dmod_xact_begin_fn *    begin;
	dmod_xact_end_fn *      end;
	dmod_xact_assess_fn *   commit;
	dmod_xact_assess_fn *   rollback;
	dmod_xact_errstr_fn *   errstr;
	dmod_xact_destroy_fn *  destroy;
};

#define dmod_xact_ops_assert_api(_ops) \
	dmod_assert_api(_ops); \
	dmod_assert_api((_ops)->begin); \
	dmod_assert_api((_ops)->end); \
	dmod_assert_api((_ops)->commit); \
	dmod_assert_api((_ops)->rollback); \
	dmod_assert_api((_ops)->errstr); \
	dmod_assert_api((_ops)->destroy)

struct dmod_xact {
	const struct dmod_xact_ops * ops;
};

#define dmod_xact_assert_api(_xact) \
	dmod_xact_ops_assert_api((_xact)->ops)

static inline int __dmod_nonull(1)
dmod_xact_begin(struct dmod_xact * xact, struct dmod_xact * parent)
{
	dmod_xact_assert_api(xact);
#if defined(DMOD_ASSERT_API)
	if (parent)
		dmod_xact_assert_api(parent);
#endif /* defined(DMOD_ASSERT_API) */

	return xact->ops->begin(xact, parent);
}

static inline int __dmod_nonull(1)
dmod_xact_end(struct dmod_xact * xact, int status)
{
	dmod_xact_assert_api(xact);

	return xact->ops->end(xact, status);
}

static inline int __dmod_nonull(1)
dmod_xact_commit(struct dmod_xact * xact)
{
	dmod_xact_assert_api(xact);

	return xact->ops->commit(xact);
}

static inline int __dmod_nonull(1)
dmod_xact_rollback(struct dmod_xact * xact)
{
	dmod_xact_assert_api(xact);

	return xact->ops->rollback(xact);
}

static inline const char *
dmod_xact_strerror(const struct dmod_xact * __restrict xact, int error)
{
	dmod_xact_assert_api(xact);

	return xact->ops->errstr(error);
}

static inline void __dmod_nonull(1)
dmod_xact_destroy(struct dmod_xact * xact)
{
	dmod_xact_assert_api(xact);

	xact->ops->destroy(xact);
}

static inline void __dmod_nonull(1, 2) __dmod_nothrow
dmod_xact_init(struct dmod_xact * __restrict           xact,
               const struct dmod_xact_ops * __restrict ops)
{
	dmod_assert_api(xact);
	dmod_xact_ops_assert_api(ops);

	xact->ops = ops;
}

static inline void __dmod_nonull(1) __dmod_nothrow
dmod_xact_free(struct dmod_xact * xact)
{
	dmod_assert_api(xact);

	free(xact);
}
#if defined(CONFIG_DMOD_XACT_NULL)

extern struct dmod_xact *
dmod_xact_create_null(void)
	__dmod_nothrow __leaf __dmod_export;

#endif /* defined(CONFIG_DMOD_XACT_NULL) */

#if defined(CONFIG_DMOD_KVSTORE)

struct dmod_xact_kvs;
struct kvs_repo;

extern struct dmod_xact_kvs *
dmod_xact_create_kvs(const struct kvs_repo * __restrict repo)
	__dmod_nonull(1) __dmod_nothrow __leaf __dmod_export;

#endif /* defined(CONFIG_DMOD_KVSTORE) */

#endif /* _DMOD_XACT_H */
