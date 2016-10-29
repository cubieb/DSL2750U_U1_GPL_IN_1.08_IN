APP_PATH= ${.CURDIR}/../../tools/athcfg

.PATH:	${APP_PATH}

PROG=	athcfg
SRCS=	athcfg.c

CFLAGS+= -g
CFLAGS+= -I${APP_PATH}
LDFLAGS+= -lm
MAN=
.include <bsd.prog.mk>
