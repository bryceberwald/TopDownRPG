default:
	g++ -std=c++11 -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a src/BlueFurry.cpp src/Functions.cpp src/main.cpp -o newSavages

