

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
	rm -rf app-simulation/build/
	mkdir app-simulation/build/
	@echo "Command executed = clean"


edit:
	@echo " "
	geany -i  app-simulation/src/cmdioprint.h app-simulation/src/cmdioprint.cpp app-simulation/src/cmdioread.h app-simulation/src/cmdioread.cpp app-simulation/src/declconst.h app-simulation/src/declfilename.h app-simulation/src/decltypedef.h app-simulation/src/drw.h app-simulation/src/drw.cpp app-simulation/src/dstdiamension.h app-simulation/src/dstdiamension.cpp app-simulation/src/dstmns.h app-simulation/src/dstmns.cpp app-simulation/src/fileioplot.h app-simulation/src/fileioplot.cpp app-simulation/src/fileioread.h app-simulation/src/fileioread.cpp app-simulation/src/fileiowrite.h app-simulation/src/fileiowrite.cpp app-simulation/src/funcglobal.h app-simulation/src/funcglobal.cpp app-simulation/src/funcpressure.h app-simulation/src/funcpressure.cpp app-simulation/src/funcvelocity.h app-simulation/src/funcvelocity.cpp app-simulation/src/functimestep.h app-simulation/src/functimestep.cpp app-simulation/src/funcdetermine.h app-simulation/src/funcdetermine.cpp app-simulation/src/funcintegration.h app-simulation/src/funcintegration.cpp app-simulation/src/funcmeasure.h app-simulation/src/funcmeasure.cpp app-simulation/src/mathlinear.h app-simulation/src/mathlinear.cpp app-simulation/src/initconmode.h app-simulation/src/initconmode.cpp app-simulation/src/proginitcongen.cpp app-simulation/src/progsimulate.cpp app-simulation/src/progplot.cpp app-simulation/src/progcolor.cpp app-simulation/src/progtest.cpp app-simulation/src/utilityrandom.h app-simulation/src/utilityrandom.cpp
	@echo "Command executed = open all files to be edited on Geany"


run/initcongen: app-simulation/build/cmdioprint.o app-simulation/build/cmdioread.o app-simulation/build/drw.o app-simulation/build/dstdiamension.o app-simulation/build/dstmns.o app-simulation/build/fileioplot.o app-simulation/build/fileioread.o app-simulation/build/fileiowrite.o app-simulation/build/funcglobal.o app-simulation/build/funcpressure.o app-simulation/build/funcvelocity.o app-simulation/build/functimestep.o app-simulation/build/funcdetermine.o app-simulation/build/funcintegration.o app-simulation/build/funcmeasure.o app-simulation/build/mathlinear.o app-simulation/build/initconmode.o app-simulation/build/utilityrandom.o app-simulation/build/proginitcongen.o
	@echo " "
	@echo "Executing command = link-compile of file [run/initcongen]"
	g++ app-simulation/build/cmdioprint.o app-simulation/build/cmdioread.o app-simulation/build/drw.o app-simulation/build/dstdiamension.o app-simulation/build/dstmns.o app-simulation/build/fileioplot.o app-simulation/build/fileioread.o app-simulation/build/fileiowrite.o app-simulation/build/funcglobal.o app-simulation/build/funcpressure.o app-simulation/build/funcvelocity.o app-simulation/build/functimestep.o app-simulation/build/funcdetermine.o app-simulation/build/funcintegration.o app-simulation/build/funcmeasure.o app-simulation/build/mathlinear.o app-simulation/build/initconmode.o app-simulation/build/utilityrandom.o app-simulation/build/proginitcongen.o -o run/initcongen


run/simulate: app-simulation/build/cmdioprint.o app-simulation/build/cmdioread.o app-simulation/build/drw.o app-simulation/build/dstdiamension.o app-simulation/build/dstmns.o app-simulation/build/fileioplot.o app-simulation/build/fileioread.o app-simulation/build/fileiowrite.o app-simulation/build/funcglobal.o app-simulation/build/funcpressure.o app-simulation/build/funcvelocity.o app-simulation/build/functimestep.o app-simulation/build/funcdetermine.o app-simulation/build/funcintegration.o app-simulation/build/funcmeasure.o app-simulation/build/mathlinear.o app-simulation/build/initconmode.o app-simulation/build/utilityrandom.o app-simulation/build/progsimulate.o
	@echo " "
	@echo "Executing command = link-compile of file [run/simulate]"
	g++ app-simulation/build/cmdioprint.o app-simulation/build/cmdioread.o app-simulation/build/drw.o app-simulation/build/dstdiamension.o app-simulation/build/dstmns.o app-simulation/build/fileioplot.o app-simulation/build/fileioread.o app-simulation/build/fileiowrite.o app-simulation/build/funcglobal.o app-simulation/build/funcpressure.o app-simulation/build/funcvelocity.o app-simulation/build/functimestep.o app-simulation/build/funcdetermine.o app-simulation/build/funcintegration.o app-simulation/build/funcmeasure.o app-simulation/build/mathlinear.o app-simulation/build/initconmode.o app-simulation/build/utilityrandom.o app-simulation/build/progsimulate.o -o run/simulate


run/plot: app-simulation/build/cmdioprint.o app-simulation/build/cmdioread.o app-simulation/build/drw.o app-simulation/build/dstdiamension.o app-simulation/build/dstmns.o app-simulation/build/fileioplot.o app-simulation/build/fileioread.o app-simulation/build/fileiowrite.o app-simulation/build/funcglobal.o app-simulation/build/funcpressure.o app-simulation/build/funcvelocity.o app-simulation/build/functimestep.o app-simulation/build/funcdetermine.o app-simulation/build/funcintegration.o app-simulation/build/funcmeasure.o app-simulation/build/mathlinear.o app-simulation/build/initconmode.o app-simulation/build/utilityrandom.o app-simulation/build/progplot.o
	@echo " "
	@echo "Executing command = link-compile of file [run/plot]"
	g++ app-simulation/build/cmdioprint.o app-simulation/build/cmdioread.o app-simulation/build/drw.o app-simulation/build/dstdiamension.o app-simulation/build/dstmns.o app-simulation/build/fileioplot.o app-simulation/build/fileioread.o app-simulation/build/fileiowrite.o app-simulation/build/funcglobal.o app-simulation/build/funcpressure.o app-simulation/build/funcvelocity.o app-simulation/build/functimestep.o app-simulation/build/funcdetermine.o app-simulation/build/funcintegration.o app-simulation/build/funcmeasure.o app-simulation/build/mathlinear.o app-simulation/build/initconmode.o app-simulation/build/utilityrandom.o app-simulation/build/progplot.o -o run/plot


run/color: app-simulation/build/cmdioprint.o app-simulation/build/cmdioread.o app-simulation/build/drw.o app-simulation/build/dstdiamension.o app-simulation/build/dstmns.o app-simulation/build/fileioplot.o app-simulation/build/fileioread.o app-simulation/build/fileiowrite.o app-simulation/build/funcglobal.o app-simulation/build/funcpressure.o app-simulation/build/funcvelocity.o app-simulation/build/functimestep.o app-simulation/build/funcdetermine.o app-simulation/build/funcintegration.o app-simulation/build/funcmeasure.o app-simulation/build/mathlinear.o app-simulation/build/initconmode.o app-simulation/build/utilityrandom.o app-simulation/build/progcolor.o
	@echo " "
	@echo "Executing command = link-compile of file [run/color]"
	g++ app-simulation/build/cmdioprint.o app-simulation/build/cmdioread.o app-simulation/build/drw.o app-simulation/build/dstdiamension.o app-simulation/build/dstmns.o app-simulation/build/fileioplot.o app-simulation/build/fileioread.o app-simulation/build/fileiowrite.o app-simulation/build/funcglobal.o app-simulation/build/funcpressure.o app-simulation/build/funcvelocity.o app-simulation/build/functimestep.o app-simulation/build/funcdetermine.o app-simulation/build/funcintegration.o app-simulation/build/funcmeasure.o app-simulation/build/mathlinear.o app-simulation/build/initconmode.o app-simulation/build/utilityrandom.o app-simulation/build/progcolor.o -o run/color


run/test: app-simulation/build/cmdioprint.o app-simulation/build/cmdioread.o app-simulation/build/drw.o app-simulation/build/dstdiamension.o app-simulation/build/dstmns.o app-simulation/build/fileioplot.o app-simulation/build/fileioread.o app-simulation/build/fileiowrite.o app-simulation/build/funcglobal.o app-simulation/build/funcpressure.o app-simulation/build/funcvelocity.o app-simulation/build/functimestep.o app-simulation/build/funcdetermine.o app-simulation/build/funcintegration.o app-simulation/build/funcmeasure.o app-simulation/build/mathlinear.o app-simulation/build/initconmode.o app-simulation/build/utilityrandom.o app-simulation/build/progtest.o
	@echo " "
	@echo "Executing command = link-compile of file [run/test]"
	g++ app-simulation/build/cmdioprint.o app-simulation/build/cmdioread.o app-simulation/build/drw.o app-simulation/build/dstdiamension.o app-simulation/build/dstmns.o app-simulation/build/fileioplot.o app-simulation/build/fileioread.o app-simulation/build/fileiowrite.o app-simulation/build/funcglobal.o app-simulation/build/funcpressure.o app-simulation/build/funcvelocity.o app-simulation/build/functimestep.o app-simulation/build/funcdetermine.o app-simulation/build/funcintegration.o app-simulation/build/funcmeasure.o app-simulation/build/mathlinear.o app-simulation/build/initconmode.o app-simulation/build/utilityrandom.o app-simulation/build/progtest.o -o run/test


app-simulation/build/cmdioprint.o: app-simulation/src/cmdioprint.h app-simulation/src/cmdioprint.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/cmdioprint.o] from [app-simulation/src/cmdioprint.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/cmdioprint.cpp -o app-simulation/build/cmdioprint.o


app-simulation/build/cmdioread.o: app-simulation/src/cmdioread.h app-simulation/src/cmdioread.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/cmdioread.o] from [app-simulation/src/cmdioread.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/cmdioread.cpp -o app-simulation/build/cmdioread.o


app-simulation/build/drw.o: app-simulation/src/drw.h app-simulation/src/drw.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/drw.o] from [app-simulation/src/drw.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/drw.cpp -o app-simulation/build/drw.o


app-simulation/build/dstdiamension.o: app-simulation/src/dstdiamension.h app-simulation/src/dstdiamension.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/dstdiamension.o] from [app-simulation/src/dstdiamension.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/dstdiamension.cpp -o app-simulation/build/dstdiamension.o


app-simulation/build/dstmns.o: app-simulation/src/dstmns.h app-simulation/src/dstmns.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/dstmns.o] from [app-simulation/src/dstmns.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/dstmns.cpp -o app-simulation/build/dstmns.o


app-simulation/build/fileioplot.o: app-simulation/src/fileioplot.h app-simulation/src/fileioplot.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/fileioplot.o] from [app-simulation/src/fileioplot.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/fileioplot.cpp -o app-simulation/build/fileioplot.o


app-simulation/build/fileioread.o: app-simulation/src/fileioread.h app-simulation/src/fileioread.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/fileioread.o] from [app-simulation/src/fileioread.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/fileioread.cpp -o app-simulation/build/fileioread.o


app-simulation/build/fileiowrite.o: app-simulation/src/fileiowrite.h app-simulation/src/fileiowrite.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/fileiowrite.o] from [app-simulation/src/fileiowrite.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/fileiowrite.cpp -o app-simulation/build/fileiowrite.o


app-simulation/build/funcglobal.o: app-simulation/src/funcglobal.h app-simulation/src/funcglobal.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/funcglobal.o] from [app-simulation/src/funcglobal.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/funcglobal.cpp -o app-simulation/build/funcglobal.o


app-simulation/build/funcpressure.o: app-simulation/src/funcpressure.h app-simulation/src/funcpressure.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/funcpressure.o] from [app-simulation/src/funcpressure.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/funcpressure.cpp -o app-simulation/build/funcpressure.o


app-simulation/build/funcvelocity.o: app-simulation/src/funcvelocity.h app-simulation/src/funcvelocity.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/funcvelocity.o] from [app-simulation/src/funcvelocity.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/funcvelocity.cpp -o app-simulation/build/funcvelocity.o


app-simulation/build/functimestep.o: app-simulation/src/functimestep.h app-simulation/src/functimestep.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/functimestep.o] from [app-simulation/src/functimestep.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/functimestep.cpp -o app-simulation/build/functimestep.o


app-simulation/build/funcdetermine.o: app-simulation/src/funcdetermine.h app-simulation/src/funcdetermine.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/funcdetermine.o] from [app-simulation/src/funcdetermine.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/funcdetermine.cpp -o app-simulation/build/funcdetermine.o


app-simulation/build/funcintegration.o: app-simulation/src/funcintegration.h app-simulation/src/funcintegration.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/funcintegration.o] from [app-simulation/src/funcintegration.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/funcintegration.cpp -o app-simulation/build/funcintegration.o


app-simulation/build/funcmeasure.o: app-simulation/src/funcmeasure.h app-simulation/src/funcmeasure.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/funcmeasure.o] from [app-simulation/src/funcmeasure.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/funcmeasure.cpp -o app-simulation/build/funcmeasure.o


app-simulation/build/mathlinear.o: app-simulation/src/mathlinear.h app-simulation/src/mathlinear.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/mathlinear.o] from [app-simulation/src/mathlinear.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/mathlinear.cpp -o app-simulation/build/mathlinear.o


app-simulation/build/initconmode.o: app-simulation/src/initconmode.h app-simulation/src/initconmode.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/initconmode.o] from [app-simulation/src/initconmode.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/initconmode.cpp -o app-simulation/build/initconmode.o


app-simulation/build/proginitcongen.o: app-simulation/src/proginitcongen.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/proginitcongen.o] from [app-simulation/src/proginitcongen.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/proginitcongen.cpp -o app-simulation/build/proginitcongen.o


app-simulation/build/progsimulate.o: app-simulation/src/progsimulate.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/progsimulate.o] from [app-simulation/src/progsimulate.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/progsimulate.cpp -o app-simulation/build/progsimulate.o


app-simulation/build/progplot.o: app-simulation/src/progplot.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/progplot.o] from [app-simulation/src/progplot.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/progplot.cpp -o app-simulation/build/progplot.o


app-simulation/build/progcolor.o: app-simulation/src/progcolor.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/progcolor.o] from [app-simulation/src/progcolor.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/progcolor.cpp -o app-simulation/build/progcolor.o


app-simulation/build/progtest.o: app-simulation/src/progtest.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/progtest.o] from [app-simulation/src/progtest.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/progtest.cpp -o app-simulation/build/progtest.o


app-simulation/build/utilityrandom.o: app-simulation/src/utilityrandom.h app-simulation/src/utilityrandom.cpp app-simulation/src/declconst.h
	@echo " "
	@echo "Executing command = compiling to make [app-simulation/build/utilityrandom.o] from [app-simulation/src/utilityrandom.cpp]"
	g++ -c -Wall -std=c++17 -Iapp-simulation/src/ app-simulation/src/utilityrandom.cpp -o app-simulation/build/utilityrandom.o
