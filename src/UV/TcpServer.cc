/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "TcpServer.hh"

using namespace Entropy::Tethys::UV;
using namespace std;

using Entropy::Tethys::Exception;

void accept_cb(uv_stream_t *handle, int status);

TcpServer::TcpServer(
	const string &h,
	const string &s,
	const function<void(Tethys::Stream<char> &)> &data,
	const function<void(Stream &)> &eof,
	const function<void(const Entropy::Exception &)> &error,
	const function<void(Tcp &)> &dis,
	const function<void(Tcp &)> &con
) :
	Tcp(
		h,
		s,
		data,
		eof,
		error,
		dis,
		bind(&TcpServer::InfoCb, this, placeholders::_1)
	),
	_loop(nullptr),
	_on_data(data),
	_on_eof(eof),
	_on_error(error),
	_on_dis(dis),
	_on_con(con)
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
	ThrowIfError("Failed to listen", uv_listen(reinterpret_cast<uv_stream_t *>(Handle()), 128, accept_cb));
}

void TcpServer::DisconnectCb(Tcp &s)
{
	_on_dis(s);

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

	_connections.emplace_back(*this, *_loop, _on_data, _on_eof, _on_error, _on_dis);
	_on_con(_connections.back());
}

TcpServer::Client::Client(
	TcpServer &s,
	Loop &loop,
	const function<void(Tethys::Stream<char> &)> &data,
	const function<void(Stream &)> &eof,
	const function<void(const Entropy::Exception &)> &error,
	const function<void(Tcp &)> &dis
) :
	Tcp(
		loop,
		data,
		eof,
		error,
		[this](auto &s) {
			if(this == &s)
				_server.DisconnectCb(s);
			else
				_on_dis(s);
		}
	),
	_server(s),
	_on_dis(dis)
{
	Handle()->data = this;

	ThrowIfError("Failed to accept connection", uv_accept(reinterpret_cast<uv_stream_t *>(s.Handle()), reinterpret_cast<uv_stream_t *>(Handle())));
	ReadStart();
}

void accept_cb(uv_stream_t *handle, int status)
{
	TcpServer *tcp = static_cast<TcpServer *>(handle->data);

	if(status == 0) {
		tcp->AcceptCb();
	} else {
		tcp->ErrorCb(AttachUvInfo(Exception("Accept Failed"), status));
	}
}
