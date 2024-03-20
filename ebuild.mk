################################################################################
# SPDX-License-Identifier: LGPL-3.0-only
#
# This file is part of DMod.
# Copyright (C) 2024 Grégor Boirie <gregor.boirie@free.fr>
################################################################################

config-in := Config.in
config-h  := $(PACKAGE)/priv/config.h

HEADERDIR := $(CURDIR)/include
headers   := $(PACKAGE)/priv/cdefs.h
headers   += $(PACKAGE)/dmod.h
headers   += $(PACKAGE)/iter.h
headers   += $(PACKAGE)/xact.h

subdirs   += src

libstroll_pkgconf  := $(call kconf_enabled,DMOD_ASSERT_API,libstroll)
libkvstore_pkgconf := $(call kconf_enabled,DMOD_KVSTORE,libkvstore)

define libdmod_pkgconf_tmpl
prefix=$(PREFIX)
exec_prefix=$${prefix}
libdir=$${exec_prefix}/lib
includedir=$${prefix}/include

Name: libdmod
Description: dmod library
Version: $(VERSION)
Requires: $(libstroll_pkgconf)
Requires.private: $(libkvstore_pkgconf)
Cflags: -I$${includedir}
Libs: -L$${libdir} -ldmod
endef

pkgconfigs      := libdmod.pc
libdmod.pc-tmpl := libdmod_pkgconf_tmpl

################################################################################
# Source code tags generation
################################################################################

tagfiles := $(shell find $(addprefix $(CURDIR)/,$(subdirs)) \
                    $(HEADERDIR) \
                    -type f)
