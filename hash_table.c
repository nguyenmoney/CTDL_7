#include "hash_table.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

// --- Trien khai cac ham cho bang bam ---

//Ham tao bang bam
BangBam* taoBangBam(int dungLuongKLT) {
    if (dungLuongKLT <= 0) {
        dungLuongKLT = BANG_BAM_DUNG_LUONG_MAC_DINH;
    }

    BangBam* bb = (BangBam*)malloc(sizeof(BangBam));
    if (bb == NULL) {
        perror("Loi cap phat bo nho");
        return NULL;
    }

    bb->dungLuong = dungLuongKLT;
    bb->soLuongPhanTu = 0;
    bb->cacBucket = (NutBam**)calloc(bb->dungLuong, sizeof(NutBam*));
    if (bb->cacBucket == NULL) {
        perror("Loi cap phat bo nho");
        free(bb);
        return NULL;
    }
    return bb;
}

//Ham giai phong bang bam
void giaiPhongBangBam(BangBam* bb) {
    if (bb == NULL) {
        return;
    }

    for (int i = 0; i < bb->dungLuong; ++i) {
        NutBam* nut = bb->cacBucket[i];
        while (nut != NULL) {
            NutBam* tam = nut;
            nut = nut->tiepTheo;
            free(tam->khoa);
            free(tam);
        }
    }
    free(bb->cacBucket);
    free(bb);
}

//Ham bam chuoi
unsigned int hamBamChuoi(const char* khoa, int dungLuongBang) {
    unsigned long giaTriBam = 5381;
    int c;
    while ((c = *khoa++)) {
        giaTriBam = ((giaTriBam << 5) + giaTriBam) + c;
    }
    return giaTriBam % dungLuongBang;
}


//Ham tim kiem trong bang bam
void* timKiemTrongBangBam(BangBam* bb, const char* khoa) {
    if (bb == NULL || khoa == NULL) {
        return NULL;
    }

    unsigned int chiSo = hamBamChuoi(khoa, bb->dungLuong);
    NutBam* nut = bb->cacBucket[chiSo];

    while (nut != NULL) {
        if (strcmp(nut->khoa, khoa) == 0) {
            return nut->giaTri;
        }
        nut = nut->tiepTheo;
    }
    return NULL;
}

//Ham xoa khoi bang bam
int xoaKhoiBangBam(BangBam* bb, const char* khoa) {
    if (bb == NULL || khoa == NULL) {
        return 0;
    }

    unsigned int chiSo = hamBamChuoi(khoa, bb->dungLuong);
    NutBam* nut = bb->cacBucket[chiSo];
    NutBam* truoc = NULL;

    while (nut != NULL) {
        if (strcmp(nut->khoa, khoa) == 0) {
            if (truoc == NULL) {
                bb->cacBucket[chiSo] = nut->tiepTheo;
            }
            else {
                truoc->tiepTheo = nut->tiepTheo;
            }
            free(nut->khoa);
            free(nut);
            bb->soLuongPhanTu--;
            return 1;
        }
        truoc = nut;
        nut = nut->tiepTheo;
    }
    return 0;
}
int rehashBangBam(BangBam** bb_ptr) {
    if (bb_ptr == NULL || *bb_ptr == NULL) {
        fprintf(stderr, "Con tro khong hop le.\n");
        return 0;
    }
    BangBam* bbCu = *bb_ptr;

    int kichThuocMoi = bbCu->dungLuong * BANG_BAM_HE_SO_TANG_KICH_THUOC;
    if (kichThuocMoi <= bbCu->dungLuong) {
        kichThuocMoi = bbCu->dungLuong + BANG_BAM_DUNG_LUONG_MAC_DINH;
    }

    BangBam* bbMoi = taoBangBam(kichThuocMoi);
    if (bbMoi == NULL) {
        fprintf(stderr, "Khong the tao bang bam moi voi kich thuoc %d.\n", kichThuocMoi);
        return 0;
    }

    printf("Thong bao: Thuc hien rehash. Kich thuoc cu: %d, Kich thuoc moi: %d, So luong phan tu: %d\n",
        bbCu->dungLuong, bbMoi->dungLuong, bbCu->soLuongPhanTu);

    for (int i = 0; i < bbCu->dungLuong; ++i) {
        NutBam* nutHienTai = bbCu->cacBucket[i];
        while (nutHienTai != NULL) {
            if (!chenVaoBangBam(bbMoi, nutHienTai->khoa, nutHienTai->giaTri)) {
                fprintf(stderr, "Khong the chen khoa '%s' vao bang bam moi.\n", nutHienTai->khoa);
                giaiPhongBangBam(bbMoi);
                return 0;
            }
            nutHienTai = nutHienTai->tiepTheo;
        }
    }

    giaiPhongBangBam(bbCu);

    *bb_ptr = bbMoi;
    printf("Rehash hoan tat. So luong phan tu trong bang moi: %d\n", (*bb_ptr)->soLuongPhanTu);
    return 1;
}


// Ham chen vao bang bam

int chenVaoBangBam(BangBam* bb, const char* khoa, void* giaTri) { 
    if (bb == NULL || khoa == NULL) {
        return 0;
    }

    unsigned int chiSo = hamBamChuoi(khoa, bb->dungLuong);
    NutBam* nutHienTai = bb->cacBucket[chiSo];

    while (nutHienTai != NULL) {
        if (strcmp(nutHienTai->khoa, khoa) == 0) {
            return 0;
        }
        nutHienTai = nutHienTai->tiepTheo;
    }

    NutBam* nutMoi = (NutBam*)malloc(sizeof(NutBam));
    if (nutMoi == NULL) {
        perror("Loi cap phat bo nho");
        return 0;
    }

    nutMoi->khoa = _strdup(khoa);
    if (nutMoi->khoa == NULL) {
        perror("Loi sao chep chuoi khoa");
        free(nutMoi);
        return 0;
    }
    nutMoi->giaTri = giaTri;
    nutMoi->tiepTheo = bb->cacBucket[chiSo];
    bb->cacBucket[chiSo] = nutMoi;
    bb->soLuongPhanTu++;

    return 1;
}