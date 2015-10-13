CC = gcc
CFLAGS = -Wall -g
LIBS = -lm

LIBDIR = ../libsrc
SRCDIR = ../src

## Summary Settings
SUMMARYDIR = Summary

TOPDIR = TOP_Value
Variance_DIR = TOP_Variance
TOPDIR_2Pass = TOP_Value_2Pass
Variance_DIR_2Pass = TOP_Variance_2Pass
Variance_DIR_exact = TOP_Variance_Exact
Variance_DIR_2W = TOP_Variance_2W
Variance_DIR_1W = TOP_Variance_1W

Exact_vs_Direct = Exact_vs_Direct
Exact_vs_2Pass  = Exact_vs_2Pass
Direct_vs_2Pass = Direct_vs_2Pass

DIRS = $(SUMMARYDIR) $(TOPDIR) $(TOPDIR_2Pass) $(Variance_DIR) $(Variance_DIR_2Pass) $(Variance_DIR_exact) $(Variance_DIR_2W) $(Variance_DIR_1W)
Sub_DIRS = $(SUMMARYDIR)/$(Exact_vs_Direct) $(SUMMARYDIR)/$(Exact_vs_2Pass) $(SUMMARYDIR)/$(Direct_vs_2Pass)

all:
	make init
	
	$(CC) $(CFLAGS) -o kary $(LIBDIR)/*.c $(SRCDIR)/kary.c  $(LIBS) -D Debug=1 -D INTERVAL=60 -D WINDOWS=3 -D HASH=5 -D KEY_bit=15 -D T=0.01
init:
	make clean
	mkdir $(DIRS)
	mkdir $(Sub_DIRS)

clean:
	test -d $(SUMMARYDIR) && rm -rf $(SUMMARYDIR) || echo "$(SUMMARYDIR) not exist"
	test -d $(TOPDIR) && rm -rf $(TOPDIR) || echo "$(TOPDIR) not exist"
	test -d $(Variance_DIR) && rm -rf $(Variance_DIR) || echo "$(SVariance_DIR) not exist"
	test -d $(TOPDIR_2Pass) && rm -rf $(TOPDIR_2Pass) || echo "$(TOPDIR_2Pass) not exist"
	test -d $(Variance_DIR_2Pass) && rm -rf $(Variance_DIR_2Pass) || echo "$(Variance_DIR_2Pass) not exist"
	test -d $(Variance_DIR_exact) && rm -rf $(Variance_DIR_exact) || echo "$(Variance_DIR_exact) not exist"
	test -d $(Variance_DIR_2W) && rm -rf $(Variance_DIR_2W) || echo "$(Variance_DIR_2W) not exist"
	test -d $(Variance_DIR_1W) && rm -rf $(Variance_DIR_1W) || echo "$(Variance_DIR_1W) not exist"
