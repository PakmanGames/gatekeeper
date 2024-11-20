INCLUDE_FLAGS=-Wall -Werror -g -I/usr/local/include -I/opt/local/include
LIBRARY_FLAGS=-L/usr/local/lib -L/usr/lib/x86_64-linux-gnu -lcrypto -lssl -lsqlite3

ptest: main.c data_encryption.h data_encryption.c database.h database.c
	gcc $(INCLUDE_FLAGS) -o ptest main.c data_encryption.c database.c $(LIBRARY_FLAGS)