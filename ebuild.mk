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

libstroll_pkgconf := $(call kconf_enabled,DMOD_ASSERT_API,libstroll)

define libdmod_pkgconf_tmpl
prefix=$(PREFIX)
exec_prefix=$${prefix}
libdir=$${exec_prefix}/lib
includedir=$${prefix}/include

Name: libdmod
Description: dmod library
Version: $(VERSION)
Requires.private: $(libstroll_pkgconf)
Cflags: -I$${includedir}
endef

pkgconfigs      := libdmod.pc
libdmod.pc-tmpl := libdmod_pkgconf_tmpl

################################################################################
# Source code tags generation
################################################################################

tagfiles := $(shell find $(HEADERDIR) -type f)
