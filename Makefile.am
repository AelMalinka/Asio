ACLOCAL_AMFLAGS = -I m4
AM_CXXFLAGS := -I$(srcdir)/src $(AM_CXXFLAGS)

asioheadersdir = $(includedir)/Entropy/Asio
asioheaders_HEADERS = \
	src/Exception.hh \
	src/Task.hh \
	src/Loop.hh \
	src/Application.hh \
	src/Buffer.hh \
	src/Buffer.impl.hh \
	src/StreamBuffer.hh \
	src/StreamBuffer.impl.hh

lib_LTLIBRARIES = libentropy-asio.la
libentropy_asio_la_SOURCES = \
	src/Logger.cc \
	src/Task.cc \
	src/Application.cc

if LIBUV
libuvheadersdir = $(includedir)/Entropy/Asio/UV
libuvheaders_HEADERS = \
	src/UV/Task.hh \
	src/UV/Loop.hh \
	src/UV/Loop.impl.hh \
	src/UV/GetAddrInfo.hh \
	src/UV/Stream.hh \
	src/UV/Tcp.hh \
	src/UV/TcpClient.hh \
	src/UV/TcpServer.hh \
	src/UV/Signal.hh

libentropy_asio_la_SOURCES += \
	src/UV/Loop.cc \
	src/UV/GetAddrInfo.cc \
	src/UV/Stream.cc \
	src/UV/Tcp.cc \
	src/UV/TcpClient.cc \
	src/UV/TcpServer.cc \
	src/UV/Signal.cc
endif

noinst_PROGRAMS = \
	getaddr \
	httpget \
	echo

getaddr_SOURCES = examples/getaddr.cc
getaddr_LDADD = libentropy-asio.la

httpget_SOURCES = examples/httpget.cc
httpget_LDADD = libentropy-asio.la

echo_SOURCES = examples/echo.cc
echo_LDADD = libentropy-asio.la

check_PROGRAMS = \
	ios-basic \
	uv-signal

ios_basic_SOURCES = test/ios/basic.cc

uv_signal_SOURCES = test/uv/signal.cc
uv_signal_LDADD = libentropy-asio.la

if GTEST
check_PROGRAMS += \
	application-application \
	application-task \
	ios-buffer

# 2016-12-17 AMR FIXME: gtest_main multiple definitions of main for gmock_main
gtestLDADD = $(GTEST_LIBS) -lgtest_main libentropy-asio.la

application_application_SOURCES = test/application/application.cc
application_application_LDADD = $(gtestLDADD)

application_task_SOURCES = test/application/task.cc
application_task_LDADD = $(gtestLDADD)

ios_buffer_SOURCES = test/ios/buffer.cc
ios_buffer_LDADD = $(gtestLDADD)

if LIBUV
check_PROGRAMS += \
	uv-loop \
	uv-addr

uv_loop_SOURCES = test/uv/loop.cc
uv_loop_LDADD = $(gtestLDADD)

uv_addr_SOURCES = test/uv/addr.cc
uv_addr_LDADD = $(gtestLDADD)
endif

if GMOCK
check_PROGRAMS += \
	ios-stream-buffer

gmockLDADD = $(GTEST_LIBS) $(GMOCK_LIBS) libentropy-asio.la

ios_stream_buffer_SOURCES = test/ios/stream-buffer.cc
ios_stream_buffer_LDADD = $(gmockLDADD)
endif
endif

TESTSUITE = $(srcdir)/test/testsuite
TESTSUITE_FILES = \
	$(srcdir)/test/application.at \
	$(srcdir)/test/ios.at \
	$(srcdir)/test/uv.at

EXTRA_DIST = \
	$(TESTSUITE_FILES) \
	$(srcdir)/test/testsuite.at \
	$(srcdir)/package.m4 \
	$(TESTSUITE) \
	$(srcdir)/test/atlocal.in

$(srcdir)/package.m4 : $(top_srcdir)/configure.ac
	:; {\
		echo '# Signature of the current package. (auto-generated)' && \
		echo 'm4_define([AT_PACKAGE_NAME],' && \
		echo '  [$(PACKAGE_NAME)])' && \
		echo 'm4_define([AT_PACKAGE_TARNAME],' && \
		echo '  [$(PACKAGE_TARNAME)])' && \
		echo 'm4_define([AT_PACKAGE_VERSION],' && \
		echo '  [$(PACKAGE_VERSION)])' && \
		echo 'm4_define([AT_PACKAGE_STRING],' && \
		echo '  [$(PACKAGE_STRING)])' && \
		echo 'm4_define([AT_PACKAGE_BUGREPORT],' && \
		echo '  [$(PACKAGE_BUGREPORT)])'; \
		echo 'm4_define([AT_PACKAGE_URL],' && \
		echo '  [$(PACKAGE_URL)])'; \
	} >'$(srcdir)/package.m4'

check-local: test/atconfig atlocal $(TESTSUITE)
	$(SHELL) '$(TESTSUITE)' AUTOTEST_PATH='$(shell pwd)' $(TESTSUITEFLAGS)

clean-local:
	test ! -f '$(TESTSUITE)' || \
		$(SHELL) '$(TESTSUITE)' --clean

AUTOM4TE = $(SHELL) $(srcdir)/build-aux/missing --run autom4te
AUTOTEST = $(AUTOM4TE) --language=autotest
$(TESTSUITE): $(srcdir)/test/testsuite.at $(TESTSUITE_FILES) $(srcdir)/package.m4
	$(AUTOTEST) -I '$(srcdir)/test' -I '$(srcdir)' -o $@.tmp $@.at
	mv $@.tmp $@
