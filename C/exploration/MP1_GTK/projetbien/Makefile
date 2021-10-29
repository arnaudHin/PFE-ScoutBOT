#
# @file  Makefile
# @author ESEOBOT (inspired by Matthias Brun's work)
# @version 1.0
# @date 15/05/2020
# @brief Defined set of tasks to be executed
#

# TO MODIFY : Keep the line below ONLY if you are doing tests
#export CCFLAGS += -DNTEST

# TO MODIFY : DEBUG to have traces / NDEBUG without TRACE
# Please, use NDEBUG with CMocka tests
export CCFLAGS += -g -DDEBUG # avec debuggage : -g -DDEBUG # without debugging : -DNDEBUG


# CMocka installation directory
# export CMOCKA =CMocka

#
# sources organization.
#

export SRCDIR = src
# export STUBDIR = fake
# export TESTDIR = test
export BINDIR = bin

# export DOC = doc

# Pour regler le probleme de Cmocka a la compilation, nous enlevons les tests
SUBDIRS = $(STUBDIR) $(SRCDIR) #$(TESTDIR)
#
# Definitions of tools.
#

# export CC = gcc

export CCFLAGS += -O0 
export CCFLAGS += -g -lm
export CCFLAGS += -MMD -MP # Automatic management of dependencies.
export CCFLAGS += -D_BSD_SOURCE -D_XOPEN_SOURCE_EXTENDED -D_XOPEN_SOURCE -D_DEFAULT_SOURCE -D_GNU_SOURCE
# remove  -pedantic
export CCFLAGS += -std=c99 -Wall
export CCFLAGS += -Wno-unused-function -Wno-unused-variable
export LDFLAGS  = -lrt -lpthread
#export LDFLAGS = -ftest-coverage -fprofile-arcs
export CCFLAGS += -D_REENTRANT -D_POSIX_C_SOURCE=199309L
export CCFLAGS += -Wall $(shell pkg-config --cflags gtk+-3.0)
export LDFLAGS += $(shell pkg-config --libs gtk+-3.0)  -lm -ldl

#
# Definitions of binaries to generate.

export PROG = $(BINDIR)/pocket
# export TEST = $(BINDIR)/pocket_test

#
# Rules of Makefile.
#

# Compilation.
all: 
	@for i in $(SUBDIRS); do (cd $$i && $(MAKE) $@); done


.PHONY: clean documentation

# Clean.
clean:
	@for i in $(SUBDIRS); do (cd $$i && $(MAKE) $@); done
	@rm -f $(PROG) $(TEST) core* $(BINDIR)/core*
	@rm -rf $(DOC)

# Generating documentation.
# documentation:
# 	$(DOXYGEN) $(DOXYGENFLAGS) $(DOXYGENCONF)

#all: 
#	$(MAKE) clean
#	@for i in $(SUBDIRS); do (cd $$i && $(MAKE) $@); done

# Nettoyage.
#.PHONY: clean

#clean:
#	@for i in $(SUBDIRS); do (cd $$i && $(MAKE) $@); done
#   @rm -f $(PROG) $(TEST) core* $(BINDIR)/core*
