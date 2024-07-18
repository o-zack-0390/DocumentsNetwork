#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "copyFile.h"
#include "mkwid.h"
#include "mklbl.h"
#include "nnsk5.h"
#include "knn.h"
#include "kk.h"
#define Dn "0001"
#define Dh "100"

// *json は {name: string, normalText: string, wakachiText: string} を保持する Json データ
void getRequestJson(cJSON *json, const char *fn1, const char *fn2, const char *fn3) {

    // JSONオブジェクトから各項目を取得する
    const char *name        = cJSON_GetObjectItem(json, "name")->valuestring;
    const char *normalText  = cJSON_GetObjectItem(json, "normalText")->valuestring;
    const char *wakachiText = cJSON_GetObjectItem(json, "wakachiText")->valuestring;

    // クエリ文書を追加した文書データセットを作成
    copyFile(fn1, "./data/copy_uid.txt", name);
    copyFile(fn2, "./data/copy_doc.txt", normalText);
    copyFile(fn3, "./data/copy_wakachi.txt", wakachiText);
}

void loadNetworkData(const char *fn1, const char *fn2, const char *fn3) {

    cJSON *root      = cJSON_CreateObject();
    cJSON *edges     = cJSON_AddArrayToObject(root, "edges");
    cJSON *nodes     = cJSON_AddArrayToObject(root, "nodes");
    cJSON *categories = cJSON_AddArrayToObject(root, "categories");

    int category, id, total, r;
    double x1, y1, x2, y2, val1, val2;
    char keyword[20], fileName[50], context[15000], color[8];

    // エッジの座標をロード
    FILE *fp = fopen(fn1, "r");
    if (!fp) {
        fprintf(stderr, "Unknown file = %s\n", fn1);
        return;
    }
    while (fscanf(fp, "%lf %lf %lf %lf", &x1, &y1, &x2, &y2) == 4) {
        cJSON *edge = cJSON_CreateObject();
        cJSON_AddNumberToObject(edge, "x1", x1);
        cJSON_AddNumberToObject(edge, "y1", y1);
        cJSON_AddNumberToObject(edge, "x2", x2);
        cJSON_AddNumberToObject(edge, "y2", y2);
        cJSON_AddItemToArray(edges, edge);
    }
    fclose(fp);

    // ノードの座標をロード
    fp = fopen(fn2, "r");
    if (!fp) {
        fprintf(stderr, "Unknown file = %s\n", fn2);
        return;
    }
    for (int i = 0; i < atoi(Dh); i++) {
        fscanf(fp, "%d %s %d %s %14999s %lf %lf %d %s", &category, keyword, &id, fileName, context, &val1, &val2, &r, color);
        cJSON *node = cJSON_CreateObject();
        cJSON_AddNumberToObject(node, "category", category);
        cJSON_AddStringToObject(node, "keyword", keyword);
        cJSON_AddNumberToObject(node, "id", id);
        cJSON_AddStringToObject(node, "fileName", fileName);
        cJSON_AddStringToObject(node, "title", context);
        cJSON_AddNumberToObject(node, "cx", val1);
        cJSON_AddNumberToObject(node, "cy", val2);
        cJSON_AddNumberToObject(node, "r", r);
        cJSON_AddStringToObject(node, "color", color);
        cJSON_AddItemToArray(nodes, node);
    }
    fclose(fp);

    // カテゴリーの座標をロード
    fp = fopen(fn3, "r");
    if (!fp) {
        fprintf(stderr, "Unknown file = %s\n", fn2);
        return;
    }
    while (fscanf(fp, "%s %d %s", keyword, &total, color) != EOF) {
        cJSON *category = cJSON_CreateObject();
        cJSON_AddStringToObject(category, "keyword", keyword);
        cJSON_AddNumberToObject(category, "total", total);
        cJSON_AddStringToObject(category, "color", color);
        cJSON_AddItemToArray(categories, category);
    }
    fclose(fp);

    char *jsonString = cJSON_Print(root);
    if (jsonString == NULL) {
        fprintf(stderr, "Failed to print JSON.\n");
        return;
    }
    printf("%s", jsonString);
    cJSON_Delete(root);
    free(jsonString);
}

void handlePreflight() {
    printf("Access-Control-Allow-Origin: *\r\n");
    printf("Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n");
    printf("Access-Control-Allow-Headers: Content-Type\r\n");
    printf("Access-Control-Max-Age: 86400\r\n");
    printf("Content-Type: text/plain\r\n\r\n");
}

void handleRequest() {

    printf("Access-Control-Allow-Origin: *\r\n");
    printf("Content-Type: application/json\r\n\r\n");

    char *input,*clen;
    long length;
    clen = (char*)getenv("CONTENT_LENGTH");
    if (!clen) {
        fprintf(stderr, "Missing CONTENT_LENGTH.\n");
        return;
    }
    length = atol(clen);
    input = (char*)malloc(length + 1);
    if (!input) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }
    if (fgets(input, length+1, stdin) == NULL) {
        fprintf(stderr, "Failed to read input.\n");
        free(input);
        return;
    }

    cJSON *json = cJSON_Parse(input);
    if (json == NULL) {
        fprintf(stderr, "Invalid JSON: %s", input);
        free(input);
        return;
    }

    // モジュールの引数
    char *mkwidArgs[] = {"./data/copy_wakachi.txt", "./data/wid.txt"};
    char *mklblArgs[] = {"./data/wid.txt", "./data/copy_wakachi.txt", "./data/lbl.txt"};
    char *nnsk5Args[] = {"./data/lbl.txt", "./data/copy_uid.txt", "./data/wid.txt", "./data/copy_doc.txt", Dn, Dh, "./result/uidk.txt", "./result/lblk.txt"};
    char *knnArgs  [] = {"./result/lblk.txt", "./result/knnk.txt"};
    char *kkArgs   [] = {"./result/knnk.txt", "./result/uidk.txt", "./result/kkedge.txt", "./result/kknode.txt", "./result/lblk.txt", "./result/kkcategory.txt"};

    // モジュールを実行
    getRequestJson(json, "./data/uid.txt", "./data/doc.txt", "./data/wakachi.txt");
    mklbl(mklblArgs);
    nnsk5(nnsk5Args);
    knn(knnArgs);
    kk(kkArgs);
    loadNetworkData("./result/kkedge.txt", "./result/kknode.txt", "./result/kkcategory.txt");

    cJSON_Delete(json);
    free(input);
}

int main() {
    const char *requestMethod = getenv("REQUEST_METHOD");
    if (requestMethod && strcmp(requestMethod, "OPTIONS") == 0) {
        // プリフライトリクエストの処理
        handlePreflight();
    } else {
        // 通常のリクエストの処理
        handleRequest();
    }
    return 0;
}
