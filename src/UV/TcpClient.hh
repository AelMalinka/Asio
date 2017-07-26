/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_ASIO_UV_TCP_CLIENT_INC
#	define ENTROPY_ASIO_UV_TCP_CLIENT_INC

#	include "Tcp.hh"

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
					public Tcp
				{
					public:
						TcpClient(const std::string &, const std::string &);
						virtual ~TcpClient();
					protected:
						virtual void InfoCb(const GetAddrInfo &);
					protected:
						virtual void ConnectCb();
					friend void ::_entropy_asio_uv_tcp_client_connect_cb(uv_connect_t *, int);
				};
			}
		}
	}

#endif
