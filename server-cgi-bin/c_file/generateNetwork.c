#include <stdio.h>
#include <string.h>
#include "se.h"
#include "mds.h"
#include "ce.h"
#include "kk.h"

const char *commonNodeColor[64] = {
		"#FF0000", "#00FF00", "#0000FF", "#FFFF00", "#FF00FF", "#00FFFF", "#800000", "#008000", 
		"#000080", "#808000", "#800080", "#008080", "#C0C0C0", "#808080", "#FFA500", "#A52A2A", 
		"#8B4513", "#FFD700", "#DAA520", "#B8860B", "#CD853F", "#D2691E", "#FF6347", "#FF4500", 
		"#FF1493", "#FF69B4", "#FFB6C1", "#FF8C00", "#ADFF2F", "#7FFF00", "#32CD32", "#00FA9A", 
		"#00FF7F", "#7FFFD4", "#40E0D0", "#4682B4", "#5F9EA0", "#6495ED", "#1E90FF", "#4169E1", 
		"#0000CD", "#8A2BE2", "#9932CC", "#BA55D3", "#9370DB", "#4B0082", "#9400D3", "#FF00FF", 
		"#EE82EE", "#DDA0DD", "#BC8F8F", "#F08080", "#FA8072", "#E9967A", "#FFA07A", "#FF4500", 
		"#DC143C", "#B22222", "#8B0000", "#CD5C5C", "#E0FFFF", "#F5FFFA", "#F0FFF0", "#FAFAD2"
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
