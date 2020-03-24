import PyModule # from the PyScripting.cpp 
import nmath


def main():
	PyModule.log("PythonScrpt.py init")
	
	entityManager = PyModule.EntityManager()
	entityManager.RegisterEntity("Footman", "mdl:Units/Unit_Footman.n3", "Examples", nmath.Point(0.0, 0.0, 5.0))


if __name__ == '__main__':
	main()