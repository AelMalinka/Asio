/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_PROTOCOL_EXCEPTION_INC
#	define ENTROPY_TETHYS_PROTOCOL_EXCEPTION_INC

#	include "../Exception.hh"

	namespace Entropy
	{
		namespace Tethys
		{
			namespace Protocol
			{
				ENTROPY_EXCEPTION(Exception, "Protocol Exception", Tethys::Exception);
			}
		}
	}

#	include "Exception.impl.hh"

#endif
