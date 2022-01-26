PROG = hello_world_example
SRCS = hello_world_example.c
OBJS = $(SRCS:.c=.o)

CLEANFILES = $(PROG)
INSTALL_DIR = ./install_artifact/usr/local/bin

# Add / change option in CFLAGS if needed
# CFLAGS += <new option>

$(PROG):  $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

all: $(PROG)
 
 
clean:
	rm -f $(CLEANFILES) $(patsubst %.c,%.o, $(SRCS)) *~

install: $(PROG)
	mkdir -p $(INSTALL_DIR)
	install $(PROG) $(INSTALL_DIR)