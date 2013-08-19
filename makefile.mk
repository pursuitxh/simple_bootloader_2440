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

cur_objs := $(filter-out %/, $(obj-y))
dep_files := $(wildcard, $(foreach f, $(cur_objs), .$(f).d))

ifneq ($(dep_files), )
	include $(dep_files)
endif

builtin-obj = built-in.o

obj-y := $(patsubst %/, %/$(builtin-obj), $(obj-y))

PHONY += $(subdir-y)

_build: $(subdir-y) $(builtin-obj) 

$(subdir-y):
	make -f $(TOPDIR)/makefile.mk -C $@

$(builtin-obj): $(obj-y)
	$(LD) -r -o $@ $^

dep_file = .$@.d

%.o:%.c
	$(CC) $(CFLAGS) -Wp,-MD,$(dep_file) -c -o $@ $<
%.o:%.S
	$(CC) $(CFLAGS) -Wp,-MD,$(dep_file) -c -o $@ $<

.PHONY: $(PHONY)
