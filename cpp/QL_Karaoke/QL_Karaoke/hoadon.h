#ifndef HOADON_H
#define HOADON_H

#include "common.h"

typedef struct HoaDon {
    wchar_t ma_hoa_don[MAX_ID];
    wchar_t ma_khach_hang[MAX_ID];
    wchar_t ma_phong[MAX_ID];
    double tong_tien;
    time_t ngay_thue;
    time_t gio_thue;
    time_t gio_ra;
} HoaDon;

int soSanhHoaDon(const void* a, const void* b);
Node* docHoaDonTuFile(const wchar_t* ten_file);
void themHoaDon(Node** goc_hoa_don, Node* cay_khach_hang, Node* cay_phong_hat, Node* cay_hang_hoa, Node** cay_chi_tiet_hoa_don, const wchar_t* ten_file_hoa_don, const wchar_t* ten_file_chi_tiet);
void timHoaDonTheoMa(Node* goc, Node* cay_chi_tiet_hoa_don, const wchar_t* ma_hoa_don);
void sapXepHoaDonTheoTongTien(Node* goc, Node* cay_chi_tiet_hoa_don);
void hienThiTatCaHoaDon(Node* goc, Node* cay_chi_tiet_hoa_don);
void inThongTinHoaDon(Node* node, Node* cay_chi_tiet_hoa_don);
int soSanhHoaDonTheoMaKhachHang(const void* a, const void* b);
void sapXepHoaDonTheoMaKhachHang(Node* goc, Node* cay_chi_tiet_hoa_don);
#endif