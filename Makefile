all: generate

generate: 
	g++ $(var) glad.c -ldl -lglfw
	./a.out

clean: 
	rm a.out
