/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_ASIO_UV_LOOP_IMPL
#	define ENTROPY_ASIO_UV_LOOP_IMPL

#	include "Loop.hh"

	namespace Entropy
	{
		namespace Asio
		{
			namespace UV
			{
				template<typename E>
				typename ::Entropy::detail::WithErrorInfo<E>::type AttachUvInfo(const E &e, const int status)
				{
					auto ret(ENTROPY_ATTACH_TRACE(e));

					ret <<
						SystemErrorCode(std::error_code(-status, std::system_category())) <<
						SystemError(uv_strerror(status));

					return ret;
				}
			}
		}
	}

#endif
