# ODP_PTHREAD
# -----------
# Check for pthreads availability
AC_DEFUN([ODP_PTHREAD], [
	AC_MSG_CHECKING([for pthread support in -pthread])
	AC_LANG_PUSH([C])
	CFLAGS="$CFLAGS -pthread"
	LDFLAGS="$LDFLAGS -pthread"
	AC_TRY_LINK_FUNC([pthread_create], [pthread=yes])
	if test x"$pthread" != "xyes"; then
		AC_MSG_FAILURE([pthread is not supported])
	fi
	AC_MSG_RESULT([yes])
	AC_LANG_POP([C])
])
