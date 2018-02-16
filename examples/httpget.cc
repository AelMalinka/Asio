/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include <cstdlib>
#include <iostream>
#include <string>
#include "Application.hh"
#include "UV/TcpClient.hh"
#include "Protocol/Http.hh"

using namespace std;
using namespace Entropy;
using namespace Entropy::Tethys;
using namespace Entropy::Tethys::Protocol;

using Entropy::Tethys::UV::TcpClient;
using Entropy::Tethys::UV::Tcp;

class Usage :
	public Task
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
		void onConnect(Tcp &);
		void onMessage(Stream<> &, HttpMessage<> &&);
	private:
		Http<::Application> _protocol;
		shared_ptr<Usage> _usage;
		shared_ptr<TcpClient> _client;
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
	: Tethys::Application(argc, argv), _protocol(*this), _usage(), _client()
{
	if(ArgC() != 3) {
		_usage = make_shared<Usage>(ArgV()[0]);

		Add(*_usage);
	} else {
		_client = make_shared<TcpClient>(ArgV()[1], ArgV()[2], _protocol);

		Add(*_client);
	}
}

void ::Application::onConnect(Tcp &s)
{
	HttpMessage<> m("GET / HTTP/1.1");

	m.Headers()["Host"] = s.Host();
	m.Headers()["Accept"] = "*/*";
	m.Headers()["Connection"] = "close";

	s << m;
}

void ::Application::onMessage(Stream<> &, HttpMessage<> &&m)
{
	cout << m.Start() << endl;
	for(auto &p : m.Headers()) {
		cout << p.first << ": " << p.second << endl;
	}

	cout << endl << m.Body() << endl;
}

Usage::Usage(const string &name)
	: _name(name)
{}

void Usage::operator () ()
{
	cout << _name << ": host port" << endl;
}
