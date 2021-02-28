client: client.cpp
	$(CC) -o $@ $^ -ltoxcore -lsodium
clean:
	-rm -f client