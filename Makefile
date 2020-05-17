
all: bin/test bin/client bin/server
bin/test: src/test.cpp
	g++ src/test.cpp -o bin/test

bin/client: src/client.cpp
	g++ src/client.cpp -o bin/client

bin/server: src/server.cpp
	g++ src/server.cpp -lpthread -o bin/server
clean:
	rm bin/*