## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = 1.9 nostdinc

SUBDIRS = src

DISTCHECK_CONFIGURE_FLAGS=@CONFIGURE_OPTIONS@

dsl_cpe_control_docdir = ${prefix}/doc/dsl_cpe_control

dsl_cpe_control_doc_DATA = \
	README\
	AUTHORS\
	ChangeLog\
	NEWS\
	TODO

EXTRA_DIST = $(dsl_cpe_control_doc_DATA)


# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

lint:
	@target=lint; \
	list='$(SUBDIRS)'; \
	for subdir in $$list; do \
		echo "Making $$target in $$subdir"; \
		if test "$$subdir" = "."; then \
		dot_seen=yes; \
		local_target="$$target-am"; \
		else \
		local_target="$$target"; \
		fi; \
		(cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
		|| eval $$failcom; \
	done;