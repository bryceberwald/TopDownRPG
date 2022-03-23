default:
	g++ -std=c++11 -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/poco/*.a -lcurl lib/libcurl.a lib/libraylib.a src/api.cpp src/apiFunctions.cpp -o newSavages

