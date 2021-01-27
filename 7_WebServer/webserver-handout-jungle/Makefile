# client: client.o csapp.o
# 	gcc -o client client.o csapp.o -lpthread
# csapp.o: csapp.c csapp.h
# 	gcc -c csapp.c csapp.h
# client.o: csapp.h client.c
# 	gcc -c csapp.h client.c

tiny: tiny.o csapp.o
	gcc -o tiny tiny.o csapp.o -lpthread
csapp.o: csapp.c csapp.h
	gcc -c csapp.c csapp.h
tiny.o: csapp.h tiny.c
	gcc -c csapp.h tiny.c

# form-adder: form-adder.o
# 	gcc -o cgi-bin/form-adder form-adder.o -lpthread
# form-adder.o: csapp.h form-adder.c
# 	gcc -c csapp.h form-adder.c
