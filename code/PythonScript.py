import PyModule # from the PyScripting.cpp 


def main():
	result = PyModule.add(3, 7)
	PyModule.log(str(result))

if __name__ == '__main__':
	main()