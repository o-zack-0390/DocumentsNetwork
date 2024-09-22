#include <stdio.h>
#include <string.h>
#include "mst.h"
#include "knn.h"
#include "hml.h"

void    generateGraph(
    const char **mstArgs,
    const char **knnArgs,
    const char **hmlArgs,
    const char *selectedGraph
){
    if (strcmp(selectedGraph, "MST") == 0) {
        mst(mstArgs);
    } 
    else if (strcmp(selectedGraph, "KNN") == 0) {
        knn(knnArgs);
    } 
    else if (strcmp(selectedGraph, "HML") == 0) {
        mst(mstArgs);
        hml(hmlArgs);
    } 
    else {
        knn(knnArgs);
    }
}
