/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include "GetAddrInfo.hh"

#include <cstring>

using namespace Entropy::Asio::UV;
using namespace std;

using Entropy::Asio::Exception;
using Entropy::SystemError;
using Entropy::SystemErrorCode;

GetAddrInfo::GetAddrInfo(const string &host, const string &service, const int type, const function<void(const GetAddrInfo &)> &cb)
	: _req(), _res(nullptr), _cb(cb), _host(host), _service(service), _hints()
{
	_req.data = this;

	memset(&_hints, 0, sizeof(_hints));
	_hints.ai_family = AF_UNSPEC;
	_hints.ai_flags = AI_PASSIVE;
	_hints.ai_socktype = type;
}

GetAddrInfo::~GetAddrInfo()
{
	uv_cancel(reinterpret_cast<uv_req_t *>(&_req));
}

void GetAddrInfo::Added(Loop &loop)
{
	ThrowIfError("Failed to start GetAddrInfo", uv_getaddrinfo(loop.Handle(), &_req, _entropy_asio_uv_getaddrinfo_cb, _host.data(), _service.data(), &_hints));
}

void GetAddrInfo::callback(struct addrinfo *res)
{
	_res = res;
	_cb(*this);
}

const struct addrinfo *GetAddrInfo::Info() const
{
	return _res;
}

void _entropy_asio_uv_getaddrinfo_cb(uv_getaddrinfo_t *req, int status, struct addrinfo *res)
{
	// 2017-03-23 AMR NOTE: not an error to be canceled
	if(status == UV_ECANCELED) {
		if(res != nullptr)
			uv_freeaddrinfo(res);
	} else {
		try
		{
			ThrowIfError("GetAddrInfo failed", status);
			reinterpret_cast<GetAddrInfo *>(req->data)->callback(res);
		}
		catch(...)
		{
			if(res != nullptr)
				uv_freeaddrinfo(res);
			throw;
		}
	}
}
