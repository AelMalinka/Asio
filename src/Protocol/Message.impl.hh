/*	Copyright 2018 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_MESSAGE_PROTOCOL_IMPL
#	define ENTROPY_TETHYS_MESSAGE_PROTOCOL_IMPL

#	include "Message.hh"
#	include <boost/algorithm/string.hpp>
#	include <boost/algorithm/string/trim_all.hpp>
#	include <boost/lexical_cast.hpp>

	namespace Entropy
	{
		namespace Tethys
		{
			namespace Protocol
			{
				template<typename stringT>
				Message<stringT>::Message()
					: _headers(), _last(_headers.end()), _body(), _has_body(false)
				{}

				template<typename stringT> Message<stringT>::Message(const Message<stringT> &) = default;
				template<typename stringT> Message<stringT>::Message(Message<stringT> &&) = default;
				template<typename stringT> Message<stringT>::~Message() = default;

				template<typename stringT>
				void Message<stringT>::addHeader(string_type &&line)
				{
					if(std::isspace(line[0])) {
						if(_last == _headers.end())
							ENTROPY_THROW(Exception("Folding without previous header"));

						// 2018-02-22 AMR TODO: allow http to reject this per RFC 7230 §3.2.4
						boost::trim_all(line);
						_last->second += string_type(1, ' ') + std::move(line);
					} else {
						// 2018-02-16 AMR TODO: optimization
						std::vector<string_type> pieces;
						boost::split(pieces, line, boost::is_any_of(string_type(1, ':')));

						if(
							pieces.size() < 2 ||
							pieces[0].size() == 0 || pieces[1].size() == 0
						) {
							ENTROPY_THROW(Exception("Malformed Header in Message") <<
								HeaderLineInfo(line)
							);
						} else if(pieces.size() > 2) {
							auto i = pieces.begin();
							i += 2;
							while(i != pieces.end()) {
								pieces[1] += string_type(1, ':') + std::move(*i++);
							}

							pieces.resize(2);
						}

						// 2018-02-22 AMR FIXME: RFC 7230 §3.2.4
						// 2018-02-22 AMR TODO: are we a proxy or a server, is it ok to server instead of proxy
						boost::trim(pieces[0]);
						boost::trim(pieces[1]);

						// 2018-02-22 AMR NOTE: folding in one call
						string_type delseq;

						delseq += '\r';
						delseq += '\n';

						boost::erase_all(pieces[1], std::move(delseq));
						boost::replace_all(pieces[1], string_type(1, '\t'), string_type(1, ' '));
						boost::trim_all(pieces[1]);

						try
						{
							// 2018-02-19 AMR NOTE: some include the \r\n and some don't
							// 2018-02-19 AMR TODO: check standard
							// 2018-02-21 AMR TODO: should this be here or in HttpMessage overload
							if(pieces[0] == "Content-Length") {
								_body.reserve(boost::lexical_cast<typename string_type::size_type>(pieces[1]));
								ENTROPY_LOG(Log, Severity::Debug) << "body capacity set to " << _body.capacity();
							}
						}
						catch(boost::bad_lexical_cast &)
						{
							ENTROPY_LOG(Log, Severity::Warning) << "failed to convert Content-Length to size_t (" << pieces[1] << ")";
						}

						Headers()[pieces[0]] = std::move(pieces[1]);
						_last = Headers().find(pieces[0]);
					}
				}

				template<typename stringT>
				void Message<stringT>::addBody(string_type &&s)
				{
					_has_body = true;
					_body = std::move(s);
				}

				template<typename stringT>
				void Message<stringT>::addBody(sock_type &s)
				{
					_has_body = true;

					auto start = s.tellg();
					auto sz = s.rdbuf()->in_avail();

					s.seekg(-2, std::ios_base::end);
					if(s.peek() == '\r') {
						s.seekg(1, std::ios_base::cur);
						if(s.peek() == '\n')
							sz -= 2;
					}

					s.seekg(start);
					auto osz = Body().size();
					Body().resize(sz + osz);
					ENTROPY_LOG(Log, Severity::Debug) << "body size set to " << _body.size();
					s.read(Body().data() + osz, sz);
				}

				template<typename stringT>
				bool Message<stringT>::hasBody() const
				{
					return _has_body;
				}

				template<typename stringT>
				stringT &Message<stringT>::Body()
				{
					return _body;
				}

				template<typename stringT>
				const stringT &Message<stringT>::Body() const
				{
					return _body;
				}

				template<typename stringT>
				typename Message<stringT>::container_type &Message<stringT>::Headers()
				{
					return _headers;
				}

				template<typename stringT>
				const typename Message<stringT>::container_type &Message<stringT>::Headers() const
				{
					return _headers;
				}
			}
		}
	}

	template<typename charT, typename traits, typename stringT>
	std::basic_ostream<charT, traits> &operator << (std::basic_ostream<charT, traits> &o, const Entropy::Tethys::Protocol::Message<stringT> &m)
	{
		for(auto &&p : m.Headers())
			o << p.first << ": " << p.second << "\r\n";

		o << "\r\n";
		if(m.hasBody())
			o << m.Body() << "\r\n";

		return o;
	}

#endif
