server: server.cpp
	g++ -std=c++11 -pthread server.cpp -o server

clean:
	rm -rf server
