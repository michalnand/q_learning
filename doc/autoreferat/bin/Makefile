# set latexfile to the name of the main file without the .tex
latexfile = book

all :
	rm bin/$(latexfile).pdf -rf
	pdflatex -shell-escape $(latexfile).tex
