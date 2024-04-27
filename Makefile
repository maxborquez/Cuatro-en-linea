CC := g++
CFLAGS := -std=c++11
.PHONY: all clean
SERVER_SRC = server.cpp
CLIENT_SRC = client.cpp

all:	$(SERVER_SRC) $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o server $(SERVER_SRC)
	$(CC) $(CFLAGS) -o client $(CLIENT_SRC)

clean:
	rm -f server client