FILE=./main.c
OUT=./temp.out

all: ./flush.c tester ${FILE}
	cat ./flush.c ${FILE} > temp.c
	gcc temp.c -o ${OUT}
	./tester ${OUT}
	@rm ${OUT}

tester: tester.c
	gcc tester.c -o ./tester
