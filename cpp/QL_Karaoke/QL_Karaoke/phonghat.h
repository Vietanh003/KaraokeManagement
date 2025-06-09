#ifndef PHONGHAT_H
#define PHONGHAT_H
#define MAX_ID 32
#include "common.h"

typedef struct PhongHat {
    wchar_t ma_phong[MAX_ID];
    wchar_t loai_phong[20];
    int so_lan_thue;
    int trang_thai; 
} PhongHat;

int soSanhPhongHat(const void* a, const void* b);
Node* docPhongHatTuFile(const wchar_t* ten_file);
void ghiPhongHatRaFile(Node* goc, const wchar_t* ten_file);
void themPhongHatMoi(Node** goc, const wchar_t* ma_phong, const wchar_t* loai_phong, const wchar_t* ten_file);
void hienThiTatCaPhongHat(Node* goc);
void thongKeTop3Phong(Node* goc);
void tangSoLanThuePhongHat(Node* goc, const wchar_t* ma_phong, const wchar_t* ten_file);
int kiemTraPhongTrong(Node* goc, const wchar_t* ma_phong);
void capNhatTrangThaiPhong(Node* goc, const wchar_t* ma_phong, const wchar_t* trang_thai, const wchar_t* ten_file);
#endif