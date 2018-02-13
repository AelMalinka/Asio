ACLOCAL_AMFLAGS = -I m4
AM_CXXFLAGS := -I$(srcdir)/src $(AM_CXXFLAGS)

asioheadersdir = $(includedir)/Entropy/Tethys
asioheaders_HEADERS = \
	src/Exception.hh \
	src/Task.hh \
	src/Loop.hh \
	src/Application.hh \
	src/Buffer.hh \
	src/Buffer.impl.hh \
	src/Stream.hh \
	src/Stream.impl.hh \
	src/StreamBuffer.hh \
	src/StreamBuffer.impl.hh \
	src/Timer.hh

protocolheadersdir = $(includedir)/Entropy/Tethys/Protocol
protocolheaders_HEADERS = \
	src/Protocol/Application.hh \
	src/Protocol/Application.impl.hh \
	src/Protocol/Exception.hh \
	src/Protocol/Line.hh \
	src/Protocol/Line.impl.hh

lib_LTLIBRARIES = libtethys.la
libtethys_la_SOURCES = \
	src/Logger.cc \
	src/Task.cc \
	src/Application.cc

noinst_HEADERS = \
	test/Mock.hh

if LIBUV
libuvheadersdir = $(includedir)/Entropy/Tethys/UV
libuvheaders_HEADERS = \
	src/UV/GetAddrInfo.hh \
	src/UV/Loop.hh \
	src/UV/Loop.impl.hh \
	src/UV/Signal.hh \
	src/UV/Stream.hh \
	src/UV/Task.hh \
	src/UV/Tcp.hh \
	src/UV/TcpClient.hh \
	src/UV/TcpServer.hh \
	src/UV/Timer.hh \
	src/UV/Timer.impl.hh

libtethys_la_SOURCES += \
	src/UV/GetAddrInfo.cc \
	src/UV/Loop.cc \
	src/UV/Signal.cc \
	src/UV/Stream.cc \
	src/UV/Task.cc \
	src/UV/Tcp.cc \
	src/UV/TcpClient.cc \
	src/UV/TcpServer.cc \
	src/UV/Timer.cc
endif

noinst_PROGRAMS = \
	getaddr \
	httpget \
	echo

getaddr_SOURCES = examples/getaddr.cc
getaddr_LDADD = libtethys.la

httpget_SOURCES = examples/httpget.cc
httpget_LDADD = libtethys.la

echo_SOURCES = examples/echo.cc
echo_LDADD = libtethys.la

check_PROGRAMS = \
	ios-basic

ios_basic_SOURCES = test/ios/basic.cc

if LIBUV
check_PROGRAMS += \
	uv-signal

uv_signal_SOURCES = test/uv/signal.cc
uv_signal_LDADD = libtethys.la
endif

if GTEST
check_PROGRAMS += \
	application-application \
	application-task \
	ios-buffer \
	protocol-application \
	protocol-line-basic \
	protocol-line

# 2016-12-17 AMR FIXME: gtest_main multiple definitions of main for gmock_main
gtestLDADD = $(GTEST_LIBS) -lgtest_main libtethys.la
gmockLDADD = $(GTEST_LIBS) $(GMOCK_LIBS) libtethys.la

application_application_SOURCES = test/application/application.cc
application_application_LDADD = $(gtestLDADD)

application_task_SOURCES = test/application/task.cc
application_task_LDADD = $(gtestLDADD)

ios_buffer_SOURCES = test/ios/buffer.cc
ios_buffer_LDADD = $(gtestLDADD)

protocol_application_SOURCES = test/protocol/basic.cc
protocol_application_CXXFLAGS = -DPROTOCOL=Application -DPROTOCOL_INCLUDE='"Protocol/Application.hh"' -DMOCK_FULL=MockApplication -DMOCK_BASE=MockApplicationOnlyLine $(AM_CXXFLAGS)
protocol_application_LDADD = $(gmockLDADD)

protocol_line_basic_SOURCES = test/protocol/basic.cc
protocol_line_basic_CXXFLAGS = -DPROTOCOL=Line -DPROTOCOL_INCLUDE='"Protocol/Line.hh"' -DMOCK_FULL=MockApplication -DMOCK_BASE=MockApplicationOnlyLine $(AM_CXXFLAGS)
protocol_line_basic_LDADD = $(gmockLDADD)

protocol_line_SOURCES = test/protocol/line.cc
protocol_line_LDADD = $(gmockLDADD)

if LIBUV
check_PROGRAMS += \
	uv-loop \
	uv-timer \
	uv-addr

uv_loop_SOURCES = test/uv/loop.cc
uv_loop_LDADD = $(gtestLDADD)

uv_timer_SOURCES = test/uv/timer.cc
uv_timer_LDADD = $(gtestLDADD)

uv_addr_SOURCES = test/uv/addr.cc
uv_addr_LDADD = $(gtestLDADD)
endif

if GMOCK
check_PROGRAMS += \
	ios-stream-buffer

ios_stream_buffer_SOURCES = test/ios/stream-buffer.cc
ios_stream_buffer_LDADD = $(gmockLDADD)
endif
endif

TESTSUITE = $(srcdir)/test/testsuite
TESTSUITE_FILES = \
	$(srcdir)/test/application.at \
	$(srcdir)/test/ios.at \
	$(srcdir)/test/protocol.at \
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
