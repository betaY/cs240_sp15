
all: test_mystring test_array test_linked_list ptr-simple simple_list

ptr-simple: ptr-simple.c 
	gcc -std=gnu99 -g -o ptr-simple ptr-simple.c

simple_list: simple_list.c 
	gcc -std=gnu99 -g -o simple_list simple_list.c

test_array: test_array.c array.c
	gcc -std=gnu99 -g -o test_array test_array.c array.c

test_mystring: test_mystring.c mystring.c
	gcc -std=gnu99 -g -o test_mystring test_mystring.c mystring.c

test_linked_list: LinkedList.c test_linked_list.c
	gcc -std=gnu99 -g -o test_linked_list test_linked_list.c LinkedList.c

clean:
	rm -f *.o *.out total.txt grades1.rt test_mystring test_array test_linked_list ptr-simple simple_list

