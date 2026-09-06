compiler = g++
target = main.cpp
outdir = build
output = bin
dependencies = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

${output}: ${target}
	mkdir -p ${outdir}
	${compiler} ${target} -o ${outdir}/${output} ${dependencies}

test: ${output}
	./${outdir}/${output}