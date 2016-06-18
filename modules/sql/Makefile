all:mysqld nsql.o
	mv mysql/*.o ../
	mv *.o ../
mysqld:
	make -C mysql/
nsql.o:
	gcc -c nsql.c
clean:
	make -C mysql/ clean
	-rm *.o
