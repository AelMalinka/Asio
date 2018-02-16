/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_UV_TCP_CLIENT_IMPL
#	define ENTROPY_TETHYS_UV_TCP_CLIENT_IMPL

#	include "TcpClient.hh"

	namespace Entropy
	{
		namespace Tethys
		{
			namespace UV
			{
				template<typename Application, typename>
				TcpClient::TcpClient(
					const std::string &h,
					const std::string &p,
					Application &a
				) :
					TcpClient::TcpClient(
						h,
						p,
						[&a](auto &s) { a.onData(s); },
						[&a](auto &s) { a.onEof(s); },
						[&a](auto &e) { a.onError(e); },
						[&a](auto &s) { a.onDisconnect(s); },
						[&a](auto &s) { a.onConnect(s); }
					)
				{}
			}
		}
	}

#endif
