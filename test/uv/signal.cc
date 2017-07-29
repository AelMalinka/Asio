/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include <cstdlib>
#include <iostream>
#include <signal.h>
#include "Application.hh"

using namespace std;
using namespace Entropy::Asio;

class Application :
	public Entropy::Asio::Application
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
	: Entropy::Asio::Application(ArgC, ArgV)
{
	setSignal(SIGINT, [this]() {
		Stop();
	});
}
