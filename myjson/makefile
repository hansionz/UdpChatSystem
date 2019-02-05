main :main.o value.o read.o write.o
	g++ -o main main.o value.o read.o write.o 
main.o:main.cpp read.h write.h
	g++ -c main.cpp -o main.o
value.o:value.cpp value.h jsexcept.h
	g++ -c value.cpp -o value.o
read.o:read.cpp read.h
	g++ -c read.cpp -o read.o
write.o:write.cpp write.h
	g++ -c write.cpp -o write.o

.PHONY:clean 
clean:
	$(RM) *.o main
