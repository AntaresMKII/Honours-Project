clean:
	rm -rf ./doc/html/
	rm -rf ./doc/latex/

documentation:
	doxygen ./Doxyfile
