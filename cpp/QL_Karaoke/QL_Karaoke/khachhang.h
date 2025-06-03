#ifndef KHACHHANG_H
#define KHACHHANG_H

#include "common.h"

typedef struct KhachHang {
    wchar_t ma_khach_hang[MAX_ID];
    wchar_t ten[MAX_NAME];
    wchar_t so_dien_thoai[15];
    Node* ds_hoa_don;
} KhachHang;

int soSanhKhachHang(const void* a, const void* b);
Node* docKhachHangTuFile(const wchar_t* ten_file);
Node* themKhachHangMoi(Node* goc, const wchar_t* ma_khach_hang, const wchar_t* ten, const wchar_t* so_dien_thoai, const wchar_t* ten_file);
void lietKeHoaDonTheoKhachHang(Node* goc, const wchar_t* ma_khach_hang);
void hienThiTatCaKhachHang(Node* goc);
void hienThiKhachHang(Node* node);
const wchar_t* timMaKhachHangTheoSoDienThoai(const wchar_t* so_dien_thoai, const wchar_t* ten_file);
#endif