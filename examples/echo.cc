/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include <cstdlib>
#include <iostream>
#include "Application.hh"
#include "UV/TcpServer.hh"

using namespace std;
using namespace Entropy;
using namespace Entropy::Tethys;

class Usage :
	public Task
{
	public:
		Usage(const string &);
		void operator () ();
	private:
		string _name;
};

class Server :
	public UV::TcpServer
{
	public:
		Server(const string &, const string &);
	private:
		void onData(Tethys::Stream<char> &);
};

class Application :
	public Tethys::Application
{
	public:
		Application(const int, char *[]);
	private:
		shared_ptr<Usage> _usage;
		shared_ptr<Server> _server;
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

::Application::Application(const int argc, char *argv[])
	: Tethys::Application(argc, argv), _usage(), _server()
{
	setSignal(SIGINT, [this]() {
		Stop();
	});

	if(ArgC() != 1 && ArgC() != 2 && ArgC() != 3) {
		_usage = make_shared<Usage>(ArgV()[0]);

		Add(*_usage);
	} else {
		string host = "localhost";
		string port = "echo";

		if(ArgC() == 3) {
			host = ArgV()[1];
			port = ArgV()[2];
		} else if(ArgC() == 2) {
			port = ArgV()[1];
		}

		_server = make_shared<Server>(host, port);

		Add(*_server);
	}
}

Server::Server(const string &host, const string &service)
	: UV::TcpServer(host, service)
{}

void Server::onData(Tethys::Stream<char> &s)
{
	while(!s.eof()) {
		string line;

		std::getline(s, line);
		s << line << endl;
	}
}

Usage::Usage(const string &name)
	: _name(name)
{}

void Usage::operator () ()
{
	cout << _name << ": [host] [port]" << endl;
}
