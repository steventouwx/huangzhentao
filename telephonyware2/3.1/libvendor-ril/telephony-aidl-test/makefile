SHELL = /bin/sh

# 定义源文件目录和头文件目录
SRC1 := ./tbox-service/tbox-service-test.cpp
SRC2 += ./tbox-client/tbox-client-test.cpp

CPPFLAGS += -I$(SRC_DIR)/include

# 定义依赖的共享库，假设它们位于 /usr/local/lib 目录
LDLIBS := -lbinder -lutils -lvendor.mediatek.hardware.tbox-V1-ndk -lbinder_ndk

TARGET1 := tbox-service-test
TARGET2 := tbox-client-test

# 定义链接选项，-shared 用于生成共享库
LDFLAGS := -std=gnu++17 -fno-rtti

# 默认目标
all: $(TARGET1) $(TARGET2) 

# 构建目标共享库
$(TARGET1): $(SRC1)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(TARGET2): $(SRC2)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# 清理编译生成的文件
clean:
	rm -f $(TARGET)
