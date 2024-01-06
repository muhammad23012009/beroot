PROGRAM = beroot
SRCS = src/beroot.c src/password.c src/permit.c

DESTDIR ?= /usr
PROG_OWNER ?= root
PROG_GROUP ?= root
PROG_MODE ?= u+s

COMPILE_FLAGS ?= -lcrypt -O2 -g

all: ${PROGRAM}

${PROGRAM}: ${SRCS}
	${CC} ${SRCS} -o ${PROGRAM} ${COMPILE_FLAGS}

install:
	mkdir -p -m 0755 ${DESTDIR}/bin
	cp ${PROGRAM} ${DESTDIR}/bin/
	chown ${PROG_OWNER}:${PROG_GROUP} ${DESTDIR}/bin/${PROGRAM}
	chmod ${PROG_MODE} ${DESTDIR}/bin/${PROGRAM}

uninstall:
	rm -f ${DESTDIR}/bin/${PROGRAM}

.PHONY = all install uninstall
