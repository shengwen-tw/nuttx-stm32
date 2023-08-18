include $(APPDIR)/Make.defs

APP_NAME = work_queue
PRIORITY = SCHED_PRIORITY_DEFAULT
STACKSIZE = 2048

MAINSRC = work_queue_ex.c

CONFIG_MY_APP_PROGNAME ?= work_queue$(EXEEXT)
PROGNAME = $(CONFIG_MY_APP_PROGNAME)

include $(APPDIR)/Application.mk
