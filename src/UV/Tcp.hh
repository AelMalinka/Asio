/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_ASIO_UV_TCP_INC
#	define ENTROPY_ASIO_UV_TCP_INC

#	include "Stream.hh"
#	include "GetAddrInfo.hh"

	namespace Entropy
	{
		namespace Asio
		{
			namespace UV
			{
				class Tcp :
					public Stream
				{
					public:
						Tcp();
						Tcp(const std::string &, const std::string &);
						virtual ~Tcp();
					protected:
						uv_tcp_t *Handle();
						virtual void InfoCb(const GetAddrInfo &) = 0;
						virtual void onEof();
					protected:
						virtual void Added(Loop &);
						virtual void onConnect(Stream &) {}
						virtual void onDisconnect(Stream &) {}
					private:
						uv_tcp_t _handle;
						std::shared_ptr<GetAddrInfo> _info;
						std::string _host;
						std::string _service;
				};
			}
		}
	}

#endif
