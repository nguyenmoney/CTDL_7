#ifndef CSV_HANDLER_H
#define CSV_HANDLER_H

#include <stdio.h> 

// --- Khai bao cac ham tien ich chung ---

FILE* moFileAnToan(const char* tenFile, const char* cheDo);

void dongFileAnToan(FILE* f);

void xoaKyTuXuongDong(char* str);

char* docDongAnToan(char* buffer, int kichThuocBuffer, FILE* stream);

int demSoDongTrongFile(const char* tenFile, int boQuaDongTieuDe);

void rutGonChuoiHienThi(char* outputBuffer, int bufferSize, const char* originalString, int displayWidth);
#endif // CSV_HANDLER_H