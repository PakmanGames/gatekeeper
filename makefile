INCLUDE_FLAGS=-Wall -Werror -g -I/usr/local/include -I/opt/local/include
LIBRARY_FLAGS=-L/usr/local/lib -L/usr/lib/x86_64-linux-gnu -lcrypto -lssl -lsqlite3

ptest: main.c data_encryption.h data_encryption.c user_interface.c user_interface.h password_manager.c password_manager.h
	gcc $(INCLUDE_FLAGS) -o ptest main.c data_encryption.c user_interface.c password_manager.c $(LIBRARY_FLAGS)

ui_test: user_interface.c user_interface.h data_encryption.h data_encryption.c 
	gcc $(INCLUDE_FLAGS) -o ui_test data_encryption.c user_interface.c $(LIBRARY_FLAGS)

clean:
	rm -f test; rm - f ui_test;