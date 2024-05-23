FILE=./main.c
TEMP=./temp.c
OUT=./temp

all: ./flush.c tester ${FILE}
	cat ./flush.c ${FILE} > ${TEMP}
	gcc ${TEMP} -o ${OUT}
	./tester ${OUT}
	@rm ${OUT} ${TEMP}

tester: tester.c
	gcc tester.c -o ./tester
