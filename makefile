./a.out: finalproject.o
	compile finalproject.o -o ./a.out

main.o: finalproject.c
	compile -c finalproject.c

clean:
	rm *.o ./a.out