CFLAGS = -pthread -g
LDFLAGS = -L/usr/local/lib -lprom -lpromhttp

includes = -I /usr/local/include/ -I include/

all: compile link clean

compile:
	gcc ${includes} ${CFLAGS} -c src/*.c 

link:
	gcc *.o -o metrics ${LDFLAGS}

exprt:
	export LD_LIBRARY_PATH=/usr/local/lib:${LD_LIBRARY_PATH}

clean:
	rm *.o

run:
	./metrics
