# make first
# $^ 依赖，不重复
# $@  目标
# @ 不显示执行命令
# - 失败之后不影响下一行代码执行

TARGET = first_make
LIBS = -lpthread
OBJS = first_make.o xdata.o

CXXFLAGS=-I../test_gcc

$(TARGET): $(OBJS)
	@#-@rm test
	@echo "begin build $(TARGET)"
	@$(CXX) $^ -o $@ $(LIBS)
	@echo "build success!"

clean:
	$(RM) $(OBJS) $(TARGET)
.PHONY: clean *clean
