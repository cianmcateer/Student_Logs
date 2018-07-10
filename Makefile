Student_store.out: $(patsubst %.cpp, %.o, $(wildcard *.cpp))
	g++ -o $@ $^
%.o: %.cpp *.h
	g++ -c $< -std=c++11
