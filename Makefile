OPT:=

CXX = u++
CXXFLAGS = -g -multi -Wall -Wno-unused-label -MMD ${OPT}
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}

OBJECTS1 = main.o bank.o parent.o nameserver.o office.o plant.o student.o truck.o vending.o watcard.o config.o
EXEC1 = soda

#OBJECTS2 = q2printer.o q2reindeer.o q2elf.o q2santa.o q2workshop.o q2main.o
#EXEC2 = northpole

OBJECTS = ${OBJECTS1} #${OBJECTS2}
DEPENDS = ${OBJECTS:.o=.d}
EXECS = ${EXEC1} #${EXEC2}

#############################################################
.PHONY : all clean

all : ${EXECS}

${EXEC1} : ${OBJECTS1}
	${CXX} ${CXXFLAGS} $^ -o $@ 

#${EXEC2} : ${OBJECTS2}
#	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################
${OBJECTS} : ${MAKEFILE_NAME}

-include ${DEPENDS}

clean :
	rm -f *.d *.o ${EXECS}
