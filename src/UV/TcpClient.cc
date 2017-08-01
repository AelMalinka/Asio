/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "TcpClient.hh"

using namespace Entropy::Tethys::UV;
using namespace std;

using Entropy::Tethys::Exception;

TcpClient::TcpClient(const string &host, const string &service)
	: Tcp(host, service)
{
	Handle()->data = this;
}

TcpClient::~TcpClient() = default;

void TcpClient::InfoCb(const GetAddrInfo &info)
{
	uv_connect_t *req = new uv_connect_t;
	req->data = this;

	// 2017-06-29 AMR TODO: try multiple address?
	ThrowIfError("Failed to connect", uv_tcp_connect(req, Handle(), info.Info()->ai_addr, _entropy_tethys_uv_tcp_client_connect_cb));
}

void TcpClient::ConnectCb()
{
	ReadStart();
	onConnect(*this);
}

void _entropy_tethys_uv_tcp_client_connect_cb(uv_connect_t *req, int status)
{
	TcpClient *tcp = static_cast<TcpClient *>(req->data);

	delete req;

	if(status == 0) {
		tcp->ConnectCb();
	} else {
		tcp->onError(AttachUvInfo(Exception("Connect Failed"), status));
	}
}
