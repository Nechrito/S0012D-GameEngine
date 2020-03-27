import PyModule # from the PyScripting.cpp 
import nmath


def main():
	PyModule.log("PythonScrpt.py init")
	
	entityManager = PyModule.EntityManager()
	entityManager.RegisterEntity("Catapult", "mdl:Units/Unit_Catapult.n3", "Examples", nmath.Point(5.0, -1.0, 0.0))
	entityManager.RegisterEntity("Footman", "mdl:Units/Unit_Footman.n3", "Examples", nmath.Point(0.0, 0.0, -1.5))


if __name__ == '__main__':
	main()