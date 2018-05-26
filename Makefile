#------------------------------------------------------------------------------
SOURCE=knock.cpp
MYPROGRAM=knock
CC=g++ -std=c++14
#------------------------------------------------------------------------------

all: $(MYPROGRAM)

$(MYPROGRAM): $(SOURCE)
	$(CC) $(SOURCE) -o $(MYPROGRAM)

clean:
	rm -f $(MYPROGRAM)
