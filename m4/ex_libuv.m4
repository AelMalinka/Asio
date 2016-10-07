dnl Copyright 2015 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
dnl Distributed under the terms of the GNU Affero General Public License v3

AC_DEFUN([EX_WITH_LIBUV], [
	AC_ARG_WITH([libuv],
		[AS_HELP_STRING([--with-libuv], [support for libuv asio library])],
		[with_libuv=$withval],
		[with_libuv=check]
	)

	AS_IF([test "x$with_libuv" != xno],
		[
			test "x$with_libuv" != xyes -a "x$with_libuv" != xcheck && LDFLAGS="${LDFLAGS} -L$with_libuv"
			EX_CHECK_LIBRARY([LIBUV], ["uv.h"], [uv], [], [
				AS_IF([test "x$with_libuv" != xcheck], [
					AC_MSG_FAILURE(["--with-libuv: libuv not found"])
				])
			])
		]
	)
	AM_CONDITIONAL([LIBUV], [test "x$LIBUV_LIBS" != x])
])
