/******************************************************************************
 * SPDX-License-Identifier: LGPL-3.0-only
 *
 * This file is part of DMod.
 * Copyright (C) 2024 Grégor Boirie <gregor.boirie@free.fr>
 ******************************************************************************/

/**
 * @file
 * Data MODel iterator interface
 *
 * @author    Grégor Boirie <gregor.boirie@free.fr>
 * @date      13 Mar 2024
 * @copyright Copyright (C) 2024 Grégor Boirie <gregor.boirie@free.fr>
 * @license   [GNU Lesser General Public License (LGPL) v3]
 *            (https://www.gnu.org/licenses/lgpl+gpl-3.0.txt)
 */
#ifndef _DMOD_ITER_H
#define _DMOD_ITER_H

#include <dmod/priv/cdefs.h>
#include <stdlib.h>

struct dmod_iter;

typedef struct dmod_object *
        (dmod_iter_step_fn)(struct dmod_iter *);

typedef int
        (dmod_iter_rewind_fn)(struct dmod_iter *);

typedef void
        (dmod_iter_fini_fn)(struct dmod_iter *);

struct dmod_iter_ops {
	dmod_iter_step_fn *   step;
	dmod_iter_rewind_fn * rewind;
	dmod_iter_fini_fn *   fini;
};

#define dmod_iter_ops_assert_api(_ops) \
	dmod_assert_api(_ops); \
	dmod_assert_api((_ops)->step); \
	dmod_assert_api((_ops)->rewind); \
	dmod_assert_api((_ops)->fini)

struct dmod_iter {
	const struct dmod_iter_ops * ops;
	int                          err;
};

#define dmod_iter_assert_api(_iter) \
	dmod_iter_ops_assert_api((_iter)->ops)

static inline void
dmod_iter_null_fini(struct dmod_iter * iter __unused)
{
	dmod_iter_assert_api(iter);
}

static inline int
dmod_iter_error(const struct dmod_iter * __restrict iter)
{
	dmod_iter_assert_api(iter);

	return iter->err;
}

static inline struct dmod_object *
dmod_iter_step(struct dmod_iter * iter)
{
	dmod_iter_assert_api(iter);
	dmod_assert_api(!iter->err);

	return iter->ops->step(iter);
}

static inline int
dmod_iter_rewind(struct dmod_iter * iter)
{
	dmod_iter_assert_api(iter);
	dmod_assert_api(!iter->err);

	iter->err = iter->ops->rewind(iter);

	return iter->err;
}

static inline void
dmod_iter_init(struct dmod_iter * __restrict           iter,
               const struct dmod_iter_ops * __restrict ops)
{
	dmod_assert_api(iter);
	dmod_iter_ops_assert_api(ops);

	iter->ops = ops;
	iter->err = 0;
}

static inline void
dmod_iter_fini(struct dmod_iter * iter)
{
	dmod_assert_api(iter);

	iter->ops->fini(iter);
}

static inline void
dmod_iter_destroy(struct dmod_iter * iter)
{
	dmod_iter_fini(iter);

	free(iter);
}

struct dmod_const_iter;

typedef const struct dmod_object *
        (dmod_const_iter_step_fn)(struct dmod_const_iter *);

typedef int
        (dmod_const_iter_rewind_fn)(struct dmod_const_iter *);

typedef void
        (dmod_const_iter_fini_fn)(struct dmod_const_iter *);


struct dmod_const_iter_ops {
	dmod_const_iter_step_fn *   step;
	dmod_const_iter_rewind_fn * rewind;
	dmod_const_iter_fini_fn *   fini;
};

struct dmod_const_iter {
	const struct dmod_const_iter_ops * ops;
	int                                err;
};

static inline void
dmod_const_iter_null_fini(struct dmod_const_iter * iter __unused)
{
	dmod_iter_assert_api(iter);
}

static inline int
dmod_const_iter_error(const struct dmod_const_iter * __restrict iter)
{
	dmod_iter_assert_api(iter);

	return iter->err;
}

static inline const struct dmod_object *
dmod_const_iter_step(struct dmod_const_iter * iter)
{
	dmod_iter_assert_api(iter);
	dmod_assert_api(!iter->err);

	return iter->ops->step(iter);
}

static inline int
dmod_const_iter_rewind(struct dmod_const_iter * iter)
{
	dmod_iter_assert_api(iter);
	dmod_assert_api(!iter->err);

	iter->err = iter->ops->rewind(iter);

	return iter->err;
}

static inline void
dmod_const_iter_init(struct dmod_const_iter * __restrict           iter,
                     const struct dmod_const_iter_ops * __restrict ops)
{
	dmod_assert_api(iter);
	dmod_iter_ops_assert_api(ops);

	iter->ops = ops;
	iter->err = 0;
}

static inline void
dmod_const_iter_fini(struct dmod_const_iter * iter)
{
	dmod_assert_api(iter);

	iter->ops->fini(iter);
}

static inline void
dmod_const_iter_destroy(struct dmod_const_iter * iter)
{
	dmod_const_iter_fini(iter);

	free(iter);
}

#endif /* _DMOD_ITER_H */
