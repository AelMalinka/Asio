/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_STREAMBUFFER_INC
#	define ENTROPY_TETHYS_STREAMBUFFER_INC

#	include <streambuf>
#	include <list>
#	include <memory>
#	include <type_traits>

#	include "Exception.hh"
#	include "Buffer.hh"

#	ifndef ENTROPY_TETHYS_STREAMBUFFER_WRITE_BUFFER_SIZE
#		define ENTROPY_TETHYS_STREAMBUFFER_WRITE_BUFFER_SIZE 4096
#	endif

	namespace Entropy
	{
		namespace Tethys
		{
			template<
				typename charT = char,
				typename traits = std::char_traits<charT>
			>
			class StreamBuffer :
				public std::basic_streambuf<charT, traits>
			{
				private:
					typedef std::basic_streambuf<charT, traits> base;
				public:
					typedef typename base::int_type int_type;
					typedef typename base::traits_type traits_type;
					typedef typename base::pos_type pos_type;
					typedef typename base::off_type off_type;
				public:
					template<
						typename Stream,
						typename = typename std::enable_if<
							std::is_class<Stream>::value
						>::type
					>
					explicit StreamBuffer(Stream &);
					template<
						typename F1,
						typename F2,
						typename = typename std::enable_if<
							std::is_function<F1>::value &&
							std::is_function<F2>::value
						>::type
					>
					explicit StreamBuffer(const F1 &, const F2 &);
					StreamBuffer(const StreamBuffer<charT, traits> &) = delete;
					StreamBuffer(StreamBuffer<charT, traits> &&);
					~StreamBuffer();
					void AddData(Buffer<charT> &&);
				private:
					pos_type seekpos(pos_type, std::ios_base::openmode = std::ios_base::in | std::ios_base::out);
					pos_type seekoff(off_type, std::ios_base::seekdir, std::ios_base::openmode = std::ios_base::in | std::ios_base::out);
					int_type underflow();
					std::streamsize showmanyc();
					int_type overflow(int_type = traits_type::eof());
					int sync();
				private:
					std::function<void(Buffer<charT> &&)> _write;
					std::function<void(std::ios_base::iostate)> _clear;
					std::list<Buffer<charT>> _buffers;
					typename std::list<Buffer<charT>>::iterator _current;
					std::shared_ptr<Buffer<charT>> _w_buff;
					bool _is_seek;
					static constexpr std::size_t write_buffer_size = ENTROPY_TETHYS_STREAMBUFFER_WRITE_BUFFER_SIZE;
			};
		}
	}

#	include "StreamBuffer.impl.hh"

#endif
