CPP = "g++"

CFLAGS += -g
#CFLAGS += -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common

LIBS += -lm -std=c++11

all: iasim

iasim: iasim.cpp
	$(CPP) $^ -o $@ $(LIBS) $(CFLAGS)

clean:
	rm -f iasim
