#include "khachhang.h"
#include "hoadon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int soSanhKhachHang(const void* a, const void* b) {
    return wcscmp(((KhachHang*)a)->ma_khach_hang, ((KhachHang*)b)->ma_khach_hang);
}
Node* docKhachHangTuFile(const wchar_t* ten_file) {
    Node* goc = NULL;
    FILE* file;
    errno_t err = _wfopen_s(&file, ten_file, L"r,ccs=UTF-8");
    if (err != 0 || !file) {
        wprintf(L"Không mở được file %ls, tạo file mới...\n", ten_file);
        err = _wfopen_s(&file, ten_file, L"w,ccs=UTF-8");
        if (err == 0 && file) fclose(file);
        return NULL;
    }

    wchar_t buffer[256];
    while (fgetws(buffer, 256, file)) {
        // Xóa newline và kiểm tra rỗng
        buffer[wcscspn(buffer, L"\n")] = L'\0';
        if (wcslen(buffer) == 0) continue;

        wchar_t ma_khach_hang[MAX_ID], ten[MAX_NAME], so_dien_thoai[15];

        int matched = swscanf_s(buffer, L"%ls\t%[^\t]\t%ls",
            ma_khach_hang, MAX_ID,
            ten, MAX_NAME,
            so_dien_thoai, 15);

        if (matched != 3) {
            wprintf(L"Dòng lỗi định dạng: %ls\n", buffer);
            continue;
        }

        KhachHang kh_tim = { 0 };
        wcscpy_s(kh_tim.ma_khach_hang, MAX_ID, ma_khach_hang);
        if (timNode(goc, &kh_tim, soSanhKhachHang)) {
            wprintf(L"Bỏ qua khách hàng trùng mã %ls\n", ma_khach_hang);
            continue;
        }

        KhachHang* kh = (KhachHang*)malloc(sizeof(KhachHang));
        if (kh) {
            wcscpy_s(kh->ma_khach_hang, MAX_ID, ma_khach_hang);
            wcscpy_s(kh->ten, MAX_NAME, ten);
            wcscpy_s(kh->so_dien_thoai, 15, so_dien_thoai);
            kh->ds_hoa_don = NULL;
            goc = chenNode(goc, kh, soSanhKhachHang);
        }
    }

    fclose(file);
    return goc;
}

Node* themKhachHangMoi(Node* goc, const wchar_t* ma_khach_hang, const wchar_t* ten, const wchar_t* so_dien_thoai, const wchar_t* ten_file) {
    KhachHang kh_tim = { 0 };
    wcscpy_s(kh_tim.ma_khach_hang, MAX_ID, ma_khach_hang);
    Node* node_kh = timNode(goc, &kh_tim, soSanhKhachHang);

    if (node_kh) {
        wprintf(L"Khách hàng với mã %ls đã tồn tại!\n", ma_khach_hang);
        return goc;
    }

    KhachHang* kh = (KhachHang*)malloc(sizeof(KhachHang));
    if (!kh) {
        wprintf(L"Lỗi cấp phát bộ nhớ!\n");
        return goc;
    }

    wcscpy_s(kh->ma_khach_hang, MAX_ID, ma_khach_hang);
    wcscpy_s(kh->ten, MAX_NAME, ten);
    wcscpy_s(kh->so_dien_thoai, 15, so_dien_thoai);
    kh->ds_hoa_don = NULL;

    goc = chenNode(goc, kh, soSanhKhachHang);

    FILE* file;
    errno_t err = _wfopen_s(&file, ten_file, L"a,ccs=UTF-8");
    if (err != 0 || !file) {
        wprintf(L"Không mở được file %ls để ghi!\n", ten_file);
    }
    else {
        fwprintf(file, L"%ls\t%ls\t%ls\n", kh->ma_khach_hang, kh->ten, kh->so_dien_thoai);
        fclose(file);
    }

    wprintf(L"Đã thêm khách hàng %ls thành công!\n", ma_khach_hang);
    return goc;
}
const wchar_t* timMaKhachHangTheoSoDienThoai(const wchar_t* so_dien_thoai, const wchar_t* ten_file) {
    static wchar_t ma_khach[MAX_ID];
    FILE* file;
    errno_t err = _wfopen_s(&file, ten_file, L"r,ccs=UTF-8");
    if (err != 0 || !file) {
        wprintf(L"Không thể mở file %ls!\n", ten_file);
        return NULL;
    }

    wchar_t buffer[256];
    while (fgetws(buffer, 256, file)) {
        // Xóa newline và kiểm tra rỗng
        buffer[wcscspn(buffer, L"\n")] = L'\0';
        if (wcslen(buffer) == 0) continue;

        wchar_t ma[MAX_ID], ten[MAX_NAME], sdt[15];
        int matched = swscanf_s(buffer, L"%ls\t%[^\t]\t%ls",
            ma, MAX_ID,
            ten, MAX_NAME,
            sdt, 15);

        if (matched != 3) {
            continue;
        }

        if (wcscmp(so_dien_thoai, sdt) == 0) {
            wcscpy_s(ma_khach, MAX_ID, ma);
            fclose(file);
            return ma_khach;
        }
    }

    fclose(file);
    return NULL;
}


Node* themKhachHangVoiHoaDon(Node* goc, const wchar_t* ma_khach_hang, const wchar_t* ten, const wchar_t* so_dien_thoai,
    const wchar_t* ma_hoa_don, const wchar_t* ma_phong, float tong_tien, time_t ngay_thue, time_t gio_thue, time_t gio_ra) {
    KhachHang kh_tim = { 0 };
    wcscpy_s(kh_tim.ma_khach_hang, MAX_ID, ma_khach_hang);
    Node* node_kh = timNode(goc, &kh_tim, soSanhKhachHang);

    if (!node_kh) {
        KhachHang* kh = (KhachHang*)malloc(sizeof(KhachHang));
        if (kh) {
            wcscpy_s(kh->ma_khach_hang, MAX_ID, ma_khach_hang);
            wcscpy_s(kh->ten, MAX_NAME, ten);
            wcscpy_s(kh->so_dien_thoai, 15, so_dien_thoai);
            kh->ds_hoa_don = NULL;
            goc = chenNode(goc, kh, soSanhKhachHang);
            node_kh = timNode(goc, kh, soSanhKhachHang);
        }
    }

    HoaDon* hd = (HoaDon*)malloc(sizeof(HoaDon));
    if (hd) {
        wcscpy_s(hd->ma_hoa_don, MAX_ID, ma_hoa_don);
        wcscpy_s(hd->ma_khach_hang, MAX_ID, ma_khach_hang);
        wcscpy_s(hd->ma_phong, MAX_ID, ma_phong);
        hd->tong_tien = tong_tien;
        hd->ngay_thue = ngay_thue;
        hd->gio_thue = gio_thue;
        hd->gio_ra = gio_ra;

        KhachHang* kh = (KhachHang*)node_kh->du_lieu;
        kh->ds_hoa_don = chenNode(kh->ds_hoa_don, hd, soSanhHoaDon);
    }

    return goc;
}

void lietKeHoaDonTheoKhachHang(Node* goc, const wchar_t* ma_khach_hang) {
    KhachHang kh_tim = { 0 };
    wcscpy_s(kh_tim.ma_khach_hang, MAX_ID, ma_khach_hang);
    Node* node_kh = timNode(goc, &kh_tim, soSanhKhachHang);

    if (!node_kh) {
        wprintf(L"Không tìm thấy khách hàng %ls!\n", ma_khach_hang);
        return;
    }

    KhachHang* kh = (KhachHang*)node_kh->du_lieu;
    wprintf(L"Hóa đơn của khách hàng %ls (%ls):\n", kh->ten, kh->ma_khach_hang);

    if (!kh->ds_hoa_don) {
        wprintf(L"Không có hóa đơn.\n");
        return;
    }

    inHoaDon(kh->ds_hoa_don, NULL);
}
void hienThiKhachHang(Node* node, int* count) {
    if (!node) return;

    hienThiKhachHang(node->left, count);
    KhachHang* kh = (KhachHang*)node->du_lieu;
    wprintf(L"Mã KH: %ls, Tên: %ls, SĐT: %ls\n", kh->ma_khach_hang, kh->ten, kh->so_dien_thoai);
    (*count)++;
    hienThiKhachHang(node->right, count);
}

void hienThiTatCaKhachHang(Node* goc) {
    if (!goc) {
        wprintf(L"Danh sách khách hàng trống!\n");
        return;
    }

    wprintf(L"\n===== DANH SÁCH KHÁCH HÀNG =====\n");
    int count = 0;
    hienThiKhachHang(goc, &count);
    if (count == 0) {
        wprintf(L"Không có khách hàng nào trong cây.\n");
    }
    wprintf(L"================================\n");
}
