#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "structs.h"    
#include "hash_table.h" 
#include <stdio.h>

typedef struct {
    LanKham* dsChoKham;
    int soLuongHienTai;
    int dungLuong;
} HangDoiUuTienMotKhoa;

// --- Khai báo các hàm ---

void khoiTaoHangDoiMotKhoa(HangDoiUuTienMotKhoa* hqmk);

int themVaoHangDoiTheoKhoa(BangBam* bbHangDoiTheoKhoa, const char* maKhoaPhong, LanKham lanKhamMoi);

int layBenhNhanTiepTheoTheoKhoa(BangBam* bbHangDoiTheoKhoa, const char* maKhoaPhong, LanKham* lanKhamLayRa);

int laHangDoiTheoKhoaRong(BangBam* bbHangDoiTheoKhoa, const char* maKhoaPhong);

void inHangDoiMotKhoaConsole(BangBam* bbHangDoiTheoKhoa, const char* maKhoaPhong,
    BangBam* bbBenhNhan,
    const BenhNhan dsBenhNhanToanBo[], int soLuongBNTB);

void giaiPhongDuLieuHangDoiMotKhoa(HangDoiUuTienMotKhoa* hqmk);

void giaiPhongTatCaHangDoiTheoKhoa(BangBam* bbHangDoiTheoKhoa);

#endif // PRIORITY_QUEUE_H