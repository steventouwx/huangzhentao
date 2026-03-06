SHELL = /bin/sh

# 定义源文件目录和头文件目录
SRC_DIR := ./interface_cpp/vendor/mediatek/hardware/tbox

# 自动查找所有的源文件 (.cpp)
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
CPPFLAGS += -I./interface_cpp/include

# 定义依赖的共享库，假设它们位于 /usr/local/lib 目录
LDLIBS := -lbinder -lutils

TARGET := libvendor.mediatek.hardware.tbox-V1-ndk.so

# 定义链接选项，-shared 用于生成共享库
LDFLAGS := -shared -std=gnu++17 -fPIC -Wall
# -fno-rtti
# 默认目标
all: $(TARGET)

# 构建目标共享库
$(TARGET): $(SOURCES)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# 清理编译生成的文件
clean:
	rm -f $(TARGET)
