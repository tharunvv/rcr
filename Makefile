STUDENT_ID=2784327

build:
	g++ -g pipeline.cpp -o exe


test:
	./exe

clean:
	rm -f exe
	rm -rf $(STUDENT_ID)-project

tar:
	make clean
	mkdir $(STUDENT_ID)-project
	cp pipeline.cpp read.me Makefile $(STUDENT_ID)-project
	tar cvzf $(STUDENT_ID)-project.tar.gz $(STUDENT_ID)-project
	rm -rf $(STUDENT_ID)-project

