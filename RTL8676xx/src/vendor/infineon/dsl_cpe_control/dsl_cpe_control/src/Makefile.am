## Process this file with automake to produce Makefile.in

bin_PROGRAMS = dsl_cpe_control

AM_CPPFLAGS = -I@srcdir@ @DSL_DRIVER_INCL_PATH@ @DSL_DEVICE_DRIVER_INCL_PATH@


if DSL_CPE_STATIC_LINKING
dsl_cpe_control_common_ldflags = -static
else
dsl_cpe_control_common_ldflags =
endif

dsl_cpe_control_LDADD = -lpthread

if INCLUDE_DSL_CPE_SOAP_SUPPORT
    dsl_cpe_control_LDADD += -lm
endif

bin_SCRIPTS = ../scripts/alias_dsl_cpe.sh ../scripts/dsl_cpe_pipe.sh

dsl_cpe_control_soap_sources = \
	stdsoap2.c \
	dsl_cpe_soap.c \
	dsl_cpe_soap_C.c \
	dsl_cpe_soap_Client.c \
	dsl_cpe_soap_Server.c

dsl_cpe_control_cli_sources = \
	dsl_cpe_cli.c \
	dsl_cpe_cli_pipe.c \
	dsl_cpe_cli_access.c \
	dsl_cpe_cli_console.c

dsl_cpe_control_cli_danube_sources = \
	dsl_cpe_debug_danube.c \
	dsl_cpe_cli_danube.c

dsl_cpe_control_cli_amazon_se_sources = \
	dsl_cpe_debug_danube.c \
	dsl_cpe_cli_danube.c

dsl_cpe_control_cli_vinax_sources = \
	dsl_cpe_debug_vinax.c \
	dsl_cpe_cli_vinax.c

dsl_cpe_control_SOURCES = \
	dsl_cpe_control.c \
	dsl_cpe_init_cfg.c \
	dsl_cpe_linux.c \
	dsl_cpe_debug.c

if INCLUDE_DSL_CPE_SOAP_SUPPORT
dsl_cpe_control_SOURCES += \
	$(dsl_cpe_control_soap_sources)
endif

if INCLUDE_DSL_CPE_CLI_SUPPORT
dsl_cpe_control_SOURCES += \
	$(dsl_cpe_control_cli_sources)

if INCLUDE_DSL_CPE_API_DANUBE
dsl_cpe_control_SOURCES += \
	$(dsl_cpe_control_cli_danube_sources)
endif

if INCLUDE_DSL_CPE_API_AMAZON_SE
dsl_cpe_control_SOURCES += \
	$(dsl_cpe_control_cli_amazon_se_sources)
endif

if INCLUDE_DSL_CPE_API_VINAX
dsl_cpe_control_SOURCES += \
	$(dsl_cpe_control_cli_vinax_sources)
endif
endif

EXTRA_DIST = \
	../LICENSE \
	$(dsl_cpe_control_soap_sources) \
	$(dsl_cpe_control_cli_sources) \
	$(dsl_cpe_control_cli_danube_sources) \
	$(dsl_cpe_control_cli_amazon_se_sources) \
	$(dsl_cpe_control_cli_vinax_sources) \
	dsl_cpe_cli_danube.c \
	dsl_cpe_cli_vinax.c \
	dsl_cpe_control.h \
	dsl_cpe_init_cfg.h \
	dsl_cpe_cli.h \
	dsl_cpe_cli_console.h \
	dsl_cpe_simulator.c \
	dsl_cpe_simulator.h \
	dsl_cpe_win32.c \
	dsl_cpe_win32.h \
	dsl_cpe_os.h \
	dsl_cpe_linux.h \
	dsl_cpe_debug.h \
	dsl_cpe_debug_danube.h \
	dsl_cpe_debug_vinax.h \
	dsl_cpe_soap_Stub.h \
	dsl_cpe_soap_H.h \
	stdsoap2.h \
	../scripts/alias_dsl_cpe.sh \
	../scripts/dsl_cpe_pipe.sh

if ADD_APPL_CFLAGS_SET
dsl_cpe_control_cflags = $(ADD_APPL_CFLAGS)
else
dsl_cpe_control_cflags =
endif

if ADD_APPL_LDFLAGS_SET
dsl_cpe_control_ldflags = $(ADD_APPL_LDFLAGS)
else
dsl_cpe_control_ldflags =
endif

dsl_cpe_control_common_cflags = -Wall -Wimplicit -Wreturn-type -Wunused -Wundef -Wswitch \
                                -Wno-format -Wcomment -Wuninitialized \
                                -DLINUX -g -s

dsl_cpe_control_CFLAGS = $(dsl_cpe_control_common_cflags) $(dsl_cpe_control_cflags)
dsl_cpe_control_LDFLAGS = $(dsl_cpe_control_common_ldflags) $(dsl_cpe_control_ldflags)


lint:
	@flint +b -vm \
	-i$(shell dirname `$(CC) -print-file-name=include`)/include \
	-i@top_srcdir@ std_lx.lnt env-cw6.lnt -i@srcdir@ \
	$(DEFS) $(AM_CPPFLAGS) $(CFLAGS) -D__LINUX__ -DLINUX $(dsl_cpe_control_SOURCES)
