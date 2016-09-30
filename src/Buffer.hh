/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_ASIO_BUFFER_INC
#	define ENTROPY_ASIO_BUFFER_INC

#	include "Exception.hh"

	namespace Entropy
	{
		namespace Asio
		{
			template<
				typename charT = char
			>
			class Buffer
			{
				public:
					Buffer();
					Buffer(const std::size_t, const std::size_t, charT *);
					Buffer(const Buffer &);
					Buffer(Buffer &&);
					~Buffer();
					std::size_t &size();
					std::size_t &length();
					charT *data();
					charT *begin();
					charT *end();
					const std::size_t &size() const;
					const std::size_t &length() const;
					const charT *data() const;
					const charT *begin() const;
					const charT *end() const;
				private:
					std::size_t _size;
					std::size_t _length;
					charT *_data;
			};
		}
	}

#	include "Buffer.impl.hh"

#endif
