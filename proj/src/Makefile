PROG=proj

SRCS = proj.c timer.c keyboard.c mouse.c utils.c KBC.c graphics.c Game.c 

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>