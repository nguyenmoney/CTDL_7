#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include "structs.h"
#include "hash_table.h"

#define FILE_DON_THUOC_CHI_TIET "data/donthuoc.csv" 

// --- Khai bao cac ham ---

DonThuocChiTiet* docDanhSachDonThuocChiTiet(const char* tenFile, int* soLuongDongConTro);

void ghiDanhSachDonThuocChiTietRaFile(const char* tenFile, const DonThuocChiTiet dsDonThuocChiTiet[], int soLuongDong);

int themDongDonThuocChiTiet(DonThuocChiTiet** dsDTCTConTro, int* soLuongDongConTro, DonThuocChiTiet dtctMoi, const char* tenFile);

int xoaDongThuocTrongDon(DonThuocChiTiet** dsDTCTConTro, int* soLuongDongConTro, int maDonThuocCanTim, const char* maThuocCanXoa, const char* tenFile);

int suaDongThuocTrongDon(DonThuocChiTiet dsDonThuocChiTiet[], int soLuongDong, int maDonThuocCanTim, const char* maThuocCanSua, DonThuocChiTiet thongTinMoi, const char* tenFile);

void inChiTietMotDonThuocConsole(int maDonThuoc,
    const DonThuocChiTiet dsDonThuocChiTiet[], int soLuongDongTongCong,
    const Thuoc dsThuocToanBo[], int soLuongThuocTB);

void inTatCaDonThuocChiTietConsole(const DonThuocChiTiet dsDonThuocChiTiet[], int soLuongDong,
    const Thuoc dsThuocToanBo[], int soLuongThuocTB);

void quanLyCapNhatChiTietDonThuoc(int maDonThuocDeCapNhat,
    DonThuocChiTiet** dsDTCTConTro, int* soLuongDongConTro,
    const Thuoc dsThuocToanBo[], int soLuongThuocTB,
    BangBam* bbThuoc, const char* tenFileCSV);

#endif // PRESCRIPTION_H