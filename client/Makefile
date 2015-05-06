#bash/bin

TOPDIR=.

TARGE=Client

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp = .o)
CC = g++

#dirs :=        Kmp buf
#all:
#         $(foreach N,$(dirs),$(make -C $(N)))

#include header
INCLUDES = -I/home/boostlib/include

#include lib path
LIBS = -L/home/boostlib/lib
LDLIBS = -lboost_system -lboost_thread -lpthread

CCFLAGS = -g -Wall 

$(TARGE):$(OBJS)
	$(CC) $^ -o $@ $(INCLUDES) $(LIBS) $(LDLIBS) $(CCFLAGS)

%.o:%.cpp
	$(CC) -c $< $(CCFLAGS)

clean:
	rm *.o $(TARGE)
