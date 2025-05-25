#include "chitiethoadon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int soSanhChiTietHoaDon(const void* a, const void* b) {
    int cmp = wcscmp(((ChiTietHoaDon*)a)->ma_hoa_don, ((ChiTietHoaDon*)b)->ma_hoa_don);
    if (cmp == 0) {
        return wcscmp(((ChiTietHoaDon*)a)->ma_hang, ((ChiTietHoaDon*)b)->ma_hang);
    }
    return cmp;
}

Node* docChiTietHoaDonTuFile(const wchar_t* ten_file) {
    Node* goc = NULL;
    FILE* file;
    errno_t err = _wfopen_s(&file, ten_file, L"r,ccs=UTF-8");
    if (err != 0 || !file) {
        wprintf(L"Không mở được file %ls, tạo file mới...\n", ten_file);
        err = _wfopen_s(&file, ten_file, L"w,ccs=UTF-8");
        if (err == 0 && file) fclose(file);
        return NULL;
    }

    wchar_t ma_hoa_don[MAX_ID], ma_hang[MAX_ID];
    int so_luong;
    double don_gia, thanh_tien;
    while (fwscanf_s(file, L"%ls %ls %d %lf %lf",
        ma_hoa_don, (unsigned)_countof(ma_hoa_don),
        ma_hang, (unsigned)_countof(ma_hang),
        &so_luong, &don_gia, &thanh_tien) == 5) {
        ChiTietHoaDon* cthd = (ChiTietHoaDon*)malloc(sizeof(ChiTietHoaDon));
        if (cthd) {
            wcscpy_s(cthd->ma_hoa_don, MAX_ID, ma_hoa_don);
            wcscpy_s(cthd->ma_hang, MAX_ID, ma_hang);
            cthd->so_luong = so_luong;
            cthd->don_gia = don_gia;
            cthd->thanh_tien = thanh_tien;
            goc = chenNode(goc, cthd, soSanhChiTietHoaDon);
        }
    }

    fclose(file);
    return goc;
}

void ghiChiTietHoaDonRaFileNode(Node* goc, FILE* file) {
    if (!goc) return;
    ghiChiTietHoaDonRaFileNode(goc->left, file);
    ChiTietHoaDon* cthd = (ChiTietHoaDon*)goc->du_lieu;
    fwprintf(file, L"%ls %ls %d %.2f %.2f\n",
        cthd->ma_hoa_don, cthd->ma_hang, cthd->so_luong, cthd->don_gia, cthd->thanh_tien);
    ghiChiTietHoaDonRaFileNode(goc->right, file);
}

void ghiChiTietHoaDonRaFile(Node* goc, const wchar_t* ten_file) {
    if (!goc) return;
    FILE* file;
    errno_t err = _wfopen_s(&file, ten_file, L"w,ccs=UTF-8");
    if (err != 0 || !file) {
        wprintf(L"Không mở được file %ls\n", ten_file);
        return;
    }
    ghiChiTietHoaDonRaFileNode(goc, file);
    fclose(file);
}
void themChiTietHoaDonMoi(Node** goc, const wchar_t* ma_hoa_don, const wchar_t* ma_hang, int so_luong, double don_gia, const wchar_t* ten_file) {
    ChiTietHoaDon* cthd = (ChiTietHoaDon*)malloc(sizeof(ChiTietHoaDon));
    if (!cthd) {
        wprintf(L"Lỗi cấp phát bộ nhớ!\n");
        return;
    }

    wcscpy_s(cthd->ma_hoa_don, MAX_ID, ma_hoa_don);
    wcscpy_s(cthd->ma_hang, MAX_ID, ma_hang);
    cthd->so_luong = so_luong;
    cthd->don_gia = don_gia; // Use the passed don_gia
    cthd->thanh_tien = so_luong * don_gia; // Calculate thanh_tien

    *goc = chenNode(*goc, cthd, soSanhChiTietHoaDon);

    // Save to file
    FILE* file;
    errno_t err = _wfopen_s(&file, ten_file, L"a,ccs=UTF-8");
    if (err != 0 || !file) {
        wprintf(L"Không mở được file %ls để ghi!\n", ten_file);
        free(cthd);
        return;
    }
    fwprintf(file, L"%ls %ls %d %.2f %.2f\n", ma_hoa_don, ma_hang, so_luong, don_gia, cthd->thanh_tien);
    fclose(file);

    wprintf(L"Đã thêm chi tiết hóa đơn cho mã %ls thành công!\n", ma_hoa_don);
}
void hienThiTatCaChiTietHoaDon(Node* goc) {
    if (!goc) {
        wprintf(L"Danh sách chi tiết hóa đơn trống!\n");
        return;
    }

    wprintf(L"\n===== DANH SÁCH CHI TIẾT HÓA ĐƠN =====\n");
    wprintf(L"%-10ls | %-10ls | %8ls | %10ls | %12ls\n",
        L"Mã HĐ", L"Mã hàng", L"Số lượng", L"Đơn giá", L"Thành tiền");
    wprintf(L"----------------------------------------------------\n");
    Node* current = goc;
    while (current) {
        ChiTietHoaDon* cthd = (ChiTietHoaDon*)current->du_lieu;
        wchar_t don_gia_vnd[64], thanh_tien_vnd[64];
        dinhDangVND(cthd->don_gia, don_gia_vnd, sizeof(don_gia_vnd) / sizeof(wchar_t));
        dinhDangVND(cthd->thanh_tien, thanh_tien_vnd, sizeof(thanh_tien_vnd) / sizeof(wchar_t));
        wprintf(L"%-10ls | %-10ls | %8d | %10ls | %12ls\n",
            cthd->ma_hoa_don, cthd->ma_hang, cthd->so_luong, don_gia_vnd, thanh_tien_vnd);
        current = current->left ? current->left : current->right;
    }
    wprintf(L"====================================\n");
}

void hienThiChiTietHoaDonTheoMaHoaDon(Node* goc, const wchar_t* ma_hoa_don) {
    if (!goc) return;
    ChiTietHoaDon* cthd = (ChiTietHoaDon*)goc->du_lieu;
    if (wcscmp(cthd->ma_hoa_don, ma_hoa_don) == 0) {
        wchar_t don_gia_vnd[64], thanh_tien_vnd[64];
        dinhDangVND(cthd->don_gia, don_gia_vnd, sizeof(don_gia_vnd) / sizeof(wchar_t));
        dinhDangVND(cthd->thanh_tien, thanh_tien_vnd, sizeof(thanh_tien_vnd) / sizeof(wchar_t));
        wprintf(L"  %-10ls | %8d | %10ls | %12ls\n",
            cthd->ma_hang, cthd->so_luong, don_gia_vnd, thanh_tien_vnd);
    }
    hienThiChiTietHoaDonTheoMaHoaDon(goc->left, ma_hoa_don);
    hienThiChiTietHoaDonTheoMaHoaDon(goc->right, ma_hoa_don);
}