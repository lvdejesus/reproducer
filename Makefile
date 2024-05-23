FILE=./main.c
OUT=./temp.out

all: ./tester.c ${FILE}
	@cat ./tester.c ${FILE} | gcc -xc - -o ${OUT}
	${OUT}
	@rm ${OUT}
