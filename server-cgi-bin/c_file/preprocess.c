#include <stdio.h>
#include "mklbl.h"
#include "nnsk5.h"

void	preprocess(
	const char **mklblArgs,
	const char **nnsk5Args
){
	fprintf(stderr, "%s\n", nnsk5Args[5]);
	mklbl(mklblArgs);
	nnsk5(nnsk5Args);
}
