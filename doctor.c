#include "doctor.h"
#include "department.h"
#include "csv_handler.h"
#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Ham tien ich noi bo đe xoa ky tu xuong dong
static void removeNewlineDoc(char* str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Ham doc danh sach bac si
BacSi* docDanhSachBacSi(const char* tenFile, int* soLuong) {
    FILE* f = fopen(tenFile, "r");
    if (f == NULL) {
        *soLuong = 0;
        return NULL;
    }

    BacSi* ds = NULL;
    int capacity = 0;
    *soLuong = 0;
    char line[500];
    char* token;

    while (fgets(line, sizeof(line), f) != NULL) {
        removeNewlineDoc(line);
        if (strlen(line) < 5) continue;

        if (*soLuong >= capacity) {
            capacity = (capacity == 0) ? 10 : capacity * 2;
            BacSi* temp = realloc(ds, capacity * sizeof(BacSi));
            if (temp == NULL) {
                perror("Loi cap phat bo nho (realloc docDanhSachBacSi)");
                free(ds);
                fclose(f);
                *soLuong = 0;
                return NULL;
            }
            ds = temp;
        }

        BacSi bs;
        token = strtok(line, ",");
        if (token) { strncpy(bs.maBacSi, token, sizeof(bs.maBacSi) - 1); bs.maBacSi[sizeof(bs.maBacSi) - 1] = '\0'; }
        else continue;

        token = strtok(NULL, ",");
        if (token) { strncpy(bs.tenBacSi, token, sizeof(bs.tenBacSi) - 1); bs.tenBacSi[sizeof(bs.tenBacSi) - 1] = '\0'; }
        else bs.tenBacSi[0] = '\0';

        token = strtok(NULL, ",");
        if (token) { strncpy(bs.chuyenKhoa, token, sizeof(bs.chuyenKhoa) - 1); bs.chuyenKhoa[sizeof(bs.chuyenKhoa) - 1] = '\0'; }
        else bs.chuyenKhoa[0] = '\0';

        token = strtok(NULL, ",");
        if (token) { strncpy(bs.soDienThoai, token, sizeof(bs.soDienThoai) - 1); bs.soDienThoai[sizeof(bs.soDienThoai) - 1] = '\0'; }
        else bs.soDienThoai[0] = '\0';

        token = strtok(NULL, ",");
        if (token) { strncpy(bs.email, token, sizeof(bs.email) - 1); bs.email[sizeof(bs.email) - 1] = '\0'; }
        else bs.email[0] = '\0';

        token = strtok(NULL, "\n");
        if (token) { strncpy(bs.maKhoaPhong, token, sizeof(bs.maKhoaPhong) - 1); bs.maKhoaPhong[sizeof(bs.maKhoaPhong) - 1] = '\0'; }
        else bs.maKhoaPhong[0] = '\0';

        ds[*soLuong] = bs;
        (*soLuong)++;
    }

    fclose(f);
    return ds;
}

// Ham ghi danh sach ra file
void ghiDanhSachBacSiRaFile(const char* tenFile, const BacSi dsBacSi[], int soLuong) {
    FILE* f = fopen(tenFile, "w");
    if (f == NULL) {
        perror("Loi mo file de ghi (ghiDanhSachBacSiRaFile)");
        return;
    }
    for (int i = 0; i < soLuong; ++i) {
        fprintf(f, "%s,%s,%s,%s,%s,%s\n",
            dsBacSi[i].maBacSi, dsBacSi[i].tenBacSi, dsBacSi[i].chuyenKhoa,
            dsBacSi[i].soDienThoai, dsBacSi[i].email, dsBacSi[i].maKhoaPhong);
    }
    fclose(f);
}

// --- Cac ham tim kiem noi bo theo cac thuoc tinh khong phai khoa chinh ---

//Ham tim bac si theo ten
static BacSi* timBacSiTheoTen(const BacSi dsBacSi[], int soLuong, const char* tenBacSi) {
    if (tenBacSi == NULL || strlen(tenBacSi) == 0) return NULL;
    char tenBacSiLower[sizeof(dsBacSi[0].tenBacSi)];
    strncpy(tenBacSiLower, tenBacSi, sizeof(tenBacSiLower) - 1);
    tenBacSiLower[sizeof(tenBacSiLower) - 1] = '\0';
    for (int k = 0; tenBacSiLower[k]; k++) tenBacSiLower[k] = tolower(tenBacSiLower[k]);

    for (int i = 0; i < soLuong; ++i) {
        char currentTenBSLower[sizeof(dsBacSi[i].tenBacSi)];
        strncpy(currentTenBSLower, dsBacSi[i].tenBacSi, sizeof(currentTenBSLower) - 1);
        currentTenBSLower[sizeof(currentTenBSLower) - 1] = '\0';
        for (int k = 0; currentTenBSLower[k]; k++) currentTenBSLower[k] = tolower(currentTenBSLower[k]);

        if (strcmp(currentTenBSLower, tenBacSiLower) == 0) {
            return (BacSi*)&dsBacSi[i];
        }
    }
    return NULL;
}

//Ham chuyen cac ki tu ve chu thuong
static void toLowerString(char* dest, const char* src, size_t dest_size) {
    if (src == NULL || dest == NULL || dest_size == 0) return;
    size_t i = 0;
    for (i = 0; src[i] && i < dest_size - 1; ++i) {
        dest[i] = tolower((unsigned char)src[i]);
    }
    dest[i] = '\0';
}

// Hàm kiem tra ten cua bac si
static int kiemTraTenBacSi(const char* tenDayDu, const char* tenTimKiemLower) {
    if (tenDayDu == NULL || tenTimKiemLower == NULL) return 0;
    char tenDayDuLower[sizeof(((BacSi*)0)->tenBacSi)];
    toLowerString(tenDayDuLower, tenDayDu, sizeof(tenDayDuLower));
    return (strstr(tenDayDuLower, tenTimKiemLower) != NULL);
}

// Hàm kiem tra chuyen khoa cua bac si
static int kiemTraChuyenKhoaBacSi(const char* chuyenKhoaBS, const char* chuyenKhoaTimKiemLower) {
    if (chuyenKhoaBS == NULL || chuyenKhoaTimKiemLower == NULL) return 0;
    char chuyenKhoaBSLower[sizeof(((BacSi*)0)->chuyenKhoa)];
    toLowerString(chuyenKhoaBSLower, chuyenKhoaBS, sizeof(chuyenKhoaBSLower));
    return (strstr(chuyenKhoaBSLower, chuyenKhoaTimKiemLower) != NULL);
}

// Hàm tim bac si theo SDT
BacSi* timBacSiTheoSDT(const BacSi dsBacSi[], int soLuong, const char* soDienThoai) { 
    if (soDienThoai == NULL || strlen(soDienThoai) == 0) return NULL;
    for (int i = 0; i < soLuong; ++i) { 
        if (strcmp(dsBacSi[i].soDienThoai, soDienThoai) == 0) {
            return (BacSi*)&dsBacSi[i];
        }
    }
    return NULL;
}

// Hàm tim bac si theo Email
BacSi* timBacSiTheoEmail(const BacSi dsBacSi[], int soLuong, const char* email) { 
    if (email == NULL || strlen(email) == 0) return NULL;
    char emailLower[sizeof(((BacSi*)0)->email)];
    toLowerString(emailLower, email, sizeof(emailLower));

    for (int i = 0; i < soLuong; ++i) {
        char currentEmailLower[sizeof(dsBacSi[i].email)];
        toLowerString(currentEmailLower, dsBacSi[i].email, sizeof(currentEmailLower));

        if (strcmp(currentEmailLower, emailLower) == 0) {
            return (BacSi*)&dsBacSi[i];
        }
    }
    return NULL;
}

//Ham tim bac si linear search
void timKiemBacSiTuyenTinh(const BacSi dsBacSi[], int soLuongBacSi,
    const char* tieuChi, const char* giaTriTimKiem,
    BacSi** ketQuaTimKiem, int* soLuongKetQua) {
    *ketQuaTimKiem = NULL;
    *soLuongKetQua = 0;
    if (soLuongBacSi == 0 || giaTriTimKiem == NULL || tieuChi == NULL || strlen(giaTriTimKiem) == 0) {
        return;
    }

    BacSi* dsKetQuaTamThoi = (BacSi*)malloc(soLuongBacSi * sizeof(BacSi));
    if (dsKetQuaTamThoi == NULL) {
        perror("Loi cap phat bo nho (timKiemBacSiTuyenTinh: dsKetQuaTamThoi)");
        return;
    }
    int count = 0;

    char tieuChiLower[50];
    toLowerString(tieuChiLower, tieuChi, sizeof(tieuChiLower));

    char giaTriTimKiemLower[sizeof(((BacSi*)0)->tenBacSi)];
    toLowerString(giaTriTimKiemLower, giaTriTimKiem, sizeof(giaTriTimKiemLower));

    for (int i = 0; i < soLuongBacSi; ++i) {
        int timThayTrongBacSiNay = 0;
        if (strcmp(tieuChiLower, "ten") == 0) {
            if (kiemTraTenBacSi(dsBacSi[i].tenBacSi, giaTriTimKiemLower)) {
                timThayTrongBacSiNay = 1;
            }
        }
        else if (strcmp(tieuChiLower, "chuyenkhoa") == 0) {
            if (kiemTraChuyenKhoaBacSi(dsBacSi[i].chuyenKhoa, giaTriTimKiemLower)) {
                timThayTrongBacSiNay = 1;
            }
        }
        else if (strcmp(tieuChiLower, "sodienthoai") == 0) {
            if (strcmp(dsBacSi[i].soDienThoai, giaTriTimKiem) == 0) {
                timThayTrongBacSiNay = 1;
            }
        }
        else if (strcmp(tieuChiLower, "email") == 0) {
            char currentEmailLower[sizeof(dsBacSi[i].email)];
            toLowerString(currentEmailLower, dsBacSi[i].email, sizeof(currentEmailLower));
            if (strcmp(currentEmailLower, giaTriTimKiemLower) == 0) {
                timThayTrongBacSiNay = 1;
            }
        }
        if (timThayTrongBacSiNay) {
            if (count < soLuongBacSi) {
                dsKetQuaTamThoi[count++] = dsBacSi[i];
            }
        }
    }

    if (count > 0) {
        *ketQuaTimKiem = (BacSi*)malloc(count * sizeof(BacSi));
        if (*ketQuaTimKiem == NULL) {
            perror("Loi cap phat bo nho (timKiemBacSiTuyenTinh: ketQuaTimKiem)");
            free(dsKetQuaTamThoi);
            *soLuongKetQua = 0;
            return;
        }
        memcpy(*ketQuaTimKiem, dsKetQuaTamThoi, count * sizeof(BacSi));
        *soLuongKetQua = count;
    }

    free(dsKetQuaTamThoi);
}

//Ham nhap thong tin bac si
int nhapThongTinBacSiTuBanPhim(BacSi* bs,
    BangBam* bbBacSiHienCo,
    const BacSi dsBacSiHienCo[], int soLuongBSHienCo,
    const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong) {
    char bufferMaBS[MAX_LEN_MA_BAC_SI];
    char bufferMaKP[MAX_LEN_MA_KHOA_PHONG];
    BacSi* bsTimThay;

    //Nhap ma bac si
    printf("Nhap Ma Bac Si (VD: BS001, toi da %zu ky tu, se duoc chuyen IN HOA): ", sizeof(bs->maBacSi) - 1);
    if (docDongAnToan(bufferMaBS, sizeof(bufferMaBS), stdin) == NULL || strlen(bufferMaBS) == 0) {
        printf("LOI: Ma Bac Si khong duoc de trong. Huy them bac si.\n");
        return 0;
    }
    for (int i = 0; bufferMaBS[i]; i++) {
        bufferMaBS[i] = toupper(bufferMaBS[i]);
    }

    if (bbBacSiHienCo != NULL) {
        if (timKiemTrongBangBam(bbBacSiHienCo, bufferMaBS) != NULL) {
            printf("LOI: Ma Bac Si '%s' da ton tai (kiem tra bang bam). Huy them bac si.\n", bufferMaBS);
            return 0;
        }
    }
    else {
        fprintf(stderr, "LOI HE THONG: Bang bam bac si khong kha dung. Khong the kiem tra trung lap MaBS. Huy them bac si.\n");
        return 0;
    }
    strncpy(bs->maBacSi, bufferMaBS, sizeof(bs->maBacSi) - 1);
    bs->maBacSi[sizeof(bs->maBacSi) - 1] = '\0';

    //Nhap ten bac si
    printf("Nhap Ten Bac Si (KHONG DUOC BO TRONG): ");
    if (docDongAnToan(bs->tenBacSi, sizeof(bs->tenBacSi), stdin) == NULL || strlen(bs->tenBacSi) == 0) {
        printf("LOI: Ten Bac Si khong duoc de trong. Huy them bac si.\n");
        return 0;
    }

    // Nhap chuyen khoa
    printf("Nhap Chuyen Khoa (KHONG DUOC BO TRONG): ");
    if (docDongAnToan(bs->chuyenKhoa, sizeof(bs->chuyenKhoa), stdin) == NULL || strlen(bs->chuyenKhoa) == 0) {
        printf("LOI: Chuyen khoa khong duoc de trong. Huy them bac si.\n");
        return 0;
    }

    //Nhap so dien thoai
    printf("Nhap So Dien Thoai (KHONG DUOC BO TRONG, chi chua so, KHONG TRUNG): ");
    if (docDongAnToan(bs->soDienThoai, sizeof(bs->soDienThoai), stdin) == NULL || strlen(bs->soDienThoai) == 0) {
        printf("LOI: So Dien Thoai khong duoc de trong. Huy them bac si.\n");
        return 0;
    }
    for (int i = 0; bs->soDienThoai[i]; ++i) {
        if (!isdigit(bs->soDienThoai[i])) {
            printf("LOI: So Dien Thoai chi duoc chua cac ky tu so. Huy them bac si.\n");
            return 0;
        }
    }
    bsTimThay = timBacSiTheoSDT(dsBacSiHienCo, soLuongBSHienCo, bs->soDienThoai);
    if (bsTimThay != NULL) {
        printf("LOI: So Dien Thoai '%s' da duoc su dung boi Bac Si MaBS: %s. Huy them bac si.\n", bs->soDienThoai, bsTimThay->maBacSi);
        return 0;
    }

    //Nhap Email
    printf("Nhap Email (vd: ten@domain.com): ");
    if (docDongAnToan(bs->email, sizeof(bs->email), stdin) == NULL || strlen(bs->email) == 0) {
        printf("LOI: Email khong duoc de trong. Huy them bac si.\n");
        return 0;
    }
    if (strchr(bs->email, '@') == NULL || strchr(bs->email, '.') == NULL) {
        printf("LOI: Dinh dang Email khong hop le. Huy them bac si.\n");
        return 0;
    }
    bsTimThay = timBacSiTheoEmail(dsBacSiHienCo, soLuongBSHienCo, bs->email);
    if (bsTimThay != NULL) {
        printf("LOI: Email '%s' da duoc su dung boi Bac Si MaBS: %s. Huy them bac si.\n", bs->email, bsTimThay->maBacSi);
        return 0;
    }

    //Nhap ma khoa phong
    printf("Nhap Ma Khoa Phong: ");
    if (docDongAnToan(bufferMaKP, sizeof(bufferMaKP), stdin) == NULL || strlen(bufferMaKP) == 0) {
        printf("Ma Khoa Phong khong duoc de trong. Huy them bac si.\n");
        return 0;
    }
    for (int i = 0; bufferMaKP[i]; i++) {
        bufferMaKP[i] = toupper(bufferMaKP[i]);
    }
    if (timKhoaPhongTheoMa(dsKhoaPhong, soLuongKhoaPhong, bufferMaKP) == NULL) {
        printf("Ma Khoa Phong '%s' khong ton tai! Huy them bac si.\n", bufferMaKP);
        return 0;
    }
    strncpy(bs->maKhoaPhong, bufferMaKP, sizeof(bs->maKhoaPhong) - 1);
    bs->maKhoaPhong[sizeof(bs->maKhoaPhong) - 1] = '\0';

    return 1;
}

// Ham them bac si
int themBacSi(BacSi** dsBacSiConTro, int* soLuongConTro,
    BangBam** bbBacSi_ptr,
    BacSi bacSiMoi, const char* tenFile) {

    BangBam* bbHienTai = *bbBacSi_ptr;

    if (bbHienTai != NULL) {
        if (timKiemTrongBangBam(bbHienTai, bacSiMoi.maBacSi) != NULL) {
            fprintf(stderr, "Ma Bac Si '%s' da ton tai trong bang bam ngay truoc khi them vao mang!\n", bacSiMoi.maBacSi);
            return 0;
        }
    }
    else {
        fprintf(stderr, "Bang bam bac si khong kha dung khi them bac si '%s'.\n", bacSiMoi.maBacSi);
        return 0;
    }

    BacSi* temp = realloc(*dsBacSiConTro, (*soLuongConTro + 1) * sizeof(BacSi));
    if (temp == NULL) {
        perror("Loi cap phat bo nho");
        return 0;
    }
    *dsBacSiConTro = temp;
    (*dsBacSiConTro)[*soLuongConTro] = bacSiMoi;
    BacSi* conTroToiBsTrongMang = &((*dsBacSiConTro)[*soLuongConTro]);

    if (!chenVaoBangBam(bbHienTai, bacSiMoi.maBacSi, conTroToiBsTrongMang)) {
        fprintf(stderr, "Khong the chen Bac Si '%s' vao bang bam.\n", bacSiMoi.maBacSi);
        return 0;
    }

    (*soLuongConTro)++;

    if ((*bbBacSi_ptr)->soLuongPhanTu / (double)(*bbBacSi_ptr)->dungLuong > BANG_BAM_NGUONG_TAI_TOI_DA) {
        printf("He so tai cua bang bam BAC SI vuot nguong (%d/%d). Dang chuan bi rehash...\n",
            (*bbBacSi_ptr)->soLuongPhanTu, (*bbBacSi_ptr)->dungLuong);
        if (!rehashBangBam(bbBacSi_ptr)) {
            fprintf(stderr, "Rehash bang bam BAC SI that bai. Chuong trinh co the tiep tuc nhung hieu suat se giam.\n");
        }
    }

    ghiDanhSachBacSiRaFile(tenFile, *dsBacSiConTro, *soLuongConTro);
    return 1;
}

//Ham sua thong tin bac si
int suaThongTinBacSi(BacSi dsBacSi[], int soLuong, const char* maBacSiCanSua, BacSi thongTinMoi,
    const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong,
    const char* tenFile) {
    int indexCanSua = -1;
    char maCanSuaUpper[MAX_LEN_MA_BAC_SI];
    strncpy(maCanSuaUpper, maBacSiCanSua, sizeof(maCanSuaUpper) - 1);
    maCanSuaUpper[sizeof(maCanSuaUpper) - 1] = '\0';
    for (int i = 0; maCanSuaUpper[i]; i++) maCanSuaUpper[i] = toupper(maCanSuaUpper[i]);

    for (int i = 0; i < soLuong; ++i) {
        if (strcmp(dsBacSi[i].maBacSi, maCanSuaUpper) == 0) {
            indexCanSua = i;
            break;
        }
    }

    if (indexCanSua == -1) {
        printf("Khong tim thay bac si voi ma '%s' de sua.\n", maBacSiCanSua);
        return 0;
    }

    BacSi* bsTimThay;

    //Sua ten bac si
    if (strlen(thongTinMoi.tenBacSi) == 0) {
        printf("Ten Bac Si khong duoc de trong.\n"); return 0;
    }

    //Sua so dien thoai
    if (strlen(thongTinMoi.soDienThoai) == 0) {
        printf("So Dien Thoai khong duoc de trong.\n"); return 0;
    }
    for (int i = 0; thongTinMoi.soDienThoai[i]; ++i) {
        if (!isdigit(thongTinMoi.soDienThoai[i])) {
            printf("So Dien Thoai moi chi duoc chua cac ky tu so.\n"); return 0;
        }
    }
    bsTimThay = timBacSiTheoSDT(dsBacSi, soLuong, thongTinMoi.soDienThoai);
    if (bsTimThay != NULL && strcmp(bsTimThay->maBacSi, maCanSuaUpper) != 0) {
        printf("Loi: So Dien Thoai '%s' da duoc su dung boi Bac Si MaBS: %s.\n", thongTinMoi.soDienThoai, bsTimThay->maBacSi);
        return 0;
    }

    //Sua email
    if (strlen(thongTinMoi.email) == 0) {
        printf("Email khong duoc de trong.\n"); return 0;
    }
    if (strchr(thongTinMoi.email, '@') == NULL || strchr(thongTinMoi.email, '.') == NULL) {
        printf("Dinh dang Email moi khong hop le.\n"); return 0;
    }
    bsTimThay = timBacSiTheoEmail(dsBacSi, soLuong, thongTinMoi.email);
    if (bsTimThay != NULL && strcmp(bsTimThay->maBacSi, maCanSuaUpper) != 0) {
        printf("Loi: Email '%s' da duoc su dung boi Bac Si MaBS: %s.\n", thongTinMoi.email, bsTimThay->maBacSi);
        return 0;
    }

    //Sua ma khoa phong
    char maKPMoiUpper[MAX_LEN_MA_KHOA_PHONG];
    if (strlen(thongTinMoi.maKhoaPhong) == 0) {
        printf("Ma Khoa Phong khong duoc de trong.\n"); return 0;
    }
    strncpy(maKPMoiUpper, thongTinMoi.maKhoaPhong, sizeof(maKPMoiUpper) - 1);
    maKPMoiUpper[sizeof(maKPMoiUpper) - 1] = '\0';
    for (int i = 0; maKPMoiUpper[i]; i++) maKPMoiUpper[i] = toupper(maKPMoiUpper[i]);

    if (timKhoaPhongTheoMa(dsKhoaPhong, soLuongKhoaPhong, maKPMoiUpper) == NULL) {
        printf("Loi: Ma Khoa Phong moi '%s' khong ton tai!\n", maKPMoiUpper);
        return 0;
    }
    strcpy(thongTinMoi.maKhoaPhong, maKPMoiUpper);

    strcpy(dsBacSi[indexCanSua].tenBacSi, thongTinMoi.tenBacSi);
    strcpy(dsBacSi[indexCanSua].chuyenKhoa, thongTinMoi.chuyenKhoa);
    strcpy(dsBacSi[indexCanSua].soDienThoai, thongTinMoi.soDienThoai);
    strcpy(dsBacSi[indexCanSua].email, thongTinMoi.email);
    strcpy(dsBacSi[indexCanSua].maKhoaPhong, thongTinMoi.maKhoaPhong);

    ghiDanhSachBacSiRaFile(tenFile, dsBacSi, soLuong);
    return 1;
}

//Ham xoa bac si
int xoaBacSi(BacSi** dsBacSiConTro, int* soLuongConTro,
    BangBam* bbBacSi,
    const char* maBacSiCanXoa, const char* tenFile) {
    char maXoaUpper[MAX_LEN_MA_BAC_SI];
    strncpy(maXoaUpper, maBacSiCanXoa, sizeof(maXoaUpper) - 1);
    maXoaUpper[sizeof(maXoaUpper) - 1] = '\0';
    for (int k = 0; maXoaUpper[k]; k++) maXoaUpper[k] = toupper(maXoaUpper[k]);

    int removedFromHashTable = 0;
    if (bbBacSi != NULL) {
        if (timKiemTrongBangBam(bbBacSi, maXoaUpper) != NULL) {
            if (xoaKhoiBangBam(bbBacSi, maXoaUpper)) {
                removedFromHashTable = 1;
            }
            else {
                fprintf(stderr, " Khong the xoa Bac Si '%s' mac du da tim thay.\n", maXoaUpper);
                return 0;
            }
        }
    }
    else {
        fprintf(stderr, "Bac si khong kha dung khi xoa.\n");
        return 0;
    }

    int viTriCanXoa = -1;
    for (int i = 0; i < *soLuongConTro; ++i) {
        if (strcmp((*dsBacSiConTro)[i].maBacSi, maXoaUpper) == 0) {
            viTriCanXoa = i;
            break;
        }
    }

    if (viTriCanXoa != -1) {
        if (!removedFromHashTable && timKiemTrongBangBam(bbBacSi, maXoaUpper) == NULL) {
            fprintf(stderr, "Bac Si '%s' tim thay trong mang nhung khong co trong bang bam truoc khi xoa khoi mang.\n", maXoaUpper);
        }

        for (int i = viTriCanXoa; i < (*soLuongConTro - 1); ++i) {
            (*dsBacSiConTro)[i] = (*dsBacSiConTro)[i + 1];
        }
        (*soLuongConTro)--;

        if (*soLuongConTro == 0) {
            free(*dsBacSiConTro);
            *dsBacSiConTro = NULL;
        }
        else {
            BacSi* temp = realloc(*dsBacSiConTro, (*soLuongConTro) * sizeof(BacSi));
            if (temp == NULL && *soLuongConTro > 0) {
                perror("Loi khi xoa bac si");
            }
            else if (*soLuongConTro > 0) {
                *dsBacSiConTro = temp;
            }
        }
        ghiDanhSachBacSiRaFile(tenFile, *dsBacSiConTro, *soLuongConTro);
        return 1;
    }

    if (removedFromHashTable && viTriCanXoa == -1) {
        fprintf(stderr, "Da xoa Bac Si '%s' khoi bang bam nhung khong tim thay trong mang.\n", maXoaUpper);
        return 0;
    }

    printf("Khong tim thay bac si voi ma '%s' trong he thong de xoa.\n", maXoaUpper);
    return 0;
}

// Hàm inDanhSachBacSi
void inDanhSachBacSiConsole(const BacSi dsBacSi[], int soLuong) {
    printf("\n--- DANH SACH BAC SI ---\n");
    if (soLuong == 0) {
        printf("Khong co bac si nao trong danh sach.\n");
        return;
    }
    printf("---------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-5s | %-10s | %-30s | %-20s | %-15s | %-25s | %-20s |\n", "STT", "Ma BS", "Ten Bac Si", "Chuyen Khoa", "So Dien Thoai", "Email", "Ma Khoa Phong");
    printf("---------------------------------------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < soLuong; ++i) {
        printf("| %-5d | %-10s | %-30s | %-20s | %-15s | %-25s | %-20s |\n",
            i + 1, dsBacSi[i].maBacSi, dsBacSi[i].tenBacSi, dsBacSi[i].chuyenKhoa,
            dsBacSi[i].soDienThoai, dsBacSi[i].email, dsBacSi[i].maKhoaPhong);
    }
    printf("---------------------------------------------------------------------------------------------------------------------------------------------------\n");
}

// HÀM MỚI: timBacSiTheoMaBangBam
BacSi* timBacSiTheoMaBangBam(BangBam* bbBacSi, const char* maBacSi) {
    if (bbBacSi == NULL || maBacSi == NULL) {
        return NULL;
    }
    char maBSTimKiem[MAX_LEN_MA_BAC_SI];
    strncpy(maBSTimKiem, maBacSi, sizeof(maBSTimKiem) - 1);
    maBSTimKiem[sizeof(maBSTimKiem) - 1] = '\0';
    for (int k = 0; maBSTimKiem[k]; k++) {
        maBSTimKiem[k] = toupper(maBSTimKiem[k]);
    }
    return (BacSi*)timKiemTrongBangBam(bbBacSi, maBSTimKiem);
}