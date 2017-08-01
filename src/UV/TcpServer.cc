/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "TcpServer.hh"

using namespace Entropy::Tethys::UV;
using namespace std;

using Entropy::Tethys::Exception;

TcpServer::TcpServer(const string &h, const string &s)
	: Tcp(h, s), _loop(nullptr)
{
	Handle()->data = this;
}

TcpServer::~TcpServer() = default;

void TcpServer::Added(Loop &loop)
{
	_loop = &loop;

	Tcp::Added(loop);
}

void TcpServer::InfoCb(const GetAddrInfo &info)
{
	// 2017-07-22 AMR TODO: IPv4 vs IPv6
	// 2017-07-22 AMR TODO: try multiple addresses?
	ThrowIfError("Failed to bind", uv_tcp_bind(Handle(), info.Info()->ai_addr, 0));
	// 2017-07-22 AMR TODO: how many accept to backlog
	// 2017-07-22 AMR TODO: move to stream?
	ThrowIfError("Failed to listen", uv_listen(reinterpret_cast<uv_stream_t *>(Handle()), 128, _entropy_tethys_uv_tcp_server_accept_cb));
}

void TcpServer::onDisconnect(Stream &s)
{
	Tcp::onDisconnect(s);

	for(auto i = _connections.begin(); i != _connections.end(); i++) {
		if(&*i == &s) {
			_connections.erase(i);
			break;
		}
	}
}

void TcpServer::AcceptCb()
{
	if(!_loop)
		ENTROPY_THROW(Exception("Loop not initialized, in accept callback"));

	_connections.emplace_back(*this, *_loop);
	onConnect(_connections.back());
}

TcpServer::Client::Client(TcpServer &s, Loop &loop)
	: Tcp(loop), _server(s)
{
	Handle()->data = this;

	ThrowIfError("Failed to accept connection", uv_accept(reinterpret_cast<uv_stream_t *>(s.Handle()), reinterpret_cast<uv_stream_t *>(Handle())));
	ReadStart();
}

void TcpServer::Client::onDisconnect(Stream &s)
{
	if(this == &s)
		_server.onDisconnect(s);

	Tcp::onDisconnect(s);
}

void TcpServer::Client::onData(Stream &s)
{
	_server.onData(s);
}

void _entropy_tethys_uv_tcp_server_accept_cb(uv_stream_t *handle, int status)
{
	TcpServer *tcp = static_cast<TcpServer *>(handle->data);

	if(status == 0) {
		tcp->AcceptCb();
	} else {
		tcp->onError(AttachUvInfo(Exception("Accept Failed"), status));
	}
}
