all: clean_terminal compile_all
	@echo " "
	@echo "all - completed"

clean_terminal:
	@echo " "
	@echo "cleaning terminal"
	clear
	@echo "terminal cleaned"
	@echo " "
	
force: clean compile_all
	@echo " "
	@echo "force - completed"

clean:
	rm -rf build
	mkdir build
	@echo " "
	@echo "clean - completed"
edit:
	@echo " "
	@echo "editing:"
	geany -i src/declconst.h src/all.cpp src/all.h src/clean.cpp src/clean.h src/edit.cpp src/edit.h src/executables.cpp src/executables.h src/file.cpp src/file.h src/force.cpp src/force.h src/necessary_compile.cpp src/necessary_compile.h src/objects.cpp src/objects.h src/print.cpp src/print.h src/utility.cpp src/utility.h genmakefile.cpp
	@echo " "
	@echo "editing - completed"
 
compile_all: build_all make_exe
	@echo " "
	@echo "compile_all - completed"
	
	@echo " "
	@echo "moving - executable:"
	mv genmakefile ../
	

make_exe:
	@echo " "
	@echo "make_exe - running:"
	g++ build/all.o build/clean.o build/edit.o build/executables.o build/file.o build/force.o build/genmakefile.o build/necessary_compile.o build/objects.o build/print.o build/utility.o -o genmakefile
	@echo "make_exe - completed"
	
build_all: build/all.o build/clean.o build/edit.o build/executables.o build/file.o build/force.o build/genmakefile.o build/necessary_compile.o build/objects.o build/print.o build/utility.o
	@echo " "
	@echo "build_all - completed"
	
build/genmakefile.o: genmakefile.cpp
	g++ -c -Isrc/ -Wall -std=c++17 genmakefile.cpp -o build/genmakefile.o

build/all.o: src/all.cpp src/all.h
	g++ -c -Isrc/ -Wall -std=c++17 src/all.cpp -o build/all.o
	
build/clean.o: src/clean.cpp src/clean.h
	g++ -c -Isrc/ -Wall -std=c++17 src/clean.cpp -o build/clean.o

build/edit.o: src/edit.cpp src/edit.h
	g++ -c -Isrc/ -Wall -std=c++17 src/edit.cpp -o build/edit.o
	
build/executables.o: src/executables.cpp src/executables.h
	g++ -c -Isrc/ -Wall -std=c++17 src/executables.cpp -o build/executables.o
	
build/file.o: src/file.cpp src/file.h
	g++ -c -Isrc/ -Wall -std=c++17 src/file.cpp -o build/file.o
	
build/force.o: src/force.cpp src/force.h
	g++ -c -Isrc/ -Wall -std=c++17 src/force.cpp -o build/force.o

build/necessary_compile.o: src/necessary_compile.cpp src/necessary_compile.h
	g++ -c -Isrc/ -Wall -std=c++17 src/necessary_compile.cpp -o build/necessary_compile.o
	
build/objects.o: src/objects.cpp src/objects.h
	g++ -c -Isrc/ -Wall -std=c++17 src/objects.cpp -o build/objects.o
	
build/print.o: src/print.cpp src/print.h
	g++ -c -Isrc/ -Wall -std=c++17 src/print.cpp -o build/print.o
	
build/utility.o: src/utility.cpp src/utility.h
	g++ -c -Isrc/ -Wall -std=c++17 src/utility.cpp -o build/utility.o
	
	
	
	
	
	
	
	
	
