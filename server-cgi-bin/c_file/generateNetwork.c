#include <stdio.h>
#include <string.h>
#include "se.h"
#include "mds.h"
#include "ce.h"
#include "kk.h"

const char *commonNodeColor[25] = {
	"#E60012", "#EB6100", "#F39800", "#FCC800", "#FFF100", "#CFDB00",
	"#8FC31F", "#22AC38", "#009944", "#009B6B", "#009E96", "#00A0C1",
	"#00A0E9", "#0086D1", "#0068B7", "#00479D", "#1D2088", "#601986",
	"#920783", "#BE0081", "#E4007F", "#E5006A", "#E5004F", "#E60033",
	"#000000"
};

void generateNetwork(
    const char **netArgs,
    const char *selectedNetwork
){
    if      (strcmp(selectedNetwork, "SE")  == 0) se(netArgs, commonNodeColor);
    else if (strcmp(selectedNetwork, "MDS") == 0) mds(netArgs, commonNodeColor);
    else if (strcmp(selectedNetwork, "CE")  == 0) ce(netArgs, commonNodeColor);
    else if (strcmp(selectedNetwork, "KK")  == 0) kk(netArgs, commonNodeColor);
    else                                          kk(netArgs, commonNodeColor);
}
