TARGET := ccci_rpcd

INCLUDES := -I. \
         -I./platform \

SRCS := \
           ./ccci_rpcd.c \
           ./platform/ccci_rpcd_platform.c \
           ./platform/power.c \
#-lpower
LIBS=$(TARGET_LDFLAGS) -lpthread -lprop -lccci -lubox -ldl

OBJS := ${SRCS:%.c=%.o}

.PHONY: all
all : $(TARGET)

${TARGET}: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) $(LIBS) -o $@

%.o: %.c
	${CC} $(CFLAGS) -c $< -o $@ $(INCLUDES) $(FLAGS)

.PHONY: clean
clean:
	$(warning "makefile ccci_rpcd_com clean")
	rm -rf $(OBJS) $(TARGET)
