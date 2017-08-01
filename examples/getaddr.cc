/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include <cstdlib>
#include <algorithm>
#include "Application.hh"
#include "UV/GetAddrInfo.hh"
#include <arpa/inet.h>

using namespace std;
using namespace Entropy;

// 2017-03-23 AMR TODO: agnostic interface?
using Entropy::Tethys::UV::GetAddrInfo;

class Usage :
	public Tethys::Task
{
	public:
		Usage(const string &);
		void operator () ();
	private:
		string _name;
};

class Application :
	public Tethys::Application
{
	public:
		Application(int, char *[]);
	private:
		void display(const GetAddrInfo &);
	private:
		shared_ptr<GetAddrInfo> _addr;
		shared_ptr<Usage> _usage;
};

int main(int ArgC, char *ArgV[])
{
	try
	{
		::Application app(ArgC, ArgV);
		app();

		return EXIT_SUCCESS;
	}
	catch(exception &e)
	{
		cerr << e << endl;
		return EXIT_FAILURE;
	}
}

::Application::Application(int argc, char *argv[])
	: Tethys::Application(argc, argv), _addr(), _usage()
{
	if(ArgC() != 3) {
		_usage = make_shared<Usage>(ArgV()[0]);

		Add(*_usage);
	} else {
		_addr = make_shared<GetAddrInfo>(
			ArgV()[1],
			ArgV()[2],
			SOCK_STREAM,
			[this](auto &res) {
				this->display(res);
			}
		);

		Add(*_addr);
	}
}

void ::Application::display(const GetAddrInfo &res)
{
	// 2017-03-23 AMR TODO: abstracted interface?
	for(auto p = res.Info(); p; p = p->ai_next) {
		char buff[max(INET_ADDRSTRLEN, INET6_ADDRSTRLEN)];
		void *addr = nullptr;
		in_port_t port = 0;

		if(p->ai_family == AF_INET) {
			addr = &reinterpret_cast<struct sockaddr_in *>(p->ai_addr)->sin_addr;
			port = reinterpret_cast<struct sockaddr_in *>(p->ai_addr)->sin_port;
		} else if(p->ai_family == AF_INET6) {
			addr = &reinterpret_cast<struct sockaddr_in6 *>(p->ai_addr)->sin6_addr;
			port = reinterpret_cast<struct sockaddr_in6 *>(p->ai_addr)->sin6_port;
		}

		if(addr != nullptr)
			cout << inet_ntop(p->ai_family, addr, buff, max(INET_ADDRSTRLEN, INET6_ADDRSTRLEN)) << " " << ntohs(port) << endl;
	}
}

Usage::Usage(const string &name)
	: _name(name)
{}

void Usage::operator () ()
{
	cout << _name << ": host service" << endl;
}
