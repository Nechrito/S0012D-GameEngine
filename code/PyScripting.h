#pragma once

#include "pybind11/embed.h"

namespace py = pybind11;

namespace GameEngine
{
	class PyScripting
	{
	public:

		PyScripting();
		~PyScripting();

		static int add(int i, int j);
		static void log(Util::StringAtom msg);


	private:
		

	};
}