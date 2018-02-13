/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_UV_TCP_CLIENT_INC
#	define ENTROPY_TETHYS_UV_TCP_CLIENT_INC

#	include "Tcp.hh"

	namespace Entropy
	{
		namespace Tethys
		{
			namespace UV
			{
				class TcpClient :
					public Tcp
				{
					public:
						TcpClient(
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
						TcpClient(
							const std::string &,
							const std::string &,
							Application &
						);
						virtual ~TcpClient();
						void InfoCb(const GetAddrInfo &);
						void ConnectCb();
					private:
						std::function<void(Tcp &)> _on_con;
				};
			}
		}
	}

#	include "TcpClient.impl.hh"

#endif
