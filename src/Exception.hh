/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_ASIO_EXCEPTION
#	define ENTROPY_ASIO_EXCEPTION

#	include <Entropy/Exception.hh>

	namespace Entropy
	{
		namespace Asio
		{
			ENTROPY_EXCEPTION_BASE(Exception, "Base Asio Exception");
		}
	}

#endif
