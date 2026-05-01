# 使用wildcard函数来匹配当前目录下所有的.c文件
# 将生成的列表赋值给src
src = $(wildcard *.c)

# 使用patsubst函数（模式替换）来转换src变量中的文件扩展名
# 将所有.c文件的扩展名替换为空（即去掉.c）
# 这样target变量就包含了所有C源文件对应的可执行文件名
target = $(patsubst %.c, %, $(src))

# 定义了一个目标ALL，它依赖于target变量中列出的所有可执行文件
ALL:$(target)
Args = -Wall -g

# 任何匹配%.c的文件（即所有.c文件）都会生成一个对应的目标文件（去掉.c扩展名的文件）
# $<代表第一个依赖文件（即当前处理的.c文件），$@代表当前的目标（即对应的可执行文件）
%:%.c
	gcc $< -o $@ $(Args)


# 声明clean是伪目标,Makefile中定义的特殊目标
.PHONY:clean
clean:
	rm $(target)
