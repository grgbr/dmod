/******************************************************************************
 * SPDX-License-Identifier: LGPL-3.0-only
 *
 * This file is part of DMod.
 * Copyright (C) 2024 Grégor Boirie <gregor.boirie@free.fr>
 ******************************************************************************/

#ifndef _DMOD_COMMON_H
#define _DMOD_COMMON_H

#include "dmod/xact.h"

#define dmod_xact_ops_assert_intern(_ops) \
	dmod_assert_intern(_ops); \
	dmod_assert_intern((_ops)->begin); \
	dmod_assert_intern((_ops)->end); \
	dmod_assert_intern((_ops)->abort); \
	dmod_assert_intern((_ops)->destroy)

#define dmod_xact_assert_intern(_xact) \
	dmod_xact_ops_assert_intern((_xact)->ops)

#endif /* _DMOD_COMMON_H */
