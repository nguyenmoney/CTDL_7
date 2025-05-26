#ifndef QLPK_STRUCTS_FULL_H
#define QLPK_STRUCTS_FULL_H

#include <time.h> 

#define MAX_LEN_MA_DINH_DANH_BN 20 
#define MAX_LEN_MA_BAC_SI 20       
#define MAX_LEN_MA_KHOA_PHONG 20   
#define MAX_LEN_MA_THUOC 30        

typedef enum {
    LK_DANG_CHO,
    LK_DANG_KHAM,
    LK_DA_HOAN_THANH,
    LK_DA_HUY,
    LK_DA_LO
} TrangThaiLanKham;

// --- Cau truc Date ---
typedef struct {
    int ngay;
    int thang;
    int nam;
} Date;

// --- Enum Muc Do Uu Tien ---
typedef enum {
    CAP_CUU = 0,
    KHAN_CAP,
    THONG_THUONG,
    TAI_KHAM,
    UU_TIEN_KHAC,
    SO_MUC_UU_TIEN
} MucDoUuTien;

// --- Enum Gioi Tinh ---
typedef enum {
    GT_NAM,
    GT_NU,
    GT_KHAC
} GioiTinh;

// --- Enum Trang Thai Lich Hen ---
typedef enum {
    HEN_MOI_DAT,
    HEN_DA_XAC_NHAN,
    HEN_DA_HUY,
    HEN_DA_HOAN_THANH,
    HEN_KHONG_DEN
} TrangThaiLichHen;

// === Cau truc Khoa/Phong Kham ===
typedef struct {
    char maKhoaPhong[MAX_LEN_MA_KHOA_PHONG]; 
    char tenKhoaPhong[100];
    char viTri[100];
    char moTa[200];
} KhoaPhong;

// === Cau truc Bac Si ===
typedef struct {
    char maBacSi[MAX_LEN_MA_BAC_SI]; 
    char tenBacSi[100];
    char chuyenKhoa[100];
    char soDienThoai[15];
    char email[100];
    char maKhoaPhong[MAX_LEN_MA_KHOA_PHONG]; 
} BacSi;

// === Cau truc Benh Nhan ===
typedef struct {
    char maDinhDanh[MAX_LEN_MA_DINH_DANH_BN];
    char bhyt[16];
    char soDienThoai[15];
    char ten[100];
    Date ngaySinh;
    int tuoi;
    GioiTinh gioiTinh;
    char diaChi[200];
    char tieuSuBenhLy[1000];
    time_t ngayTaoHoSo;
} BenhNhan;

// === Cau truc Lan Kham ===
typedef struct {
    int maLanKham;                            
    char maDinhDanh[MAX_LEN_MA_DINH_DANH_BN];
    char tenBenhNhan[100];
    char maBacSi[MAX_LEN_MA_BAC_SI];          
    char maKhoaPhong[MAX_LEN_MA_KHOA_PHONG];  
    time_t ngayGioKham;
    char lyDoKham[500];
    char chanDoan[500];
    char phuongPhapDieuTri[1000];
    char ghiChuBacSi[500];
    Date ngayTaiKham;
    int coLichTaiKham;
    MucDoUuTien mucDoUuTien;
    time_t gioDangKyThanhCong;
    int coDonThuoc;
    TrangThaiLanKham trangThai;
    int maDonThuocChiTiet; 
} LanKham;

// === Cau truc Lich Hen ===
typedef struct {
    int maLichHen;
    char maDinhDanh[MAX_LEN_MA_DINH_DANH_BN];
    char maBacSi[MAX_LEN_MA_BAC_SI];          
    char maKhoaPhong[MAX_LEN_MA_KHOA_PHONG]; 
    time_t ngayGioHen;
    char lyDoHen[200];
    TrangThaiLichHen trangThai;
    char ghiChu[300];
} LichHen;

// === Cau truc Thuoc ===
typedef struct {
    char maThuoc[MAX_LEN_MA_THUOC]; 
    char tenThuoc[150];
} Thuoc;

// === Cau truc Chi Tiet Don Thuoc ===
typedef struct {
    int maDonThuocChiTiet;             
    char maThuoc[MAX_LEN_MA_THUOC]; 
    int soLuong;
    char lieuDung[100];
    char cachDung[100];
} DonThuocChiTiet;

#endif // QLPK_STRUCTS_FULL_H