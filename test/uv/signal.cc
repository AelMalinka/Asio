/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include <cstdlib>
#include <iostream>
#include <signal.h>
#include "Application.hh"
#include "UV/Timer.hh"
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace Entropy::Tethys;

using boost::lexical_cast;

class Application :
	public Entropy::Tethys::Application
{
	public:
		Application(const int, char *[]);
		bool fail;
	private:
		UV::Timer _t;
};

int main(int ArgC, char *ArgV[])
{
	try
	{
		::Application app(ArgC, ArgV);
		app();

		return (app.fail ? EXIT_FAILURE : EXIT_SUCCESS);
	}
	catch(exception &e)
	{
		cerr << e << endl;
		return EXIT_FAILURE;
	}
}

::Application::Application(const int ArgC, char *ArgV[]) :
	Entropy::Tethys::Application(ArgC, ArgV),
	fail(true),
	_t(chrono::seconds(0), chrono::seconds(1), [this](){})
{
	auto signal = SIGINT;

	if(ArgC == 2) {
		signal = lexical_cast<decltype(signal)>(ArgV[1]);
	}

	setSignal(signal, [this]() {
		fail = false;
		Stop();
	});

	Add(_t);
}
