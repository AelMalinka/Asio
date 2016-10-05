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
					explicit Buffer(const std::size_t);
					Buffer(const std::size_t, const std::size_t, charT *);
					Buffer(const Buffer &) = delete;
					Buffer(Buffer &&);
					~Buffer();
					bool operator == (const Buffer<charT> &);
					std::size_t &size();
					charT *data();
					charT *begin();
					charT *end();
					const std::size_t &size() const;
					const std::size_t &capacity() const;
					const charT *data() const;
					const charT *begin() const;
					const charT *end() const;
				private:
					std::size_t _size;
					std::size_t _capacity;
					charT *_data;
			};
		}
	}

#	include "Buffer.impl.hh"

#endif
