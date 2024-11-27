INCLUDE_FLAGS=-Wall -Werror -g -I/usr/local/include -I/opt/local/include
LIBRARY_FLAGS=-L/usr/local/lib -L/usr/lib/x86_64-linux-gnu -lcrypto -lssl

gatekeeper: main.c data_encryption.h data_encryption.c
	gcc $(INCLUDE_FLAGS) -o ptest main.c data_encryption.c $(LIBRARY_FLAGS)

ptest: main.c data_encryption.h data_encryption.c
	gcc $(INCLUDE_FLAGS) -o ptest --coverage main.c data_encryption.c $(LIBRARY_FLAGS)