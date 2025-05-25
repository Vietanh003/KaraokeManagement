#ifndef HANGHOA_H
#define HANGHOA_H

#include "common.h"

typedef struct HangHoa {
    wchar_t ma_hang[MAX_ID];
    wchar_t ten_hang[MAX_NAME];
    double gia_tien;
    double giam_gia;
} HangHoa;

int soSanhHangHoa(const void* a, const void* b);
Node* docHangHoaTuFile(const wchar_t* ten_file);
void ghiHangHoaRaFile(Node* goc, const wchar_t* ten_file);
void themHangHoaMoi(Node** goc, const wchar_t* ten_hang, double gia_tien, double giam_gia, const wchar_t* ten_file);
void hienThiTatCaHangHoa(Node* goc);
void chinhSuaHangHoa(Node* goc, const wchar_t* ma_hang_can_sua);
Node* xoaHangHoa(Node* goc, const wchar_t* ma_hang_can_xoa, int* da_xoa);
HangHoa* timHangHoaTheoMa(Node* goc, const wchar_t* ma_hang_can_tim);
#endif