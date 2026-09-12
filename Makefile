compiler = g++
srcdir = src
target = ${srcdir}/main.cpp ${srcdir}/terrain.cpp
outdir = build
output = bin
dependencies = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

${output}: ${target}
	gtags
	mkdir -p ${outdir}
	${compiler} ${target} -o ${outdir}/${output} ${dependencies}

test: ${output}
	${outdir}/${output}