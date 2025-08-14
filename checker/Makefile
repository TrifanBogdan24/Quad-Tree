CC=gcc -g
QTREE=quadtree
PATH_TO_CS='coding style'

build: $(QTREE)

$(QTREE):
	$(CC) tema-2.c -o $(QTREE)

clean:
	rm -f *.o $(QTREE)

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(QTREE)

coding-style:
	cd $(PATH_TO_CS) ; ./cs.sh ../; cd ../

checker:
	./check.sh

zip:
	zip -r tema-2.zip $(QTREE) *.c *.h Makefile README
