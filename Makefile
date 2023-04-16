
all:

	# Making object files
	g++ -c -std=c++17 src/cmdio/cmdioprint.cpp -Isrc/ -o build/cmdioprint.o
	g++ -c -std=c++17 src/cmdio/cmdioread.cpp -Isrc/ -o build/cmdioread.o
	g++ -c -std=c++17 src/drw/drw.cpp -Isrc/ -o build/drw.o
	g++ -c -std=c++17 src/dst/dstdiamension.cpp -Isrc/ -o build/dstdiamension.o
	g++ -c -std=c++17 src/dst/dstmns.cpp -Isrc/ -o build/dstmns.o
	g++ -c -std=c++17 src/fileio/fileioplot.cpp -Isrc/ -o build/fileioplot.o
	g++ -c -std=c++17 src/fileio/fileioread.cpp -Isrc/ -o build/fileioread.o
	g++ -c -std=c++17 src/fileio/fileiowrite.cpp -Isrc/ -o build/fileiowrite.o
	g++ -c -std=c++17 src/func/funcconnection.cpp -Isrc/ -o build/funcconnection.o
	g++ -c -std=c++17 src/func/funcdetermine.cpp -Isrc/ -o build/funcdetermine.o
	g++ -c -std=c++17 src/func/funcglobal.cpp -Isrc/ -o build/funcglobal.o
	g++ -c -std=c++17 src/func/funclinear.cpp -Isrc/ -o build/funclinear.o
	g++ -c -std=c++17 src/initcon/initconmode.cpp -Isrc/ -o build/initconmode.o
	g++ -c -std=c++17 src/prog/proginitcongen.cpp -Isrc/ -o build/proginitcongen.o
	g++ -c -std=c++17 src/prog/progsimulate.cpp -Isrc/ -o build/progsimulate.o
	g++ -c -std=c++17 src/utility/utilityrandom.cpp -Isrc/ -o build/utilityrandom.o


	# Clear Plot Folder
	rm -rf run/output_files/plots
	mkdir run/output_files/plots

	# Create Executables

	 g++ build/proginitcongen.o build/cmdioprint.o build/cmdioread.o build/drw.o build/dstdiamension.o build/dstmns.o build/fileioplot.o build/fileioread.o build/fileiowrite.o build/funcconnection.o build/funcdetermine.o build/funcglobal.o build/funclinear.o build/initconmode.o build/utilityrandom.o -o run/proginitcongen

	 g++ build/progsimulate.o build/cmdioprint.o build/cmdioread.o build/drw.o build/dstdiamension.o build/dstmns.o build/fileioplot.o build/fileioread.o build/fileiowrite.o build/funcconnection.o build/funcdetermine.o build/funcglobal.o build/funclinear.o build/initconmode.o build/utilityrandom.o -o run/progsimulate

edit:

# Requesting Geany to open the following files to be edited
	geany src/cmdio/cmdioprint.cpp src/cmdio/cmdioprint.h src/cmdio/cmdioread.cpp src/cmdio/cmdioread.h src/decl/declconst.h src/decl/declfilename.h src/decl/decltypedef.h src/drw/drw.cpp src/drw/drw.h src/dst/dstdiamension.cpp src/dst/dstdiamension.h src/dst/dstmns.cpp src/dst/dstmns.h src/fileio/fileioplot.cpp src/fileio/fileioplot.h src/fileio/fileioread.cpp src/fileio/fileioread.h src/fileio/fileiowrite.cpp src/fileio/fileiowrite.h src/func/funcconnection.cpp src/func/funcconnection.h src/func/funcdetermine.cpp src/func/funcdetermine.h src/func/funcglobal.cpp src/func/funcglobal.h src/func/funclinear.cpp src/func/funclinear.h src/initcon/initconmode.cpp src/initcon/initconmode.h src/prog/proginitcongen.cpp src/prog/progsimulate.cpp src/utility/utilityrandom.cpp src/utility/utilityrandom.h 

gen:

# Make the genmakefile which edites the makefile itself!
	g++ src/make/genmakefile.cpp -Isrc/ -o genmakefile
