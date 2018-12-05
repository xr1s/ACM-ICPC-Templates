Index.pdf: Index.tex DP DS
	xelatex Index.tex

.PHONY: DP DS M
DP: $(shell find DP)
DS: $(shell find DS)
M: $(shell find M)

clean:
	-rm *.aux *.toc *.log *.pdf
