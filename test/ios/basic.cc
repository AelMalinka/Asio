/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include <cstdlib>
#include <iostream>
#include <streambuf>
#include <vector>
#include <stdexcept>

using namespace std;

template<typename charT = char>
class Buffer :
	public basic_streambuf<charT>
{
	private:
		typedef basic_streambuf<charT> base;
	public:
		typedef typename base::int_type int_type;
		typedef typename base::traits_type traits_type;
		typedef typename base::pos_type pos_type;
		typedef typename base::off_type off_type;
	public:
		Buffer();
	private:
		pos_type seekpos(pos_type, ios_base::openmode = ios_base::in | ios_base::out);
		pos_type seekoff(off_type, ios_base::seekdir, ios_base::openmode = ios_base::in | ios_base::out);
		int_type underflow();
		streamsize showmanyc();
		int_type overflow(int_type = traits_type::eof());
		int_type sync();
	private:
		void print_pos();
	private:
		vector<charT> _data;
		static constexpr size_t buffer_size = 16;
		static constexpr size_t put_back_size = 0;
		static constexpr size_t size = 1024;
};

int main()
{
	Buffer<> buffer;
	iostream test(&buffer);

	size_t n = test.rdbuf()->in_avail();
	char *s = new char[n];
	test.read(s, n);

	cout << test.rdbuf()->in_avail() << endl;

	return EXIT_SUCCESS;
}

template<typename charT>
Buffer<charT>::Buffer()
	: _data(buffer_size)
{
	char *start = _data.data();
	char *current = start + put_back_size;
	char *end = start + buffer_size;

	cout << "Buffer<>::Buffer()" << endl;
	print_pos();
	base::setg(start, current, end);
}

template<typename charT>
typename Buffer<charT>::pos_type Buffer<charT>::seekpos(pos_type p, ios_base::openmode)
{
	cout << "Buffer<>::seekpos(): " << p << endl;

	char *start = base::eback();
	char *current = start + p;
	char *end = base::egptr();

	base::setg(start, current, end);
	print_pos();
	return start - current;
}

template<typename charT>
typename Buffer<charT>::pos_type Buffer<charT>::seekoff(off_type p, ios_base::seekdir w, ios_base::openmode)
{
	cout << "Buffer<>::seekoff(): " << p << " towards: " << w << endl;

	char *start = base::eback();
	char *current = base::gptr();
	char *end = base::egptr();

	switch(w)
	{
		case ios_base::beg:
			current = start + p;
		break;
		case ios_base::cur:
			current += p;
		break;
		case ios_base::end:
			current = end + p;
		break;
		default:
			throw std::runtime_error("invalid direction");
	}

	base::setg(start, current, end);
	print_pos();
	return start - current;
}

template<typename charT>
streamsize Buffer<charT>::showmanyc()
{
	cout << "Buffer<>::showmanyc()" << endl;

	char *current = base::gptr();
	char *end = base::egptr();

	intptr_t dist = end - current;
	return size - dist;
}

template<typename charT>
typename Buffer<charT>::int_type Buffer<charT>::underflow()
{
	cout << "Buffer<>::underflow()" << endl;
	return traits_type::eof();
}

template<typename charT>
typename Buffer<charT>::int_type Buffer<charT>::overflow(int_type c)
{
	cout << "Buffer<>::overflow(): " << c << endl;
	return traits_type::eof();
}

template<typename charT>
typename Buffer<charT>::int_type Buffer<charT>::sync()
{
	cout << "Buffer<>::sync()" << endl;
	return traits_type::eof();
}

template<typename charT>
void Buffer<charT>::print_pos()
{
	char *eback = base::eback();
	char *gptr = base::gptr();
	char *egptr = base::egptr();

	intptr_t start = reinterpret_cast<intptr_t>(eback);
	intptr_t current = reinterpret_cast<intptr_t>(gptr) - start;
	intptr_t end = reinterpret_cast<intptr_t>(egptr) - start;

	cout << "Buffer<>::position: 0 " << current << " " << end << endl;
}
