#include "stdneb.h"
#include "PyScripting.h"

namespace GameEngine
{
	PyScripting::PyScripting()
	{

	}

	PyScripting::~PyScripting()
	{

	}

	int PyScripting::add(int i, int j)
	{
		return i + j;
	}

	void PyScripting::log(Util::StringAtom msg)
	{
		n_printf(msg.Value());
	}

	PYBIND11_EMBEDDED_MODULE(PyModule, m)
	{
		m.doc() = "pybind11";

		// todo
		py::class_<Util::StringAtom>(m, "StringAtom").def(py::init<const char*>());
		py::implicitly_convertible<const char*, Util::StringAtom>();
		
		m.def("add", &PyScripting::add, py::arg("i"), py::arg("j"));
		m.def("log", &PyScripting::log, py::arg("msg"));

	}
}
