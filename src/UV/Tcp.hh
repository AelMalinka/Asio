/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_UV_TCP_INC
#	define ENTROPY_TETHYS_UV_TCP_INC

#	include "Stream.hh"
#	include "GetAddrInfo.hh"

	namespace Entropy
	{
		namespace Tethys
		{
			namespace UV
			{
				class Tcp :
					public Stream
				{
					public:
						Tcp(
							Loop &,
							const std::function<void(Tethys::Stream<char> &)> &,
							const std::function<void(Stream &)> &,
							const std::function<void(const Entropy::Exception &)> &,
							const std::function<void(Tcp &)> &
						);
						Tcp(
							const std::string &,
							const std::string &,
							const std::function<void(Tethys::Stream<char> &)> &,
							const std::function<void(Stream &)> &,
							const std::function<void(const Entropy::Exception &)> &,
							const std::function<void(Tcp &)> &,
							const std::function<void(const GetAddrInfo &)> &
						);
						virtual ~Tcp();
						virtual void Added(Loop &);
						virtual void EofCb();
						const std::string &Host() const;
						const std::string &Service() const;
					protected:
						uv_tcp_t *Handle();
					private:
						uv_tcp_t _handle;
						std::function<void(Tcp &)> _on_dis;
						std::function<void(const GetAddrInfo &)> _on_info;
						std::shared_ptr<GetAddrInfo> _info;
						std::string _host;
						std::string _service;
				};
			}
		}
	}

#endif
