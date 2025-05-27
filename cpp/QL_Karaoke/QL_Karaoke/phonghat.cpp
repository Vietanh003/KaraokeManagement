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

void thongKeTop3Phong(Node* goc) {
    wprintf(L"Thống kê Top 3 phòng hát có số lần thuê nhiều nhất:\n");

    if (!goc) {
        wprintf(L"Không có phòng hát.\n");
        return;
    }

   
    Node* top1 = NULL;
    Node* top2 = NULL;
    Node* top3 = NULL;

  
    std::stack<Node*> stack;
    stack.push(goc);

    while (!stack.empty()) {
        Node* current = stack.top();
        stack.pop();

    
        if (!top1 || current->data.so_lan_thue > top1->data.so_lan_thue) {
            top3 = top2;
            top2 = top1;
            top1 = current;
        }
        else if (!top2 || current->data.so_lan_thue > top2->data.so_lan_thue) {
            top3 = top2;
            top2 = current;
        }
        else if (!top3 || current->data.so_lan_thue > top3->data.so_lan_thue) {
            top3 = current;
        }

        if (current->phai) stack.push(current->phai);
        if (current->trai) stack.push(current->trai);
    }

  
    if (top1) {
        wprintf(L"1. Mã phòng: %ls, Loại: %ls, Số lần thuê: %d\n", top1->data.ma_phong, top1->data.loai_phong, top1->data.so_lan_thue);
    }
    if (top2) {
        wprintf(L"2. Mã phòng: %ls, Loại: %ls, Số lần thuê: %d\n", top2->data.ma_phong, top2->data.loai_phong, top2->data.so_lan_thue);
    }
    if (top3) {
        wprintf(L"3. Mã phòng: %ls, Loại: %ls, Số lần thuê: %d\n", top3->data.ma_phong, top3->data.loai_phong, top3->data.so_lan_thue);
    }
}
