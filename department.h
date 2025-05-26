#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include "structs.h"
#include "hash_table.h"
#include <stdio.h>

#define FILE_KHOA_PHONG "data/khoaphong.csv"

// --- Khai bao cac ham ---

KhoaPhong* docDanhSachKhoaPhong(const char* tenFile, int* soLuong);

void ghiDanhSachKhoaPhongRaFile(const char* tenFile, const KhoaPhong dsKhoaPhong[], int soLuong);

int themKhoaPhong(KhoaPhong** dsKhoaPhongConTro, int* soLuongConTro,
    BangBam** bbKhoaPhong_ptr,
    KhoaPhong khoaPhongMoi, const char* tenFile);

int suaThongTinKhoaPhong(KhoaPhong dsKhoaPhong[], int soLuong, const char* maKhoaPhongCanSua, KhoaPhong thongTinMoi, const char* tenFile);

int xoaKhoaPhong(KhoaPhong** dsKhoaPhongConTro, int* soLuongConTro,
    BangBam* bbKhoaPhong,
    const char* maKhoaPhongCanXoa, const char* tenFile);

void inDanhSachKhoaPhongConsole(const KhoaPhong dsKhoaPhong[], int soLuong);

KhoaPhong* timKhoaPhongTheoMa(const KhoaPhong dsKhoaPhong[], int soLuong, const char* maKhoaPhong);

KhoaPhong* timKhoaPhongTheoMaBangBam(BangBam* bbKhoaPhong, const char* maKhoaPhong);

int nhapThongTinKhoaPhongTuBanPhim(KhoaPhong* kp,
    BangBam* bbKhoaPhongHienCo,
    const KhoaPhong dsKhoaPhongHienCo[], int soLuongHienCo);
void timKiemKhoaPhongTuyenTinh(const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong,
    const char* tieuChi, const char* giaTriTimKiem,
    KhoaPhong** ketQuaTimKiem, int* soLuongKetQua);

#endif // DEPARTMENT_H