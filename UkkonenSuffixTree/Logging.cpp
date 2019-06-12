#pragma once

#include "Logging.h"

void LogPointer(std::string message, void* ptr) {
	std::cout << message << ptr;
}