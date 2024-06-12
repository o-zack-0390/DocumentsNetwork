#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

#define Dm 10

double MatW[Dm][2] = {
    {0, 0}, {10, 20}, {20, 40}, {30, 60}, {40, 80},
    {50, 100}, {60, 120}, {70, 140}, {80, 160}, {90, 180}
};
int NumV[Dm] = {2, 3, 2, 3, 2, 2, 3, 2, 3, 2};
int AdjM[Dm][Dm] = {{0}};
int VecC[Dm] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
char *MatF[Dm] = {"Node1", "Node2", "Node3", "Node4", "Node5", "Node6", "Node7", "Node8", "Node9", "Node10"};
char *name[10] = {"#00ff00", "#ff0000", "#0000ff", "#ffff00", "#00ffff", "#ff00ff", "#ff7700", "#ff7777", "#ff77ff", "#000000"};

void convertToJSONAndRespond() {
    cJSON *root = cJSON_CreateObject();
    cJSON *nodes = cJSON_AddArrayToObject(root, "nodes");

    for (int i = 0; i < Dm; i++) {
        cJSON *node = cJSON_CreateObject();
        cJSON_AddNumberToObject(node, "x", MatW[i][0]);
        cJSON_AddNumberToObject(node, "y", MatW[i][1]);
        cJSON_AddNumberToObject(node, "colorIndex", (VecC[i]-1)%10);
        cJSON_AddStringToObject(node, "color", name[(VecC[i]-1)%10]);
        cJSON_AddStringToObject(node, "label", MatF[i]);
        cJSON_AddItemToArray(nodes, node);
    }

    char *jsonString = cJSON_Print(root);
    if (jsonString == NULL) {
        fprintf(stderr, "Failed to print JSON.\n");
        return;
    }

    // HTTPレスポンスの送信
    printf("Content-Type: application/json\r\n\r\n");
    printf("%s", jsonString);

    // メモリ解放
    cJSON_Delete(root);
    free(jsonString);
}

int main() {
    convertToJSONAndRespond();
    return 0;
}
