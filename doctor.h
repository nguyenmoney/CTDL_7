#ifndef DOCTOR_H
#define DOCTOR_H

#include "structs.h"
#include "hash_table.h" 
#include <stdio.h>

#define FILE_BAC_SI "data/bacsi.csv"

BacSi* docDanhSachBacSi(const char* tenFile, int* soLuong);

void ghiDanhSachBacSiRaFile(const char* tenFile, const BacSi dsBacSi[], int soLuong);

int themBacSi(BacSi** dsBacSiConTro, int* soLuongConTro,
    BangBam** bbBacSi_ptr,
    BacSi bacSiMoi, const char* tenFile);

int suaThongTinBacSi(BacSi dsBacSi[], int soLuong, const char* maBacSiCanSua, BacSi thongTinMoi,
    const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong, 
    const char* tenFile);

int xoaBacSi(BacSi** dsBacSiConTro, int* soLuongConTro,
    BangBam* bbBacSi,
    const char* maBacSiCanXoa, const char* tenFile);

void inDanhSachBacSiConsole(const BacSi dsBacSi[], int soLuong);

int nhapThongTinBacSiTuBanPhim(BacSi* bs,
    BangBam* bbBacSiHienCo,
    const BacSi dsBacSiHienCo[], int soLuongBSHienCo, 
    const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong); 

BacSi* timBacSiTheoMaBangBam(BangBam* bbBacSi, const char* maBacSi);
void timKiemBacSiTuyenTinh(const BacSi dsBacSi[], int soLuongBacSi,
    const char* tieuChi, const char* giaTriTimKiem,
    BacSi** ketQuaTimKiem, int* soLuongKetQua);

BacSi* timBacSiTheoSDT(const BacSi dsBacSi[], int soLuong, const char* soDienThoai);
BacSi* timBacSiTheoEmail(const BacSi dsBacSi[], int soLuong, const char* email);
#endif // DOCTOR_H