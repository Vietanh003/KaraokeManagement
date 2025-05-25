#include "hanghoa.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "common.h"
#define _CRT_SECURE_NO_WARNINGS

int soSanhHangHoa(const void* a, const void* b) {
    return wcscmp(((HangHoa*)a)->ma_hang, ((HangHoa*)b)->ma_hang);
}
Node* docHangHoaTuFile(const wchar_t* ten_file) {
    Node* goc = NULL;
    FILE* file;
    errno_t err = _wfopen_s(&file, ten_file, L"r,ccs=UTF-8");
    if (err != 0 || !file) {
        wprintf(L"Không mở được file %ls, tạo file mới...\n", ten_file);
        err = _wfopen_s(&file, ten_file, L"w,ccs=UTF-8");
        if (err == 0 && file) fclose(file);
        return NULL;
    }

    wchar_t line[256];
    while (fgetws(line, sizeof(line) / sizeof(wchar_t), file)) {
        trimString(line); // Xóa xuống dòng
        wchar_t* token = NULL;
        wchar_t* next_token = NULL;

        token = wcstok_s(line, L"|", &next_token);
        if (!token) continue;
        wchar_t ma_hang[MAX_ID];
        wcscpy_s(ma_hang, MAX_ID, token);

        token = wcstok_s(NULL, L"|", &next_token);
        if (!token) continue;
        wchar_t ten_hang[MAX_NAME];
        wcscpy_s(ten_hang, MAX_NAME, token);

        token = wcstok_s(NULL, L"|", &next_token);
        if (!token) continue;
        double gia_tien = wcstod(token, NULL);

        token = wcstok_s(NULL, L"|", &next_token);
        if (!token) continue;
        double giam_gia = wcstod(token, NULL);


        HangHoa* hh = (HangHoa*)malloc(sizeof(HangHoa));
        if (hh) {
            wcscpy_s(hh->ma_hang, MAX_ID, ma_hang);
            wcscpy_s(hh->ten_hang, MAX_NAME, ten_hang);
            hh->gia_tien = gia_tien;
            hh->giam_gia = giam_gia;
            goc = chenNode(goc, hh, soSanhHangHoa);
        }
    }

    fclose(file);
    return goc;
}

void ghiNodeHangHoa(Node* node, FILE* file) {
    if (!node) return;
    ghiNodeHangHoa(node->left, file);
    HangHoa* hh = (HangHoa*)node->du_lieu;
 
    if (fwprintf(file, L"%ls|%ls|%.2lf|%.2lf\n", hh->ma_hang, hh->ten_hang, hh->gia_tien, hh->giam_gia) < 0) {
        wprintf(L"Lỗi khi ghi dữ liệu hàng hóa %ls vào file!\n", hh->ma_hang);
    }
    ghiNodeHangHoa(node->right, file);
}
void ghiHangHoaRaFile(Node* goc, const wchar_t* ten_file) {
    if (!goc) return;

    FILE* file;
    errno_t err = _wfopen_s(&file, ten_file, L"w,ccs=UTF-8");
    if (err != 0 || !file) {
        wprintf(L"Không mở được file %ls để ghi!\n", ten_file);
        return;
    }

    ghiNodeHangHoa(goc, file);
    if (ferror(file)) {
        wprintf(L"Lỗi xảy ra khi ghi vào file %ls!\n", ten_file);
    }
    fclose(file);
}

Node* timNodeMax(Node* goc) {
    while (goc && goc->right)
        goc = goc->right;
    return goc;
}

void themHangHoaMoi(Node** goc, const wchar_t* ten_hang, double gia_tien, double giam_gia, const wchar_t* ten_file) {
    // Tìm mã cuối cùng hiện có trong cây
    wchar_t ma_cuoi[MAX_ID] = L"HH000";
    Node* node_cuoi = timNodeMax(*goc);
    if (node_cuoi) {
        HangHoa* hh_cuoi = (HangHoa*)node_cuoi->du_lieu;
        wcscpy_s(ma_cuoi, MAX_ID, hh_cuoi->ma_hang);
    }
    wchar_t ma_moi[MAX_ID];
    taoMaTuDong(L"HH", ma_cuoi, ma_moi, sizeof(ma_moi) / sizeof(wchar_t));

    HangHoa hh_tim = { 0 };
    wcscpy_s(hh_tim.ma_hang, MAX_ID, ma_moi);
    Node* node_hh = timNode(*goc, &hh_tim, soSanhHangHoa);
    if (node_hh) {
        wprintf(L"Hàng hóa với mã %ls đã tồn tại, không thể thêm!\n", ma_moi);
        return;
    }

    // Cấp phát và thêm hàng hóa mới
    HangHoa* hh = (HangHoa*)malloc(sizeof(HangHoa));
    if (!hh) {
        wprintf(L"Lỗi cấp phát bộ nhớ!\n");
        return;
    }

    wcscpy_s(hh->ma_hang, MAX_ID, ma_moi);
    wcscpy_s(hh->ten_hang, MAX_NAME, ten_hang);
    hh->gia_tien = gia_tien;
    hh->giam_gia = giam_gia;

    *goc = chenNode(*goc, hh, soSanhHangHoa);
    ghiHangHoaRaFile(*goc, ten_file);

    wprintf(L"Đã thêm hàng hóa %ls - %ls thành công!\n", ma_moi, ten_hang);
}
void inHangHoa(Node* node) {
    if (node) {
        inHangHoa(node->left);
        HangHoa* hh = (HangHoa*)node->du_lieu;

        wchar_t gia_vnd[64];
        dinhDangVND(hh->gia_tien, gia_vnd, sizeof(gia_vnd) / sizeof(wchar_t));

        wchar_t giam_gia_vnd[64];
        dinhDangVND(hh->giam_gia, giam_gia_vnd, sizeof(giam_gia_vnd) / sizeof(wchar_t));

        wprintf(L"Mã hàng: %ls, Tên hàng: %ls, Giá tiền: %ls, Giảm giá: %ls\n",
            hh->ma_hang, hh->ten_hang, gia_vnd, giam_gia_vnd);
        inHangHoa(node->right);
    }
}

void hienThiTatCaHangHoa(Node* goc) {
    if (!goc) {
        wprintf(L"Danh sách hàng hóa trống!\n");
        return;
    }

    wprintf(L"\n===== DANH SÁCH TẤT CẢ HÀNG HÓA =====\n");
  
    inHangHoa(goc);
    wprintf(L"======================================\n");
}
void chinhSuaHangHoa(Node* goc, const wchar_t* ma_hang_can_sua) {
    if (!goc) {
        wprintf(L"Cây hàng hóa rỗng!\n");
        return;
    }

    HangHoa temp = { 0 };
    wcscpy_s(temp.ma_hang, MAX_ID, ma_hang_can_sua);
    Node* node = timNode(goc, &temp, soSanhHangHoa);
    if (!node) {
        wprintf(L"Không tìm thấy hàng hóa với mã %ls!\n", ma_hang_can_sua);
        return;
    }

    HangHoa* hh = (HangHoa*)node->du_lieu;

    wprintf(L"===> Đang chỉnh sửa hàng hóa mã: %ls\n", hh->ma_hang);
    wprintf(L"Tên hàng hiện tại: %ls\n", hh->ten_hang);
    wprintf(L"Nhập tên hàng mới (Enter để giữ nguyên): ");
    wchar_t ten_moi[MAX_NAME];
    if (fgetws(ten_moi, MAX_NAME, stdin)) {
        trimString(ten_moi);
        if (wcslen(ten_moi) > 0) {
            wcscpy_s(hh->ten_hang, MAX_NAME, ten_moi);
        }
    }

    wprintf(L"Giá tiền hiện tại: %.2lf\n", hh->gia_tien);
    wprintf(L"Nhập giá tiền mới (âm để giữ nguyên): ");
    double gia_moi;
    if (wscanf_s(L"%lf", &gia_moi) == 1 && gia_moi >= 0) {
        hh->gia_tien = gia_moi;
    }
    while (getwchar() != L'\n'); // Xóa bộ đệm

    wprintf(L"Giảm giá hiện tại: %.2lf\n", hh->giam_gia);
    wprintf(L"Nhập giảm giá mới (âm để giữ nguyên): ");
    double giam_moi;
    if (wscanf_s(L"%lf", &giam_moi) == 1 && giam_moi >= 0) {
        hh->giam_gia = giam_moi;
    }
    while (getwchar() != L'\n');

    wprintf(L"Đã cập nhật thông tin hàng hóa %ls thành công!\n", hh->ma_hang);
}
Node* xoaHangHoa(Node* goc, const wchar_t* ma_hang_can_xoa, int* da_xoa) {
    if (!goc) return NULL;

    HangHoa* hh = (HangHoa*)goc->du_lieu;
    int cmp = wcscmp(ma_hang_can_xoa, hh->ma_hang);

    if (cmp < 0) {
        goc->left = xoaHangHoa(goc->left, ma_hang_can_xoa, da_xoa);
    }
    else if (cmp > 0) {
        goc->right = xoaHangHoa(goc->right, ma_hang_can_xoa, da_xoa);
    }
    else {
        *da_xoa = 1;

        // Trường hợp có 1 con
        if (!goc->left || !goc->right) {
            Node* temp = goc->left ? goc->left : goc->right;
            free(goc->du_lieu);
            free(goc);
            return temp;
        }

        // Trường hợp có 2 con: tìm node nhỏ nhất bên phải
        Node* minNode = goc->right;
        while (minNode->left) minNode = minNode->left;

        // Sao chép dữ liệu từ minNode sang node hiện tại
        HangHoa* hh_moi = (HangHoa*)malloc(sizeof(HangHoa));
        if (hh_moi) {
            *hh_moi = *(HangHoa*)minNode->du_lieu;
            free(goc->du_lieu); // Giải phóng dữ liệu cũ
            goc->du_lieu = hh_moi;
        }

        // Xóa node nhỏ nhất ở cây con phải
        goc->right = xoaHangHoa(goc->right, hh_moi->ma_hang, da_xoa);
    }

    // Cân bằng lại cây
    return canBangLai(goc, (SoSanh)wcscmp);
}
HangHoa* timHangHoaTheoMa(Node* goc, const wchar_t* ma_hang_can_tim) {
    if (!goc) return NULL;

    HangHoa temp;
    wcscpy_s(temp.ma_hang, MAX_ID, ma_hang_can_tim);

    Node* node = timNode(goc, &temp, soSanhHangHoa);
    if (node) {
        return (HangHoa*)node->du_lieu;
    }

    return NULL;
}
