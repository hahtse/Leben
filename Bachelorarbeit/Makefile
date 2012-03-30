# ------------------ Compilation options ------------------------
# Location of include files for Vista
INC = 


# Libraries to link with:
LIBS = -lviaio -lgsl -lgslcblas -lblas -lm


# Flags for the C compiler:
CFLAGS = -ansi -Wall -O3 -fopenmp
#CFLAGS = -ansi -Wall -g
CC = gcc
# --------------------- Code modules ----------------------------

# Name of final program module
PROG = vnmf

# List all source and object files
SRC = vnmf.c ALS.c NMF.c ECM.c Prob.c SymmetricNMF.c
OBJ = vnmf.o ALS.o NMF.o ECM.o Prob.o SymmetricNMF.o

# List header files
HDRS = 

# ------------------------ Rules --------------------------------

${PROG}: ${OBJ}
	${CC} -o ${PROG} ${CFLAGS} ${OBJ} ${LIBS}

%.o:%.c
	${CC} -c ${CFLAGS} ${INC} -c $< -o $@

clean:
	-rm -f *.o *~
