# 指定编译选项
CXX = g++
CXXFLAGS = -Wall -I$(include_dir) 
LDFLAGS = -Wall -lyaml-cpp
VPATH = ../include

# 用于给文件名添加路径前缀，第一个参数为路径，第二个参数为文件名列表
add_path = $(2:%=$(1)/%)
target_bin = $(call add_path,$(bin_dir),$@)

# 指定各编译文件路径
src_dir = .
include_dir = ../include
bin_dir = ../bin

# 各文件名
srcs = main.cpp common.cpp graph.cpp  
objs = $(srcs:.cpp=.o)
templates = graph.h
srcs_full = $(call add_path,$(src_dir),$(srcs))
objs_full = $(call add_path,$(bin_dir),$(objs))

main: $(objs) 
	$(CXX) $(objs_full) -o $(target_bin) $(LDFLAGS)  
	$(target_bin)
    
main.o: main.cpp $(templates)
	$(CXX) $(CXXFLAGS) -c $< -o $(target_bin)

%.o: %.cpp %.h 
	$(CXX) $(CXXFLAGS) -c $< -o $(target_bin)

.PHONY:clean
clean:
	-rm $(bin_dir)/*
