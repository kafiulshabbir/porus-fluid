

all: necessary_compile
	@echo " "
	@echo "Command executed = all/default make"


necessary_compile: run/initcongen run/simulate run/plot run/color run/test
	@echo " "
	@echo "Command executed = necessary_compile"
	rm -rf run/output_files/plots
	mkdir run/output_files/plots


force: clean necessary_compile
	@echo " "
	@echo "Command executed = force"


clean:
	@echo " "
	rm -f build/cmdioprint.o build/cmdioread.o build/drw.o build/dstdiamension.o build/dstmns.o build/fileioplot.o build/fileioread.o build/fileiowrite.o build/funcglobal.o build/funcpressure.o build/funcvelocity.o build/functimestep.o build/funcdetermine.o build/funcintegration.o build/funcmeasure.o build/mathlinear.o build/initconmode.o build/proginitcongen.o build/progsimulate.o build/progplot.o build/progcolor.o build/progtest.o build/utilityrandom.o
	@echo "Command executed = clean"


edit:
	@echo " "
	geany -i src/cmdio/cmdioprint.h src/cmdio/cmdioprint.cpp src/cmdio/cmdioread.h src/cmdio/cmdioread.cpp src/decl/declconst.h src/decl/declfilename.h src/decl/decltypedef.h src/drw/drw.h src/drw/drw.cpp src/dst/dstdiamension.h src/dst/dstdiamension.cpp src/dst/dstmns.h src/dst/dstmns.cpp src/fileio/fileioplot.h src/fileio/fileioplot.cpp src/fileio/fileioread.h src/fileio/fileioread.cpp src/fileio/fileiowrite.h src/fileio/fileiowrite.cpp src/func/funcglobal.h src/func/funcglobal.cpp src/func/funcpressure.h src/func/funcpressure.cpp src/func/funcvelocity.h src/func/funcvelocity.cpp src/func/functimestep.h src/func/functimestep.cpp src/func/funcdetermine.h src/func/funcdetermine.cpp src/func/funcintegration.h src/func/funcintegration.cpp src/func/funcmeasure.h src/func/funcmeasure.cpp src/math/mathlinear.h src/math/mathlinear.cpp src/initcon/initconmode.h src/initcon/initconmode.cpp src/prog/proginitcongen.cpp src/prog/progsimulate.cpp src/prog/progplot.cpp src/prog/progcolor.cpp src/prog/progtest.cpp src/utility/utilityrandom.h src/utility/utilityrandom.cpp
	@echo "Command executed = open all files to be edited on Geany"


gen:
	@echo " "
	@echo "Make the genmakefile which edites the makefile itself!"
	g++ -c -Isrc/ -Wall -std=c++17 src/make/all.cpp -o build/genmakefile_all.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/clean.cpp -o build/genmakefile_clean.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/edit.cpp -o build/genmakefile_edit.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/executables.cpp -o build/genmakefile_executables.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/file.cpp -o build/genmakefile_file.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/force.cpp -o build/genmakefile_force.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/gen.cpp -o build/genmakefile_gen.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/genmakefile.cpp -o build/genmakefile_genmakefile.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/necessary_compile.cpp -o build/genmakefile_necessary_compile.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/objects.cpp -o build/genmakefile_objects.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/print.cpp -o build/genmakefile_print.o
	g++ -c -Isrc/ -Wall -std=c++17 src/make/utility.cpp -o build/genmakefile_utility.o
	g++  build/genmakefile_all.o build/genmakefile_clean.o build/genmakefile_edit.o build/genmakefile_executables.o build/genmakefile_file.o build/genmakefile_force.o build/genmakefile_gen.o build/genmakefile_genmakefile.o build/genmakefile_necessary_compile.o build/genmakefile_objects.o build/genmakefile_print.o build/genmakefile_utility.o -o genmakefile


run/initcongen: build/cmdioprint.o build/cmdioread.o build/drw.o build/dstdiamension.o build/dstmns.o build/fileioplot.o build/fileioread.o build/fileiowrite.o build/funcglobal.o build/funcpressure.o build/funcvelocity.o build/functimestep.o build/funcdetermine.o build/funcintegration.o build/funcmeasure.o build/mathlinear.o build/initconmode.o build/utilityrandom.o build/proginitcongen.o
	@echo " "
	@echo "Executing command = link-compile of file [run/initcongen]"
	g++ build/cmdioprint.o build/cmdioread.o build/drw.o build/dstdiamension.o build/dstmns.o build/fileioplot.o build/fileioread.o build/fileiowrite.o build/funcglobal.o build/funcpressure.o build/funcvelocity.o build/functimestep.o build/funcdetermine.o build/funcintegration.o build/funcmeasure.o build/mathlinear.o build/initconmode.o build/utilityrandom.o build/proginitcongen.o -o run/initcongen


run/simulate: build/cmdioprint.o build/cmdioread.o build/drw.o build/dstdiamension.o build/dstmns.o build/fileioplot.o build/fileioread.o build/fileiowrite.o build/funcglobal.o build/funcpressure.o build/funcvelocity.o build/functimestep.o build/funcdetermine.o build/funcintegration.o build/funcmeasure.o build/mathlinear.o build/initconmode.o build/utilityrandom.o build/progsimulate.o
	@echo " "
	@echo "Executing command = link-compile of file [run/simulate]"
	g++ build/cmdioprint.o build/cmdioread.o build/drw.o build/dstdiamension.o build/dstmns.o build/fileioplot.o build/fileioread.o build/fileiowrite.o build/funcglobal.o build/funcpressure.o build/funcvelocity.o build/functimestep.o build/funcdetermine.o build/funcintegration.o build/funcmeasure.o build/mathlinear.o build/initconmode.o build/utilityrandom.o build/progsimulate.o -o run/simulate


run/plot: build/cmdioprint.o build/cmdioread.o build/drw.o build/dstdiamension.o build/dstmns.o build/fileioplot.o build/fileioread.o build/fileiowrite.o build/funcglobal.o build/funcpressure.o build/funcvelocity.o build/functimestep.o build/funcdetermine.o build/funcintegration.o build/funcmeasure.o build/mathlinear.o build/initconmode.o build/utilityrandom.o build/progplot.o
	@echo " "
	@echo "Executing command = link-compile of file [run/plot]"
	g++ build/cmdioprint.o build/cmdioread.o build/drw.o build/dstdiamension.o build/dstmns.o build/fileioplot.o build/fileioread.o build/fileiowrite.o build/funcglobal.o build/funcpressure.o build/funcvelocity.o build/functimestep.o build/funcdetermine.o build/funcintegration.o build/funcmeasure.o build/mathlinear.o build/initconmode.o build/utilityrandom.o build/progplot.o -o run/plot


run/color: build/cmdioprint.o build/cmdioread.o build/drw.o build/dstdiamension.o build/dstmns.o build/fileioplot.o build/fileioread.o build/fileiowrite.o build/funcglobal.o build/funcpressure.o build/funcvelocity.o build/functimestep.o build/funcdetermine.o build/funcintegration.o build/funcmeasure.o build/mathlinear.o build/initconmode.o build/utilityrandom.o build/progcolor.o
	@echo " "
	@echo "Executing command = link-compile of file [run/color]"
	g++ build/cmdioprint.o build/cmdioread.o build/drw.o build/dstdiamension.o build/dstmns.o build/fileioplot.o build/fileioread.o build/fileiowrite.o build/funcglobal.o build/funcpressure.o build/funcvelocity.o build/functimestep.o build/funcdetermine.o build/funcintegration.o build/funcmeasure.o build/mathlinear.o build/initconmode.o build/utilityrandom.o build/progcolor.o -o run/color


run/test: build/cmdioprint.o build/cmdioread.o build/drw.o build/dstdiamension.o build/dstmns.o build/fileioplot.o build/fileioread.o build/fileiowrite.o build/funcglobal.o build/funcpressure.o build/funcvelocity.o build/functimestep.o build/funcdetermine.o build/funcintegration.o build/funcmeasure.o build/mathlinear.o build/initconmode.o build/utilityrandom.o build/progtest.o
	@echo " "
	@echo "Executing command = link-compile of file [run/test]"
	g++ build/cmdioprint.o build/cmdioread.o build/drw.o build/dstdiamension.o build/dstmns.o build/fileioplot.o build/fileioread.o build/fileiowrite.o build/funcglobal.o build/funcpressure.o build/funcvelocity.o build/functimestep.o build/funcdetermine.o build/funcintegration.o build/funcmeasure.o build/mathlinear.o build/initconmode.o build/utilityrandom.o build/progtest.o -o run/test


build/cmdioprint.o: src/cmdio/cmdioprint.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/cmdio/cmdioprint.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/cmdio/cmdioprint.cpp -o build/cmdioprint.o


build/cmdioread.o: src/cmdio/cmdioread.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/cmdio/cmdioread.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/cmdio/cmdioread.cpp -o build/cmdioread.o


build/drw.o: src/drw/drw.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/drw/drw.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/drw/drw.cpp -o build/drw.o


build/dstdiamension.o: src/dst/dstdiamension.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/dst/dstdiamension.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/dst/dstdiamension.cpp -o build/dstdiamension.o


build/dstmns.o: src/dst/dstmns.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/dst/dstmns.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/dst/dstmns.cpp -o build/dstmns.o


build/fileioplot.o: src/fileio/fileioplot.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/fileio/fileioplot.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/fileio/fileioplot.cpp -o build/fileioplot.o


build/fileioread.o: src/fileio/fileioread.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/fileio/fileioread.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/fileio/fileioread.cpp -o build/fileioread.o


build/fileiowrite.o: src/fileio/fileiowrite.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/fileio/fileiowrite.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/fileio/fileiowrite.cpp -o build/fileiowrite.o


build/funcglobal.o: src/func/funcglobal.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/func/funcglobal.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/func/funcglobal.cpp -o build/funcglobal.o


build/funcpressure.o: src/func/funcpressure.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/func/funcpressure.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/func/funcpressure.cpp -o build/funcpressure.o


build/funcvelocity.o: src/func/funcvelocity.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/func/funcvelocity.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/func/funcvelocity.cpp -o build/funcvelocity.o


build/functimestep.o: src/func/functimestep.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/func/functimestep.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/func/functimestep.cpp -o build/functimestep.o


build/funcdetermine.o: src/func/funcdetermine.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/func/funcdetermine.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/func/funcdetermine.cpp -o build/funcdetermine.o


build/funcintegration.o: src/func/funcintegration.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/func/funcintegration.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/func/funcintegration.cpp -o build/funcintegration.o


build/funcmeasure.o: src/func/funcmeasure.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/func/funcmeasure.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/func/funcmeasure.cpp -o build/funcmeasure.o


build/mathlinear.o: src/math/mathlinear.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/math/mathlinear.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/math/mathlinear.cpp -o build/mathlinear.o


build/initconmode.o: src/initcon/initconmode.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/initcon/initconmode.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/initcon/initconmode.cpp -o build/initconmode.o


build/proginitcongen.o: src/prog/proginitcongen.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/prog/proginitcongen.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/prog/proginitcongen.cpp -o build/proginitcongen.o


build/progsimulate.o: src/prog/progsimulate.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/prog/progsimulate.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/prog/progsimulate.cpp -o build/progsimulate.o


build/progplot.o: src/prog/progplot.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/prog/progplot.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/prog/progplot.cpp -o build/progplot.o


build/progcolor.o: src/prog/progcolor.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/prog/progcolor.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/prog/progcolor.cpp -o build/progcolor.o


build/progtest.o: src/prog/progtest.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/prog/progtest.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/prog/progtest.cpp -o build/progtest.o


build/utilityrandom.o: src/utility/utilityrandom.cpp
	@echo " "
	@echo "Executing command = compile/make object[.o] file for [src/utility/utilityrandom.cpp]"
	g++ -c -Isrc/ -Wall -std=c++17 src/utility/utilityrandom.cpp -o build/utilityrandom.o
