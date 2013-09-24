include config.mk

OBJ=bin/blog.o

all: mkbin options slackblog

mkbin:
	@mkdir -p bin

options:
	@echo slackblog build options:
	@echo "CFLAGS  = ${CFLAGS}"
	@echo "LDFLAGS = ${LDFLAGS}"
	@echo "CC      = ${CC}"

bin/%.o: src/%.c
	@echo CC $<
	@${CC} -static -o $@ -c ${CFLAGS} $<

slackblog: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}
	@echo $@ finished compiling.

clean:
	@echo cleaning
	@rm -f bin/*.o slackboat
