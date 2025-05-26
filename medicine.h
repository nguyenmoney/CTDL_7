#ifndef MEDICINE_H
#define MEDICINE_H

#include "structs.h"
#include "hash_table.h" 
#include <stdio.h>

#define FILE_THUOC "data/thuoc.csv"

// --- Khai bao cac ham ---

Thuoc* docDanhSachThuoc(const char* tenFile, int* soLuong);

void ghiDanhSachThuocRaFile(const char* tenFile, const Thuoc dsThuoc[], int soLuong);

int themThuoc(Thuoc** dsThuocConTro, int* soLuongConTro,
    BangBam** bbThuoc_ptr,
    Thuoc thuocMoi, const char* tenFile);

int suaThongTinThuoc(Thuoc dsThuoc[], int soLuong, const char* maThuocCanSua, Thuoc thongTinMoi, const char* tenFile);

int xoaThuoc(Thuoc** dsThuocConTro, int* soLuongConTro,
    BangBam* bbThuoc,
    const char* maThuocCanXoa, const char* tenFile);

void inDanhSachThuocConsole(const Thuoc dsThuoc[], int soLuong);

Thuoc* timThuocTheoMa(const Thuoc dsThuoc[], int soLuong, const char* maThuoc);

Thuoc* timThuocTheoMaBangBam(BangBam* bbThuoc, const char* maThuoc);

int nhapThongTinThuocTuBanPhim(Thuoc* tConTro,
    BangBam* bbThuocHienCo,
    const Thuoc dsThuocHienCo[], int soLuongThuocHienCo);

void timKiemThuocTheoTenTuyenTinh(const Thuoc dsThuoc[], int soLuongThuoc,
    const char* tenThuocTimKiem,
    Thuoc** ketQuaTimKiem, int* soLuongKetQua);
#endif // MEDICINE_H