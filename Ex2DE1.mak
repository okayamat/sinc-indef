# Makefile ( n source file 1 execute file version )

PACKAGE	= Ex2DE1
SRCS	= $(PACKAGE).c DE_trans.c
HEADS	= DE_trans.h
OBJS	= $(SRCS:.c=.o)
FILES	= Ex2DE1 $(HEADS) $(SRCS)
VER	= `date +%Y%m%d`


### command and flags ###
# uncomment when debugging
#DEBUG	= -ggdb -pg # -lefence

# common (*.o)
LD	= gcc
GSL_LD	= `gsl-config --libs-without-cblas`
LDFLAGS	= -g $(DEBUG) $(GSL_LD)
LDLIBS	= 

# C (*.c)
CC	= gcc
OPTIMIZE= -O2
CFLAGS	= -g $(OPTIMIZE) -Wall $(DEBUG) $(GSL_FL)
CPPFLAGS= -I.

# C++ (*.cpp)
CXX	= g++
GSL_FL	= `gsl-config --cflags`
CXXFLAGS= -g $(OPTIMIZE) -Wall $(DEBUG) $(GSL_FL)
# $(GSL_FL) 

# Fortran77 (*.f)
FC	= f77
FFLAGS	= -Wall $(DEBUG)

# Pascal (*.p)
PC	= pc
PFLAGS	= -Wall $(DEBUG)

# etc
SHELL	= /bin/sh
RM	= rm -f
PROF	= gprof


### rules ###

.SUFFIXES:
.SUFFIXES: .o .c .cpp .f .p

all: $(PACKAGE)

$(PACKAGE): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)

$(OBJS): $(HEADS) $(PACKAGE).mak

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
.cpp.o:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
.f.o:
	$(FC) $(FFLAGS) -c $< -o $@
.p.o:
	$(PC) $(PFLAGS) $(CPPFLAGS) -c $< -o $@


### useful commands ###

clean:
	$(RM) $(PACKAGE) $(OBJS)
	$(RM) core gmon.out *~ #*#

tar:
	@echo $(PACKAGE)-$(VER) > .package
	@$(RM) -r `cat .package`
	@mkdir `cat .package`
	@ln $(FILES) `cat .package`
	tar cvf - `cat .package` | gzip -9 > `cat .package`.tar.gz
	@$(RM) -r `cat .package` .package

zip:
	zip -9 $(PACKAGE)-$(VER).zip $(FILES)


prof: run
	$(PROF) $(PACKAGE) | less

run: all
	./$(PACKAGE) | tee $(PACKAGE).dat
