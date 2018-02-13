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
						std::bind(&Application::onData, &a, std::placeholders::_1),
						std::bind(&Application::onEof, &a, std::placeholders::_1),
						std::bind(&Application::onError, &a, std::placeholders::_1),
						std::bind(&Application::onDisconnect, &a, std::placeholders::_1),
						std::bind(&Application::onConnect, &a, std::placeholders::_1)
					)
				{}
			}
		}
	}

#endif
