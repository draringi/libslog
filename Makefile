CSTD = -std=c99
WARNINGS = -Wall -Wpedantic -Wextra -Wconversion -Wmissing-declarations -Wstack-protector -Wformat -Wformat-security
PROTECTION = -fstack-protector
CFLAGS += -fPIC -c $(WARNINGS) $(CSTD) $(PROTECTION)
LDFLAGS += -shared -Wl,-soname,libslog.so.1 -Wl,-z,relro $(WARNINGS) $(PROTECTION)
SRCDIR = src
OBJDIR = obj

OBJS = $(OBJDIR)/json.o
LIB = libslog.so

$(LIB): $(OBJS)
	@$(CC) $(LDFLAGS) -o $(LIB) $(OBJS)

obj/json.o: $(SRCDIR)/json.c $(SRCDIR)/json.h
	@$(CC) $(CFLAGS) -o obj/json.o $(SRCDIR)/json.c

clean:
	-@$(RM) $(OBJS) $(LIB)
