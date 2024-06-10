clean:
	rm -rf ./doc/html/
	rm -rf ./doc/latex/

doc:
	doxygen
