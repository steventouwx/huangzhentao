# SPDX-License-Identifier:MediaTekProprietary
INCLUDES = -Iinclude
#-lpower
LIBS=$(TARGET_LDFLAGS) -L. -lprop  -lnvram -lubox -lnvram -lfile_op -lpthread -lccci

# Makefile

all: ccci_mdinit

ccci_mdinit:
	$(CC) $(INCLUDES) $(CFLAGS)   -o $@ main.c \
	ccci_legacy.c \
	ccci_log.c \
	env_setting.c \
	magic_pattern.c \
	time_clib_srv.c \
	ccci_support_utils.c \
	md_init_fsm.c \
	platform/md_init.c \
	platform/mdinit_platform.c \
	platform/mdinit_relate.c \
	ccci_fsd/ccci_fsd.c \
	ccci_fsd/platform/power.c \
	ccci_fsd/platform/fsd_platform.c $(LDFLAGS) $(LIBS)

clean:
	rm -f ccci_mdinit
