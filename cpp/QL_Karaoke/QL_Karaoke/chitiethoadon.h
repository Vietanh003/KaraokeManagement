#pragma once
#ifndef CHITIETHOADON_H
#define CHITIETHOADON_H

#include "common.h"
#include "hanghoa.h"  

typedef struct ChiTietHoaDon {
    wchar_t ma_hoa_don[MAX_ID];   
    wchar_t ma_hang[MAX_ID];
    int so_luong;                 
    float don_gia;              
    float thanh_tien;        
} ChiTietHoaDon;

int soSanhChiTietHoaDon(const void* a, const void* b);
Node* docChiTietHoaDonTuFile(const wchar_t* ten_file);
void ghiChiTietHoaDonRaFile(Node* goc, const wchar_t* ten_file);
void themChiTietHoaDonMoi(Node** goc, const wchar_t* ma_hoa_don, const wchar_t* ma_hang, int so_luong, double don_gia, const wchar_t* ten_file);
void hienThiTatCaChiTietHoaDon(Node* goc);
void hienThiChiTietHoaDonTheoMaHoaDon(Node* goc, const wchar_t* ma_hoa_don);

#endif

