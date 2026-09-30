compiler = g++
srcdir = src
target = ${srcdir}/main.cpp ${srcdir}/terrain.cpp
outdir = build
output = bin
dependencies = -lraylib -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon -lGL -lm -lpthread -ldl -lrt

${output}: ${target}
	mkdir -p ${outdir}
	${compiler} ${target} -o ${outdir}/${output} ${dependencies}

test: ${output}
	${outdir}/${output}