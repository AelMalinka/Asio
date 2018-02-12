/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_STREAM_IMPL
#	define ENTROPY_TETHYS_STREAM_IMPL

#	include "Stream.hh"

	namespace Entropy
	{
		namespace Tethys
		{
			template<typename charT, typename traits>
			template<typename Application, typename> 
			Stream<charT, traits>::Stream(Application &app) :
				std::basic_iostream<charT, traits>(&_buffer),
				_buffer(*this),
				_on_data(std::bind(&Application::onData, &app, std::placeholders::_1))
			{}

			template<typename charT, typename traits>
			template<typename F1, typename>
			Stream<charT, traits>::Stream(const F1 &f1) :
				std::basic_iostream<charT, traits>(&_buffer),
				_buffer(*this),
				_on_data(f1)
			{}

			template<typename charT, typename traits> Stream<charT, traits>::Stream(Stream<charT, traits> &&) = default;
			template<typename charT, typename traits> Stream<charT, traits>::~Stream() = default;

			template<typename charT, typename traits>
			void Stream<charT, traits>::Read(Buffer<charT> &&buffer)
			{
				_buffer.AddData(std::move(buffer));
				_on_data(*this);
			}
		}
	}

#endif
