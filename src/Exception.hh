/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_EXCEPTION
#	define ENTROPY_TETHYS_EXCEPTION

#	include <Entropy/Exception.hh>
#	include <Entropy/Log.hh>

	namespace Entropy
	{
		namespace Tethys
		{
			ENTROPY_EXCEPTION_BASE(Exception, "Base Asio Exception");

			extern Entropy::Log::Logger Log;
		}
	}

#endif
