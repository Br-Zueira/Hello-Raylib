compiler = g++
srcdir = src
target = main.cpp
outdir = build
output = bin
dependencies = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

${output}: ${srcdir}/${target}
	gtags
	mkdir -p ${outdir}
	${compiler} ${srcdir}/${target} -o ${outdir}/${output} ${dependencies}

test: ${output}
	${outdir}/${output}