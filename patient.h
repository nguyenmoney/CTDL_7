#ifndef PATIENT_H
#define PATIENT_H

#include "structs.h"
#include "hash_table.h"
#include <stdio.h>

#define FILE_BENH_NHAN "data/benhnhan.csv"

BenhNhan* docDanhSachBenhNhan(const char* tenFile, int* soLuongBenhNhan);
void ghiDanhSachBenhNhanRaFile(const char* tenFile, const BenhNhan dsBenhNhan[], int soLuongBenhNhan);

int themBenhNhan(BenhNhan** dsBenhNhanConTro, int* soLuongBenhNhan,
    BangBam** bbBenhNhan_ptr,
    BenhNhan benhNhanMoi, const char* tenFile);

int suaThongTinBenhNhan(BenhNhan dsBenhNhan[], int soLuongBenhNhan, char* maDinhDanhCanSua, BenhNhan thongTinMoi, const char* tenFile);

int xoaBenhNhan(BenhNhan** dsBenhNhanConTro, int* soLuongBenhNhan, BangBam* bbBenhNhan, char* maDinhDanhCanXoa, const char* tenFile);

void inDanhSachBenhNhanConsole(const BenhNhan dsBenhNhan[], int soLuongBenhNhan);

void timKiemBenhNhan(const BenhNhan dsBenhNhan[], int soLuongBenhNhan,
    const char* tieuChi, const char* giaTriTimKiem,
    BenhNhan** ketQuaTimKiem, int* soLuongKetQua);

int nhapThongTinBacSiTuBanPhim(BacSi* bs,
    BangBam* bbBacSiHienCo,
    const BacSi dsBacSiHienCo[], int soLuongBSHienCo,
    const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong);

const BenhNhan* timBenhNhanTheoMaChuoi(const BenhNhan dsBenhNhan[], int soLuong, const char* maDD_str);

BenhNhan* timBenhNhanTheoMaBangBam(BangBam* bb, const char* maDD_str);


const char* gioiTinhToString(GioiTinh gt);
GioiTinh stringToGioiTinh(const char* str);

#endif // PATIENT_H