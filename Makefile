

all: necessary_compile
	@echo "Command executed = all/default make"


necessary_compile: run/initcongen run/simulate
	@echo "Command executed = necessary_compile"
	rm -rf run/output_files/plots
	mkdir run/output_files/plots


force: clean necessary_compile
	@echo "Command executed = force"


clean:
	rm -f build/cmdioprint.o build/cmdioread.o build/drw.o build/dstdiamension.o build/dstmns.o build/fileioplot.o build/fileioread.o build/fileiowrite.o build/funcconnection.o build/funcdetermine.o build/funcglobal.o build/funclinear.o build/initconmode.o build/proginitcongen.o build/progsimulate.o build/utilityrandom.o
	@echo "Command executed = clean"


edit:
	geany -i src/cmdio/cmdioprint.h src/cmdio/cmdioprint.cpp src/cmdio/cmdioread.h src/cmdio/cmdioread.cpp src/decl/declconst.h src/decl/declfilename.h src/decl/decltypedef.h src/drw/drw.h src/drw/drw.cpp src/dst/dstdiamension.h src/dst/dstdiamension.cpp src/dst/dstmns.h src/dst/dstmns.cpp src/fileio/fileioplot.h src/fileio/fileioplot.cpp src/fileio/fileioread.h src/fileio/fileioread.cpp src/fileio/fileiowrite.h src/fileio/fileiowrite.cpp src/func/funcconnection.h src/func/funcconnection.cpp src/func/funcdetermine.h src/func/funcdetermine.cpp src/func/funcglobal.h src/func/funcglobal.cpp src/func/funclinear.h src/func/funclinear.cpp src/initcon/initconmode.h src/initcon/initconmode.cpp src/prog/proginitcongen.cpp src/prog/progsimulate.cpp src/utility/utilityrandom.h src/utility/utilityrandom.cpp
	@echo "Command executed = open all files to be edited on Geany"


gen:
	@echo "Make the genmakefile which edites the makefile itself!"
	g++ -c -Isrc/ -Wall -std=c++17 src/make/all.cpp -o build/genmakefile_all.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/clean.cpp -o build/genmakefile_clean.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/edit.cpp -o build/genmakefile_edit.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/excutables.cpp -o build/genmakefile_excutables.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/file.cpp -o build/genmakefile_file.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/force.cpp -o build/genmakefile_force.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/gen.cpp -o build/genmakefile_gen.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/genmakefile.cpp -o build/genmakefile_genmakefile.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/necessary_compile.cpp -o build/genmakefile_necessary_compile.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/objects.cpp -o build/genmakefile_objects.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/print.cpp -o build/genmakefile_print.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/utility.cpp -o build/genmakefile_utility.o
	g++ -Isrc/ -o genmakefile.cpp


run/initcongen: build/cmdioprint.o build/cmdioread.o build/drw.o build/dstdiamension.o build/dstmns.o build/fileioplot.o build/fileioread.o build/fileiowrite.o build/funcconnection.o build/funcdetermine.o build/funcglobal.o build/funclinear.o build/initconmode.o build/proginitcongen.o build/progsimulate.o build/utilityrandom.o
	@echo "Executing command = link-compile of file [run/initcongen]"
	g++ build/cmdioprint.o build/cmdioread.o build/drw.o build/dstdiamension.o build/dstmns.o build/fileioplot.o build/fileioread.o build/fileiowrite.o build/funcconnection.o build/funcdetermine.o build/funcglobal.o build/funclinear.o build/initconmode.o build/proginitcongen.o build/progsimulate.o build/utilityrandom.o -o run/initcongen


run/simulate: build/cmdioprint.o build/cmdioread.o build/drw.o build/dstdiamension.o build/dstmns.o build/fileioplot.o build/fileioread.o build/fileiowrite.o build/funcconnection.o build/funcdetermine.o build/funcglobal.o build/funclinear.o build/initconmode.o build/proginitcongen.o build/progsimulate.o build/utilityrandom.o
	@echo "Executing command = link-compile of file [run/simulate]"
	g++ build/cmdioprint.o build/cmdioread.o build/drw.o build/dstdiamension.o build/dstmns.o build/fileioplot.o build/fileioread.o build/fileiowrite.o build/funcconnection.o build/funcdetermine.o build/funcglobal.o build/funclinear.o build/initconmode.o build/proginitcongen.o build/progsimulate.o build/utilityrandom.o -o run/simulate


build/cmdioprint.o: src/cmdio/cmdioprint.cpp
	@echo "Executing command = compile/make object[.o] file for [src/cmdio/cmdioprint.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/cmdio/cmdioprint.cpp -o build/cmdioprint.o


build/cmdioread.o: src/cmdio/cmdioread.cpp
	@echo "Executing command = compile/make object[.o] file for [src/cmdio/cmdioread.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/cmdio/cmdioread.cpp -o build/cmdioread.o


build/drw.o: src/drw/drw.cpp
	@echo "Executing command = compile/make object[.o] file for [src/drw/drw.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/drw/drw.cpp -o build/drw.o


build/dstdiamension.o: src/dst/dstdiamension.cpp
	@echo "Executing command = compile/make object[.o] file for [src/dst/dstdiamension.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/dst/dstdiamension.cpp -o build/dstdiamension.o


build/dstmns.o: src/dst/dstmns.cpp
	@echo "Executing command = compile/make object[.o] file for [src/dst/dstmns.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/dst/dstmns.cpp -o build/dstmns.o


build/fileioplot.o: src/fileio/fileioplot.cpp
	@echo "Executing command = compile/make object[.o] file for [src/fileio/fileioplot.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/fileio/fileioplot.cpp -o build/fileioplot.o


build/fileioread.o: src/fileio/fileioread.cpp
	@echo "Executing command = compile/make object[.o] file for [src/fileio/fileioread.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/fileio/fileioread.cpp -o build/fileioread.o


build/fileiowrite.o: src/fileio/fileiowrite.cpp
	@echo "Executing command = compile/make object[.o] file for [src/fileio/fileiowrite.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/fileio/fileiowrite.cpp -o build/fileiowrite.o


build/funcconnection.o: src/func/funcconnection.cpp
	@echo "Executing command = compile/make object[.o] file for [src/func/funcconnection.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/func/funcconnection.cpp -o build/funcconnection.o


build/funcdetermine.o: src/func/funcdetermine.cpp
	@echo "Executing command = compile/make object[.o] file for [src/func/funcdetermine.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/func/funcdetermine.cpp -o build/funcdetermine.o


build/funcglobal.o: src/func/funcglobal.cpp
	@echo "Executing command = compile/make object[.o] file for [src/func/funcglobal.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/func/funcglobal.cpp -o build/funcglobal.o


build/funclinear.o: src/func/funclinear.cpp
	@echo "Executing command = compile/make object[.o] file for [src/func/funclinear.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/func/funclinear.cpp -o build/funclinear.o


build/initconmode.o: src/initcon/initconmode.cpp
	@echo "Executing command = compile/make object[.o] file for [src/initcon/initconmode.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/initcon/initconmode.cpp -o build/initconmode.o


build/proginitcongen.o: src/prog/proginitcongen.cpp
	@echo "Executing command = compile/make object[.o] file for [src/prog/proginitcongen.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/prog/proginitcongen.cpp -o build/proginitcongen.o


build/progsimulate.o: src/prog/progsimulate.cpp
	@echo "Executing command = compile/make object[.o] file for [src/prog/progsimulate.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/prog/progsimulate.cpp -o build/progsimulate.o


build/utilityrandom.o: src/utility/utilityrandom.cpp
	@echo "Executing command = compile/make object[.o] file for [src/utility/utilityrandom.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/utility/utilityrandom.cpp -o build/utilityrandom.o
