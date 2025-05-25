// src/common.c
#include "common.h"

int layChieuCao(Node* node) {
    return node ? node->chieu_cao : 0;
}

int layHeSoCanBang(Node* node) {
    return node ? layChieuCao(node->left) - layChieuCao(node->right) : 0;
}

Node* xoayPhai(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->chieu_cao = (layChieuCao(y->left) > layChieuCao(y->right) ? layChieuCao(y->left) : layChieuCao(y->right)) + 1;
    x->chieu_cao = (layChieuCao(x->left) > layChieuCao(x->right) ? layChieuCao(x->left) : layChieuCao(x->right)) + 1;
    return x;
}

Node* xoayTrai(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->chieu_cao = (layChieuCao(x->left) > layChieuCao(x->right) ? layChieuCao(x->left) : layChieuCao(x->right)) + 1;
    y->chieu_cao = (layChieuCao(y->left) > layChieuCao(y->right) ? layChieuCao(y->left) : layChieuCao(y->right)) + 1;
    return y;
}

Node* taoNode(void* du_lieu) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node) {
        node->du_lieu = du_lieu;
        node->left = node->right = NULL;
        node->chieu_cao = 1;
    }
    return node;
}
Node* canBangLai(Node* node, SoSanh so_sanh) {
    if (!node) return NULL;

    node->chieu_cao = (layChieuCao(node->left) > layChieuCao(node->right) ? layChieuCao(node->left) : layChieuCao(node->right)) + 1;
    int he_so = layHeSoCanBang(node);

    if (he_so > 1 && layHeSoCanBang(node->left) >= 0)
        return xoayPhai(node);
    if (he_so > 1 && layHeSoCanBang(node->left) < 0) {
        node->left = xoayTrai(node->left);
        return xoayPhai(node);
    }
    if (he_so < -1 && layHeSoCanBang(node->right) <= 0)
        return xoayTrai(node);
    if (he_so < -1 && layHeSoCanBang(node->right) > 0) {
        node->right = xoayPhai(node->right);
        return xoayTrai(node);
    }

    return node;
}

Node* chenNode(Node* goc, void* du_lieu, SoSanh so_sanh) {
    if (!goc) return taoNode(du_lieu);

    int ket_qua = so_sanh(du_lieu, goc->du_lieu);
    if (ket_qua < 0) {
        goc->left = chenNode(goc->left, du_lieu, so_sanh);
    }
    else if (ket_qua > 0) {
        goc->right = chenNode(goc->right, du_lieu, so_sanh);
    }
    else {
        return goc; // Dữ liệu trùng, không thêm
    }

    return canBangLai(goc, so_sanh);
}


Node* timNode(Node* goc, void* du_lieu, SoSanh so_sanh) {
    if (!goc || so_sanh(goc->du_lieu, du_lieu) == 0) return goc;
    return so_sanh(du_lieu, goc->du_lieu) < 0 ? timNode(goc->left, du_lieu, so_sanh) : timNode(goc->right, du_lieu, so_sanh);
}
void freeTree(Node* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node->du_lieu); 
    free(node);         
}
#include <locale.h>

// định dạng tiền tệ Việt Nam
void dinhDangVND(double so_tien, wchar_t* ket_qua, size_t kich_thuoc) {
    wchar_t tam[64];
    swprintf(tam, sizeof(tam) / sizeof(wchar_t), L"%.0lf", so_tien);  // Lấy phần nguyên

    int len = wcslen(tam);
    int dem = 0;
    int j = 0;
    wchar_t ketqua_dao[64] = { 0 };

    // Đảo chuỗi và chèn dấu chấm sau mỗi 3 chữ số
    for (int i = len - 1; i >= 0; i--) {
        ketqua_dao[j++] = tam[i];
        dem++;
        if (dem == 3 && i != 0) {
            ketqua_dao[j++] = L'.';
            dem = 0;
        }
    }

    ketqua_dao[j] = L'\0';

    // Đảo lại kết quả cuối cùng và thêm " VNĐ"
    wchar_t ketqua_chinh[64] = { 0 };
    int len_kq = wcslen(ketqua_dao);
    for (int i = 0; i < len_kq; i++) {
        ketqua_chinh[i] = ketqua_dao[len_kq - i - 1];
    }

    swprintf(ket_qua, kich_thuoc, L"%ls ₫", ketqua_chinh);
}
void taoMaTuDong(const wchar_t* tien_to, const wchar_t* ma_cuoi, wchar_t* ma_moi, size_t kich_thuoc) {
    int so_moi = 1;
    size_t do_dai_tien_to = wcslen(tien_to);

    if (ma_cuoi && wcslen(ma_cuoi) > do_dai_tien_to) {
        const wchar_t* so_phan = ma_cuoi + do_dai_tien_to;
        if (iswdigit(*so_phan)) {
            swscanf_s(so_phan, L"%d", &so_moi);
            so_moi++;  // Tăng lên 1
        }
    }

    swprintf(ma_moi, kich_thuoc, L"%ls%03d", tien_to, so_moi);  // %03d là 3 chữ số
}
// Hàm làm sạch chuỗi
void trimString(wchar_t* str) {
    // Loại bỏ ký tự xuống dòng
    str[wcscspn(str, L"\n")] = 0;
    // Loại bỏ khoảng trắng đầu
    wchar_t* start = str;
    while (*start && iswspace(*start)) start++;
    // Loại bỏ khoảng trắng cuối
    size_t len = wcslen(start);
    while (len > 0 && iswspace(start[len - 1])) {
        start[--len] = 0;
    }
    // Di chuyển chuỗi đã làm sạch về đầu
    if (start != str) {
        wcscpy_s(str, MAX_NAME, start);
    }
}