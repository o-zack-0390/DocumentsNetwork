#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void copyFile(const char* src_path, const char* dest_path, const char* add_line)
{
    // コピー元とコピー先が同じ
    if (strcmp(src_path, dest_path) == 0) {
        fprintf(stderr, "Common file error\n");
        return;
    }

    int result = 1;

    FILE* fp_add = fopen(dest_path, "w");
    if (fp_add == NULL) {
        fprintf(stderr, "Failed to open destination file\n");
        return;
    }
    fprintf(fp_add, "%s\n", add_line); // 指定された文字を先頭に記載
    fclose(fp_add);

    // テキストモードで開く
    FILE* fp_src = fopen(src_path, "r");
    FILE* fp_dest = fopen(dest_path, "a");
    if (fp_src == NULL || fp_dest == NULL) {
        result = 0;
    }

    if (result != 0) {
        for (;;) {
            int c;

            // 1文字読み込む
            c = fgetc(fp_src);
            if (c == EOF) {
                if (feof(fp_src)) {
                    break;
                } else {
                    result = 0;
                    break;
                }
            }

            // 1文字書き込む
            if (fputc(c, fp_dest) == EOF) {
                result = 0;
                break;
            }
        }
    }

    if (fp_dest != NULL) {
        if (fclose(fp_dest) == EOF) {
            result = 0;
        }
    }
    if (fp_src != NULL) {
        if (fclose(fp_src) == EOF) {
            result = 0;
        }
    }

    if (!result) {
        fprintf(stderr, "Copy file error\n");
        return;
    }
}
