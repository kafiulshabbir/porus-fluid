dsrc = src/
dbuild = build/
drun = run/

stest = test
smns = mns
sdiamension = diamension
sfileread = fileread
sprintdata = printdata
sprog = test

dtest = prog/${stest}
dmns = dst/${smns}
ddiamension = dst/${sdiamension}
dfileread = fileio/${sfileread}
dprintdata = fileio/${sprintdata}
dprog = ${drun}${sprog}

dsources = ${dtest} ${dmns} ${ddiamension} ${dfileread} ${dprintdata}
dsrccpp = $(addprefix ${dsrc}, ${dsources})
dsrccpp := $(addsuffix .cpp, ${dsrccpp})

all:
	g++ ${dsrccpp} -Iinclude -o ${dprog}
