/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_ASIO_STREEAMBUFFER_IMPL
#	define ENTROPY_ASIO_STREEAMBUFFER_IMPL

#	include "StreamBuffer.hh"

#	include <cmath>

	namespace Entropy
	{
		namespace Asio
		{
			template<typename Stream, typename charT, typename traits>
			StreamBuffer<Stream, charT, traits>::StreamBuffer(Stream &s)
				: std::basic_streambuf<charT, traits>(), _stream(s), _buffers(), _current(_buffers.end()), _write(nullptr), _is_seek(false)
			{
				_stream.onRead([this](Buffer<charT> &&buffer) {
					ENTROPY_LOG(Log, Severity::Debug) << "StreamBuffer<>: Adding newly read data";

					_buffers.push_back(std::move(buffer));


					for(auto &&i : _buffers) {
						ENTROPY_LOG(Log, Severity::Debug) << "StreamBuffer<>: current buffers: '" << std::basic_string<charT>(i.data(), i.size()) << "'";
					}

					if(_current == _buffers.end()) {
						ENTROPY_LOG(Log, Severity::Debug) << "StreamBuffer<>: New data is only data";

						_current = _buffers.begin();
						this->setg(_current->begin(), _current->begin(), _current->end());
					}
				});

				_write = new Buffer<charT>(write_buffer_size);
			}

			template<typename Stream, typename charT, typename traits>
			StreamBuffer<Stream, charT, traits>::~StreamBuffer()
			{
				delete _write;
			}

			template<typename Stream, typename charT, typename traits>
			std::streamsize StreamBuffer<Stream, charT, traits>::Available() const
			{
				using namespace std;

				streamsize ret = 0;

				for(auto &&b : _buffers) {
					ret += b.size();
				}

				return ret;
			}

			template<typename Stream, typename charT, typename traits>
			typename traits::pos_type StreamBuffer<Stream, charT, traits>::seekpos(typename traits::pos_type dist, std::ios_base::openmode mode)
			{
				return seekoff(dist, std::ios_base::beg, mode);
			}

			template<typename Stream, typename charT, typename traits>
			typename traits::pos_type StreamBuffer<Stream, charT, traits>::seekoff(typename traits::off_type dist, std::ios_base::seekdir dir, std::ios_base::openmode mode)
			{
				if(mode & std::ios_base::out)
					return typename traits::pos_type(typename traits::off_type(-1));

				charT *pos;

				ENTROPY_LOG(Log, Severity::Debug) << "StreamBuffer<>::seek()";
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

				ENTROPY_LOG(Log, Severity::Debug) << "StreamBuffer<>::seek(): setting position to 0 " << pos - _current->begin() << " " << _current->end() - _current->begin() << " on buffer: '" << std::basic_string<charT>(_current->data(), _current->size()) << "'";
				this->setg(_current->begin(), pos, _current->end());
				if(pos == _current->end())
					_is_seek = true;

				std::streamsize ret = 0;
				for(auto &&j : _buffers) {
					if(j == *_current)
						break;

					ret += j.size();
				}

				ret += dist;

				return ret;
			}

			template<typename Stream, typename charT, typename traits>
			typename traits::int_type StreamBuffer<Stream, charT, traits>::underflow()
			{
				if(_is_seek) {
					_is_seek = false;
					return traits::to_int_type(*this->gptr());
				}

				ENTROPY_LOG(Log, Severity::Debug) << "StreamBuffer<>::underflow()";

				if(_current == _buffers.end())
					return traits::eof();

				auto i = _current++;
				_buffers.erase(i);

				if(_current == _buffers.end())
					return traits::eof();

				this->setg(_current->begin(), _current->begin(), _current->end());

				return traits::to_int_type(*this->gptr());
			}

			template<typename Stream, typename charT, typename traits>
			std::streamsize StreamBuffer<Stream, charT, traits>::showmanyc()
			{
				return Available();
			}
		}
	}

#endif
