#include <stdio.h>
#include "mst.h"
#include "knn.h"
#include "hml.h"
#include "kk.h"

void    generateNetwork(
    const char **mstArgs,
    const char **knnArgs,
    const char **hmlArgs,
    const char **kkArgs,
    const char selectedNetwork
){
    enum graphs {SE, MDS, CE, KK};
    switch(selectedNetwork) {
        case SE:
            kk(kkArgs);
            break;
        case MDS:
            kk(kkArgs);
            break;
        case CE:
            kk(kkArgs);
            break;
        case KK:
            kk(kkArgs);
            break;
        default:
            kk(kkArgs);
    }
}
