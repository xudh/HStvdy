CROSS =
CPP = @echo " G++   $@"; $(CROSS)g++
AR = @echo " AR    $@"; $(CROSS)ar
RM = @echo " RM    $@"; rm -f

TOPDIR = ./
SRCDIR = $(TOPDIR)Source/
TESTDIR = $(TOPDIR)Test/
LIBDIR = $(TOPDIR)Lib/
BINDIR = $(TOPDIR)Bin/

CFLAGS += -I$(TOPDIR)Include
CFLAGS += -Wall -Os -O2
CFLAGS += -fomit-frame-pointer

AFLAGS  += -rv

LIB_TARGET= $(LIBDIR)libVirtualTest.a

LIB_OBJS =	$(SRCDIR)Test1Layer.o   \
		$(SRCDIR)Test2Layer.o   \
		$(SRCDIR)Test3LayerA.o  \
		$(SRCDIR)Test3LayerB.o  \
		$(SRCDIR)Test3Layer.o

TEST_OBJS = $(TESTDIR)Main.o

TEST_TARGET = $(BINDIR)VirtualTest

TARGET  = $(LIB_TARGET) $(TEST_TARGET)

staticlib: $(LIB_TARGET)
test: $(TEST_TARGET)
all: staticlib test

$(LIB_TARGET): $(LIB_OBJS)
	$(RM) $@;
	$(AR) $(AFLAGS) $@ $^

$(TEST_TARGET): $(TEST_OBJS) $(LIB_TARGET)
	$(CPP) -o $@ $^ $(LDFLAGS)

.cpp.o:
	$(CPP) -c $(CFLAGS) $^ -o $@

clean:
	$(RM) $(LIB_TARGET) $(TEST_TARGET) $(LIB_OBJS) $(TEST_OBJS)

