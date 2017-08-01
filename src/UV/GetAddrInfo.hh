/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_UV_GETADDRINFO_INC
#	define ENTROPY_TETHYS_UV_GETADDRINFO_INC

#	include "Loop.hh"

	extern "C" {
		void _entropy_tethys_uv_getaddrinfo_cb(uv_getaddrinfo_t *, int, struct addrinfo *);
	}

	namespace Entropy
	{
		namespace Tethys
		{
			namespace UV
			{
				class GetAddrInfo :
					public Task
				{
					public:
						GetAddrInfo(const std::string &, const std::string &, const int, const std::function<void(const GetAddrInfo &)> &);
						GetAddrInfo(const GetAddrInfo &) = delete;
						~GetAddrInfo();
						void Added(Loop &);
						const struct addrinfo *Info() const;
					private:
						void callback(struct addrinfo *);
					private:
						uv_getaddrinfo_t _req;
						struct addrinfo *_res;
						std::function<void(const GetAddrInfo &)> _cb;
						std::string _host;
						std::string _service;
						struct addrinfo _hints;
					friend void ::_entropy_tethys_uv_getaddrinfo_cb(uv_getaddrinfo_t *, int, struct addrinfo *);
				};
			}
		}
	}

#endif
