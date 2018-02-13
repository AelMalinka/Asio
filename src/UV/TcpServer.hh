/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_UV_TCP_SERVER_INC
#	define ENTROPY_TETHYS_UV_TCP_SERVER_INC

#	include "Tcp.hh"

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
						TcpServer(
							const std::string &,
							const std::string &,
							const std::function<void(Tethys::Stream<char> &)> &,
							const std::function<void(Stream &)> &,
							const std::function<void(const Entropy::Exception &)> &,
							const std::function<void(Tcp &)> &,
							const std::function<void(Tcp &)> &
						);
						template<typename Application,
							typename = typename std::enable_if<
								std::is_class<Application>::value
							>::type
						>
						TcpServer(
							const std::string &,
							const std::string &,
							Application &
						);
						virtual ~TcpServer();
						virtual void Added(Loop &);
						virtual void DisconnectCb(Tcp &); 
						virtual void InfoCb(const GetAddrInfo &);
						virtual void AcceptCb();
					private:
						std::list<Client> _connections;
						Loop *_loop;
						std::function<void(Tethys::Stream<char> &)> _on_data;
						std::function<void(Stream &)> _on_eof;
						std::function<void(const Entropy::Exception &)> _on_error;
						std::function<void(Tcp &)> _on_dis;
						std::function<void(Tcp &)> _on_con;
					public:
						class Client :
							public Tcp
						{
							public:
								Client(
									TcpServer &,
									Loop &,
									const std::function<void(Tethys::Stream<char> &)> &,
									const std::function<void(Stream &)> &,
									const std::function<void(const Entropy::Exception &)> &,
									const std::function<void(Tcp &)> &
								);
							private:
								TcpServer &_server;
								std::function<void(Tcp &)> _on_dis;
						};
					friend Client;
				};
			}
		}
	}

#	include "TcpServer.impl.hh"

#endif
