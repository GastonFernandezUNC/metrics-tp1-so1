CFLAGS = -pthread
LDFLAGS = -L/usr/local/lib -lprom -lpromhttp

includes = -I /usr/local/include/ -I include/

all: compile link clean run

compile:
	gcc ${includes} -c src/*.c ${CFLAGS}

link:
	gcc ${includes} *.o -o metrics ${LDFLAGS}

exprt:
	export LD_LIBRARY_PATH=/usr/local/lib:${LD_LIBRARY_PATH}

clean:
	rm *.o

run:
	./metrics
