/******************************************************************************
 * SPDX-License-Identifier: LGPL-3.0-only
 *
 * This file is part of DMod.
 * Copyright (C) 2024 Grégor Boirie <gregor.boirie@free.fr>
 ******************************************************************************/

/**
 * @file
 * Data MODel layer interface
 *
 * @author    Grégor Boirie <gregor.boirie@free.fr>
 * @date      20 Feb 2024
 * @copyright Copyright (C) 2024 Grégor Boirie <gregor.boirie@free.fr>
 * @license   [GNU Lesser General Public License (LGPL) v3]
 *            (https://www.gnu.org/licenses/lgpl+gpl-3.0.txt)
 */
#ifndef _DMOD_H
#define _DMOD_H

#include <dmod/priv/cdefs.h>
#include <stdbool.h>
#include <errno.h>

/******************************************************************************
 * Data model object
 ******************************************************************************/

enum dmod_object_state {
	DMOD_EMPTY_STATE = 0,
	DMOD_CLEAN_STATE,
	DMOD_DIRTY_STATE,
	DMOD_STATE_NR
};

struct dmod_object {
	enum dmod_object_state state;
};

static inline void
dmod_object_mark(struct dmod_object * __restrict obj,
                 enum dmod_object_state          state)
{
	dmod_assert_api(obj);
	dmod_assert_api(obj->state >= 0);
	dmod_assert_api(obj->state < DMOD_STATE_NR);

	obj->state = state;
}

static inline void
dmod_object_mark_empty(struct dmod_object * __restrict obj)
{
	dmod_object_mark(obj, DMOD_EMPTY_STATE);
}

static inline void
dmod_object_mark_clean(struct dmod_object * __restrict obj)
{
	dmod_object_mark(obj, DMOD_CLEAN_STATE);
}

static inline void
dmod_object_mark_dirty(struct dmod_object * __restrict obj)
{
	dmod_object_mark(obj, DMOD_DIRTY_STATE);
}

static inline enum dmod_object_state
dmod_object_state(const struct dmod_object * __restrict obj)
{
	dmod_assert_api(obj);
	dmod_assert_api(obj->state >= 0);
	dmod_assert_api(obj->state < DMOD_STATE_NR);

	return obj->state;
}

static inline bool
dmod_object_is_empty(const struct dmod_object * __restrict obj)
{
	return dmod_object_state(obj) == DMOD_EMPTY_STATE;
}

static inline bool
dmod_object_is_clean(const struct dmod_object * __restrict obj)
{
	return dmod_object_state(obj) == DMOD_CLEAN_STATE;
}

static inline bool
dmod_object_is_dirty(const struct dmod_object * __restrict obj)
{
	return dmod_object_state(obj) == DMOD_DIRTY_STATE;
}

static inline void
dmod_object_init(struct dmod_object * __restrict obj)
{
	dmod_assert_api(obj);

	obj->state = DMOD_EMPTY_STATE;
}

#if defined(CONFIG_DMOD_ASSERT_API)

static inline void
dmod_object_fini(struct dmod_object * __restrict obj)
{
	obj->state = DMOD_STATE_NR;
}

#else  /* !defined(CONFIG_DMOD_ASSERT_API) */

static inline void
dmod_object_fini(struct dmod_object * __restrict obj __unused)
{
}

#endif /* !defined(CONFIG_DMOD_ASSERT_API) */

/******************************************************************************
 * Data model object mapper
 ******************************************************************************/

struct dmod_mapper;
struct dmod_xact;

typedef int
        (dmod_mapper_save_fn)(struct dmod_mapper *,
                              struct dmod_object *,
                              struct dmod_xact *);

typedef const char *
        (dmod_mapper_errstr_fn)(int);

struct dmod_mapper_ops {
	dmod_mapper_save_fn *   save;
	dmod_mapper_errstr_fn * errstr;
};

#define dmod_mapper_ops_assert_api(_ops) \
	dmod_assert_api(_ops); \
	dmod_assert_api((_ops)->save); \
	dmod_assert_api((_ops)->errstr)

#define dmod_mapper_ops_assert_intern(_ops) \
	dmod_assert_intern(_ops); \
	dmod_assert_intern((_ops)->save); \
	dmod_assert_intern((_ops)->errstr)

struct dmod_mapper {
	const struct dmod_mapper_ops * ops;
};

#define dmod_mapper_assert_api(_mapper) \
	dmod_assert_api(_mapper); \
	dmod_mapper_ops_assert_api((_mapper)->ops)

#define dmod_mapper_assert_intern(_mapper) \
	dmod_assert_intern(_mapper); \
	dmod_mapper_ops_assert_intern((_mapper)->ops)

static inline const char *
dmod_mapper_strerror(const struct dmod_mapper * map, int error)
{
	dmod_mapper_assert_api(map);

	return map->ops->errstr(error);
}

static inline int
dmod_mapper_rdonly_save(struct dmod_mapper * __restrict mapper __unused,
                        struct dmod_object * __restrict object __unused,
                        struct dmod_xact * __restrict   xact __unused)
{
	dmod_mapper_assert_api(mapper);
	dmod_assert_api(object);

	return -ENOTSUP;
}

static inline int
dmod_mapper_save(struct dmod_mapper * map,
                 struct dmod_object * obj,
                 void *               data)
{
	dmod_mapper_assert_api(map);
	dmod_assert_api(obj);

	int ret;

	switch (dmod_object_state(obj)) {
	case DMOD_CLEAN_STATE:
		return 0;

	case DMOD_DIRTY_STATE:
		break;

	default:
		dmod_assert_api(0);
	}

	ret = map->ops->save(map, obj, data);
	if (ret)
		return ret;

	dmod_object_mark_clean(obj);

	return 0;
}

static inline void
dmod_mapper_init(struct dmod_mapper * map, const struct dmod_mapper_ops * ops)
{
	dmod_assert_api(map);
	dmod_mapper_ops_assert_api(ops);

	map->ops = ops;
}

static inline void
dmod_mapper_fini(struct dmod_mapper * map  __unused)
{
}

#endif /* _DMOD_H */
