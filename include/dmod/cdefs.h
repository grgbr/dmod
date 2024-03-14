/******************************************************************************
 * SPDX-License-Identifier: LGPL-3.0-only
 *
 * This file is part of DMod.
 * Copyright (C) 2024 Grégor Boirie <gregor.boirie@free.fr>
 ******************************************************************************/

/**
 * @file
 * Common definitions
 *
 * @author    Grégor Boirie <gregor.boirie@free.fr>
 * @date      20 Feb 2024
 * @copyright Copyright (C) 2024 Grégor Boirie <gregor.boirie@free.fr>
 * @license   [GNU Lesser General Public License (LGPL) v3]
 *            (https://www.gnu.org/licenses/lgpl+gpl-3.0.txt)
 */

#ifndef _DMOD_CDEFS_H
#define _DMOD_CDEFS_H

#include <dmod/priv/config.h>
#include <stroll/cdefs.h>

#define __dmod_export  __export_public

#if defined(CONFIG_DMOD_ASSERT_API) || defined(CONFIG_DMOD_ASSERT_INTERN)

#define __dmod_nonull(_arg_index, ...)
#define __dmod_const
#define __dmod_pure
#define __dmod_nothrow

#else   /* !(defined(CONFIG_DMOD_ASSERT_API) || \
             defined(CONFIG_DMOD_ASSERT_INTERN)) */

#define __dmod_nonull(_arg_index, ...) \
	__nonull(_arg_index, ## __VA_ARGS__)

#define __dmod_const   __const
#define __dmod_pure    __pure
#define __dmod_nothrow __nothrow

#endif /* defined(CONFIG_DMOD_ASSERT_API) || \
          defined(CONFIG_DMOD_ASSERT_INTERN) */

#if defined(CONFIG_DMOD_ASSERT_API)

#include <stroll/assert.h>

#define dmod_assert_api(_cond) \
	stroll_assert("dmod", _cond)

#else  /* !defined(CONFIG_DMOD_ASSERT_API) */

#define dmod_assert_api(_cond)

#endif /* defined(CONFIG_DMOD_ASSERT_API) */

#if defined(CONFIG_DMOD_ASSERT_INTERN)

#define dmod_assert_intern(_cond) \
	stroll_assert("dmod", _cond)

#else  /* !defined(CONFIG_DMOD_ASSERT_INTERN) */

#define dmod_assert_intern(_cond) \
	do {} while (0)

#endif /* defined(CONFIG_DMOD_ASSERT_INTERN) */

#endif /* _DMOD_CDEFS_H */
