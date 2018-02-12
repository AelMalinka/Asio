/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>'
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_STREAM_INC
#	define ENTROPY_TETHYS_STREAM_INC

#	include "StreamBuffer.hh"

	namespace Entropy
	{
		namespace Tethys
		{
			template<
				typename charT = char,
				typename traits = std::char_traits<charT>
			>
			class Stream :
				public std::basic_iostream<charT>
			{
				public:
					template<
						typename Application,
						typename = typename std::enable_if<
							std::is_class<Application>::value
						>::type
					>
					explicit Stream(Application &);
					template<
						typename F1,
						typename = typename std::enable_if<
							std::is_function<F1>::value
						>::type
					>
					explicit Stream(const F1 &);
					Stream(const Stream<charT, traits> &) = delete;
					Stream(Stream<charT, traits> &&);
					virtual ~Stream();
					virtual void Write(Buffer<charT> &&) = 0;
					virtual void Read(Buffer<charT> &&);
				private:
					StreamBuffer<charT, traits> _buffer;
					std::function<void(Stream<charT, traits> &)> _on_data;
			};
		}
	}

#	include "Stream.impl.hh"

#endif
