#include "examination.h"
#include "patient.h"        
#include "csv_handler.h"    
#include "doctor.h"         
#include "department.h"     
#include "priority_queue.h" 
#include "hash_table.h"     
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#define TEN_BN_LK_CONSOLE_WIDTH 
#define TEN_BN_LK_CONSOLE_WIDTH 25      
#define LY_DO_KHAM_LK_CONSOLE_WIDTH 20  
#define CHAN_DOAN_LK_CONSOLE_WIDTH 15  
#define GHI_CHU_BS_LK_CONSOLE_WIDTH 15  
#define MA_BAC_SI_LK_CONSOLE_WIDTH 15   
#define MA_LAN_KHAM_STR_LEN 12 

// Ham noi bo chuyen ma lan kham sang dang string
static void intMaLKToString(int maLK, char* maLKStr, size_t bufferSize) {
    snprintf(maLKStr, bufferSize, "%d", maLK);
}
// Ham tien ich de so sanh hai chuoi khong phan biet hoa thuong
int my_strcasecmp(const char* s1, const char* s2) { 
    int c1, c2;
    do {
        c1 = tolower((unsigned char)*s1++);
        c2 = tolower((unsigned char)*s2++);
    } while (c1 == c2 && c1 != 0);
    return c1 - c2;
}
// Cac ham chuyen doi tu Enum sang String va nguoc lai
const char* trangThaiLanKhamToString(TrangThaiLanKham tt) {
    switch (tt) {
    case LK_DANG_CHO: return "DangCho";
    case LK_DANG_KHAM: return "DangKham";
    case LK_DA_HOAN_THANH: return "DaHoanThanh";
    case LK_DA_HUY: return "DaHuy";
    case LK_DA_LO: return "DaLo";
    default: return "KhongXacDinh";
    }
}

TrangThaiLanKham stringToTrangThaiLanKham(const char* str) {
    if (str == NULL || strlen(str) == 0) return LK_DANG_CHO;
    char lowerStr[50];
    strncpy(lowerStr, str, sizeof(lowerStr) - 1); lowerStr[sizeof(lowerStr) - 1] = '\0';
    for (int i = 0; lowerStr[i]; i++) lowerStr[i] = tolower(lowerStr[i]);

    if (my_strcasecmp(lowerStr, "dangcho") == 0) return LK_DANG_CHO;
    if (my_strcasecmp(lowerStr, "dangkham") == 0) return LK_DANG_KHAM;
    if (my_strcasecmp(lowerStr, "dahoanthanh") == 0) return LK_DA_HOAN_THANH;
    if (my_strcasecmp(lowerStr, "dahuy") == 0) return LK_DA_HUY;
    if (my_strcasecmp(lowerStr, "dalo") == 0) return LK_DA_LO;
    return LK_DANG_CHO; 
}

const char* mucDoUuTienToString(MucDoUuTien md) {
    switch (md) {
    case CAP_CUU: return "CapCuu";
    case KHAN_CAP: return "KhanCap";
    case THONG_THUONG: return "ThongThuong";
    case TAI_KHAM: return "TaiKham";
    case UU_TIEN_KHAC: return "UuTienKhac";
    default: return "KhongXacDinh";
    }
}

MucDoUuTien stringToMucDoUuTien(const char* str) {
    if (str == NULL || strlen(str) == 0) return THONG_THUONG;
    char lowerStr[50];
    strncpy(lowerStr, str, sizeof(lowerStr) - 1); lowerStr[sizeof(lowerStr) - 1] = '\0';
    for (int i = 0; lowerStr[i]; i++) lowerStr[i] = tolower(lowerStr[i]);

    if (my_strcasecmp(lowerStr, "capcuu") == 0) return CAP_CUU;
    if (my_strcasecmp(lowerStr, "khancap") == 0) return KHAN_CAP;
    if (my_strcasecmp(lowerStr, "thongthuong") == 0) return THONG_THUONG;
    if (my_strcasecmp(lowerStr, "taikham") == 0) return TAI_KHAM;
    if (my_strcasecmp(lowerStr, "uutienkhac") == 0) return UU_TIEN_KHAC;
    return THONG_THUONG; 
}
// Ham doc danh sach lan kham tu file CSV
LanKham* docDanhSachLanKham(const char* tenFile, int* soLuong) {
    FILE* f = fopen(tenFile, "r");
    if (f == NULL) {
        *soLuong = 0;
        return NULL;
    }

    LanKham* ds = NULL;
    int capacity = 0;
    *soLuong = 0;
    char line[2048]; 
    int lineNumber = 0;

    while (fgets(line, sizeof(line), f) != NULL) {
        lineNumber++;
        char orgLine[2048]; 
        strncpy(orgLine, line, sizeof(orgLine) - 1);
        orgLine[sizeof(orgLine) - 1] = '\0';
        xoaKyTuXuongDong(orgLine);

        xoaKyTuXuongDong(line);
        if (strlen(line) < 10) {
            if (strlen(line) > 0) {
                fprintf(stderr, "CANH BAO (docDSLK Dong %d): Dong qua ngan, bo qua: \"%s\"\n", lineNumber, orgLine);
            }
            continue;
        }

        if (*soLuong >= capacity) {
            capacity = (capacity == 0) ? 20 : capacity * 2;
            LanKham* temp = realloc(ds, capacity * sizeof(LanKham));
            if (temp == NULL) {
                perror("Loi cap phat bo nho cho docDanhSachLanKham");
                free(ds); fclose(f); *soLuong = 0; return NULL;
            }
            ds = temp;
        }

        LanKham lk;
        //Khoi tao ve gia tri an toan
        lk.maLanKham = 0;                lk.maDinhDanh[0] = '\0';        lk.tenBenhNhan[0] = '\0';
        lk.maBacSi[0] = '\0';            lk.maKhoaPhong[0] = '\0';       lk.ngayGioKham = 0;
        lk.lyDoKham[0] = '\0';           lk.chanDoan[0] = '\0';          lk.phuongPhapDieuTri[0] = '\0';
        lk.ghiChuBacSi[0] = '\0';        lk.ngayTaiKham.ngay = -1;       lk.ngayTaiKham.thang = -1;
        lk.ngayTaiKham.nam = -1;         lk.coLichTaiKham = 0;           lk.mucDoUuTien = THONG_THUONG;
        lk.gioDangKyThanhCong = 0;       lk.coDonThuoc = 0;              lk.trangThai = LK_DANG_CHO;
        lk.maDonThuocChiTiet = -1;

        char mucDoUuTienStr[50] = "";    char trangThaiStr[50] = "";
        long tempTimestamp;              char* token;
        char* lineCopyForStrtok = strdup(line); 
        if (!lineCopyForStrtok) { perror("Loi strdup lineCopyForStrtok"); continue; }
        char* p = lineCopyForStrtok;

        token = strtok(p, ",");
        if (token && strlen(token) > 0 && strspn(token, "0123456789") == strlen(token)) { lk.maLanKham = atoi(token); }
        else { fprintf(stderr, "LOI DOC CSV (LanKham dong %d): MaLanKham ('%s') khong hop le. Bo qua dong: \"%s\"\n", lineNumber, token ? token : "NULL", orgLine); free(lineCopyForStrtok); continue; }

        token = strtok(NULL, ",");
        if (token && strlen(token) > 0) { strncpy(lk.maDinhDanh, token, sizeof(lk.maDinhDanh) - 1); lk.maDinhDanh[sizeof(lk.maDinhDanh) - 1] = '\0'; }
        else { fprintf(stderr, "LOI DOC CSV (LanKham dong %d): Thieu maDinhDanh cho MaLK %d. Bo qua dong.\n", lineNumber, lk.maLanKham); free(lineCopyForStrtok); continue; }

        token = strtok(NULL, ","); if (token) { strncpy(lk.tenBenhNhan, token, sizeof(lk.tenBenhNhan) - 1); lk.tenBenhNhan[sizeof(lk.tenBenhNhan) - 1] = '\0'; }
        else { strcpy(lk.tenBenhNhan, "Khong Biet"); } 

        token = strtok(NULL, ","); if (token && strlen(token) > 0) { strncpy(lk.maBacSi, token, sizeof(lk.maBacSi) - 1); lk.maBacSi[sizeof(lk.maBacSi) - 1] = '\0'; }
        else { strcpy(lk.maBacSi, "CHUA_GAN"); }

        token = strtok(NULL, ","); 
        if (token && strlen(token) > 0) { strncpy(lk.maKhoaPhong, token, sizeof(lk.maKhoaPhong) - 1); lk.maKhoaPhong[sizeof(lk.maKhoaPhong) - 1] = '\0'; }
        else { fprintf(stderr, "LOI DOC CSV (LanKham dong %d): Thieu maKhoaPhong cho MaLK %d. Bo qua dong.\n", lineNumber, lk.maLanKham); free(lineCopyForStrtok); continue; }


        token = strtok(NULL, ","); 
        if (token && strlen(token) > 0 && strspn(token, "0123456789") == strlen(token)) { tempTimestamp = atol(token); if (tempTimestamp >= 0) lk.ngayGioKham = (time_t)tempTimestamp; }

        token = strtok(NULL, ","); 
        if (token && strlen(token) > 0) { strncpy(lk.lyDoKham, token, sizeof(lk.lyDoKham) - 1); lk.lyDoKham[sizeof(lk.lyDoKham) - 1] = '\0'; }
        else { strcpy(lk.lyDoKham, "Khong ro ly do"); }

        token = strtok(NULL, ","); 
        if (token && strlen(token) > 0) { strncpy(lk.chanDoan, token, sizeof(lk.chanDoan) - 1); lk.chanDoan[sizeof(lk.chanDoan) - 1] = '\0'; }
        else { strcpy(lk.chanDoan, "CHUA_KHAM"); }

        token = strtok(NULL, ","); 
        if (token && strlen(token) > 0) { strncpy(lk.phuongPhapDieuTri, token, sizeof(lk.phuongPhapDieuTri) - 1); lk.phuongPhapDieuTri[sizeof(lk.phuongPhapDieuTri) - 1] = '\0'; }
        else { strcpy(lk.phuongPhapDieuTri, "CHUA_KHAM"); }

        token = strtok(NULL, ","); 
        if (token) { strncpy(lk.ghiChuBacSi, token, sizeof(lk.ghiChuBacSi) - 1); lk.ghiChuBacSi[sizeof(lk.ghiChuBacSi) - 1] = '\0'; }
        else { strcpy(lk.ghiChuBacSi, "Khong"); }

        token = strtok(NULL, ","); if (token && strlen(token) > 0 && strspn(token, "-0123456789") == strlen(token)) lk.ngayTaiKham.ngay = atoi(token); else lk.ngayTaiKham.ngay = -1;
        token = strtok(NULL, ","); if (token && strlen(token) > 0 && strspn(token, "-0123456789") == strlen(token)) lk.ngayTaiKham.thang = atoi(token); else lk.ngayTaiKham.thang = -1;
        token = strtok(NULL, ","); if (token && strlen(token) > 0 && strspn(token, "-0123456789") == strlen(token)) lk.ngayTaiKham.nam = atoi(token); else lk.ngayTaiKham.nam = -1;
        token = strtok(NULL, ","); if (token && strlen(token) > 0 && strspn(token, "01") == strlen(token)) lk.coLichTaiKham = atoi(token); else lk.coLichTaiKham = 0;

        token = strtok(NULL, ","); // mucDoUuTien
        if (token && strlen(token) > 0) { lk.mucDoUuTien = stringToMucDoUuTien(token); }

        token = strtok(NULL, ","); // gioDangKyThanhCong
        if (token && strlen(token) > 0 && strspn(token, "0123456789") == strlen(token)) { tempTimestamp = atol(token); if (tempTimestamp >= 0) lk.gioDangKyThanhCong = (time_t)tempTimestamp; }

        token = strtok(NULL, ","); if (token && strlen(token) > 0 && strspn(token, "01") == strlen(token)) lk.coDonThuoc = atoi(token); else lk.coDonThuoc = 0;

        token = strtok(NULL, ","); // trangThai (phải là token cuối cùng trước maDonThuoc)
        if (token && strlen(token) > 0) { lk.trangThai = stringToTrangThaiLanKham(token); }

        token = strtok(NULL, "\n"); // maDonThuocChiTiet
        if (token && strlen(token) > 0 && strspn(token, "-0123456789") == strlen(token)) lk.maDonThuocChiTiet = atoi(token); else lk.maDonThuocChiTiet = -1;

        // Kiểm tra bắt buộc sau khi đọc trạng thái
        if (lk.trangThai == LK_DA_HOAN_THANH) {
            if (strlen(lk.chanDoan) == 0 || strcmp(lk.chanDoan, "CHUA_KHAM") == 0) {
                fprintf(stderr, "CANH BAO (docDSLK Dong %d): ChanDoan rong/mac dinh cho MaLK %d (DaHoanThanh). Dat 'CHUA_NHAP_CD_DOC'.\n", lineNumber, lk.maLanKham);
                strcpy(lk.chanDoan, "CHUA_NHAP_CD_DOC");
            }
            if (strlen(lk.phuongPhapDieuTri) == 0 || strcmp(lk.phuongPhapDieuTri, "CHUA_KHAM") == 0) {
                fprintf(stderr, "CANH BAO (docDSLK Dong %d): PPDieuTri rong/mac dinh cho MaLK %d (DaHoanThanh). Dat 'CHUA_NHAP_PPTT_DOC'.\n", lineNumber, lk.maLanKham);
                strcpy(lk.phuongPhapDieuTri, "CHUA_NHAP_PPTT_DOC");
            }
        }

        free(lineCopyForStrtok);
        ds[*soLuong] = lk;
        (*soLuong)++;
    }
    fclose(f);
    return ds;
}

// Ham xuat danh sach lan kham ra file CSV
void ghiDanhSachLanKhamRaFile(const char* tenFile, const LanKham dsLanKham[], int soLuong) {
    FILE* f = fopen(tenFile, "w");
    if (f == NULL) {
        perror("Loi mo file de ghi (ghiDanhSachLanKhamRaFile)");
        return;
    }
    for (int i = 0; i < soLuong; ++i) {
        const char* maKhoaPhongOut = (strlen(dsLanKham[i].maKhoaPhong) > 0) ? dsLanKham[i].maKhoaPhong : "KP_KHONG_THOA_MAN";
        const char* lyDoKhamOut = (strlen(dsLanKham[i].lyDoKham) > 0) ? dsLanKham[i].lyDoKham : "Khong ro";
        const char* chanDoanOut = (strlen(dsLanKham[i].chanDoan) > 0) ? dsLanKham[i].chanDoan : "CHUA_KHAM";
        const char* ppdtOut = (strlen(dsLanKham[i].phuongPhapDieuTri) > 0) ? dsLanKham[i].phuongPhapDieuTri : "CHUA_KHAM";

        if (dsLanKham[i].trangThai == LK_DA_HOAN_THANH) {
            if (strcmp(chanDoanOut, "CHUA_KHAM") == 0 || strlen(chanDoanOut) == 0) chanDoanOut = "CHUA_NHAP_CD_GHI";
            if (strcmp(ppdtOut, "CHUA_KHAM") == 0 || strlen(ppdtOut) == 0) ppdtOut = "CHUA_NHAP_PPTT_GHI";
        }

        fprintf(f, "%d,%s,%s,%s,%s,%ld,%s,%s,%s,%s,%d,%d,%d,%d,%s,%ld,%d,%s,%d\n",
            dsLanKham[i].maLanKham,
            (strlen(dsLanKham[i].maDinhDanh) > 0 ? dsLanKham[i].maDinhDanh : "BN_KHONG_THOA_MAN"),
            (strlen(dsLanKham[i].tenBenhNhan) > 0 ? dsLanKham[i].tenBenhNhan : "Ten Khong Biet"),
            (strlen(dsLanKham[i].maBacSi) == 0 ? "CHUA_GAN" : dsLanKham[i].maBacSi),
            maKhoaPhongOut,
            (long)dsLanKham[i].ngayGioKham,
            lyDoKhamOut,
            chanDoanOut,
            ppdtOut,
            (strlen(dsLanKham[i].ghiChuBacSi) == 0 ? "Khong" : dsLanKham[i].ghiChuBacSi),
            dsLanKham[i].ngayTaiKham.ngay, dsLanKham[i].ngayTaiKham.thang, dsLanKham[i].ngayTaiKham.nam,
            dsLanKham[i].coLichTaiKham,
            mucDoUuTienToString(dsLanKham[i].mucDoUuTien),
            (long)dsLanKham[i].gioDangKyThanhCong,
            dsLanKham[i].coDonThuoc,
            trangThaiLanKhamToString(dsLanKham[i].trangThai),
            dsLanKham[i].maDonThuocChiTiet
        );
    }
    fclose(f);
}

// Ham tao ma lan kham moi
int taoMaLanKhamMoi(const LanKham dsLanKham[], int soLuong) {
    if (soLuong == 0) return 1;
    int maxId = 0;
    for (int i = 0; i < soLuong; ++i) {
        if (dsLanKham[i].maLanKham > maxId) maxId = dsLanKham[i].maLanKham;
    }
    return maxId + 1;
}
// Tim kiem lan kham theo ma trong bang bam
LanKham* timLanKhamTheoMaBangBam(BangBam* bbLanKham, int maLK) {
    if (bbLanKham == NULL) return NULL;
    char maLKStr[MA_LAN_KHAM_STR_LEN];
    intMaLKToString(maLK, maLKStr, sizeof(maLKStr));
    return (LanKham*)timKiemTrongBangBam(bbLanKham, maLKStr);
}
// Tim kiem lan kham theo tuyen tinh
void timKiemLanKham(const LanKham dsLanKham[], int soLuongLanKham,
    const char* tieuChi, const char* giaTriTimKiem,
    LanKham** ketQuaTimKiem, int* soLuongKetQua) {
    *ketQuaTimKiem = NULL;
    *soLuongKetQua = 0;
    if (soLuongLanKham == 0 || giaTriTimKiem == NULL || tieuChi == NULL || strlen(giaTriTimKiem) == 0) {
        return;
    }

    LanKham* dsKQTemp = (LanKham*)malloc(soLuongLanKham * sizeof(LanKham));
    if (dsKQTemp == NULL) {
        perror("Loi cap phat bo nho");
        return;
    }
    int count = 0;

    char tieuChiLower[50];
    strncpy(tieuChiLower, tieuChi, sizeof(tieuChiLower) - 1);
    tieuChiLower[sizeof(tieuChiLower) - 1] = '\0';
    for (int idx = 0; tieuChiLower[idx]; idx++) tieuChiLower[idx] = tolower(tieuChiLower[idx]);

    char giaTriTimKiemLower[100];
    strncpy(giaTriTimKiemLower, giaTriTimKiem, sizeof(giaTriTimKiemLower) - 1);
    giaTriTimKiemLower[sizeof(giaTriTimKiemLower) - 1] = '\0';
    if (strcmp(tieuChiLower, "malankham") != 0) { 
        for (int idx = 0; giaTriTimKiemLower[idx]; idx++) giaTriTimKiemLower[idx] = tolower(giaTriTimKiemLower[idx]);
    }


    for (int i = 0; i < soLuongLanKham; ++i) {
        int timThayTrongLanKhamNay = 0;

        if (strcmp(tieuChiLower, "malankham") == 0) {
            if (dsLanKham[i].maLanKham == atoi(giaTriTimKiem)) { 
                timThayTrongLanKhamNay = 1;
            }
        }
        else if (strcmp(tieuChiLower, "madinhdanhbn") == 0) {
            char maDDBNLower[MAX_LEN_MA_DINH_DANH_BN + 1]; 
            strncpy(maDDBNLower, dsLanKham[i].maDinhDanh, sizeof(maDDBNLower) - 1);
            maDDBNLower[sizeof(maDDBNLower) - 1] = '\0';
            for (int k = 0; maDDBNLower[k]; k++) maDDBNLower[k] = tolower(maDDBNLower[k]);
            if (strcmp(maDDBNLower, giaTriTimKiemLower) == 0) {
                timThayTrongLanKhamNay = 1;
            }
        }
        else if (strcmp(tieuChiLower, "trangthai") == 0) {
            char trangThaiLower[50];
            strncpy(trangThaiLower, trangThaiLanKhamToString(dsLanKham[i].trangThai), sizeof(trangThaiLower) - 1);
            trangThaiLower[sizeof(trangThaiLower) - 1] = '\0';
            for (int k = 0; trangThaiLower[k]; k++) {
                trangThaiLower[k] = tolower(trangThaiLower[k]);
            }
            if (strcmp(trangThaiLower, giaTriTimKiemLower) == 0) {
                timThayTrongLanKhamNay = 1;
            }
        }
        else if (strcmp(tieuChiLower, "bacsi") == 0) { 
            char maBacSiLower[MAX_LEN_MA_BAC_SI + 1]; 
            strncpy(maBacSiLower, dsLanKham[i].maBacSi, sizeof(maBacSiLower) - 1);
            maBacSiLower[sizeof(maBacSiLower) - 1] = '\0';
            for (int k = 0; maBacSiLower[k]; k++) {
                maBacSiLower[k] = tolower(maBacSiLower[k]);
            }
            if (strstr(maBacSiLower, giaTriTimKiemLower) != NULL) { 
                timThayTrongLanKhamNay = 1;
            }
        }
        else if (strcmp(tieuChiLower, "makhoaphong") == 0) { 
            char maLKLower[MAX_LEN_MA_KHOA_PHONG + 1];
            strncpy(maLKLower, dsLanKham[i].maKhoaPhong, sizeof(maLKLower) - 1);
            maLKLower[sizeof(maLKLower) - 1] = '\0';
            for (int k = 0; maLKLower[k]; k++) {
                maLKLower[k] = tolower(maLKLower[k]);
            }
            if (strcmp(maLKLower, giaTriTimKiemLower) == 0) {
                timThayTrongLanKhamNay = 1;
            }
        }


        if (timThayTrongLanKhamNay) {
            dsKQTemp[count++] = dsLanKham[i];
        }
    }

    if (count > 0) {
        *ketQuaTimKiem = (LanKham*)malloc(count * sizeof(LanKham));
        if (*ketQuaTimKiem != NULL) {
            memcpy(*ketQuaTimKiem, dsKQTemp, count * sizeof(LanKham));
            *soLuongKetQua = count;
        }
        else {
            perror("Loi cap phat bo nho");
            *soLuongKetQua = 0;
        }
    }
    free(dsKQTemp);
}
// Ham dang ky lan kham moi
int dangKyKhamMoi(LanKham** dsLanKhamConTro, int* soLuongLanKhamConTro,
    BenhNhan** dsBenhNhanConTro, int* soLuongBenhNhanConTro,
    BangBam* bbBenhNhan,
    KhoaPhong* dsKhoaPhong, int soLuongKhoaPhong,
    BangBam* bbKhoaPhong,
    BangBam** bbLanKham_ptr,
    BangBam* bbHangDoiTheoKhoa,
    const char* fileLanKhamCSV, const char* fileBenhNhanCSV) {
// 
    char maBN_input[MAX_LEN_MA_DINH_DANH_BN];
    char maBN_upper[MAX_LEN_MA_DINH_DANH_BN];
    BenhNhan* bnTimThay = NULL;

    printf("Nhap Ma Dinh Danh Benh Nhan (KHONG BO TRONG): ");
    if (docDongAnToan(maBN_input, sizeof(maBN_input), stdin) == NULL || strlen(maBN_input) == 0) {
        printf("LOI: Ma Dinh Danh Benh Nhan khong duoc de trong. Huy dang ky.\n");
        return -1;
    }
    strncpy(maBN_upper, maBN_input, sizeof(maBN_upper) - 1);
    maBN_upper[sizeof(maBN_upper) - 1] = '\0';
    for (int k = 0; maBN_upper[k]; k++) maBN_upper[k] = toupper(maBN_upper[k]);

    if (bbBenhNhan != NULL) {
        bnTimThay = timBenhNhanTheoMaBangBam(bbBenhNhan, maBN_upper);
    }
    if (bnTimThay == NULL && dsBenhNhanConTro != NULL && *dsBenhNhanConTro != NULL) {
        bnTimThay = (BenhNhan*)timBenhNhanTheoMaChuoi(*dsBenhNhanConTro, *soLuongBenhNhanConTro, maBN_upper);
    }

    if (bnTimThay == NULL) {
        printf("Benh nhan voi Ma Dinh Danh %s khong ton tai.\n", maBN_upper);
        char choice_str[10];
        printf("Ban co muon tao moi ho so benh nhan khong? (c/k): ");
        docDongAnToan(choice_str, sizeof(choice_str), stdin);
        if (strlen(choice_str) > 0 && tolower(choice_str[0]) == 'c') {
            BenhNhan bnMoiTam;
            printf("--- Nhap Thong Tin Benh Nhan Moi ---\n");
            if (nhapThongTinBenhNhanTuBanPhim(&bnMoiTam, *dsBenhNhanConTro, *soLuongBenhNhanConTro, bbBenhNhan)) { 
                
                if (!themBenhNhan(dsBenhNhanConTro, soLuongBenhNhanConTro, &bbBenhNhan, bnMoiTam, fileBenhNhanCSV)) {
                    printf("Loi: Khong thể tao benh nhan moi.\n");
                    return -1;
                }
                bnTimThay = timBenhNhanTheoMaBangBam(bbBenhNhan, bnMoiTam.maDinhDanh);
                if (bnTimThay == NULL) {
                    bnTimThay = (BenhNhan*)timBenhNhanTheoMaChuoi(*dsBenhNhanConTro, *soLuongBenhNhanConTro, bnMoiTam.maDinhDanh);
                }
                if (bnTimThay == NULL) {
                    fprintf(stderr, "LOI NGHIEM TRONG: Khong tim thay benh nhan %s ngay sau khi tao.\n", bnMoiTam.maDinhDanh);
                    return -1;
                }
                printf("Da tao benh nhan moi voi Ma Dinh Danh: %s\n", bnTimThay->maDinhDanh);
                strncpy(maBN_upper, bnTimThay->maDinhDanh, sizeof(maBN_upper) - 1);
                maBN_upper[sizeof(maBN_upper) - 1] = '\0';

            }
            else {
                printf("Huy tao benh nhan moi. Dang ky kham bi huy.\n");
                return -1;
            }
        }
        else {
            printf("Dang ky kham bi huy do benh nhan khong ton tai va khong tao moi.\n");
            return -1;
        }
    }
    // Chon khoa phong
    char maKP_input[MAX_LEN_MA_KHOA_PHONG];
    char maKP_upper_norm[MAX_LEN_MA_KHOA_PHONG];
    KhoaPhong* kpChon = NULL;

    do {
        printf("\nDanh sach Khoa Phong hien co:\n");
        if (dsKhoaPhong && soLuongKhoaPhong > 0) { 
            inDanhSachKhoaPhongConsole(dsKhoaPhong, soLuongKhoaPhong); 
        }
        else {
            printf("Khong co khoa phong nao trong he thong.\n");
            printf("Vui long them khoa phong truoc khi dang ky kham. Huy dang ky.\n");
            return -1;
        }

        printf("Nhap Ma Khoa Phong muon dang ky kham (KHONG DUOC BO TRONG): ");
        if (docDongAnToan(maKP_input, sizeof(maKP_input), stdin) == NULL || strlen(maKP_input) == 0) {
            printf("LOI: Ma Khoa Phong khong duoc de trong. Huy dang ky.\n");
            return -1;
        }
        strncpy(maKP_upper_norm, maKP_input, sizeof(maKP_upper_norm) - 1);
        maKP_upper_norm[sizeof(maKP_upper_norm) - 1] = '\0';
        for (int k = 0; maKP_upper_norm[k]; k++) maKP_upper_norm[k] = toupper(maKP_upper_norm[k]);

        if (bbKhoaPhong) {
            kpChon = timKhoaPhongTheoMaBangBam(bbKhoaPhong, maKP_upper_norm); 
        }
        else if (dsKhoaPhong) {
            kpChon = timKhoaPhongTheoMa(dsKhoaPhong, soLuongKhoaPhong, maKP_upper_norm); 
        }

        if (kpChon == NULL) {
            printf("LOI: Ma Khoa Phong '%s' khong ton tai. Vui long chon lai.\n", maKP_upper_norm);
        }
    } while (kpChon == NULL);

    LanKham lkMoi;
    lkMoi.maLanKham = taoMaLanKhamMoi(*dsLanKhamConTro, *soLuongLanKhamConTro); 

    strncpy(lkMoi.maDinhDanh, bnTimThay->maDinhDanh, sizeof(lkMoi.maDinhDanh) - 1);
    lkMoi.maDinhDanh[sizeof(lkMoi.maDinhDanh) - 1] = '\0';
    strncpy(lkMoi.tenBenhNhan, bnTimThay->ten, sizeof(lkMoi.tenBenhNhan) - 1);
    lkMoi.tenBenhNhan[sizeof(lkMoi.tenBenhNhan) - 1] = '\0';

    strncpy(lkMoi.maKhoaPhong, kpChon->maKhoaPhong, sizeof(lkMoi.maKhoaPhong) - 1);
    lkMoi.maKhoaPhong[sizeof(lkMoi.maKhoaPhong) - 1] = '\0';

    printf("Nhap Ly Do Kham (KHONG DUOC BO TRONG): ");
    if (docDongAnToan(lkMoi.lyDoKham, sizeof(lkMoi.lyDoKham), stdin) == NULL || strlen(lkMoi.lyDoKham) == 0) {
        printf("LOI: Ly do kham khong duoc de trong. Huy dang ky.\n");
        return -1;
    }

    char mucDoUTStr[30];
    printf("Nhap Muc Do Uu Tien (CapCuu, KhanCap, ThongThuong, TaiKham, UuTienKhac) [ThongThuong]: ");
    docDongAnToan(mucDoUTStr, sizeof(mucDoUTStr), stdin);
    lkMoi.mucDoUuTien = (strlen(mucDoUTStr) == 0) ? THONG_THUONG : stringToMucDoUuTien(mucDoUTStr); 

    lkMoi.gioDangKyThanhCong = time(NULL);
    lkMoi.trangThai = LK_DANG_CHO;
    strcpy(lkMoi.maBacSi, "CHUA_GAN");
    lkMoi.ngayGioKham = 0;
    strcpy(lkMoi.chanDoan, "CHUA_KHAM");
    strcpy(lkMoi.phuongPhapDieuTri, "CHUA_KHAM");
    strcpy(lkMoi.ghiChuBacSi, "-1");
    lkMoi.ngayTaiKham.ngay = -1; lkMoi.ngayTaiKham.thang = -1; lkMoi.ngayTaiKham.nam = -1;
    lkMoi.coLichTaiKham = 0;
    lkMoi.coDonThuoc = 0;
    lkMoi.maDonThuocChiTiet = -1;

    if (themLanKham(dsLanKhamConTro, soLuongLanKhamConTro, bbLanKham_ptr, lkMoi, fileLanKhamCSV)) { 
        // Them vao hang doi cua khoa cu the
        if (bbHangDoiTheoKhoa) {
            if (!themVaoHangDoiTheoKhoa(bbHangDoiTheoKhoa, lkMoi.maKhoaPhong, lkMoi)) { 
                fprintf(stderr, "LOI: Khong them duoc Lan Kham %d vao hang doi khoa %s.\n", lkMoi.maLanKham, lkMoi.maKhoaPhong);
                fprintf(stderr, "Xoa Lan Kham %d vua them.\n", lkMoi.maLanKham);

                if (!xoaLanKham(dsLanKhamConTro, soLuongLanKhamConTro, *bbLanKham_ptr, lkMoi.maLanKham, fileLanKhamCSV)) { 
                    fprintf(stderr, "LOI ROLLBACK: Khong the xoa Lan Kham %d khoi dsLanKham/bbLanKham.\n", lkMoi.maLanKham);
                    
                }
                else {
                    printf("Da xoa Lan Kham %d.\n", lkMoi.maLanKham);
                }
                return -1; 
            }
        }
        else {
            fprintf(stderr, "CANH BAO: Bang bam hang doi theo khoa khong kha dung. Lan kham %d chua duoc them vao hang doi khoa phong.\n", lkMoi.maLanKham);
        }
        printf("Dang ky kham thanh cong!\n");
        printf("  Ma Lan Kham: %d\n", lkMoi.maLanKham);
        printf("  Benh Nhan: %s (Ma: %s)\n", lkMoi.tenBenhNhan, lkMoi.maDinhDanh);
        printf("  Khoa Phong: %s (Ma: %s)\n", kpChon->tenKhoaPhong, lkMoi.maKhoaPhong);
        printf("  Muc Do Uu Tien: %s\n", mucDoUuTienToString(lkMoi.mucDoUuTien));
        return lkMoi.maLanKham;
    }
    else {
        printf("Loi: Khong the them lan kham vao he thong.\n");
        return -1;
    }
}
// Ham them lan kham
int themLanKham(LanKham** dsLanKhamConTro, int* soLuongConTro,
    BangBam** bbLanKham_ptr,
    LanKham lanKhamMoi, const char* tenFile) {
    if (bbLanKham_ptr == NULL || *bbLanKham_ptr == NULL) {
        fprintf(stderr, "LOI HE THONG.\n");
        return 0;
    }
    if (strlen(lanKhamMoi.maDinhDanh) == 0 || strlen(lanKhamMoi.maKhoaPhong) == 0 || strlen(lanKhamMoi.lyDoKham) == 0) {
        fprintf(stderr, "LOI: Thong tin lan kham moi khong hop le!\n");
        return 0;
    }

    char maLKStr[MA_LAN_KHAM_STR_LEN];
    intMaLKToString(lanKhamMoi.maLanKham, maLKStr, sizeof(maLKStr));

    if (timKiemTrongBangBam(*bbLanKham_ptr, maLKStr) != NULL) {
        fprintf(stderr, "LOI: Ma Lan Kham %d (%s) da ton tai!\n", lanKhamMoi.maLanKham, maLKStr);
        return 0;
    }

    LanKham* temp = realloc(*dsLanKhamConTro, (*soLuongConTro + 1) * sizeof(LanKham));
    if (temp == NULL) {
        perror("Loi cap phat bo nho");
        return 0;
    }
    *dsLanKhamConTro = temp;
    (*dsLanKhamConTro)[*soLuongConTro] = lanKhamMoi;
    LanKham* conTroToiLKTrongMang = &((*dsLanKhamConTro)[*soLuongConTro]);

    if (!chenVaoBangBam(*bbLanKham_ptr, maLKStr, conTroToiLKTrongMang)) {
        fprintf(stderr, "LOI NGHIEM TRONG: Khong the chen Lan Kham %d (%s) vao bang bam.\n", lanKhamMoi.maLanKham, maLKStr);
     
        return 0;
    }

    (*soLuongConTro)++;

    if ((*bbLanKham_ptr)->soLuongPhanTu / (double)(*bbLanKham_ptr)->dungLuong > BANG_BAM_NGUONG_TAI_TOI_DA) {
        if (!rehashBangBam(bbLanKham_ptr)) {
            fprintf(stderr, "LOI: Rehash bang bam LAN KHAM that bai.\n");
        }
    }

    ghiDanhSachLanKhamRaFile(tenFile, *dsLanKhamConTro, *soLuongConTro);
    return 1;
}

// Ham xoa lan kham
int xoaLanKham(LanKham** dsLanKhamConTro, int* soLuongConTro,
    BangBam* bbLanKham,
    int maLanKhamCanXoa, const char* tenFile) {
    if (bbLanKham == NULL) {
        fprintf(stderr, "LOI HE THONG: Bang bam Lan Kham khong kha dung khi xoa.\n");
        return 0;
    }
    char maLKStr[MA_LAN_KHAM_STR_LEN];
    intMaLKToString(maLanKhamCanXoa, maLKStr, sizeof(maLKStr));
    int removedFromHashTable = 0;

    if (timKiemTrongBangBam(bbLanKham, maLKStr) != NULL) {
        if (xoaKhoiBangBam(bbLanKham, maLKStr)) {
            removedFromHashTable = 1;
        }
        else {
            fprintf(stderr, "LOI: Khong the xoa Lan Kham %d (%s) khoi bang bam.\n", maLanKhamCanXoa, maLKStr);
            return 0;
        }
    } 

    int viTri = -1;
    for (int i = 0; i < *soLuongConTro; ++i) {
        if ((*dsLanKhamConTro)[i].maLanKham == maLanKhamCanXoa) {
            viTri = i;
            break;
        }
    }
    if (viTri != -1) {
        if (!removedFromHashTable && timKiemTrongBangBam(bbLanKham, maLKStr) == NULL) { 
         
            fprintf(stderr, "CANH BAO: Lan Kham %d tim thay trong mang nhung khong co trong bang bam truoc khi xoa (hoac da duoc xoa khoi bang bam).\n", maLanKhamCanXoa);
        }

        for (int i = viTri; i < (*soLuongConTro - 1); ++i) {
            (*dsLanKhamConTro)[i] = (*dsLanKhamConTro)[i + 1];
        }
        (*soLuongConTro)--;
        if (*soLuongConTro == 0) {
            free(*dsLanKhamConTro); *dsLanKhamConTro = NULL;
        }
        else {
            LanKham* temp_ds = realloc(*dsLanKhamConTro, (*soLuongConTro) * sizeof(LanKham));
            if (temp_ds == NULL && *soLuongConTro > 0) {
                perror("Loi cap phat bo nho");
            }
            else if (*soLuongConTro > 0) *dsLanKhamConTro = temp_ds;
          
        }
        ghiDanhSachLanKhamRaFile(tenFile, *dsLanKhamConTro, *soLuongConTro);
        return 1;
    }
    if (removedFromHashTable && viTri == -1) { 
        fprintf(stderr, "LOI DONG BO: Da xoa Lan Kham %d khoi bang bam nhung khong tim thay trong mang.\n", maLanKhamCanXoa);
    }
    return 0; 
}

// Ham sua thong tin lan kham
int capNhatThongTinLanKham(LanKham dsLanKham[], int soLuongLanKham, int maLanKhamCanCapNhat,
    const BacSi dsBacSi[], int soLuongBacSi,
    const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong,
    BangBam* bbBacSi,
    BangBam* bbKhoaPhong,
    const char* fileLanKhamCSV) {
    int indexLK = -1;
    for (int i = 0; i < soLuongLanKham; ++i) {
        if (dsLanKham[i].maLanKham == maLanKhamCanCapNhat) {
            indexLK = i;
            break;
        }
    }

    if (indexLK == -1) {
        printf("Khong tim thay Lan Kham voi Ma: %d\n", maLanKhamCanCapNhat);
        return 0;
    }

    LanKham* lk = &dsLanKham[indexLK];
    TrangThaiLanKham trangThaiCu = lk->trangThai; 
    char chanDoanCu[sizeof(lk->chanDoan)];
    strcpy(chanDoanCu, lk->chanDoan);

    if (lk->trangThai == LK_DA_HOAN_THANH || lk->trangThai == LK_DA_HUY || lk->trangThai == LK_DA_LO) {
        printf("Lan kham (Ma: %d) da o trang thai '%s'. Khong the cap nhat theo quy trinh nay.\n",
            lk->maLanKham, trangThaiLanKhamToString(lk->trangThai));
        return 0;
    }
    if (lk->trangThai != LK_DANG_CHO && lk->trangThai != LK_DANG_KHAM) {
        printf("Lan kham (Ma: %d) khong o trang thai phu hop de cap nhat (hien tai: '%s').\n",
            lk->maLanKham, trangThaiLanKhamToString(lk->trangThai));
        return 0;
    }


    printf("\n--- CAP NHAT THONG TIN LAN KHAM MA: %d (Benh nhan: %s, Khoa: %s) ---\n", lk->maLanKham, lk->tenBenhNhan, lk->maKhoaPhong);

    char denKhamChoice_str[10];
    printf("Benh nhan co den kham khong? (c/k): ");
    docDongAnToan(denKhamChoice_str, sizeof(denKhamChoice_str), stdin);

    if (strlen(denKhamChoice_str) > 0 && tolower(denKhamChoice_str[0]) == 'k') {
        
        strcpy(lk->ghiChuBacSi, "Benh nhan khong den");
    }
    else if (strlen(denKhamChoice_str) > 0 && tolower(denKhamChoice_str[0]) == 'c') {
        lk->trangThai = LK_DA_HOAN_THANH;

        // Nhap ma bac si
        char maBacSiNhap[MAX_LEN_MA_BAC_SI];
        const BacSi* bacSiTimThay = NULL;
        do {
            printf("Nhap Ma Bac Si kham: ");
            if (docDongAnToan(maBacSiNhap, sizeof(maBacSiNhap), stdin) == NULL || strlen(maBacSiNhap) == 0) {
                inThongDiepLoi("Ma Bac Si khong duoc de trong. Vui long nhap lai.");

                continue;
            }
            for (int i = 0; maBacSiNhap[i]; i++) maBacSiNhap[i] = toupper(maBacSiNhap[i]);

            bacSiTimThay = NULL;
            if (bbBacSi != NULL) {
                bacSiTimThay = timBacSiTheoMaBangBam(bbBacSi, maBacSiNhap);
            }
            else {

                inThongDiepLoi("He thong du lieu Bac Si (Bang Bam) khong kha dung. Khong the xac thuc Ma Bac Si.");

                strcpy(lk->maBacSi, "CHUA_GAN_LOI_BB");
                inThongDiepLoi("Khong the gan Ma Bac Si do loi he thong.");
                break;
            }

            if (bacSiTimThay != NULL) {
                strncpy(lk->maBacSi, bacSiTimThay->maBacSi, sizeof(lk->maBacSi) - 1); 
                lk->maBacSi[sizeof(lk->maBacSi) - 1] = '\0';
                printf("Da chon Bac Si: %s (Ma: %s)\n", bacSiTimThay->tenBacSi, bacSiTimThay->maBacSi);
                break; 
            }
            else {
               
                printf("CANH BAO: Ma Bac Si '%s' khong tim thay trong he thong.\n", maBacSiNhap);
                printf("Ban co chac chan muon su dung Ma Bac Si '%s' nay (co the khong ton tai)? (c/k): ", maBacSiNhap);
                char confirmBS[10];
                docDongAnToan(confirmBS, sizeof(confirmBS), stdin);
                if (strlen(confirmBS) > 0 && tolower(confirmBS[0]) == 'c') {
                    strncpy(lk->maBacSi, maBacSiNhap, sizeof(lk->maBacSi) - 1); 
                    lk->maBacSi[sizeof(lk->maBacSi) - 1] = '\0';
                    inThongDiepThanhCong("Da ghi nhan Ma Bac Si theo y nguoi dung.");
                    break; 
                }
                inThongDiepLoi("Vui long nhap lai Ma Bac Si hoac chon mot Ma Bac Si ton tai.");
            }
        } while (1);

        printf("Nhap Chan Doan: ");
        docDongAnToan(lk->chanDoan, sizeof(lk->chanDoan), stdin);
        if (strlen(lk->chanDoan) == 0) {
            printf("LOI: Chan Doan khong duoc de trong cho lan kham da hoan thanh. Cap nhat that bai.\n");
            lk->trangThai = trangThaiCu;
            return 0;
        }

        printf("Nhap Phuong Phap Dieu Tri: ");
        docDongAnToan(lk->phuongPhapDieuTri, sizeof(lk->phuongPhapDieuTri), stdin);
        if (strlen(lk->phuongPhapDieuTri) == 0) {
            printf("LOI: Phuong Phap Dieu Tri khong duoc de trong cho lan kham da hoan thanh. Cap nhat that bai.\n");
            strcpy(lk->chanDoan, chanDoanCu); 
            lk->trangThai = trangThaiCu;    
            return 0;
        }        printf("Nhap Ghi Chu Bac Si: ");
        docDongAnToan(lk->ghiChuBacSi, sizeof(lk->ghiChuBacSi), stdin);
        if (strlen(lk->ghiChuBacSi) == 0) {
            strcpy(lk->ghiChuBacSi, "Khong");
        }

        char taiKhamChoice_str_capnhat[10];
        printf("Co lich tai kham khong? (c/k): ");
        docDongAnToan(taiKhamChoice_str_capnhat, sizeof(taiKhamChoice_str_capnhat), stdin);
        if (strlen(taiKhamChoice_str_capnhat) > 0 && tolower(taiKhamChoice_str_capnhat[0]) == 'c') {
            lk->coLichTaiKham = 1;
            char bufferNgayTK[30];
            int ngay, thang, nam;
            int nhapNgayThanhCong = 0;
            do {
                printf("Nhap Ngay Tai Kham (dd mm yyyy, vd: 20 12 2025): ");
                docDongAnToan(bufferNgayTK, sizeof(bufferNgayTK), stdin);
                if (sscanf(bufferNgayTK, "%d %d %d", &ngay, &thang, &nam) == 3) {
                  
                    if (ngay >= 1 && ngay <= 31 && thang >= 1 && thang <= 12 && nam >= 2024 && nam <= 2100) {
                        
                        time_t now = lk->ngayGioKham == 0 ? time(NULL) : lk->ngayGioKham; 
                        struct tm partsTK = { 0 };
                        partsTK.tm_mday = ngay;
                        partsTK.tm_mon = thang - 1; 
                        partsTK.tm_year = nam - 1900; // 
                        time_t thoiGianTK = mktime(&partsTK);

                        if (difftime(thoiGianTK, now) > 0) {
                            lk->ngayTaiKham.ngay = ngay;
                            lk->ngayTaiKham.thang = thang;
                            lk->ngayTaiKham.nam = nam;
                            nhapNgayThanhCong = 1;
                        }
                        else {                    
                            inThongDiepLoi("Ngay tai kham phai sau ngay kham/ngay hien tai.");
                        }
                    }
                    else {
                        inThongDiepLoi("Ngay, thang, hoac nam tai kham khong hop le.");
                    }
                }
                else {
                    inThongDiepLoi("Dinh dang ngay tai kham khong dung. Vui long nhap lai.");
                }
            } while (!nhapNgayThanhCong);
        }
        else { 
            lk->coLichTaiKham = 0;
            lk->ngayTaiKham.ngay = -1;  
            lk->ngayTaiKham.thang = -1;
            lk->ngayTaiKham.nam = -1;
        }

        char donThuocChoice_str_capnhat[10];
        printf("Co ke don thuoc khong? (c/k): ");
        docDongAnToan(donThuocChoice_str_capnhat, sizeof(donThuocChoice_str_capnhat), stdin);
        if (strlen(donThuocChoice_str_capnhat) > 0 && tolower(donThuocChoice_str_capnhat[0]) == 'c') {
            lk->coDonThuoc = 1;
            
            lk->maDonThuocChiTiet = lk->maLanKham;
        }
        else { 
            lk->coDonThuoc = 0;
            lk->maDonThuocChiTiet = -1; 
        }
        printf("Da cap nhat thong tin lan kham thanh cong (trang thai: Da Hoan Thanh).\n");
    }
    else {
        printf("Lua chon khong hop le. Khong thay doi.\n");
        return 0;
    }

    ghiDanhSachLanKhamRaFile(fileLanKhamCSV, dsLanKham, soLuongLanKham);
    return 1;
}

// Ham sua chi tiet lan kham duoc chon
int suaChiTietLanKham(LanKham dsLanKham[], int soLuongLanKham, int maLanKhamCanSua,
    const BacSi dsBacSi[], int soLuongBacSi,
    const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong,
    BangBam* bbBacSi,
    BangBam* bbKhoaPhong,
    const char* fileLanKhamCSV) {

    int indexLK = -1;
    for (int i = 0; i < soLuongLanKham; ++i) {
        if (dsLanKham[i].maLanKham == maLanKhamCanSua) {
            indexLK = i;
            break;
        }
    }
    if (indexLK == -1) {
        char errorMessage[100];
        snprintf(errorMessage, sizeof(errorMessage), "Khong tim thay Lan Kham voi Ma: %d de sua.", maLanKhamCanSua);
        inThongDiepLoi(errorMessage);
        return 0;
    }
    LanKham* lkCanSua = &dsLanKham[indexLK];
    LanKham lkTamSua = *lkCanSua;

    int luaChonSuaNoiBo;
    char bufferNhap[512];

    do {
        xoaManHinhConsole();
        printf("\n--- SUA CHI TIET LAN KHAM (Ma LK: %d, Benh Nhan: %s, Khoa: %s, Trang Thai: %s) ---\n",
            lkTamSua.maLanKham, lkTamSua.tenBenhNhan, lkTamSua.maKhoaPhong, trangThaiLanKhamToString(lkTamSua.trangThai));
        printf("  1. Ma Bac Si        (Hien tai: %s)\n", (strlen(lkTamSua.maBacSi) > 0 && strcmp(lkTamSua.maBacSi, "CHUA_GAN") != 0) ? lkTamSua.maBacSi : "CHUA_GAN");
        printf("  2. Ma Khoa Phong    (Hien tai: %s)\n", (strlen(lkTamSua.maKhoaPhong) > 0 && strcmp(lkTamSua.maKhoaPhong, "CHUA_GAN") != 0) ? lkTamSua.maKhoaPhong : "CHUA_GAN");
        printf("  3. Ly Do Kham       (Hien tai: %s)\n", strlen(lkTamSua.lyDoKham) > 0 ? lkTamSua.lyDoKham : "Khong ro");
        printf("  4. Chan Doan        (Hien tai: %s)\n", (strlen(lkTamSua.chanDoan) > 0 && strcmp(lkTamSua.chanDoan, "CHUA_KHAM") != 0) ? lkTamSua.chanDoan : "CHUA_KHAM/CHUA_NHAP");
        printf("  5. Phuong Phap Dieu Tri (Hien tai: %s)\n", (strlen(lkTamSua.phuongPhapDieuTri) > 0 && strcmp(lkTamSua.phuongPhapDieuTri, "CHUA_KHAM") != 0) ? lkTamSua.phuongPhapDieuTri : "CHUA_KHAM/CHUA_NHAP");
        printf("  6. Ghi Chu Bac Si   (Hien tai: %s)\n", (strlen(lkTamSua.ghiChuBacSi) > 0 && strcmp(lkTamSua.ghiChuBacSi, "-1") != 0) ? lkTamSua.ghiChuBacSi : "Khong co");
        printf("  7. Thay doi Trang Thai\n");
        printf("-------------------------------------------------\n");
        printf("  0. HOAN TAT SUA DOI VA LUU\n");
        printf("  9. Huy bo tat ca thay doi va quay lai\n");

        luaChonSuaNoiBo = nhapSoNguyenCoGioiHan("Lua chon cua ban", 0, 9);
        if (luaChonSuaNoiBo < 0) { luaChonSuaNoiBo = 9; }

        switch (luaChonSuaNoiBo) {
        case 1: // Sua ma bac si
            printf("Nhap Ma Bac Si moi (KHONG DUOC BO TRONG): ");
            docDongAnToan(bufferNhap, sizeof(bufferNhap), stdin);
            if (strlen(bufferNhap) == 0) {
                inThongDiepLoi("Ma Bac Si moi khong duoc de trong. Thao tac huy.");
                break;
            }
            for (int k = 0; bufferNhap[k]; ++k) bufferNhap[k] = toupper(bufferNhap[k]);
            const BacSi* bsKiemTra = NULL;
            if (bbBacSi != NULL) {
                bsKiemTra = timBacSiTheoMaBangBam(bbBacSi, bufferNhap);
            }
            else {
                inThongDiepLoi("He thong du lieu Bac Si khong kha dung. Khong the kiem tra MaBS moi.");
                break;
            }
            if (bsKiemTra != NULL) {
                printf("LOI: Ma Bac Si moi '%s' da ton tai. Vui long chon ma khac.\n", bufferNhap);
            }
            else {

                if (bbBacSi != NULL) {
                    xoaKhoiBangBam(bbBacSi, lkTamSua.maBacSi);
                }
                strncpy(lkTamSua.maBacSi, bufferNhap, sizeof(lkTamSua.maBacSi) - 1);
                lkTamSua.maBacSi[sizeof(lkTamSua.maBacSi) - 1] = '\0';
                inThongDiepThanhCong("Ma Bac Si se duoc cap nhat thanh '%s' khi ban luu thay doi.", lkTamSua.maBacSi);
            }
            break;
        case 2: // Sua ma khoa phong
            printf("Nhap Ma Khoa Phong moi: ");
            docDongAnToan(bufferNhap, sizeof(bufferNhap), stdin);
            if (strlen(bufferNhap) > 0) {
                for (int k = 0; bufferNhap[k]; ++k) bufferNhap[k] = toupper(bufferNhap[k]);
                const KhoaPhong* kp = bbKhoaPhong ? timKhoaPhongTheoMaBangBam(bbKhoaPhong, bufferNhap) : timKhoaPhongTheoMa(dsKhoaPhong, soLuongKhoaPhong, bufferNhap);
                if (kp) strcpy(lkTamSua.maKhoaPhong, kp->maKhoaPhong);
                else printf("CANH BAO: Ma Khoa Phong '%s' khong tim thay. Giu nguyen ma cu.\n", bufferNhap);
            }
            else printf("LOI: Ma Khoa Phong khong duoc de trong. Giu nguyen ma cu.\n");
            break;
        case 3: // Sua ly do kham
            printf("Nhap Ly Do Kham moi: ");
            docDongAnToan(lkTamSua.lyDoKham, sizeof(lkTamSua.lyDoKham), stdin);
            if (strlen(lkTamSua.lyDoKham) == 0) {
                printf("LOI: Ly Do Kham khong duoc de trong. Khoi phuc gia tri cu.\n");
                strcpy(lkTamSua.lyDoKham, lkCanSua->lyDoKham);
            }
            break;
        case 4: // Sua chuan doan
            printf("Nhap Chan Doan moi : ");
            docDongAnToan(lkTamSua.chanDoan, sizeof(lkTamSua.chanDoan), stdin);
            if (lkTamSua.trangThai == LK_DA_HOAN_THANH && strlen(lkTamSua.chanDoan) == 0) {
                printf("LOI: Chan Doan khong duoc de trong khi Da Hoan Thanh. Khoi phuc gia tri cu.\n");
                strcpy(lkTamSua.chanDoan, lkCanSua->chanDoan);
            }
            else if (strlen(lkTamSua.chanDoan) == 0) {
                strcpy(lkTamSua.chanDoan, "CHUA_KHAM");
            }
            break;
        case 5: // Sua phuong phap dieu tri
            printf("Nhap Phuong Phap Dieu Tri moi:  ");
            docDongAnToan(lkTamSua.phuongPhapDieuTri, sizeof(lkTamSua.phuongPhapDieuTri), stdin);
            if (lkTamSua.trangThai == LK_DA_HOAN_THANH && strlen(lkTamSua.phuongPhapDieuTri) == 0) {
                printf("LOI: Phuong Phap Dieu Tri khong duoc de trong khi Da Hoan Thanh. Khoi phuc gia tri cu.\n");
                strcpy(lkTamSua.phuongPhapDieuTri, lkCanSua->phuongPhapDieuTri);
            }
            else if (strlen(lkTamSua.phuongPhapDieuTri) == 0) {
                strcpy(lkTamSua.phuongPhapDieuTri, "CHUA_KHAM");
            }
            break;
        case 6: // Sua ghi chu
            printf("Nhap Ghi Chu Bac Si moi:  ");
            docDongAnToan(lkTamSua.ghiChuBacSi, sizeof(lkTamSua.ghiChuBacSi), stdin);
            if (strlen(lkTamSua.ghiChuBacSi) == 0) strcpy(lkTamSua.ghiChuBacSi, "Khong");
            break;
        case 7: // Sua trang thai
            printf("Chon Trang Thai moi (DangCho, DangKham, DaHoanThanh, DaHuy, DaLo): ");
            docDongAnToan(bufferNhap, sizeof(bufferNhap), stdin);
            if (strlen(bufferNhap) > 0) {
                TrangThaiLanKham ttMoi = stringToTrangThaiLanKham(bufferNhap);

                if (ttMoi == LK_DANG_CHO && my_strcasecmp(bufferNhap, "DangCho") != 0 && strlen(bufferNhap) > 0) {
                    printf("CANH BAO: Trang thai nhap khong hop le. Giu nguyen trang thai cu.\n");
                }
                else {
                    lkTamSua.trangThai = ttMoi;

                    if (ttMoi == LK_DANG_CHO || ttMoi == LK_DANG_KHAM) {
                        strcpy(lkTamSua.chanDoan, "CHUA_KHAM");
                        strcpy(lkTamSua.phuongPhapDieuTri, "CHUA_KHAM");
                        strcpy(lkTamSua.maBacSi, "CHUA_GAN");

                    }
                }
            }
            else {
                printf("LOI: Trang thai khong duoc de trong. Giu nguyen trang thai cu.\n");
            }
            break;
        case 0: // Hoan tat va luu

            if (strlen(lkTamSua.maDinhDanh) == 0) {
                inThongDiepLoi("LOI HE THONG: Ma Dinh Danh Benh Nhan bi rong. Vui long bao cao loi nay.");
                luaChonSuaNoiBo = -1;
                dungManHinhCho();
                break;
            }
            if (strlen(lkTamSua.maKhoaPhong) == 0 || strcmp(lkTamSua.maKhoaPhong, "CHUA_GAN") == 0) {
                printf("LOI KHI LUU: Ma Khoa Phong khong duoc de trong/CHUA_GAN. Vui long sua lai.\n");
                luaChonSuaNoiBo = -1; dungManHinhCho(); break;
            }
            if (strlen(lkTamSua.lyDoKham) == 0) {
                printf("LOI KHI LUU: Ly Do Kham khong duoc de trong. Vui long sua lai.\n");
                luaChonSuaNoiBo = -1; dungManHinhCho(); break;
            }

            if (lkTamSua.trangThai == LK_DA_HOAN_THANH) {
                if (strlen(lkTamSua.chanDoan) == 0 || strcmp(lkTamSua.chanDoan, "CHUA_KHAM") == 0 || my_strcasecmp(lkTamSua.chanDoan, "CHUA_NHAP_CD_DOC") == 0 || my_strcasecmp(lkTamSua.chanDoan, "CHUA_NHAP_CD_GHI") == 0) {
                    printf("LOI KHI LUU: Chan Doan khong duoc de trong/mac dinh cho lan kham da hoan thanh. Vui long sua lai.\n");
                    luaChonSuaNoiBo = -1; dungManHinhCho(); break;
                }
                if (strlen(lkTamSua.phuongPhapDieuTri) == 0 || strcmp(lkTamSua.phuongPhapDieuTri, "CHUA_KHAM") == 0 || my_strcasecmp(lkTamSua.phuongPhapDieuTri, "CHUA_NHAP_PPTT_DOC") == 0 || my_strcasecmp(lkTamSua.phuongPhapDieuTri, "CHUA_NHAP_PPTT_GHI") == 0) {
                    printf("LOI KHI LUU: Phuong Phap Dieu Tri khong duoc de trong/mac dinh. Vui long sua lai.\n");
                    luaChonSuaNoiBo = -1; dungManHinhCho(); break;
                }
                if (strlen(lkTamSua.maBacSi) == 0 || strcmp(lkTamSua.maBacSi, "CHUA_GAN") == 0) {
                    printf("LOI KHI LUU: Ma Bac Si khong duoc de trong/CHUA_GAN cho lan kham da hoan thanh. Vui long sua lai.\n");
                    luaChonSuaNoiBo = -1; dungManHinhCho(); break;
                }
            }
            *lkCanSua = lkTamSua;
            ghiDanhSachLanKhamRaFile(fileLanKhamCSV, dsLanKham, soLuongLanKham);
            printf("THANH CONG: Da luu cac thay doi cho Lan Kham Ma: %d.\n", maLanKhamCanSua);

            break;
        case 9: // Huy bo
            printf("Da huy bo tat ca thay doi.\n");

            return 0;
        default:
            printf("LOI: Lua chon khong hop le.\n");
            dungManHinhCho();
            break;
        }
        if (luaChonSuaNoiBo >= 1 && luaChonSuaNoiBo <= 7 && luaChonSuaNoiBo != 0 && luaChonSuaNoiBo != 9) {
            printf("Thong tin da duoc cap nhat tam thoi. Chon truong khac de sua hoac Luu/Huy.\n");
            dungManHinhCho();
        }
    } while (luaChonSuaNoiBo != 0 && luaChonSuaNoiBo != 9);

    return (luaChonSuaNoiBo == 0);
}
// Ham in danh sach lan kham ra man hinh 
void inDanhSachLanKhamConsole(const LanKham dsLanKham[], int soLuongLanKham,
    const BenhNhan dsBenhNhan[], int soLuongBenhNhan, 
    BangBam* bbBenhNhan) {
    printf("\n--- DANH SACH LAN KHAM BENH ---\n");
    if (soLuongLanKham == 0) {
        printf("Chua co lan kham nao.\n");
        return;
    }
    // ... (Code in tiêu đề bảng như trước) ...
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-3s | %-7s | %-18s | %-25s | %-12s | %-15s | %-18s | %-20s | %-20s | %-10s | %-10s | %-15s | %-20s |\n",
        "STT", "Ma LK", "Ma BN", "Ten Benh Nhan", "Trang Thai", "Muc Do UT", "Ma Khoa Phong", "Ly Do Kham", "Chan Doan", "Tai Kham", "Don Thuoc", "Ma Bac Si", "Ghi Chu BS");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");


    for (int i = 0; i < soLuongLanKham; ++i) {
        const LanKham* lk = &dsLanKham[i];
        char tenBNHienThi[101] = "N/A";
        char tenBNRutGon[26];
        char lyDoKhamRutGon[21];
        char chanDoanRutGon[21];
        char ghiChuBSRutGon[21];
        char maBacSiDisplay[16];


        // Lấy tên bệnh nhân
        if (bbBenhNhan != NULL && strlen(lk->maDinhDanh) > 0) {
            BenhNhan* bn = timBenhNhanTheoMaBangBam(bbBenhNhan, lk->maDinhDanh);
            if (bn != NULL) {
                strncpy(tenBNHienThi, bn->ten, sizeof(tenBNHienThi) - 1);
                tenBNHienThi[sizeof(tenBNHienThi) - 1] = '\0';
            }
        }
        else if (dsBenhNhan != NULL && strlen(lk->maDinhDanh) > 0) { 
            const BenhNhan* bnTuyenTinh = timBenhNhanTheoMaChuoi(dsBenhNhan, soLuongBenhNhan, lk->maDinhDanh);
            if (bnTuyenTinh) {
                strncpy(tenBNHienThi, bnTuyenTinh->ten, sizeof(tenBNHienThi) - 1);
                tenBNHienThi[sizeof(tenBNHienThi) - 1] = '\0';
            }
        }
        else if (strlen(lk->tenBenhNhan) > 0 && strcmp(lk->tenBenhNhan, "Khong Biet") != 0) {
            strncpy(tenBNHienThi, lk->tenBenhNhan, sizeof(tenBNHienThi) - 1);
            tenBNHienThi[sizeof(tenBNHienThi) - 1] = '\0';
        }
        rutGonChuoiHienThi(tenBNRutGon, sizeof(tenBNRutGon), tenBNHienThi, 25);
        rutGonChuoiHienThi(lyDoKhamRutGon, sizeof(lyDoKhamRutGon), lk->lyDoKham, 20);


        char ngayTaiKhamStr[12] = "-";
        if (lk->coLichTaiKham && lk->ngayTaiKham.ngay > 0) {
            snprintf(ngayTaiKhamStr, sizeof(ngayTaiKhamStr), "%02d/%02d/%04d", lk->ngayTaiKham.ngay, lk->ngayTaiKham.thang, lk->ngayTaiKham.nam);
        }

        
        const char* cdDisplay = "N/A";
        const char* ppdtDisplay = "N/A"; 
        const char* bsDisplay = "N/A";
        const char* gcDisplay = "Khong";

        if (lk->trangThai == LK_DA_HOAN_THANH || lk->trangThai == LK_DA_HUY || lk->trangThai == LK_DA_LO) {
            cdDisplay = (strlen(lk->chanDoan) > 0 && strcmp(lk->chanDoan, "CHUA_KHAM") != 0) ? lk->chanDoan : "CHUA_NHAP";
            bsDisplay = (strlen(lk->maBacSi) > 0 && strcmp(lk->maBacSi, "CHUA_GAN") != 0) ? lk->maBacSi : "N/A";
            gcDisplay = (strlen(lk->ghiChuBacSi) > 0 && strcmp(lk->ghiChuBacSi, "Khong") != 0) ? lk->ghiChuBacSi : "Khong";
        }
        rutGonChuoiHienThi(chanDoanRutGon, sizeof(chanDoanRutGon), cdDisplay, 20);
        rutGonChuoiHienThi(maBacSiDisplay, sizeof(maBacSiDisplay), bsDisplay, 15);
        rutGonChuoiHienThi(ghiChuBSRutGon, sizeof(ghiChuBSRutGon), gcDisplay, 20);


        printf("| %-3d | %-7d | %-18s | %-25s | %-12s | %-15s | %-18s | %-20s | %-20s | %-10s | %-10s | %-15s | %-20s |\n",
            i + 1,
            lk->maLanKham,
            lk->maDinhDanh,
            tenBNRutGon,
            trangThaiLanKhamToString(lk->trangThai),
            mucDoUuTienToString(lk->mucDoUuTien),
            lk->maKhoaPhong,
            lyDoKhamRutGon,
            chanDoanRutGon,
            ngayTaiKhamStr,
            lk->coDonThuoc ? "Co" : "Khong",
            maBacSiDisplay,
            ghiChuBSRutGon
        );
    }
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}


