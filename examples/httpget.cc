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

using boost::split;
using boost::is_any_of;

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
		string _url;
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
	: Tethys::Application(argc, argv), _protocol(*this), _usage(), _client(), _url("/")
{
	if(ArgC() != 2) {
		_usage = make_shared<Usage>(ArgV()[0]);

		Add(*_usage);
	} else {
		string uri = ArgV()[1];
		string port = "http";
		string host;

		vector<string> t;
		split(t, uri, is_any_of("/"));

		if(t.size() > 4) {
			auto i = t.begin();
			i += 4;
			while(i != t.end()) {
				t[3] += "/" + std::move(*i++);
			}
			t.resize(4);
		}

		if(t.size() == 3) t.resize(4);

		if(t.size() != 4 || t[0] != "http:" || t[1] != "") {
			_usage = make_shared<Usage>(ArgV()[0]);

			Add(*_usage);
		} else {
			if(t[2].find(":") != string::npos) {
				vector<string> t2;
				split(t2, t[2], is_any_of(":"));

				host = t2[0];
				port = t2[1];
			} else {
				host = t[2];
			}

			if(t[3] != "")
				_url = "/" + t[3];

			_client = make_shared<TcpClient>(host, port, _protocol);

			Add(*_client);
		}
	}
}

void ::Application::onConnect(Tcp &s)
{
	HttpMessage<> m(_url);

	m.Headers()["Host"s] = s.Host();
	m.Headers()["Accept"s] = "*/*"s;
	m.Headers()["Connection"s] = "close"s;

	s << m << flush;
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
	cout << _name << ": uri" << endl
		<< endl
		<< "where uri is http://addresss[:port]/url" << endl;
}
