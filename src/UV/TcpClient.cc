/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "TcpClient.hh"

using namespace Entropy::Asio::UV;
using namespace std;

using Entropy::Asio::Exception;

TcpClient::TcpClient(const string &host, const string &service)
	: Stream(reinterpret_cast<uv_stream_t *>(&_handle)), _handle(), _info()
{
	_info = make_shared<GetAddrInfo>(host, service, SOCK_STREAM, [this](auto &) {
		this->InfoCb();
	});
}

TcpClient::~TcpClient() = default;

void TcpClient::Added(Loop &loop)
{
	ThrowIfError("Failed to initialize Tcp Client", uv_tcp_init(loop.Handle(), &_handle));

	loop.Add(*_info);
}

void TcpClient::InfoCb()
{
	uv_connect_t *req = new uv_connect_t;
	req->data = this;

	// 2017-06-29 AMR TODO: try multiple address?
	ThrowIfError("Failed to connect", uv_tcp_connect(req, &_handle, _info->Info()->ai_addr, _entropy_asio_uv_tcp_client_connect_cb));
}

void TcpClient::ConnectCb()
{
	_info.reset();

	ReadStart();

	onConnect();
}

void TcpClient::onDone()
{
	onDisconnect();
}

void _entropy_asio_uv_tcp_client_connect_cb(uv_connect_t *req, int status)
{
	TcpClient *tcp = static_cast<TcpClient *>(req->data);

	delete req;

	if(status == 0) {
		tcp->ConnectCb();
	} else {
		tcp->onError(AttachUvInfo(Exception("Connect Failed"), status));
	}
}
