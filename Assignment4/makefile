objects = main.o src/Matrix.o

matrix_out : $(objects)
	g++ -o matrix_out $(objects)

$(objects) : src/Matrix.hpp

.PHONY : clean
clean :
	rm matrix_out $(objects)
