SRCDIR = src
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
PROGRAM = piet-i

LIBPNG_CXXFLAGS = $(shell libpng-config --cppflags)
LIBPNG_LDFLAGS = $(shell libpng-config --ldflags)
LIBPNG_LIBS = $(shell libpng-config --libs)
CXX = clang++
CXXFLAGS = -Wall -Wextra -Weffc++ -std=c++11 $(LIBPNG_CXXFLAGS) $(LIBPNG_LDFLAGS) $(LIBPNG_LIBS)

all: $(PROGRAM)

$(PROGRAM): $(SRCS)
	$(CXX) -o $@ $(SRCS) $(CXXFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJS)
