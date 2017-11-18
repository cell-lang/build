cellc:
	rm -rf tmp/
	mkdir tmp
	bin/build-runtime-src-file.py ../runtime/src/ tmp/runtime-sources.cell tmp/runtime-sources-empty.cell
	bin/cellc project.txt
	mv generated.* tmp/
	cat tmp/generated.cpp | bin/ren-fns > tmp/cellc.cpp
	echo >> tmp/cellc.cpp
	echo >> tmp/cellc.cpp
	cat src/hacks.cpp >> tmp/cellc.cpp
	g++ -O3 -DNDEBUG tmp/cellc.cpp -o cellc

clean:
	@rm -rf tmp/