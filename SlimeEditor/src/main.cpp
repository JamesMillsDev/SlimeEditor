#include <cstdlib>

#include <algorithm>
#include <iostream>
#include <string>

#include "SlimeGameInstance.h"
#include "Catalyst/Application.h"

using Catalyst::Application;

int main()
{
	std::cout << "Are you the server or client? ";

	std::string response;
	std::getline(std::cin, response);

	std::ranges::transform(response, response.begin(), [](unsigned char c) { return std::tolower(c); });

	Application* app = new Application;

	app->Run(new SlimeGameInstance(response == "host"));

	return EXIT_SUCCESS;
}
