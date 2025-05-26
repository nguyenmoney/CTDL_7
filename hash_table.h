#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BANG_BAM_DUNG_LUONG_MAC_DINH 101
#define BANG_BAM_NGUONG_TAI_TOI_DA 0.75
#define BANG_BAM_HE_SO_TANG_KICH_THUOC 2
#define BANG_BAM_DUNG_LUONG_MAC_DINH 101

// Cau truc cho mot nut trong danh sach lien ket tai moi bucket
typedef struct NutBam {
    char* khoa; 
    void* giaTri;
    struct NutBam* tiepTheo;
} NutBam;

// Cau truc cho bang bam
typedef struct BangBam {
    NutBam** cacBucket; 
    int dungLuong;     
    int soLuongPhanTu;  
} BangBam;

// --- Khai bao cac ham cho bang bam ---

BangBam* taoBangBam(int dungLuongKLT);

void giaiPhongBangBam(BangBam* bb);

unsigned int hamBamChuoi(const char* khoa, int dungLuongBang);

int chenVaoBangBam(BangBam* bb, const char* khoa, void* giaTri);

void* timKiemTrongBangBam(BangBam* bb, const char* khoa);

int xoaKhoiBangBam(BangBam* bb, const char* khoa);
int rehashBangBam(BangBam** bb_ptr);


#endif // HASH_TABLE_H