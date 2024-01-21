#include <iostream>
#include <spdlog/spdlog.h>
#include "Quickframe/src/Engine.h"

int main()
{
	try
	{
		Engine* wApp = Engine::GetInstance();
		wApp->Initialize();
		wApp->Run();
		delete wApp;
	}
	catch (std::exception& e)
	{
		spdlog::critical({ "0:s" }, e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}