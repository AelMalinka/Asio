/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "TcpClient.hh"

using namespace Entropy::Tethys::UV;
using namespace std;

using Entropy::Tethys::Exception;

extern "C" void connect_cb(uv_connect_t *req, int status);

TcpClient::TcpClient(
	const string &host,
	const string &service,
	const std::function<void(Tethys::Stream<char> &)> &data,
	const std::function<void(Stream &)> &eof,
	const std::function<void(const Entropy::Exception &)> &error,
	const std::function<void(Tcp &)> &dis,
	const std::function<void(Tcp &)> &con
) :
	Tcp(
		host,
		service,
		data,
		eof,
		error,
		dis,
		bind(&TcpClient::InfoCb, this, std::placeholders::_1)
	),
	_on_con(con)
{
	Handle()->data = this;
}

TcpClient::~TcpClient() = default;

void TcpClient::InfoCb(const GetAddrInfo &info)
{
	uv_connect_t *req = new uv_connect_t;
	req->data = this;

	try
	{
		// 2017-06-29 AMR TODO: try multiple address?
		ThrowIfError("Failed to connect", uv_tcp_connect(req, Handle(), info.Info()->ai_addr, connect_cb));
	}
	catch(...)
	{
		delete req;
		throw;
	}
}

void TcpClient::ConnectCb()
{
	ReadStart();
	_on_con(*this);
}

void connect_cb(uv_connect_t *req, int status)
{
	TcpClient *tcp = static_cast<TcpClient *>(req->data);

	delete req;

	if(status == 0) {
		tcp->ConnectCb();
	} else {
		tcp->ErrorCb(AttachUvInfo(Exception("Connect Failed"), status));
	}
}
