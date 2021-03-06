#include "stdneb.h"
#include "PyScripting.h"
#include "BaseEntity.h"
#include "EntityManager.h"
#include "TransformComponent.h"
#include "GraphicsComponent.h"

namespace GameEngine
{
	PyScripting::PyScripting()
	{

	}

	PyScripting::~PyScripting()
	{

	}

	void PyScripting::log(const Util::StringAtom& msg)
	{
		n_printf(msg.Value());
	}

	PYBIND11_EMBEDDED_MODULE(PyModule, m)
	{
		// policy, cutting down on the code 
		const pybind11::return_value_policy reference = py::return_value_policy::reference;
		
		// take const char* from py script and convert to Util::StringAtom 
		py::class_<Util::StringAtom>(m, "StringAtom").def(py::init<const char*>());
		py::implicitly_convertible<const char*, Util::StringAtom>();

		m.def("log", &PyScripting::log, py::arg("msg"), py::return_value_policy::reference);

		// EntityManager
		// todo: Ptr<...> instead? https://pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html
		py::class_ <EntityManager, std::unique_ptr<EntityManager, py::nodelete>> (m, "EntityManager")
		.def(py::init([]()
		{
			return std::unique_ptr<EntityManager, py::nodelete>(EntityManager::Instance());
		}), reference)
		.def("Init", &EntityManager::Init, reference)
		.def("Update", &EntityManager::Update, reference)
		.def("Shutdown", &EntityManager::Shutdown, reference)
		.def("RegisterEntity", py::overload_cast<const Util::StringAtom&, const Util::StringAtom&, const Util::StringAtom&, const Math::point&>(&EntityManager::RegisterEntity), 
			py::arg("name"), py::arg("uri"), py::arg("tag"), py::arg("position"), reference)
		.def("GetEntity", py::overload_cast<const Util::StringAtom&>(&EntityManager::GetEntity), py::arg("name"), reference)
		.def("GetEntity", py::overload_cast<int>(&EntityManager::GetEntity), py::arg("id"), reference);
		
		// Entity
		py::class_<BaseEntity>(m, "BaseEntity")
			.def("Transform", [](BaseEntity* e) { return std::unique_ptr<TransformComponent, py::nodelete>
			(dynamic_cast<TransformComponent*>(e->GetComponent("Transform"))); })
			.def("Graphics", [](BaseEntity* e) { return std::unique_ptr<GraphicsComponent, py::nodelete>
			(dynamic_cast<GraphicsComponent*>(e->GetComponent("Graphics"))); })
				.def("RegisterVariable", py::overload_cast<const Util::StringAtom&, const Util::Variant&, bool>(&BaseEntity::RegisterVariable),
					py::arg("name"), py::arg("variable"), py::arg("overrideExisting"), reference)
				.def("Init", &BaseEntity::Init, reference)
				.def("Update", &BaseEntity::Update, reference)
				.def("Shutdown", &BaseEntity::Shutdown, reference)
			;
		
		// Component - Transform
		py::class_<TransformComponent>(m, "TransformComponent")
			.def("Init", &TransformComponent::Init, reference)
			.def("Update", &TransformComponent::Update, reference)
			.def("Shutdown", &TransformComponent::Shutdown, reference)
		
			.def("Translate", py::overload_cast<Math::float4>(&TransformComponent::Translate), py::arg("direction"), reference)
		
			.def("Rotate", py::overload_cast<float, float, float, float>(&TransformComponent::Rotate),
				py::arg("x"), py::arg("y"), py::arg("z"), py::arg("angle"), reference)
		
			.def("SetPosition", py::overload_cast<Math::float4>(&TransformComponent::SetPosition), py::arg("position"), reference)
		
			.def("SetRotation", py::overload_cast<float, float, float>(&TransformComponent::SetRotation), 
				py::arg("x"), py::arg("y"), py::arg("z"), reference)
		
			.def("SetScale", py::overload_cast<Math::float4>(&TransformComponent::SetScale), py::arg("scale"), reference)
			;
		
		// Component - Graphics
		py::class_<GraphicsComponent>(m, "GraphicsComponent")
			.def("Init", &GraphicsComponent::Init, reference)
			.def("Update", &GraphicsComponent::Update, reference)
			.def("Shutdown", &GraphicsComponent::Shutdown, reference)
			.def("Load", py::overload_cast<const Resources::ResourceName&, const Util::StringAtom&>(&GraphicsComponent::Load), 
			py::arg("resource"), py::arg("tag"), reference)
			;
		
	}
}
