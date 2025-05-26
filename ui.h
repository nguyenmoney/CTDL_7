#ifndef UI_H
#define UI_H

#include "structs.h"
#include "patient.h"
#include "examination.h"
#include "department.h"
#include "doctor.h"
#include "medicine.h"
#include "prescription.h"
#include "priority_queue.h"
#include "csv_handler.h" 
#include "hash_table.h" 
// --- Khai bao cac ham giao dien ---

void hienThiMenuChinh();

void xuLyLuaChonMenuChinh(
    int choice,
    BenhNhan** dsBenhNhan, int* soLuongBenhNhan, BangBam** bbBenhNhan_ptr, const char* fileBN,
    LanKham** dsLanKham, int* soLuongLanKham, BangBam** bbLanKham_ptr, const char* fileLK,
    KhoaPhong** dsKhoaPhong, int* soLuongKhoaPhong, BangBam** bbKhoaPhong_ptr, const char* fileKP,
    BacSi** dsBacSi, int* soLuongBacSi, BangBam** bbBacSi_ptr, const char* fileBS,
    Thuoc** dsThuoc, int* soLuongThuoc, BangBam** bbThuoc_ptr, const char* fileT,
    DonThuocChiTiet** dsDonThuocChiTiet, int* soLuongDTCT, const char* fileDTCT,
    BangBam* bbHangDoiTheoKhoa 
);


// --- Cac ham menu con ---

void menuQuanLyBenhNhan(BenhNhan** dsBenhNhan, int* soLuongBenhNhan, BangBam** bbBenhNhan_ptr, const char* tenFileCSV);

void menuQuanLyLanKham(
    LanKham** dsLanKham, int* soLuongLanKham, BangBam** bbLanKham_ptr,
    BenhNhan* dsBenhNhan, int soLuongBenhNhan, BangBam* bbBenhNhan,
    BacSi* dsBacSi, int soLuongBacSi, BangBam* bbBacSi,
    KhoaPhong* dsKhoaPhong, int soLuongKhoaPhong, BangBam* bbKhoaPhong,
    const char* fileLanKhamCSV);

void menuQuanLyBacSi(
    BacSi** dsBacSi, int* soLuongBacSi,
    BangBam** bbBacSi_ptr, 
    const char* tenFileCSV,
    KhoaPhong* dsKhoaPhong, int soLuongKhoaPhong, BangBam* bbKhoaPhong
);

void menuQuanLyThuoc(Thuoc** dsThuoc, int* soLuongThuoc, BangBam** bbThuoc_ptr, const char* tenFileCSV); 

void menuQuanLyKhoaPhong(KhoaPhong** dsKhoaPhong, int* soLuongKhoaPhong, BangBam** bbKhoaPhong_ptr, const char* tenFileCSV);


// --- Cac ham tien ich giao dien ---
void xoaManHinhConsole();
void dungManHinhCho();
int nhapSoNguyenCoGioiHan(const char* prompt, int min, int max);
int nhapSoNguyenKhongAm(const char* prompt);
void inThongDiepLoi(const char* message);
void inThongDiepThanhCong(const char* message);

// Ham hien thi thong tin bang bam
void hienThiThongTinBangBam(BangBam* bb, const char* tenBangBam);
void menuXemThongTinBangBam(
    BangBam* bbBenhNhan,
    BangBam* bbBacSi,
    BangBam* bbKhoaPhong,
    BangBam* bbThuoc,
    BangBam* bbLanKham
);


#endif // UI_H