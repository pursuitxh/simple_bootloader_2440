# file: makefile.mk
# created on: 2013-08-19
# author: pusuitxh  
# email: pursuitxh@gmail.com

PHONY := _build
_build:

obj-y :=
subdir-y :=

include Makefile

_subdir-y := $(patsubst %/, %, $(filter %/, $(obj-y)))
subdir-y += $(_subdir-y)

subdir_objs := $(foreach f, $(subdir-y), $(f)/built-in.o)
cur_objs := $(filter-out %/, $(obj-y))

dep_files := $(wildcard, $(foreach f, $(cur_objs), .$(f).d))

ifneq ($(dep_files), )
	include $(dep_files)
endif

PHONY += $(subdir-y)

_build: $(subdir-y) built-in.o

$(subdir-y):
	make -f $(TOPDIR)/makefile.mk -C $@

built-in.o: $(cur_objs) $(subdir_objs)
	$(LD) -r -o $@ $^

dep_file = .$@.d

%.o:%c
	$(CC) $(CFLAGS) -Wp,-MD, $(dep_file) -c -o $@ $<
%.o:%S
	$(CC) $(CFLAGS) -Wp,-MD, $(dep_file) -c -o $@ $<

.PHONY: $(PHONY)
