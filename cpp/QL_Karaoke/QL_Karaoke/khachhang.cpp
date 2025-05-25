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
    // Check if customer already exists (using provided ma_khach_hang for initial check)
    KhachHang kh_tim = { 0 };
    wcscpy_s(kh_tim.ma_khach_hang, MAX_ID, ma_khach_hang);
    Node* node_kh = timNode(goc, &kh_tim, soSanhKhachHang);

    if (node_kh) {
        wprintf(L"Khách hàng với mã %ls đã tồn tại!\n", ma_khach_hang);
        return goc;
    }

    // Validate phone number (so_dien_thoai)
    size_t phone_len = wcslen(so_dien_thoai);
    if (phone_len != 10) {
        wprintf(L"Số điện thoại phải có đúng 10 chữ số!\n");
        return goc;
    }

    for (size_t i = 0; i < phone_len; i++) {
        if (!iswdigit(so_dien_thoai[i])) {
            wprintf(L"Số điện thoại chỉ được chứa các chữ số!\n");
            return goc;
        }
    }

    // Check Vietnamese mobile prefix (03, 05, 07, 08, 09)
    const wchar_t* valid_prefixes[] = { L"03", L"05", L"07", L"08", L"09" };
    int valid_prefix = 0;
    for (size_t i = 0; i < sizeof(valid_prefixes) / sizeof(valid_prefixes[0]); i++) {
        if (wcsncmp(so_dien_thoai, valid_prefixes[i], 2) == 0) {
            valid_prefix = 1;
            break;
        }
    }
    if (!valid_prefix) {
        wprintf(L"Số điện thoại phải bắt đầu bằng 03, 05, 07, 08 hoặc 09!\n");
        return goc;
    }

    // Validate name (ten)
    size_t name_len = wcslen(ten);
    if (name_len < 2 || name_len >= MAX_NAME) {
        wprintf(L"Tên khách hàng phải từ 2 đến %d ký tự!\n", MAX_NAME - 1);
        return goc;
    }

    for (size_t i = 0; i < name_len; i++) {
        if (!iswalpha(ten[i]) && !iswspace(ten[i])) {
            wprintf(L"Tên khách hàng chỉ được chứa chữ cái và khoảng trắng!\n");
            return goc;
        }
    }

    // Generate automatic ma_khach_hang if not provided or invalid
    wchar_t new_ma_khach_hang[MAX_ID];
    if (wcslen(ma_khach_hang) == 0 || wcsncmp(ma_khach_hang, L"KH", 2) != 0) {
        Node* last_kh = goc;
        while (last_kh && last_kh->right) last_kh = last_kh->right;
        taoMaTuDong(L"KH", last_kh ? ((KhachHang*)last_kh->du_lieu)->ma_khach_hang : NULL, new_ma_khach_hang, MAX_ID);
    }
    else {
        wcscpy_s(new_ma_khach_hang, MAX_ID, ma_khach_hang);
    }

    KhachHang* kh = (KhachHang*)malloc(sizeof(KhachHang));
    if (!kh) {
        wprintf(L"Lỗi cấp phát bộ nhớ!\n");
        return goc;
    }

    wcscpy_s(kh->ma_khach_hang, MAX_ID, new_ma_khach_hang);
    wcscpy_s(kh->ten, MAX_NAME, ten);
    wcscpy_s(kh->so_dien_thoai, 15, so_dien_thoai);
    kh->ds_hoa_don = NULL;

    goc = chenNode(goc, kh, soSanhKhachHang);


    FILE* file;
    errno_t err = _wfopen_s(&file, ten_file, L"a,ccs=UTF-8");
    if (err != 0 || !file) {
        wprintf(L"Không mở được file %ls để ghi!\n", ten_file);
        free(kh);
        return goc;
    }
    else {
        fwprintf(file, L"%ls\t%ls\t%ls\n", kh->ma_khach_hang, kh->ten, kh->so_dien_thoai);
        fclose(file);
    }

    wprintf(L"Đã thêm khách hàng %ls thành công!\n", kh->ma_khach_hang);
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
