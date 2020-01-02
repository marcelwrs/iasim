CPP = "g++"

CFLAGS += -g
#CFLAGS += -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common

LIBS += -lm -std=c++11

all: ia iasim

ia: ia.cpp
	$(CPP) $^ -o $@ $(LIBS) $(CFLAGS)

iasim: iasim.cpp
	$(CPP) $^ -o $@ $(LIBS) $(CFLAGS)

clean:
	rm -f iasim ia
