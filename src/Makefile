CXX=g++
CXXFLAGS=-std=c++11 -march=native -Ofast -Wall -Wextra -pedantic
LINKAGE=-lsfml-system -lsfml-graphics -lsfml-window -lGL $(shell pkg-config --cflags --libs glew)

SRC=main.cc julia.cc shader.cc
BINARY=julia

.PHONY: julia

all: julia

julia:
	${CXX} ${CXXFLAGS} -o ${BINARY} ${SRC} ${LINKAGE}
