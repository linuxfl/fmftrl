CXX = g++
INC = -I. -I../include
CFLAGS = -std=c++0x -fPIC -fvisibility=hidden -Wall -O2 -Wno-deprecated
LFLAGS = 
LIBDIR = ../lib
LIBS = 
BIN = ../bin
all:
	mkdir -p $(LIBDIR)
	mkdir -p $(BIN)
	$(CXX) -o $(BIN)/fmftrl train_FMFtrl.cpp FM_FTRL_machine.cpp str_util.cpp conf_util.cpp  $(INC) $(CFLAGS) $(LFLAGS) $(addprefix -L,$(LIBDIR)) $(addprefix -l,$(LIBS))
	$(CXX) -o $(BIN)/predict_fmftrl FM_FTRL_predict.cpp str_util.cpp conf_util.cpp FM_FTRL_machine.cpp $(INC) $(CFLAGS) $(LFLAGS) $(addprefix -L,$(LIBDIR)) $(addprefix -l,$(LIBS))
clean:
	rm -rf *.o *.dSYM $(BIN)/fmftrl
