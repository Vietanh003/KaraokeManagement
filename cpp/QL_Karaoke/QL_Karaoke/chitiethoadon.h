#pragma once
#ifndef CHITIETHOADON_H
#define CHITIETHOADON_H

#include "common.h"
#include "hanghoa.h"  // nếu cần truy cập struct HangHoa

typedef struct ChiTietHoaDon {
    wchar_t ma_hoa_don[MAX_ID];   // mã hóa đơn mà chi tiết này thuộc về
    wchar_t ma_hang[MAX_ID];      // mã hàng hóa trong chi tiết hóa đơn
    int so_luong;                 // số lượng hàng mua
    float don_gia;               // đơn giá tại thời điểm mua (có thể khác giá chuẩn)
    float thanh_tien;            // thành tiền = so_luong * don_gia (có thể tính động hoặc lưu sẵn)
} ChiTietHoaDon;

int soSanhChiTietHoaDon(const void* a, const void* b);
Node* docChiTietHoaDonTuFile(const wchar_t* ten_file);
void ghiChiTietHoaDonRaFile(Node* goc, const wchar_t* ten_file);
void themChiTietHoaDonMoi(Node** goc, const wchar_t* ma_hoa_don, const wchar_t* ma_hang, int so_luong, double don_gia, const wchar_t* ten_file);
void hienThiTatCaChiTietHoaDon(Node* goc);
void hienThiChiTietHoaDonTheoMaHoaDon(Node* goc, const wchar_t* ma_hoa_don);

#endif

