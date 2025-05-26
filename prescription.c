#include "prescription.h"
#include "medicine.h"    
#include "csv_handler.h" 
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Ham noi bo 
static void xoaDongDonThuoc(char* str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Ham doc danh sach don thuoc tu file CSV
DonThuocChiTiet* docDanhSachDonThuocChiTiet(const char* tenFile, int* soLuongDongConTro) {
    FILE* f = fopen(tenFile, "r");
    if (f == NULL) {
        *soLuongDongConTro = 0;
        return NULL;
    }
    DonThuocChiTiet* ds = NULL;
    int capacity = 0;
    *soLuongDongConTro = 0;
    char line[512]; 
    int lineNumber = 0;

    while (fgets(line, sizeof(line), f) != NULL) {
        lineNumber++;
        xoaDongDonThuoc(line);
        if (strlen(line) < 5) continue;

        if (*soLuongDongConTro >= capacity) {
            capacity = (capacity == 0) ? 20 : capacity * 2; 
            DonThuocChiTiet* temp = realloc(ds, capacity * sizeof(DonThuocChiTiet));
            if (temp == NULL) {
                perror("Loi cap phat bo nho");
                free(ds); fclose(f); *soLuongDongConTro = 0; return NULL;
            }
            ds = temp;
        }
        DonThuocChiTiet dtct;
    
        char* token;

        token = strtok(line, ",");
        if (token && strlen(token) > 0) dtct.maDonThuocChiTiet = atoi(token);
        else {
            fprintf(stderr, "CANH BAO (docDanhSachDonThuocChiTiet Dong %d): Ma don thuoc chi tiet khong hop le. Bo qua dong.\n", lineNumber);
            continue;
        }

        token = strtok(NULL, ",");
        if (token && strlen(token) > 0) { 
            strncpy(dtct.maThuoc, token, sizeof(dtct.maThuoc) - 1);
            dtct.maThuoc[sizeof(dtct.maThuoc) - 1] = '\0';
        }
        else {
            fprintf(stderr, "CANH BAO (docDanhSachDonThuocChiTiet Dong %d): Ma thuoc rong cho MaDon %d. Bo qua dong.\n", lineNumber, dtct.maDonThuocChiTiet);
            continue;
        }

        token = strtok(NULL, ",");
        if (token && atoi(token) > 0) { 
            dtct.soLuong = atoi(token);
        }
        else {
            fprintf(stderr, "CANH BAO (docDanhSachDonThuocChiTiet Dong %d): So luong khong hop le cho MaDon %d, MaThuoc %s. Bo qua dong.\n", lineNumber, dtct.maDonThuocChiTiet, dtct.maThuoc);
            continue;
        }

        token = strtok(NULL, ",");
        if (token && strlen(token) > 0) { 
            strncpy(dtct.lieuDung, token, sizeof(dtct.lieuDung) - 1);
            dtct.lieuDung[sizeof(dtct.lieuDung) - 1] = '\0';
        }
        else {
            
            fprintf(stderr, "CANH BAO (docDanhSachDonThuocChiTiet Dong %d): Lieu dung rong cho MaDon %d, MaThuoc %s. Dat mac dinh 'Chua co'.\n", lineNumber, dtct.maDonThuocChiTiet, dtct.maThuoc);
            strcpy(dtct.lieuDung, "Chưa có");
        }

        token = strtok(NULL, "\n");
        if (token && strlen(token) > 0) { 
            strncpy(dtct.cachDung, token, sizeof(dtct.cachDung) - 1);
            dtct.cachDung[sizeof(dtct.cachDung) - 1] = '\0';
        }
        else {
            fprintf(stderr, "CANH BAO (docDanhSachDonThuocChiTiet Dong %d): Cach dung rong cho MaDon %d, MaThuoc %s. Dat mac dinh 'Chua co'.\n", lineNumber, dtct.maDonThuocChiTiet, dtct.maThuoc);
            strcpy(dtct.cachDung, "Chưa có");
        }

        ds[*soLuongDongConTro] = dtct;
        (*soLuongDongConTro)++;
    }
    fclose(f);
    return ds;
}

// Ghi danh sach don thuoc ra file CSV
void ghiDanhSachDonThuocChiTietRaFile(const char* tenFile, const DonThuocChiTiet dsDonThuocChiTiet[], int soLuongDong) {
    FILE* f = fopen(tenFile, "w");
    if (f == NULL) {
        perror("Loi mo file (ghiDanhSachDonThuocChiTietRaFile)");
        return;
    }

    for (int i = 0; i < soLuongDong; ++i) {
        
        const char* maThuocOut = (strlen(dsDonThuocChiTiet[i].maThuoc) > 0) ? dsDonThuocChiTiet[i].maThuoc : "MA_THUOC_KHONG_THOA_MAN";
        int soLuongOut = (dsDonThuocChiTiet[i].soLuong > 0) ? dsDonThuocChiTiet[i].soLuong : 1; 
        const char* lieuDungOut = (strlen(dsDonThuocChiTiet[i].lieuDung) > 0) ? dsDonThuocChiTiet[i].lieuDung : "Chua co";
        const char* cachDungOut = (strlen(dsDonThuocChiTiet[i].cachDung) > 0) ? dsDonThuocChiTiet[i].cachDung : "Chua co";

        fprintf(f, "%d,%s,%d,%s,%s\n",
            dsDonThuocChiTiet[i].maDonThuocChiTiet,
            maThuocOut,
            soLuongOut,
            lieuDungOut,
            cachDungOut);
    }
    fclose(f);
}

// Ham them don thuoc 
int themDongDonThuocChiTiet(DonThuocChiTiet** dsDTCTConTro, int* soLuongDongConTro, DonThuocChiTiet dtctMoi, const char* tenFile) {
    if (strlen(dtctMoi.maThuoc) == 0 || dtctMoi.soLuong <= 0 ||
        strlen(dtctMoi.lieuDung) == 0 || strlen(dtctMoi.cachDung) == 0) {
        fprintf(stderr, "LOI (themDongDonThuocChiTiet): Thong tin chi tiet don thuoc khong hop le (truong rong hoac so luong <=0).\n");
        return 0;
    }
    for (int i = 0; i < *soLuongDongConTro; ++i) {
        if ((*dsDTCTConTro)[i].maDonThuocChiTiet == dtctMoi.maDonThuocChiTiet &&
            strcmp((*dsDTCTConTro)[i].maThuoc, dtctMoi.maThuoc) == 0) {
            
            return 0; 
        }
    }

    DonThuocChiTiet* temp = realloc(*dsDTCTConTro, (*soLuongDongConTro + 1) * sizeof(DonThuocChiTiet));
    if (temp == NULL) {
        perror("Loi cap phat bo nho");
        return 0;
    }
    *dsDTCTConTro = temp;
    (*dsDTCTConTro)[*soLuongDongConTro] = dtctMoi;
    (*soLuongDongConTro)++;
    ghiDanhSachDonThuocChiTietRaFile(tenFile, *dsDTCTConTro, *soLuongDongConTro);
    return 1;
}

// Ham xoa thuoc trong don thuoc
int xoaDongThuocTrongDon(DonThuocChiTiet** dsDTCTConTro, int* soLuongDongConTro, int maDonThuocCanTim, const char* maThuocCanXoa, const char* tenFile) {
    int viTriCanXoa = -1;
    char maThuocXoaUpper[MAX_LEN_MA_THUOC];
    strncpy(maThuocXoaUpper, maThuocCanXoa, sizeof(maThuocXoaUpper) - 1);
    maThuocXoaUpper[sizeof(maThuocXoaUpper) - 1] = '\0';
    for (int i = 0; maThuocXoaUpper[i]; i++) maThuocXoaUpper[i] = toupper(maThuocXoaUpper[i]);


    for (int i = 0; i < *soLuongDongConTro; ++i) {
        if ((*dsDTCTConTro)[i].maDonThuocChiTiet == maDonThuocCanTim &&
            strcmp((*dsDTCTConTro)[i].maThuoc, maThuocXoaUpper) == 0) { 
            viTriCanXoa = i;
            break;
        }
    }

    if (viTriCanXoa != -1) {
        for (int i = viTriCanXoa; i < (*soLuongDongConTro - 1); ++i) {
            (*dsDTCTConTro)[i] = (*dsDTCTConTro)[i + 1];
        }
        (*soLuongDongConTro)--;
        if (*soLuongDongConTro == 0) {
            free(*dsDTCTConTro);
            *dsDTCTConTro = NULL;
        }
        else {
            DonThuocChiTiet* temp = realloc(*dsDTCTConTro, (*soLuongDongConTro) * sizeof(DonThuocChiTiet));
            if (temp == NULL && *soLuongDongConTro > 0) {
                perror("Loi realloc (xoaDongThuocTrongDon)");
            }
            else if (*soLuongDongConTro > 0) {
                *dsDTCTConTro = temp;
            }
        }
        ghiDanhSachDonThuocChiTietRaFile(tenFile, *dsDTCTConTro, *soLuongDongConTro);
        return 1;
    }
    return 0;
}

// Ham Sua thuoc trong don thuoc
int suaDongThuocTrongDon(DonThuocChiTiet dsDonThuocChiTiet[], int soLuongDong, int maDonThuocCanTim, const char* maThuocCanSua, DonThuocChiTiet thongTinMoi, const char* tenFile) {
    if (thongTinMoi.soLuong <= 0 || strlen(thongTinMoi.lieuDung) == 0 || strlen(thongTinMoi.cachDung) == 0) {
        fprintf(stderr, "LOI (suaDongThuocTrongDon): Thong tin thuoc moi khong hop le (so luong, lieu dung hoac cach dung rong/invalid).\n");
        return 0;
    }

    char maThuocSuaUpper[MAX_LEN_MA_THUOC];
    strncpy(maThuocSuaUpper, maThuocCanSua, sizeof(maThuocSuaUpper) - 1);
    maThuocSuaUpper[sizeof(maThuocSuaUpper) - 1] = '\0';
    for (int i = 0; maThuocSuaUpper[i]; i++) maThuocSuaUpper[i] = toupper(maThuocSuaUpper[i]);

    for (int i = 0; i < soLuongDong; ++i) {
        if (dsDonThuocChiTiet[i].maDonThuocChiTiet == maDonThuocCanTim &&
            strcmp(dsDonThuocChiTiet[i].maThuoc, maThuocSuaUpper) == 0) { 

            dsDonThuocChiTiet[i].soLuong = thongTinMoi.soLuong;
            strncpy(dsDonThuocChiTiet[i].lieuDung, thongTinMoi.lieuDung, sizeof(dsDonThuocChiTiet[i].lieuDung) - 1);
            dsDonThuocChiTiet[i].lieuDung[sizeof(dsDonThuocChiTiet[i].lieuDung) - 1] = '\0';
            strncpy(dsDonThuocChiTiet[i].cachDung, thongTinMoi.cachDung, sizeof(dsDonThuocChiTiet[i].cachDung) - 1);
            dsDonThuocChiTiet[i].cachDung[sizeof(dsDonThuocChiTiet[i].cachDung) - 1] = '\0';

            ghiDanhSachDonThuocChiTietRaFile(tenFile, dsDonThuocChiTiet, soLuongDong);
            return 1;
        }
    }
    return 0;
}

// Ham in don thuoc ra man hinh - 1 don thuoc 
void inChiTietMotDonThuocConsole(int maDonThuoc,
    const DonThuocChiTiet dsDonThuocChiTiet[], int soLuongDongTongCong,
    const Thuoc dsThuocToanBo[], int soLuongThuocTB) {
    printf("\n--- CHI TIET DON THUOC MA: %d ---\n", maDonThuoc);
    int stt = 0;
    int timThayDon = 0;

    for (int i = 0; i < soLuongDongTongCong; ++i) {
        if (dsDonThuocChiTiet[i].maDonThuocChiTiet == maDonThuoc) {
            if (!timThayDon) {
                printf("-----------------------------------------------------------------------------------------------------------------------------------------\n");
                printf("| %-4s | %-20s | %-35s | %-10s | %-25s | %-25s |\n", "STT", "Ma Thuoc", "Ten Thuoc", "So Luong", "Lieu Dung", "Cach Dung");
                printf("-----------------------------------------------------------------------------------------------------------------------------------------\n");

                timThayDon = 1;
            }
            stt++;
            const Thuoc* thuocInfo = timThuocTheoMa(dsThuocToanBo, soLuongThuocTB, dsDonThuocChiTiet[i].maThuoc);
            char tenThuocHienThi[151] = "Khong tim thay ten";
            if (thuocInfo != NULL) {
                strncpy(tenThuocHienThi, thuocInfo->tenThuoc, 150);
                tenThuocHienThi[150] = '\0';
            }

            printf("| %-4d | %-20.20s | %-35.35s | %-10d | %-25.25s | %-25.25s |\n",
                stt,
                dsDonThuocChiTiet[i].maThuoc,
                tenThuocHienThi,
                dsDonThuocChiTiet[i].soLuong,
                dsDonThuocChiTiet[i].lieuDung,
                dsDonThuocChiTiet[i].cachDung);
        }
    }

    if (!timThayDon) {
        printf("Khong co chi tiet nao cho don thuoc ma %d hoac don thuoc khong ton tai.\n", maDonThuoc);
    }
    else {
        printf("-----------------------------------------------------------------------------------------------------------------------------------------\n");
    }
}

// Ham in tat ca thuoc ra man hinh
void inTatCaDonThuocChiTietConsole(const DonThuocChiTiet dsDonThuocChiTiet[], int soLuongDong,
    const Thuoc dsThuocToanBo[], int soLuongThuocTB) {
    printf("\n--- TAT CA CHI TIET CAC DON THUOC ---\n");
    if (soLuongDong == 0) {
        printf("Khong co chi tiet don thuoc nao.\n");
        return;
    }
    printf("-----------------------------------------------------------------------------------------------------------------------------------------\n"); 
    printf("| %-4s | %-20s | %-35s | %-10s | %-25s | %-25s |\n", "STT", "Ma Thuoc", "Ten Thuoc", "So Luong", "Lieu Dung", "Cach Dung");
    printf("-----------------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < soLuongDong; ++i) {
        const Thuoc* thuocInfo = timThuocTheoMa(dsThuocToanBo, soLuongThuocTB, dsDonThuocChiTiet[i].maThuoc);
        char tenThuocHienThi[151] = "N/A";
        if (thuocInfo != NULL) {
            strncpy(tenThuocHienThi, thuocInfo->tenThuoc, 150);
            tenThuocHienThi[150] = '\0';
        }
        printf("| %-4d | %-20.20s | %-35.35s | %-10d | %-25.25s | %-25.25s |\n",
            dsDonThuocChiTiet[i].maDonThuocChiTiet,
            dsDonThuocChiTiet[i].maThuoc,
            tenThuocHienThi,
            dsDonThuocChiTiet[i].soLuong,
            dsDonThuocChiTiet[i].lieuDung,
            dsDonThuocChiTiet[i].cachDung);
    }
    printf("-----------------------------------------------------------------------------------------------------------------------------------------\n"); 
}


// Ham cap nhat chi tiet 1 don thuoc
void quanLyCapNhatChiTietDonThuoc(int maDonThuocDeCapNhat,
    DonThuocChiTiet** dsDTCTConTro, int* soLuongDongConTro,
    const Thuoc dsThuocToanBo[], int soLuongThuocTB,
    BangBam* bbThuoc, 
    const char* tenFileCSV) {
    int choice;
    char buffer[128]; 

    if (dsThuocToanBo == NULL || soLuongThuocTB == 0 || bbThuoc == NULL) { 
        printf("LOI: Khong co du lieu thuoc hoac bang bam thuoc khong kha dung de ke don.\n");
        printf("Vui long cap nhat danh sach thuoc va khoi tao bang bam thuoc truoc.\n");
        return;
    }

    do {
        inChiTietMotDonThuocConsole(maDonThuocDeCapNhat, *dsDTCTConTro, *soLuongDongConTro, dsThuocToanBo, soLuongThuocTB);
        printf("\n--- QUAN LY DON THUOC MA: %d ---\n", maDonThuocDeCapNhat);
        printf("1. Them thuoc vao don\n");
        printf("2. Sua thong tin thuoc trong don\n");
        printf("3. Xoa thuoc khoi don\n");
        printf("0. Quay lai\n");
        printf("Lua chon cua ban: ");

        choice = nhapSoNguyenCoGioiHan("Lua chon cua ban", 0, 3); 
        if (choice < 0) choice = -1; 

        switch (choice) {

        case 1: { // Them thuoc
            xoaManHinhConsole();
            DonThuocChiTiet dtctMoi;
            dtctMoi.maDonThuocChiTiet = maDonThuocDeCapNhat;

            printf("\n--- Danh Sach Thuoc Hien Co (co the tim theo Ma hoac Ten) ---\n");
            inDanhSachThuocConsole(dsThuocToanBo, soLuongThuocTB);

            printf("Nhap Ma Thuoc can them (IN HOA, KHONG DUOC BO TRONG): ");
            if (docDongAnToan(dtctMoi.maThuoc, sizeof(dtctMoi.maThuoc), stdin) == NULL || strlen(dtctMoi.maThuoc) == 0) {
                printf("LOI: Ma Thuoc khong duoc de trong. Huy them.\n");
                dungManHinhCho();
                xoaManHinhConsole();
                break;
            }
            for (int i = 0; dtctMoi.maThuoc[i]; i++) dtctMoi.maThuoc[i] = toupper(dtctMoi.maThuoc[i]);

            if (timThuocTheoMaBangBam(bbThuoc, dtctMoi.maThuoc) == NULL) {
                printf("Loi: Ma Thuoc '%s' khong ton tai trong danh muc thuoc! Huy them.\n", dtctMoi.maThuoc);
                dungManHinhCho(); 
                xoaManHinhConsole();
                break;
            }
            int thuocDaCoTrongDon = 0;
            for (int i = 0; i < *soLuongDongConTro; ++i) {
                if ((*dsDTCTConTro)[i].maDonThuocChiTiet == maDonThuocDeCapNhat &&
                    strcmp((*dsDTCTConTro)[i].maThuoc, dtctMoi.maThuoc) == 0) {
                    thuocDaCoTrongDon = 1;
                    break;
                }
            }
            if (thuocDaCoTrongDon) {
                printf("Loi: Thuoc '%s' da ton tai trong don thuoc %d. Hay sua so luong/thong tin neu can.\n", dtctMoi.maThuoc, maDonThuocDeCapNhat);
                dungManHinhCho();
                xoaManHinhConsole();
                break;

            }


            printf("Nhap So Luong (phai > 0): ");
            if (docDongAnToan(buffer, sizeof(buffer), stdin) == NULL || sscanf(buffer, "%d", &dtctMoi.soLuong) != 1 || dtctMoi.soLuong <= 0) {
                printf("LOI: So luong khong hop le hoac khong phai la so nguyen duong. Huy them.\n");
                dungManHinhCho();
                xoaManHinhConsole();
                break;
            }

            printf("Nhap Lieu Dung (KHONG DUOC BO TRONG): ");
            if (docDongAnToan(dtctMoi.lieuDung, sizeof(dtctMoi.lieuDung), stdin) == NULL || strlen(dtctMoi.lieuDung) == 0) {
                printf("LOI: Lieu dung khong duoc de trong. Huy them.\n");
                dungManHinhCho();
                xoaManHinhConsole();
                break;
            }
            printf("Nhap Cach Dung (KHONG DUOC BO TRONG): ");
            if (docDongAnToan(dtctMoi.cachDung, sizeof(dtctMoi.cachDung), stdin) == NULL || strlen(dtctMoi.cachDung) == 0) {
                printf("LOI: Cach dung khong duoc de trong. Huy them.\n");
                dungManHinhCho();
                xoaManHinhConsole();
                break;
            }

            if (themDongDonThuocChiTiet(dsDTCTConTro, soLuongDongConTro, dtctMoi, tenFileCSV)) {
                printf("Them thuoc vao don thanh cong!\n");
                dungManHinhCho();
                xoaManHinhConsole();
            }
            else {
                printf("Them thuoc vao don THAT BAI (co the do loi logic hoac du lieu khong hop le da duoc kiem tra).\n");
                xoaManHinhConsole();
            }
            dungManHinhCho();
            break;
        }
        case 2: { 
            
            char maThuocSua[MAX_LEN_MA_THUOC];
            DonThuocChiTiet thongTinMoiSua; 
            printf("Nhap Ma Thuoc trong don can sua (IN HOA, KHONG DUOC BO TRONG): ");
            if (docDongAnToan(maThuocSua, sizeof(maThuocSua), stdin) == NULL || strlen(maThuocSua) == 0) {
                printf("LOI: Ma Thuoc can sua khong duoc de trong. Huy sua.\n");
                dungManHinhCho();
                xoaManHinhConsole();
                break;
            }
            for (int i = 0; maThuocSua[i]; i++) maThuocSua[i] = toupper(maThuocSua[i]);

            int timThayThuocTrongDon = 0;
            for (int i = 0; i < *soLuongDongConTro; ++i) {
                if ((*dsDTCTConTro)[i].maDonThuocChiTiet == maDonThuocDeCapNhat && strcmp((*dsDTCTConTro)[i].maThuoc, maThuocSua) == 0) {
                    timThayThuocTrongDon = 1;
                    break;
                }
            }
            if (!timThayThuocTrongDon) {
                printf("Thuoc '%s' khong co trong don thuoc %d. Huy sua.\n", maThuocSua, maDonThuocDeCapNhat);
                dungManHinhCho();
                xoaManHinhConsole();
                break;
            }

            printf("Nhap So Luong moi (phai > 0): ");
            if (docDongAnToan(buffer, sizeof(buffer), stdin) == NULL || sscanf(buffer, "%d", &thongTinMoiSua.soLuong) != 1 || thongTinMoiSua.soLuong <= 0) {
                printf("LOI: So luong moi khong hop le. Huy sua.\n");
                dungManHinhCho();
                xoaManHinhConsole();
                break;
            }

            printf("Nhap Lieu Dung moi (KHONG DUOC BO TRONG): ");
            if (docDongAnToan(thongTinMoiSua.lieuDung, sizeof(thongTinMoiSua.lieuDung), stdin) == NULL || strlen(thongTinMoiSua.lieuDung) == 0) {
                printf("LOI: Lieu dung moi khong duoc de trong. Huy sua.\n");
                dungManHinhCho();
                xoaManHinhConsole();
                break;
            }
            printf("Nhap Cach Dung moi (KHONG DUOC BO TRONG): ");
            if (docDongAnToan(thongTinMoiSua.cachDung, sizeof(thongTinMoiSua.cachDung), stdin) == NULL || strlen(thongTinMoiSua.cachDung) == 0) {
                printf("LOI: Cach dung moi khong duoc de trong. Huy sua.\n");
                dungManHinhCho();
                xoaManHinhConsole();
                break;
            }

            if (suaDongThuocTrongDon(*dsDTCTConTro, *soLuongDongConTro, maDonThuocDeCapNhat, maThuocSua, thongTinMoiSua, tenFileCSV)) {
                printf("Sua thong tin thuoc thanh cong!\n");
                dungManHinhCho();
                xoaManHinhConsole();
            }
            else {
                printf("Sua thong tin thuoc THAT BAI.\n");
                dungManHinhCho();
                xoaManHinhConsole();
            }
            dungManHinhCho();
            break;
        }
        case 3: {
            xoaManHinhConsole();
            char maThuocXoa[MAX_LEN_MA_THUOC];
            printf("Nhap Ma Thuoc trong don can xoa (IN HOA, KHONG DUOC BO TRONG): ");
            if (docDongAnToan(maThuocXoa, sizeof(maThuocXoa), stdin) == NULL || strlen(maThuocXoa) == 0) {
                printf("LOI: Ma Thuoc can xoa khong duoc de trong. Huy xoa.\n");
                dungManHinhCho();
                xoaManHinhConsole();
                break;
            }
            for (int i = 0; maThuocXoa[i]; i++) maThuocXoa[i] = toupper(maThuocXoa[i]);

            if (xoaDongThuocTrongDon(dsDTCTConTro, soLuongDongConTro, maDonThuocDeCapNhat, maThuocXoa, tenFileCSV)) {
                printf("Xoa thuoc khoi don thanh cong!\n");
                dungManHinhCho();
                xoaManHinhConsole();

            }
            else {
                printf("Xoa thuoc khoi don THAT BAI (co the khong tim thay thuoc '%s' trong don %d).\n", maThuocXoa, maDonThuocDeCapNhat);
                xoaManHinhConsole();
                
            }
            dungManHinhCho();
            break;
        }
        case 0: //Ra ngoai menu
            break;
        default:
            inThongDiepLoi("Lua chon khong hop le. Vui long chon lai."); 
            dungManHinhCho(); 
            xoaManHinhConsole();
            break;
        }
    } while (choice != 0); 
}
