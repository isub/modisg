EXTRA_CFLAGS += -O3 -std=gnu89 --no-warnings
OBJS = modisg_mod.o modisg_list.o modisg_dev.o modisg_user_profile.o modisg_profile.o modisg_nat.o modisg_hook.o
TARGET = mod_isg

obj-m := $(TARGET).o
$(TARGET)-objs := $(OBJS)
KDIR = /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

$(TARGET).o: $(OBJS)
	$(LD) -r -o $@ $(OBJS)