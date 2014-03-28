# $Id: Makefile 6739 2006-06-05 07:52:38Z song $
#include des/Makefile

CXX = /usr/bin/g++
CXXFLAGS = $(ZEBRA_COMPILE_OPT) -I. -Wall -D_GNU_SOURCE
CXXLFLAGS = $(ZEBRA_COMPILE_OPT) -Wall -lpthread -lencdec -lz -L/usr/local/lib 

OBJS = tinyxml2.o Code.o
LIB_SRIPT = libscript.a
SRCS = $(OBJS:%.o=%.cpp)
DEPS = $(OBJS:%.o=.%.d)

all: $(LIB_SRIPT)

include $(DEPS)
$(DEPS): .%.d: %.cpp
	@$(CXX) -M $(CXXFLAGS) $< > $@.$$$$; sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; rm -f $@.$$$$

$(OBJS): %.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(LIB_SRIPT): $(OBJS) 
	@rm -rf $(LIB_SRIPT)
	@ar cru $(LIB_SRIPT) $(OBJS)
	@ranlib $(LIB_SRIPT)

clean:
	@rm -rf .*.d* *.o $(OBJS) $(LIB_SRIPT) core *~

