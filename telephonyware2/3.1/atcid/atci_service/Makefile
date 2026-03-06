TARGET := atci_service

FLAGS += -DENABLE_GPS_AT_CMD
CFFFLAGS += -std=c++17
INCLUDES := -Isrc/ \
            -I ../atci/src \


SRCS := \
           src/at_tok.c \
           src/atci_battery_cmd.c \
           src/atci_generic_cmd_dispatch.c \
           src/atci_gm_cmd.c \
           src/atci_gps_cmd.c \
           src/atci_lcdbacklight_vibrator_cmd.c \
           src/atci_service.c \
           src/atci_system_cmd.c \
           src/atci_efuse_cmd.c \
           src/atci_touch_cmd.c \
           src/atci_touchpanel_cmd.c \
           src/atci_util.c \

CXXSRCS := \
           src/atci_pq_cmd.cpp \

EXTRA_CXXFLAGS := -std=c++11

OBJS := ${SRCS:%.c=%.o}

CXXOBJS := ${CXXSRCS:%.cpp=%.o}

#LDFLAGS = -L. -lz -ldl -lpthread -lstdc++

.PHONY: all
all : $(TARGET)

${TARGET}: $(OBJS) $(CXXOBJS)
	$(CC) $(OBJS) $(CXXOBJS) $(LDFLAGS) -L. -rdynamic -lpqinterface -lstdc++ -ldl -lcutils -lutils -llog -ldl $(LIBS) -o $@

%.o: %.c
	${CC} $(CFLAGS) -c $< -o $@ $(INCLUDES) $(FLAGS)

%.o: %.cpp
	${CXX} -L. -rdynamic -lpqinterface -lcutils -lutils -ldl -lstdc++ -llog -ldl $(CXXFLAGS) -c $< -o $@ $(INCLUDES) $(CFFFLAGS) $(FLAGS) $(EXTRA_CXXFLAGS)

.PHONY: clean
clean:
	$(warning "makefile atci_service clean")
	rm -rf $(OBJS) $(CXXOBJS) $(TARGET)

.PHONY: install
install: $(TARGET)

	install -d $(DESTDIR)$(bindir)
	install -m 0755 atci_service $(DESTDIR)$(bindir)

