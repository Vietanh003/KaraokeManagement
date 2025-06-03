#include "phonghat.h"

int soSanhPhongHat(const void* a, const void* b) {
    return wcscmp(((PhongHat*)a)->ma_phong, ((PhongHat*)b)->ma_phong);
}

Node* docPhongHatTuFile(const wchar_t* ten_file) {
    Node* goc = NULL;
    FILE* file;
    errno_t err = _wfopen_s(&file, ten_file, L"r,ccs=UTF-8");
    if (err != 0 || !file) {
        wprintf(L"Không mở được file %ls, tạo file mới...\n", ten_file);
        err = _wfopen_s(&file, ten_file, L"w,ccs=UTF-8");
        if (err == 0 && file) fclose(file);
        return NULL;
    }

    wchar_t ma_phong[MAX_ID], loai_phong[20];
    int so_lan_thue;
    while (fwscanf_s(file, L"%ls %ls %d", ma_phong, (unsigned)_countof(ma_phong),
        loai_phong, (unsigned)_countof(loai_phong), &so_lan_thue) == 3) {
        PhongHat* ph = (PhongHat*)malloc(sizeof(PhongHat));
        if (ph) {
            wcscpy_s(ph->ma_phong, MAX_ID, ma_phong);
            wcscpy_s(ph->loai_phong, 20, loai_phong);
            ph->so_lan_thue = so_lan_thue;
            goc = chenNode(goc, ph, soSanhPhongHat);
        }
    }

    fclose(file);
    return goc;
}

void ghiNode(Node* node, FILE* file) {
    if (!node) return;
    ghiNode(node->left, file);
    PhongHat* ph = (PhongHat*)node->du_lieu;
    fwprintf(file, L"%ls %ls %d\n", ph->ma_phong, ph->loai_phong, ph->so_lan_thue);
    ghiNode(node->right, file);
}

void ghiPhongHatRaFile(Node* goc, const wchar_t* ten_file) {
    if (!goc) return;

    FILE* file;
    errno_t err = _wfopen_s(&file, ten_file, L"w,ccs=UTF-8");
    if (err != 0 || !file) {
        wprintf(L"Không mở được file %ls để ghi!\n", ten_file);
        return;
    }

    ghiNode(goc, file);
    fclose(file);
}

void themPhongHatMoi(Node** goc, const wchar_t* ma_phong, const wchar_t* loai_phong, const wchar_t* ten_file) {
    PhongHat ph_tim = { 0 };
    wcscpy_s(ph_tim.ma_phong, MAX_ID, ma_phong);
    Node* node_ph = timNode(*goc, &ph_tim, soSanhPhongHat);

    if (node_ph) {
        wprintf(L"Phòng hát với mã %ls đã tồn tại!\n", ma_phong);
        return;
    }

    PhongHat* ph = (PhongHat*)malloc(sizeof(PhongHat));
    if (!ph) {
        wprintf(L"Lỗi cấp phát bộ nhớ!\n");
        return;
    }

    wcscpy_s(ph->ma_phong, MAX_ID, ma_phong);
    wcscpy_s(ph->loai_phong, 20, loai_phong);
    ph->so_lan_thue = 0;

    *goc = chenNode(*goc, ph, soSanhPhongHat);
    ghiPhongHatRaFile(*goc, ten_file);

    wprintf(L"Đã thêm phòng hát %ls thành công!\n", ma_phong);
}

void inPhongHat(Node* node) {
    if (node) {
        inPhongHat(node->left);
        PhongHat* ph = (PhongHat*)node->du_lieu;
        wprintf(L"Phòng: %ls, Loại: %ls, Số lần thuê: %d\n",
            ph->ma_phong, ph->loai_phong, ph->so_lan_thue);
        inPhongHat(node->right);
    }
}

void hienThiTatCaPhongHat(Node* goc) {
    if (!goc) {
        wprintf(L"Danh sách phòng hát trống!\n");
        return;
    }

    wprintf(L"\n===== DANH SÁCH TẤT CẢ PHÒNG HÁT =====\n");
    inPhongHat(goc);
    wprintf(L"======================================\n");
}

typedef struct {
    PhongHat* phong;
    int so_lan_thue;
} TopPhong;

void timTop3(Node* goc, TopPhong top[3]) {
    if (!goc) return;

    timTop3(goc->left, top);

    PhongHat* ph = (PhongHat*)goc->du_lieu;

    for (int i = 0; i < 3; ++i) {
        if (top[i].phong == NULL || ph->so_lan_thue > top[i].so_lan_thue) {
            // Dịch phần tử phía sau
            for (int j = 2; j > i; --j) {
                top[j] = top[j - 1];
            }
            top[i].phong = ph;
            top[i].so_lan_thue = ph->so_lan_thue;
            break;
        }
    }

    timTop3(goc->right, top);
}

void tangSoLanThuePhongHat(Node* goc, const wchar_t* ma_phong, const wchar_t* ten_file) {
    if (!goc) {
        wprintf(L"Danh sách phòng hát trống!\n");
        return;
    }

    PhongHat temp = { 0 };
    wcscpy_s(temp.ma_phong, MAX_ID, ma_phong);
    Node* node = timNode(goc, &temp, soSanhPhongHat);

    if (!node) {
        wprintf(L"Không tìm thấy phòng hát có mã %ls!\n", ma_phong);
        return;
    }

    PhongHat* ph = (PhongHat*)node->du_lieu;
    ph->so_lan_thue += 1;

    ghiPhongHatRaFile(goc, ten_file);

    wprintf(L"Đã tăng số lần thuê của phòng %ls lên %d.\n", ma_phong, ph->so_lan_thue);
}


void thongKeTop3Phong(Node* goc) {
    wprintf(L"Thống kê Top 3 phòng hát có số lần thuê nhiều nhất:\n");

    if (!goc) {
        wprintf(L"Không có phòng hát.\n");
        return;
    }

    TopPhong top[3] = { {NULL, 0}, {NULL, 0}, {NULL, 0} };

    timTop3(goc, top);

    for (int i = 0; i < 3; ++i) {
        if (top[i].phong) {
            wprintf(L"%d. Mã phòng: %ls, Loại: %ls, Số lần thuê: %d\n",
                i + 1,
                top[i].phong->ma_phong,
                top[i].phong->loai_phong,
                top[i].phong->so_lan_thue);
        }
    }
}

