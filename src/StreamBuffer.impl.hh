/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_STREEAMBUFFER_IMPL
#	define ENTROPY_TETHYS_STREEAMBUFFER_IMPL

#	include "StreamBuffer.hh"

#	include <cmath>

	namespace Entropy
	{
		namespace Tethys
		{
			template<typename charT, typename traits>
			StreamBuffer<charT, traits>::StreamBuffer(
				const std::function<void(Buffer<charT> &&)> &f1,
				const std::function<void(std::ios_base::iostate)> &f2
			) :
				std::basic_streambuf<charT, traits>(),
				_write(f1),
				_clear(f2),
				_buffers(),
				_current(_buffers.end()),
				_w_buff(),
				_is_seek(false)
			{
				_w_buff = std::make_shared<Buffer<charT>>(write_buffer_size);
				this->setp(_w_buff->begin(), _w_buff->begin() + _w_buff->capacity());
			}

			template<typename charT, typename traits>
			template<typename Stream, typename>
			StreamBuffer<charT, traits>::StreamBuffer(Stream &s) :
				StreamBuffer<charT, traits>::StreamBuffer(
					std::bind(&Stream::Write, &s, std::placeholders::_1),
					std::bind(&Stream::clear, &s, std::placeholders::_1)
				)
			{}

			template<typename charT, typename traits> StreamBuffer<charT, traits>::StreamBuffer(StreamBuffer<charT, traits> &&) = default;
			template<typename charT, typename traits> StreamBuffer<charT, traits>::~StreamBuffer() = default;

			template<typename charT, typename traits>
			void StreamBuffer<charT, traits>::AddData(Buffer<charT> &&buffer)
			{
				ENTROPY_LOG(Log, Severity::Debug) << "Add Data";
				_buffers.emplace_back(std::move(buffer));

				if(_current == _buffers.end()) {
					--_current;

					this->setg(_current->begin(), _current->begin(), _current->end());
					_clear(std::ios::goodbit);
				}
			}

			template<typename charT, typename traits>
			typename StreamBuffer<charT, traits>::pos_type StreamBuffer<charT, traits>::seekpos(pos_type dist, std::ios_base::openmode mode)
			{
				return seekoff(dist, std::ios_base::beg, mode);
			}

			template<typename charT, typename traits>
			typename StreamBuffer<charT, traits>::pos_type StreamBuffer<charT, traits>::seekoff(off_type dist, std::ios_base::seekdir dir, std::ios_base::openmode mode)
			{
				if(mode & std::ios_base::out)
					return typename traits::pos_type(typename traits::off_type(-1));

				charT *pos;

				switch(dir)
				{
					case std::ios_base::beg:
						_current = _buffers.begin();
						pos = _current->begin();
					break;
					case std::ios_base::cur:
						pos = this->gptr();
					break;
					case std::ios_base::end:
						_current = _buffers.end();
						--_current;
						pos = _current->end();
					break;
					default:
						ENTROPY_THROW(Exception("Invalid seekdir"));
					break;
				}

				while(pos + dist >= _current->end()) {
					dist -= (_current->end() - pos);
					if(++_current == _buffers.end())
						return typename traits::pos_type(typename traits::off_type(-1));
					pos = _current->begin();
				}
				while(pos + dist < _current->begin()) {
					dist += (pos - _current->begin());
					if(_current == _buffers.begin())
						return typename traits::pos_type(typename traits::off_type(-1));
					--_current;
					pos = _current->end();
				}
				pos += dist;

				this->setg(_current->begin(), pos, _current->end());
				if(pos == _current->end())
					_is_seek = true;

				std::streamsize ret = 0;
				for(auto &&j : _buffers) {
					if(j == *_current)
						break;

					ret += j.size();
				}

				// 2018-02-16 AMR NOTE: apparently we get called w/ dist == 0, looking for curent position
				ret += (pos - _current->begin());

				return ret;
			}

			template<typename charT, typename traits>
			typename StreamBuffer<charT, traits>::int_type StreamBuffer<charT, traits>::underflow()
			{
				if(_is_seek) {
					_is_seek = false;
					return traits::to_int_type(*this->gptr());
				}

				if(_current == _buffers.end())
					return traits::eof();

				auto i = _current++;
				_buffers.erase(i);

				if(_current == _buffers.end())
					return traits::eof();

				ENTROPY_LOG(Log, Severity::Debug) << "Underflow";
				this->setg(_current->begin(), _current->begin(), _current->end());

				return traits::to_int_type(*this->gptr());
			}

			template<typename charT, typename traits>
			std::streamsize StreamBuffer<charT, traits>::showmanyc()
			{
				using namespace std;

				streamsize ret = 0;

				for(auto &&b : _buffers) {
					ret += b.size();
				}

				return ret;
			}

			template<typename charT, typename traits>
			typename StreamBuffer<charT, traits>::int_type StreamBuffer<charT, traits>::overflow(int_type c)
			{
				ENTROPY_LOG(Log, Severity::Debug) << "Overflow";

				_w_buff->size() = this->pptr() - this->pbase();
				_write(std::move(*_w_buff));

				_w_buff = std::make_shared<Buffer<charT>>(write_buffer_size);
				this->setp(_w_buff->begin(), _w_buff->begin() + _w_buff->capacity());

				*this->pptr() = c;
				this->pbump(1);

				return c;
			}

			template<typename charT, typename traits>
			int StreamBuffer<charT, traits>::sync()
			{
				ENTROPY_LOG(Log, Severity::Debug) << "Sync";

				_w_buff->size() = this->pptr() - this->pbase();

				if(_w_buff->size() > 0) {
					_write(std::move(*_w_buff));

					_w_buff = std::make_shared<Buffer<charT>>(write_buffer_size);
					this->setp(_w_buff->begin(), _w_buff->begin() + _w_buff->capacity());
				}

				return 0;
			}
		}
	}

#endif
