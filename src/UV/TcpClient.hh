/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_ASIO_UV_TCP_CLIENT_INC
#	define ENTROPY_ASIO_UV_TCP_CLIENT_INC

#	include "Stream.hh"
#	include "GetAddrInfo.hh"

	extern "C" {
		void _entropy_asio_uv_tcp_client_connect_cb(uv_connect_t *, int);
	}

	namespace Entropy
	{
		namespace Asio
		{
			namespace UV
			{
				class TcpClient :
					public Stream
				{
					public:
						TcpClient(const std::string &, const std::string &);
						virtual ~TcpClient();
					protected:
						virtual void Added(Loop &);
					protected:
						virtual void onConnect() {}
						virtual void onDisconnect() {}
					protected:
						virtual void onDone();
						virtual void ConnectCb();
						virtual void InfoCb();
					private:
						uv_tcp_t _handle;
						std::shared_ptr<GetAddrInfo> _info;
					friend void ::_entropy_asio_uv_tcp_client_connect_cb(uv_connect_t *, int);
				};
			}
		}
	}

#endif
