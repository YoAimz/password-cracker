PROG = programmet.exe
SOURCES = main.cpp generateUsernamePasswordHash.cpp
DEPS =
CC = g++
CFLAGS = -Wall -std=c++20
DEBUG ?= 1

ifeq ($(DEBUG), 1)
	CFLAGS += -g
	OUTPUTDIR = bin/debug
	PROG = programmet-debug.exe
else
	CFLAGS += -g0 -O3
	OUTPUTDIR = bin/release
endif

OBJS = $(addprefix $(OUTPUTDIR)/,$(SOURCES:.cpp=.o))
LDFLAGS = -lssl -lcrypto  

$(PROG): $(OUTPUTDIR) $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LDFLAGS)

$(OUTPUTDIR)/%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@del /q "$(OUTPUTDIR)"
	@del /q $(PROG)

$(OUTPUTDIR):
	@mkdir "$(OUTPUTDIR)"

.PHONY: clean test