/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_ASIO_BUFFER_IMPL
#	define ENTROPY_ASIO_BUFFER_IMPL

	namespace Entropy
	{
		namespace Asio
		{
			template<typename charT> Buffer<charT>::Buffer()
				: _size(0), _capacity(0), _data(nullptr)
			{}

			template<typename charT>
			Buffer<charT>::Buffer(const std::size_t capacity)
				: _size(0), _capacity(capacity), _data(new charT[_capacity])
			{}

			template<typename charT>
			Buffer<charT>::Buffer(const std::size_t size, const std::size_t capacity, charT *data)
				: _size(size), _capacity(capacity), _data(data)
			{}

			template<typename charT>
			Buffer<charT>::Buffer(Buffer<charT> &&o)
				: Buffer()
			{
				using std::swap;

				swap(_size, o._size);
				swap(_capacity, o._capacity);
				swap(_data, o._data);
			}

			template<typename charT>
			Buffer<charT>::~Buffer()
			{
				delete[] _data;
			}

			template<typename charT>
			bool Buffer<charT>::operator == (const Buffer<charT> &o)
			{
				return
					_data == o._data &&
					_size == o._size &&
					_capacity == o._capacity;
			}

			template<typename charT>
			std::size_t &Buffer<charT>::size()
			{
				return _size;
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
				return _data + _size;
			}

			template<typename charT>
			const std::size_t &Buffer<charT>::size() const
			{
				return _size;
			}

			template<typename charT>
			const std::size_t &Buffer<charT>::capacity() const
			{
				return _capacity;
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
				return _data + _size;
			}
		}
	}

#endif
