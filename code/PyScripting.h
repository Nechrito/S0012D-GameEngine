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

		static void log(const Util::StringAtom& msg);

		static void setAttribute(const Util::StringAtom& entityName, const Util::StringAtom& attributeName, const Util::Variant& variable);


	private:
		

	};
}