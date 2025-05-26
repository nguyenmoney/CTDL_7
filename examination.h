#ifndef EXAMINATION_H
#define EXAMINATION_H

#include "structs.h"
#include "patient.h" 
#include "hash_table.h" 
#include "priority_queue.h"

#define FILE_LAN_KHAM "data/lankham.csv"

// --- Khai bao cac ham --- 

LanKham* docDanhSachLanKham(const char* tenFile, int* soLuong);

void ghiDanhSachLanKhamRaFile(const char* tenFile, const LanKham dsLanKham[], int soLuong);

int taoMaLanKhamMoi(const LanKham dsLanKham[], int soLuong); 

int dangKyKhamMoi(LanKham** dsLanKhamConTro, int* soLuongLanKhamConTro,
    BenhNhan** dsBenhNhanConTro, int* soLuongBenhNhanConTro,
    BangBam* bbBenhNhan,
    KhoaPhong* dsKhoaPhong, int soLuongKhoaPhong,
    BangBam* bbKhoaPhong, 
    BangBam** bbLanKham_ptr, 
    BangBam* bbHangDoiTheoKhoa, 
    const char* fileLanKhamCSV, const char* fileBenhNhanCSV);

int capNhatThongTinLanKham(LanKham dsLanKham[], int soLuongLanKham, int maLanKhamCanCapNhat,
    const BacSi dsBacSi[], int soLuongBacSi,
    const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong,
    BangBam* bbBacSi, 
    BangBam* bbKhoaPhong, 
    const char* fileLanKhamCSV);

int themLanKham(LanKham** dsLanKhamConTro, int* soLuongConTro,
    BangBam** bbLanKham_ptr,
    LanKham lanKhamMoi, const char* tenFile);

int xoaLanKham(LanKham** dsLanKhamConTro, int* soLuongConTro,
    BangBam* bbLanKham,
    int maLanKhamCanXoa, const char* tenFile);

void inDanhSachLanKhamConsole(const LanKham dsLanKham[], int soLuongLanKham,
    const BenhNhan dsBenhNhan[], int soLuongBenhNhan,
    BangBam* bbBenhNhan); 

void timKiemLanKham(const LanKham dsLanKham[], int soLuongLanKham,
    const char* tieuChi, const char* giaTriTimKiem,
    LanKham** ketQuaTimKiem, int* soLuongKetQua);

LanKham* timLanKhamTheoMaBangBam(BangBam* bbLanKham, int maLK);

int suaChiTietLanKham(LanKham dsLanKham[], int soLuongLanKham, int maLanKhamCanSua,
    const BacSi dsBacSi[], int soLuongBacSi,
    const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong,
    BangBam* bbBacSi,  
    BangBam* bbKhoaPhong, 
    const char* fileLanKhamCSV);

const char* trangThaiLanKhamToString(TrangThaiLanKham tt);
TrangThaiLanKham stringToTrangThaiLanKham(const char* str);
const char* mucDoUuTienToString(MucDoUuTien md);
MucDoUuTien stringToMucDoUuTien(const char* str);

#endif // EXAMINATION_H