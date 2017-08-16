/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_UV_TCP_SERVER_INC
#	define ENTROPY_TETHYS_UV_TCP_SERVER_INC

#	include "Tcp.hh"

	extern "C" {
		void _entropy_tethys_uv_tcp_server_accept_cb(uv_stream_t *, int);
	}

	namespace Entropy
	{
		namespace Tethys
		{
			namespace UV
			{
				class TcpServer :
					public Tcp
				{
					public:
						class Client;
					public:
						TcpServer(const std::string &, const std::string &);
						virtual ~TcpServer();
						virtual void Added(Loop &);
					protected:
						virtual void onDisconnect(Stream &); 
					protected:
						virtual void InfoCb(const GetAddrInfo &);
						virtual void AcceptCb();
					private:
						std::list<Client> _connections;
						Loop *_loop;
					public:
						class Client :
							public Tcp
						{
							public:
								Client(TcpServer &, Loop &);
							protected:
								virtual void onDisconnect(Stream &);
								virtual void onData(Stream &);
							private:
								void InfoCb(const GetAddrInfo &) {}
							private:
								TcpServer &_server;
						};
					friend void ::_entropy_tethys_uv_tcp_server_accept_cb(uv_stream_t *, int);
					friend Client;
				};
			}
		}
	}

#endif
