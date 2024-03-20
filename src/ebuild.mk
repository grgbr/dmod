################################################################################
# SPDX-License-Identifier: LGPL-3.0-only
#
# This file is part of DMod.
# Copyright (C) 2024 Grégor Boirie <gregor.boirie@free.fr>
################################################################################

common-cflags  := -Wall \
                  -Wextra \
                  -Wformat=2 \
                  -Wconversion \
                  -Wundef \
                  -Wshadow \
                  -Wcast-qual \
                  -Wcast-align \
                  -Wmissing-declarations \
                  -D_GNU_SOURCE \
                  -I $(TOPDIR)/include \
                  -I $(TOPDIR) \
                  $(EXTRA_CFLAGS) \
                  -fvisibility=hidden
common-ldflags := $(common-cflags) \
                  -L $(BUILDDIR)/../common \
                  $(EXTRA_LDFLAGS) \
                  -Wl,--as-needed \
                  -Wl,-z,start-stop-visibility=hidden

# When assertions are enabled, ensure NDEBUG macro is not set to enable glibc
# assertions.
ifneq ($(filter y,$(CONFIG_DMOD_ASSERT_API) $(CONFIG_DMOD_ASSERT_INTERN)),)
common-cflags  := $(filter-out -DNDEBUG,$(common-cflags))
common-ldflags := $(filter-out -DNDEBUG,$(common-ldflags))
endif # ($(filter y,$(CONFIG_DMOD_ASSERT_API) $(CONFIG_DMOD_ASSERT_INTERN)),)

solibs             := libdmod.so
libdmod.so-objs     = shared/common.o
libdmod.so-objs    += $(call kconf_enabled,DMOD_KVSTORE,shared/kvstore.o)
libdmod.so-cflags  := $(filter-out -fpie -fPIE,$(common-cflags)) -fpic
libdmod.so-ldflags := $(filter-out -fpie -fPIE,$(common-ldflags)) \
                      -shared -fpic -Bsymbolic \
                      -Wl,-soname,libdmod.so
libdmod.so-pkgconf  = $(call kconf_enabled,DMOD_ASSERT_API,libstroll)
libdmod.so-pkgconf += $(call kconf_enabled,DMOD_KVSTORE,libkvstore)

arlibs             := libdmod.a
libdmod.a-objs      = static/common.o
libdmod.a-objs     += $(call kconf_enabled,DMOD_KVSTORE,static/kvstore.o)
libdmod.a-cflags   := $(common-cflags)
libdmod.a-pkgconf   = $(call kconf_enabled,DMOD_ASSERT_API,libstroll)
libdmod.a-pkgconf  += $(call kconf_enabled,DMOD_KVSTORE,libkvstore)

# vim: filetype=make :
