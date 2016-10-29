dnl $Id: ensc_developing.m4,v 1.1.1.1 2003/11/25 22:14:04 jharrell Exp $


AC_DEFUN([ENSC_DEVELOPING],
[
	AC_PATH_PROGS(TR, tr)
	AC_PATH_PROGS(ECHO, echo)

	default=$2
	: ${default:=no}

	AC_MSG_CHECKING([whether to enable developing])
	AC_ARG_ENABLE(developing,
		      [AC_HELP_STRING([--enable-developing],
				      [Use settings for developing; DO
				      NOT use in final build (default:
				      OFF)])],
		      [], [ enable_developing=${default} ])

	enable_developing=`${ECHO} ${enable_developing} | ${TR} A-Z a-z`

	if test x"${enable_developing}" = xno; then
		$1=no
		AC_MSG_RESULT(no)
		DEVELOPING_CFLAGS=
	else
		$1=yes
		AC_MSG_RESULT(yes)
		enable_stl_malloc_default=yes

		DEVELOPING_STRICT_CFLAGS=['-Wold-style-cast -Wunreachable-code -Wshadow']
		DEVELOPING_CFLAGS=['-g3 -O0 -Woverloaded-virtual -Wsign-promo -Wsynth -Wchar-subscripts
				    -Wparentheses -Wsequence-point -Wswitch -Werror -W -Wfloat-equal
				    -Wpointer-arith']

		AC_MSG_CHECKING([whether to define __USE_MALLOC])
		AC_ARG_ENABLE(stl-malloc,
			      [AC_HELP_STRING([--disable-stl-malloc],
			                      [Do not use the slow malloc-allocator of the STL.
                                               This generates faster code but may hide problems
                                               like mem-leaks or overflows. This option will be
                                               ignored when not using '--enable-developing'; then
                                               it is on by default])],
                              [case "${enableval}" in
				  yes|no) enable_stl_malloc="${enableval}";;
		 		  *)      AC_MSG_ERROR([bad value ${enableval} for --disable-stl-malloc]) ;;
			       esac],
                              [ enable_stl_malloc=${enable_stl_malloc_default} ])
		AC_MSG_RESULT([${enable_stl_malloc}])

		if test x"${enable_stl_malloc}" = xyes; then
			AC_DEFINE([__USE_MALLOC], [1], [Use slow malloc-allocators allowing tracing of memory-leaks])
		fi
	fi

	AC_SUBST([$1])
])

AC_DEFUN([__ENSC_RELEASE],
[
	default=${ensc_is_release_default:-no}
	: ${default:=no}

	AC_MSG_CHECKING(whether to enable release-mode)
	AC_ARG_ENABLE(release,
		      AC_HELP_STRING([--disable-release],
				     [disable settings which are creating faster/smaller code]),
		      [case "${enableval}" in
			  yes|no) enable_release="${enableval}";;
	 		  *)      AC_MSG_ERROR([bad value ${enableval} for --disable-release]) ;;
		       esac],
	              [ enable_release=${default} ])

	if test x"${enable_release}" = xno; then
		ensc_is_release=no
		RELEASE_CFLAGS=
	else
		ensc_is_release=yes
		RELEASE_CFLAGS='-O3'
		AC_DEFINE(NDEBUG, [(1==1)], [The NDEBUG macro as specified in C99, 7.2])
	fi
])

AC_DEFUN([ENSC_RELEASE],
[
	ensc_is_release_default=$2
	AC_REQUIRE([__ENSC_RELEASE])

	$1=${ensc_is_release}
	AC_SUBST([$1])
	AC_MSG_RESULT([$$1])
])
