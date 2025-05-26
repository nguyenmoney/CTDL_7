#include "csv_handler.h"
#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 
// Ham mo file an toan
FILE* moFileAnToan(const char* tenFile, const char* cheDo) {
    FILE* f = fopen(tenFile, cheDo);
    if (f == NULL) {
        fprintf(stderr, "LOI: Khong the mo file '%s' voi che do '%s'.\n", tenFile, cheDo);
        perror("Nguyen nhan chi tiet (fopen)");
    }
    return f;
}

// Ham dong file an toan
void dongFileAnToan(FILE* f) {
    if (f != NULL) {
        if (fclose(f) == EOF) {
            perror("Loi khi dong file");
        }
    }
}

//Ham xoa ky tu xuong dong
void xoaKyTuXuongDong(char* str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }
}

//Ham doc an toan
char* docDongAnToan(char* buffer, int kichThuocBuffer, FILE* stream) {
    if (buffer == NULL || kichThuocBuffer <= 0 || stream == NULL) {
        if (buffer != NULL && kichThuocBuffer > 0) {
            buffer[0] = '\0';
        }
        return NULL;
    }

    if (fgets(buffer, kichThuocBuffer, stream) != NULL) {
        xoaKyTuXuongDong(buffer);
        return buffer;
    }
    else {
        buffer[0] = '\0';
        if (ferror(stream)) {
        }
        return NULL;
    }
}

//Ham dem so dong
int demSoDongTrongFile(const char* tenFile, int boQuaDongTieuDe) {
    FILE* f = moFileAnToan(tenFile, "r");
    if (f == NULL) {
        return -1;
    }

    int soDong = 0;
    char line_buffer[2048];

    if (boQuaDongTieuDe) {
        if (docDongAnToan(line_buffer, sizeof(line_buffer), f) == NULL) {
            if (ferror(f)) {
                fprintf(stderr, "Loi doc dong tieu de file '%s' khi dem so dong.\n", tenFile);
                dongFileAnToan(f);
                return -1;
            }
            dongFileAnToan(f);
            return 0;
        }
    }

    while (docDongAnToan(line_buffer, sizeof(line_buffer), f) != NULL) {
        if (strlen(line_buffer) > 0) {
            soDong++;
        }
    }

    if (ferror(f)) {
        fprintf(stderr, "Loi doc file '%s' trong qua trinh dem so dong.\n", tenFile);
        dongFileAnToan(f);
        return -1;
    }

    dongFileAnToan(f);
    return soDong;
}

//Ham rut gon chuoi
void rutGonChuoiHienThi(char* outputBuffer, int bufferSize, const char* originalString, int displayWidth) {
    if (outputBuffer == NULL || bufferSize <= 0 || originalString == NULL || displayWidth <= 0) {
        if (outputBuffer != NULL && bufferSize > 0) {
            outputBuffer[0] = '\0';
        }
        return;
    }

    size_t originalLen = strlen(originalString);

    if (originalLen > (size_t)displayWidth) {
        if (displayWidth < 3) { 
            int lenToCopy = (bufferSize - 1 < displayWidth) ? (bufferSize - 1) : displayWidth;
            if (lenToCopy < 0) lenToCopy = 0;

            strncpy(outputBuffer, originalString, lenToCopy);
            outputBuffer[lenToCopy] = '\0';
        }
        else {

            if (bufferSize >= displayWidth + 1) {
                // Sao chep displayWidth - 3 ky tu dau
                // snprintf se tu dong them null terminator
                // Phai ep kieu (size_t) cho displayWidth - 3 + 1 de tranh warning voi snprintf
                snprintf(outputBuffer, (size_t)displayWidth - 3 + 1, "%s", originalString);
                strcat(outputBuffer, "...");
            }
            else if (bufferSize > 0) { // Neu buffer khong du lon cho ca "...", cat bot toi da co the
                strncpy(outputBuffer, originalString, bufferSize - 1);
                outputBuffer[bufferSize - 1] = '\0';
            }
        }
    }
    else {
        if (bufferSize > originalLen) {
            strcpy(outputBuffer, originalString);
        }
        else if (bufferSize > 0) {
            strncpy(outputBuffer, originalString, bufferSize - 1);
            outputBuffer[bufferSize - 1] = '\0';
        }
    }
}