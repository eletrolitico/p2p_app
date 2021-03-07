WX_LD_FLAGS = $(shell wx-config --cxxflags --libs std)

client: client.cpp
	$(CC) -o $@ $^ $(WX_LD_FLAGS) -ltoxcore -lsodium
clean:
	-rm -f client