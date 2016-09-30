/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_ASIO_BUFFER_IMPL
#	define ENTROPY_ASIO_BUFFER_IMPL

	namespace Entropy
	{
		namespace Asio
		{
			template<typename charT> Buffer<charT>::Buffer() = default;
			template<typename charT> Buffer<charT>::Buffer(const Buffer<charT> &) = default;
			template<typename charT> Buffer<charT>::Buffer(Buffer<charT> &&) = default;
			template<typename charT> Buffer<charT>::~Buffer() = default;

			template<typename charT>
			Buffer<charT>::Buffer(const std::size_t size, const std::size_t length, charT *data)
				: _size(size), _length(length), _data(data)
			{}

			template<typename charT>
			std::size_t &Buffer<charT>::size()
			{
				return _size;
			}

			template<typename charT>
			std::size_t &Buffer<charT>::length()
			{
				return _length;
			}

			template<typename charT>
			charT *Buffer<charT>::data()
			{
				return _data;
			}

			template<typename charT>
			charT *Buffer<charT>::begin()
			{
				return _data;
			}

			template<typename charT>
			charT *Buffer<charT>::end()
			{
				return _data + _length;
			}

			template<typename charT>
			const std::size_t &Buffer<charT>::size() const
			{
				return _size;
			}

			template<typename charT>
			const std::size_t &Buffer<charT>::length() const
			{
				return _length;
			}

			template<typename charT>
			const charT *Buffer<charT>::data() const
			{
				return _data;
			}

			template<typename charT>
			const charT *Buffer<charT>::begin() const
			{
				return _data;
			}

			template<typename charT>
			const charT *Buffer<charT>::end() const
			{
				return _data + _length;
			}
		}
	}

#endif
