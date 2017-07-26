/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com> Distributed under the terms of the GNU Affero General Public License v3
*/

#include "Tcp.hh"

using namespace Entropy::Asio::UV;
using namespace std;

Tcp::Tcp()
	: Stream(reinterpret_cast<uv_stream_t *>(&_handle)), _handle(), _info(), _host(), _service()
{}

Tcp::Tcp(const string &host, const string &service)
	: Stream(reinterpret_cast<uv_stream_t *>(&_handle)), _handle(), _info(), _host(host), _service(service)
{}

Tcp::~Tcp() = default;

uv_tcp_t *Tcp::Handle()
{
	return &_handle;
}

void Tcp::Added(Loop &loop)
{
	ThrowIfError("Failed to initialize Tcp Client", uv_tcp_init(loop.Handle(), Handle()));

	// 2017-07-21 AMR TODO: can we move this to constructor w/ virtual call in cb?
	_info = make_shared<GetAddrInfo>(_host, _service, SOCK_STREAM, [this](auto &i) {
		this->InfoCb(i);
		this->_info.reset();
	});
	loop.Add(*_info);
}

void Tcp::onEof()
{
	onDisconnect(*this);

	Stream::onEof();
}
