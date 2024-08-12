all:
	pdflatex --shell-escape report.tex
	biber report
	pdflatex --shell-escape report.tex
	pdflatex --shell-escape report.tex

clean:
	rm *.aux
	rm *.log
	rm *.lof
	rm *.toc
	rm *.bbl
	rm *.bcf
	rm *.blg
	rm *.run.xml
	rm *.loa 
	rm *.lot 
	rm svg-inkscape/*
	rmdir svg-inkscape
