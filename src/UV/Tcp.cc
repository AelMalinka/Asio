/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com> Distributed under the terms of the GNU Affero General Public License v3
*/

#include "Tcp.hh"

using namespace Entropy::Tethys::UV;
using namespace std;

Tcp::Tcp(
	Loop &loop,
	const function<void(Entropy::Tethys::Stream<char> &)> &data,
	const function<void(Stream &)> &eof,
	const function<void(const Entropy::Exception &)> &error,
	const function<void(Tcp &)> &dis
) :
	Stream(
		reinterpret_cast<uv_stream_t *>(&_handle),
		data,
		eof,
		error
	),
	_handle(),
	_on_dis(dis),
	_on_info(),
	_info(),
	_host(),
	_service()
{
	ThrowIfError("Failed to initialize Tcp Object", uv_tcp_init(loop.Handle(), &_handle));
}

Tcp::Tcp(
	const string &host,
	const string &service,
	const function<void(Entropy::Tethys::Stream<char> &)> &data,
	const function<void(Stream &)> &eof,
	const function<void(const Entropy::Exception &)> &error,
	const function<void(Tcp &)> &dis,
	const function<void(const GetAddrInfo &)> &info
) :
	Stream(
		reinterpret_cast<uv_stream_t *>(&_handle),
		data,
		eof,
		error
	),
	_handle(),
	_on_dis(dis),
	_on_info(info),
	_info(),
	_host(host),
	_service(service)
{}

Tcp::~Tcp() = default;

const string &Tcp::Host() const
{
	return _host;
}

const string &Tcp::Service() const
{
	return _service;
}

uv_tcp_t *Tcp::Handle()
{
	return &_handle;
}

void Tcp::Added(Loop &loop)
{
	ThrowIfError("Failed to initialize tcp object", uv_tcp_init(loop.Handle(), Handle()));

	// 2017-07-21 AMR TODO: can we move this to constructor w/ virtual call in cb?
	_info = make_shared<GetAddrInfo>(_host, _service, SOCK_STREAM, [this](auto &i) {
		this->_on_info(i);
		this->_info.reset();
	});
	loop.Add(*_info);
}

void Tcp::EofCb()
{
	Stream::EofCb();
	_on_dis(*this);
}
