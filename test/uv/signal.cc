/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include <cstdlib>
#include <iostream>
#include <signal.h>
#include "Application.hh"

using namespace std;
using namespace Entropy::Tethys;

class Application :
	public Entropy::Tethys::Application
{
	public:
		Application(const int, char *[]);
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

::Application::Application(const int ArgC, char *ArgV[])
	: Entropy::Tethys::Application(ArgC, ArgV)
{
	setSignal(SIGINT, [this]() {
		Stop();
	});
}
