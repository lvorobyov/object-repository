.PHONY: all
all: dao.puml

dao.puml: $(wildcard ../include/*.h)
	hpp2plantuml $(patsubst %,-i %,$^) -o $@

