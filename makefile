main: main.cpp
	g++ -g main.cpp cluster.cpp job.cpp -o a.out

clean: 
	$(RM) myprog
