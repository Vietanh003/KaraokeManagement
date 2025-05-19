#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <fcntl.h>
#include <io.h>
#include <windows.h>

#define MAX_ID 10
#define MAX_NAME 50

// Cấu trúc node chung cho BST (AVL)
typedef struct Node {
    void* du_lieu;
    struct Node* left;
    struct Node* right;
    int chieu_cao;
} Node;

// Cấu trúc khách hàng
typedef struct KhachHang {
    wchar_t ma_khach_hang[MAX_ID];
    wchar_t ten[MAX_NAME];
    wchar_t so_dien_thoai[15];
    Node* ds_hoa_don; // Cây BST hóa đơn
} KhachHang;

// Cấu trúc hóa đơn
typedef struct HoaDon {
    wchar_t ma_hoa_don[MAX_ID];
    wchar_t ma_khach_hang[MAX_ID];
    wchar_t ma_phong[MAX_ID];
    float tong_tien;
    time_t ngay_thue;
} HoaDon;

// Cấu trúc phòng hát
typedef struct PhongHat {
    wchar_t ma_phong[MAX_ID];
    wchar_t loai_phong[20];
    int so_lan_thue;
} PhongHat;

// Hàm so sánh
typedef int (*SoSanh)(const void*, const void*);

int soSanhKhachHang(const void* a, const void* b) {
    return wcscmp(((KhachHang*)a)->ma_khach_hang, ((KhachHang*)b)->ma_khach_hang);
}

int soSanhHoaDon(const void* a, const void* b) {
    return wcscmp(((HoaDon*)a)->ma_hoa_don, ((HoaDon*)b)->ma_hoa_don);
}

int soSanhPhongHat(const void* a, const void* b) {
    return wcscmp(((PhongHat*)a)->ma_phong, ((PhongHat*)b)->ma_phong);
}
void hienThiKhachHang(Node* node);
// Hàm hỗ trợ AVL
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

// Tạo node mới
Node* taoNode(void* du_lieu) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node) {
        node->du_lieu = du_lieu;
        node->left = node->right = NULL;
        node->chieu_cao = 1;
    }
    return node;
}

// Chèn node vào cây AVL
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
        return goc;
    }

    goc->chieu_cao = (layChieuCao(goc->left) > layChieuCao(goc->right) ? layChieuCao(goc->left) : layChieuCao(goc->right)) + 1;

    int he_so = layHeSoCanBang(goc);

    if (he_so > 1 && so_sanh(du_lieu, goc->left->du_lieu) < 0) return xoayPhai(goc);
    if (he_so < -1 && so_sanh(du_lieu, goc->right->du_lieu) > 0) return xoayTrai(goc);
    if (he_so > 1 && so_sanh(du_lieu, goc->left->du_lieu) > 0) {
        goc->left = xoayTrai(goc->left);
        return xoayPhai(goc);
    }
    if (he_so < -1 && so_sanh(du_lieu, goc->right->du_lieu) < 0) {
        goc->right = xoayPhai(goc->right);
        return xoayTrai(goc);
    }

    return goc;
}

// Tìm kiếm node
Node* timNode(Node* goc, void* du_lieu, SoSanh so_sanh) {
    if (!goc || so_sanh(goc->du_lieu, du_lieu) == 0) return goc;
    return so_sanh(du_lieu, goc->du_lieu) < 0 ? timNode(goc->left, du_lieu, so_sanh) : timNode(goc->right, du_lieu, so_sanh);
}

// Đọc khách hàng từ file
Node* docKhachHangTuFile(const wchar_t* ten_file) {
    Node* goc = NULL;
    FILE* file;
    errno_t err = _wfopen_s(&file, ten_file, L"r,ccs=UTF-8");
    if (err != 0 || !file) {
        wprintf(L"Không mở được file %ls\n", ten_file);
        return NULL;
    }

    wchar_t ma_khach_hang[MAX_ID], ten[MAX_NAME], so_dien_thoai[15];
    while (fwscanf_s(file, L"%ls %l[^\n] %ls", ma_khach_hang, (unsigned)_countof(ma_khach_hang),
        ten, (unsigned)_countof(ten), so_dien_thoai, (unsigned)_countof(so_dien_thoai)) == 3) {
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

// Đọc hóa đơn từ file
Node* docHoaDonTuFile(const wchar_t* ten_file) {
    Node* goc = NULL;
    FILE* file;
    errno_t err = _wfopen_s(&file, ten_file, L"r,ccs=UTF-8");
    if (err != 0 || !file) {
        wprintf(L"Không mở được file %ls\n", ten_file);
        return NULL;
    }

    wchar_t ma_hoa_don[MAX_ID], ma_khach_hang[MAX_ID], ma_phong[MAX_ID];
    float tong_tien;
    wchar_t ngay[26];
    while (fwscanf_s(file, L"%ls %ls %ls %f %l[^\n]", ma_hoa_don, (unsigned)_countof(ma_hoa_don),
        ma_khach_hang, (unsigned)_countof(ma_khach_hang), ma_phong, (unsigned)_countof(ma_phong),
        &tong_tien, ngay, (unsigned)_countof(ngay)) == 5) {
        HoaDon* hd = (HoaDon*)malloc(sizeof(HoaDon));
        if (hd) {
            wcscpy_s(hd->ma_hoa_don, MAX_ID, ma_hoa_don);
            wcscpy_s(hd->ma_khach_hang, MAX_ID, ma_khach_hang);
            wcscpy_s(hd->ma_phong, MAX_ID, ma_phong);
            hd->tong_tien = tong_tien;
            struct tm tm = { 0 };
            swscanf_s(ngay, L"%d-%d-%d %d:%d:%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
                &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
            tm.tm_year -= 1900;
            tm.tm_mon -= 1;
            hd->ngay_thue = mktime(&tm);
            goc = chenNode(goc, hd, soSanhHoaDon);
        }
    }

    fclose(file);
    return goc;
}

// Ghi khách hàng ra file
void ghiKhachHangRaFile(Node* goc, const wchar_t* ten_file) {
    if (!goc) return;

    FILE* file;
    errno_t err = _wfopen_s(&file, ten_file, L"a,ccs=UTF-8");
    if (err != 0 || !file) {
        wprintf(L"Không mở được file %ls\n", ten_file);
        return;
    }

    KhachHang* kh = (KhachHang*)goc->du_lieu;
    fwprintf(file, L"%ls %ls %ls\n", kh->ma_khach_hang, kh->ten, kh->so_dien_thoai);
    fclose(file);

    ghiKhachHangRaFile(goc->left, ten_file);
    ghiKhachHangRaFile(goc->right, ten_file);
}

// Ghi hóa đơn ra file
void ghiHoaDonRaFile(Node* goc, const wchar_t* ten_file) {
    if (!goc) return;

    FILE* file;
    errno_t err = _wfopen_s(&file, ten_file, L"a,ccs=UTF-8");
    if (err != 0 || !file) {
        wprintf(L"Không mở được file %ls\n", ten_file);
        return;
    }

    HoaDon* hd = (HoaDon*)goc->du_lieu;
    wchar_t ngay[26];
    struct tm tm;
    localtime_s(&tm, &hd->ngay_thue);
    wcsftime(ngay, sizeof(ngay) / sizeof(wchar_t), L"%Y-%m-%d %H:%M:%S", &tm);
    fwprintf(file, L"%ls %ls %ls %.2f %ls\n", hd->ma_hoa_don, hd->ma_khach_hang, hd->ma_phong, hd->tong_tien, ngay);
    fclose(file);

    ghiHoaDonRaFile(goc->left, ten_file);
    ghiHoaDonRaFile(goc->right, ten_file);
}

// Thêm khách hàng mới
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
        fwprintf(file, L"%ls %ls %ls\n", kh->ma_khach_hang, kh->ten, kh->so_dien_thoai);
        fclose(file);
    }

    wprintf(L"Đã thêm khách hàng %ls thành công!\n", ma_khach_hang);
    return goc;
}

// Thêm hóa đơn
void themHoaDon(Node** goc, const wchar_t* ma_hoa_don, const wchar_t* ma_khach_hang, const wchar_t* ma_phong, float tong_tien, time_t ngay_thue, const wchar_t* ten_file) {
    HoaDon* hd = (HoaDon*)malloc(sizeof(HoaDon));
    if (!hd) {
        wprintf(L"Lỗi cấp phát bộ nhớ!\n");
        return;
    }

    wcscpy_s(hd->ma_hoa_don, MAX_ID, ma_hoa_don);
    wcscpy_s(hd->ma_khach_hang, MAX_ID, ma_khach_hang);
    wcscpy_s(hd->ma_phong, MAX_ID, ma_phong);
    hd->tong_tien = tong_tien;
    hd->ngay_thue = ngay_thue;

    *goc = chenNode(*goc, hd, soSanhHoaDon);

    FILE* file;
    errno_t err = _wfopen_s(&file, ten_file, L"a,ccs=UTF-8");
    if (err != 0 || !file) {
        wprintf(L"Không mở được file %ls để ghi!\n", ten_file);
    }
    else {
        wchar_t ngay[26];
        struct tm tm;
        localtime_s(&tm, &ngay_thue);
        wcsftime(ngay, sizeof(ngay) / sizeof(wchar_t), L"%Y-%m-%d %H:%M:%S", &tm);
        fwprintf(file, L"%ls %ls %ls %.2f %ls\n", ma_hoa_don, ma_khach_hang, ma_phong, tong_tien, ngay);
        fclose(file);
    }

    wprintf(L"Đã thêm hóa đơn %ls thành công!\n", ma_hoa_don);
}

// Thêm khách hàng mới với hóa đơn
Node* themKhachHangVoiHoaDon(Node* goc, const wchar_t* ma_khach_hang, const wchar_t* ten, const wchar_t* so_dien_thoai,
    const wchar_t* ma_hoa_don, const wchar_t* ma_phong, float tong_tien, time_t ngay_thue) {
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

        KhachHang* kh = (KhachHang*)node_kh->du_lieu;
        kh->ds_hoa_don = chenNode(kh->ds_hoa_don, hd, soSanhHoaDon);
    }

    return goc;
}

// In hóa đơn
void inHoaDon(Node* node) {
    if (node) {
        inHoaDon(node->left);
        HoaDon* hd = (HoaDon*)node->du_lieu;
        wchar_t ngay[26];
        struct tm tm;
        localtime_s(&tm, &hd->ngay_thue);
        wcsftime(ngay, sizeof(ngay) / sizeof(wchar_t), L"%Y-%m-%d %H:%M:%S", &tm);
        wprintf(L"Hóa đơn: %ls, KH: %ls, Phòng: %ls, Tổng tiền: %.2f, Ngày: %ls\n",
            hd->ma_hoa_don, hd->ma_khach_hang, hd->ma_phong, hd->tong_tien, ngay);
        inHoaDon(node->right);
    }
}

// Liệt kê hóa đơn theo mã khách hàng
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

    inHoaDon(kh->ds_hoa_don);
}

// Hiển thị tất cả khách hàng
void hienThiTatCaKhachHang(Node* goc) {
    if (!goc) {
        wprintf(L"Danh sách khách hàng trống!\n");
        return;
    }

    wprintf(L"\n===== DANH SÁCH TẤT CẢ KHÁCH HÀNG =====\n");

    hienThiKhachHang(goc);
    wprintf(L"======================================\n");
}

void hienThiKhachHang(Node* node) {
    if (!node) return;

    hienThiKhachHang(node->left);
    KhachHang* kh = (KhachHang*)node->du_lieu;
    wprintf(L"Mã KH: %ls, Tên: %ls, Số điện thoại: %ls\n",
        kh->ma_khach_hang, kh->ten, kh->so_dien_thoai);
    hienThiKhachHang(node->right);
}

// Hiển thị tất cả hóa đơn
void hienThiTatCaHoaDon(Node* goc) {
    if (!goc) {
        wprintf(L"Danh sách hóa đơn trống!\n");
        return;
    }

    wprintf(L"\n===== DANH SÁCH TẤT CẢ HÓA ĐƠN =====\n");
    inHoaDon(goc);
    wprintf(L"====================================\n");
}

// Hiển thị tất cả phòng hát
void hienThiTatCaPhongHat(Node* goc) {
    if (!goc) {
        wprintf(L"Danh sách phòng hát trống!\n");
        return;
    }

    wprintf(L"\n===== DANH SÁCH TẤT CẢ PHÒNG HÁT =====\n");
    //inPhongHat(goc);
    wprintf(L"======================================\n");
}

// Đọc phòng hát từ file
Node* docPhongHatTuFile(const wchar_t* ten_file) {
    Node* goc = NULL;
    FILE* file;
    errno_t err = _wfopen_s(&file, ten_file, L"r,ccs=UTF-8");
    if (err != 0 || !file) {
        wprintf(L"Không mở được file %ls\n", ten_file);
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

// Thêm phòng hát mới
void themPhongHatMoi(Node** goc, const wchar_t* ma_phong, const wchar_t* loai_phong) {
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
    wprintf(L"Đã thêm phòng hát %ls thành công!\n", ma_phong);
}

// In phòng hát
void inPhongHat(Node* node) {
    if (node) {
        inPhongHat(node->left);
        PhongHat* ph = (PhongHat*)node->du_lieu;
        wprintf(L"Phòng: %ls, Loại: %ls, Số lần thuê: %d\n",
            ph->ma_phong, ph->loai_phong, ph->so_lan_thue);
        inPhongHat(node->right);
    }
}

// Thống kê top 3 phòng
void thongKeTop3Phong(Node* goc) {
    wprintf(L"Thống kê phòng hát:\n");
    if (!goc) {
        wprintf(L"Không có phòng hát.\n");
        return;
    }

    inPhongHat(goc);
}

// Tìm hóa đơn theo mã
void timHoaDonTheoMa(Node* goc, const wchar_t* ma_hoa_don) {
    HoaDon hd_tim = { 0 };
    wcscpy_s(hd_tim.ma_hoa_don, MAX_ID, ma_hoa_don);
    Node* node_hd = timNode(goc, &hd_tim, soSanhHoaDon);
    if (node_hd) {
        inHoaDon(node_hd);
    }
    else {
        wprintf(L"Không tìm thấy hóa đơn %ls!\n", ma_hoa_don);
    }
}

// Sắp xếp hóa đơn theo tổng tiền
void sapXepHoaDonTheoTongTien(Node* goc) {
    wprintf(L"Danh sách hóa đơn:\n");
    inHoaDon(goc);
}

// Sắp xếp hóa đơn theo mã khách hàng
void sapXepHoaDonTheoMaKhachHang(Node* goc) {
    wprintf(L"Danh sách hóa đơn:\n");
    inHoaDon(goc);
}

// Thống kê doanh thu
void thongKeDoanhThu(Node* goc, const wchar_t* bat_dau, const wchar_t* ket_thuc) {
    wprintf(L"Thống kê doanh thu từ %ls đến %ls:\n", bat_dau, ket_thuc);
    float tong_doanh_thu = 0;
   /* thongKeDoanhThuNode(goc, &tong_doanh_thu);*/
    wprintf(L"Tổng doanh thu: %.2f\n", tong_doanh_thu);
}

void thongKeDoanhThuNode(Node* node, float* tong) {
    if (node) {
        thongKeDoanhThuNode(node->left, tong);
        HoaDon* hd = (HoaDon*)node->du_lieu;
        *tong += hd->tong_tien;
        thongKeDoanhThuNode(node->right, tong);
    }
}

// Hàm hiển thị menu
void hienThiMenu() {
    wprintf(L"\n===== HỆ THỐNG QUẢN LÝ KARAOKE =====\n");
    wprintf(L"1. Đọc dữ liệu khách hàng từ file\n");
    wprintf(L"2. Thêm khách hàng mới với hóa đơn\n");
    wprintf(L"3. Tìm kiếm và liệt kê hóa đơn theo mã khách hàng\n");
    wprintf(L"4. Đọc dữ liệu phòng hát từ file\n");
    wprintf(L"5. Thêm phòng hát mới\n");
    wprintf(L"6. Thống kê top 3 phòng thuê nhiều nhất\n");
    wprintf(L"7. Đọc dữ liệu hóa đơn từ file\n");
    wprintf(L"8. Thêm hóa đơn và chi tiết hóa đơn\n");
    wprintf(L"9. Tìm kiếm hóa đơn theo số hóa đơn\n");
    wprintf(L"10. Sắp xếp hóa đơn theo tổng tiền (giảm dần)\n");
    wprintf(L"11. Sắp xếp hóa đơn theo mã khách hàng (tăng dần)\n");
    wprintf(L"12. Thống kê doanh thu theo khoảng thời gian\n");
    wprintf(L"13. Thêm một khách hàng mới\n");
    wprintf(L"14. Hiển thị tất cả khách hàng\n");
    wprintf(L"15. Hiển thị tất cả hóa đơn\n");
    wprintf(L"16. Hiển thị tất cả phòng hát\n");
    wprintf(L"20. Thoát\n");
    wprintf(L"Nhập lựa chọn (1-20): ");
}

// Hàm xử lý menu
void xuLyMenu(Node** cay_khach_hang, Node** cay_phong_hat, Node** cay_hoa_don) {
    int lua_chon;
    wchar_t ma_khach_hang[MAX_ID], ten[MAX_NAME], so_dien_thoai[15];
    wchar_t ma_hoa_don[MAX_ID], ma_phong[MAX_ID];
    float tong_tien;
    time_t ngay_thue;
    wchar_t bat_dau[20], ket_thuc[20];
    wchar_t loai_phong[20];
    const wchar_t* ten_file_khach_hang = L"khachhang.txt";
    const wchar_t* ten_file_phong_hat = L"phonghat.txt";
    const wchar_t* ten_file_hoa_don = L"hoadon.txt";

    // Đọc tất cả các file khi bắt đầu chương trình
    wprintf(L"Đang đọc dữ liệu từ các file...\n");
    *cay_khach_hang = docKhachHangTuFile(ten_file_khach_hang);
    wprintf(L"Đã đọc dữ liệu khách hàng từ file %ls\n", ten_file_khach_hang);
    *cay_phong_hat = docPhongHatTuFile(ten_file_phong_hat);
    wprintf(L"Đã đọc dữ liệu phòng hát từ file %ls\n", ten_file_phong_hat);
    *cay_hoa_don = docHoaDonTuFile(ten_file_hoa_don);
    wprintf(L"Đã đọc dữ liệu hóa đơn từ file %ls\n", ten_file_hoa_don);

  do {
        hienThiMenu();
        // Improved input handling
        wchar_t input[10];
        fgetws(input, 10, stdin);
        if(swscanf_s(input, L"%d", &lua_chon) != 1) {
            wprintf(L"Lựa chọn không hợp lệ!\n");
            continue;
        }
        switch (lua_chon) {
        case 1:
            *cay_khach_hang = docKhachHangTuFile(ten_file_khach_hang);
            wprintf(L"Đã đọc dữ liệu khách hàng từ file %ls\n", ten_file_khach_hang);
            break;
        case 2:
            wprintf(L"Nhập mã khách hàng: ");
            fgetws(ma_khach_hang, MAX_ID, stdin);
            ma_khach_hang[wcscspn(ma_khach_hang, L"\n")] = 0;
            wprintf(L"Nhập tên khách hàng: ");
            fgetws(ten, MAX_NAME, stdin);
            ten[wcscspn(ten, L"\n")] = 0;
            wprintf(L"Nhập số điện thoại: ");
            fgetws(so_dien_thoai, 15, stdin);
            so_dien_thoai[wcscspn(so_dien_thoai, L"\n")] = 0;
            wprintf(L"Nhập mã hóa đơn: ");
            fgetws(ma_hoa_don, MAX_ID, stdin);
            ma_hoa_don[wcscspn(ma_hoa_don, L"\n")] = 0;
            wprintf(L"Nhập mã phòng: ");
            fgetws(ma_phong, MAX_ID, stdin);
            ma_phong[wcscspn(ma_phong, L"\n")] = 0;
            wprintf(L"Nhập tổng tiền: ");
            scanf_s("%f", &tong_tien);
            ngay_thue = time(NULL);
            *cay_khach_hang = themKhachHangVoiHoaDon(*cay_khach_hang, ma_khach_hang, ten, so_dien_thoai,
                ma_hoa_don, ma_phong, tong_tien, ngay_thue);
            ghiKhachHangRaFile(*cay_khach_hang, ten_file_khach_hang);
            wprintf(L"Đã thêm khách hàng và hóa đơn\n");
            while (getwchar() != L'\n');
            break;
        case 3:
            wprintf(L"Nhập mã khách hàng: ");
            fgetws(ma_khach_hang, MAX_ID, stdin);
            ma_khach_hang[wcscspn(ma_khach_hang, L"\n")] = 0;
            lietKeHoaDonTheoKhachHang(*cay_khach_hang, ma_khach_hang);
            break;
        case 4:
            *cay_phong_hat = docPhongHatTuFile(ten_file_phong_hat);
            wprintf(L"Đã đọc dữ liệu phòng hát từ file %ls\n", ten_file_phong_hat);
            break;
        case 5:
            wprintf(L"Nhập mã phòng: ");
            fgetws(ma_phong, MAX_ID, stdin);
            ma_phong[wcscspn(ma_phong, L"\n")] = 0;
            wprintf(L"Nhập loại phòng: ");
            fgetws(loai_phong, 20, stdin);
            loai_phong[wcscspn(loai_phong, L"\n")] = 0;
            themPhongHatMoi(cay_phong_hat, ma_phong, loai_phong);
            break;
        case 6:
            thongKeTop3Phong(*cay_phong_hat);
            break;
        case 7:
            *cay_hoa_don = docHoaDonTuFile(ten_file_hoa_don);
            wprintf(L"Đã đọc dữ liệu hóa đơn từ file %ls\n", ten_file_hoa_don);
            break;
        case 8:
            wprintf(L"Nhập mã hóa đơn: ");
            fgetws(ma_hoa_don, MAX_ID, stdin);
            ma_hoa_don[wcscspn(ma_hoa_don, L"\n")] = 0;
            wprintf(L"Nhập mã khách hàng: ");
            fgetws(ma_khach_hang, MAX_ID, stdin);
            ma_khach_hang[wcscspn(ma_khach_hang, L"\n")] = 0;
            wprintf(L"Nhập mã phòng: ");
            fgetws(ma_phong, MAX_ID, stdin);
            ma_phong[wcscspn(ma_phong, L"\n")] = 0;
            wprintf(L"Nhập tổng tiền: ");
            scanf_s("%f", &tong_tien);
            ngay_thue = time(NULL);
            themHoaDon(cay_hoa_don, ma_hoa_don, ma_khach_hang, ma_phong, tong_tien, ngay_thue, ten_file_hoa_don);
            while (getwchar() != L'\n');
            break;
        case 9:
            wprintf(L"Nhập mã hóa đơn: ");
            fgetws(ma_hoa_don, MAX_ID, stdin);
            ma_hoa_don[wcscspn(ma_hoa_don, L"\n")] = 0;
            timHoaDonTheoMa(*cay_hoa_don, ma_hoa_don);
            break;
        case 10:
            sapXepHoaDonTheoTongTien(*cay_hoa_don);
            break;
        case 11:
            sapXepHoaDonTheoMaKhachHang(*cay_hoa_don);
            break;
        case 12:
            wprintf(L"Nhập ngày bắt đầu (YYYY-MM-DD): ");
            fgetws(bat_dau, 20, stdin);
            bat_dau[wcscspn(bat_dau, L"\n")] = 0;
            wprintf(L"Nhập ngày kết thúc (YYYY-MM-DD): ");
            fgetws(ket_thuc, 20, stdin);
            ket_thuc[wcscspn(ket_thuc, L"\n")] = 0;
            thongKeDoanhThu(*cay_hoa_don, bat_dau, ket_thuc);
            break;
        case 13:
            wprintf(L"Nhập mã khách hàng: ");
            fgetws(ma_khach_hang, MAX_ID, stdin);
            ma_khach_hang[wcscspn(ma_khach_hang, L"\n")] = 0;
            wprintf(L"Nhập tên khách hàng: ");
            fgetws(ten, MAX_NAME, stdin);
            ten[wcscspn(ten, L"\n")] = 0;
            wprintf(L"Nhập số điện thoại: ");
            fgetws(so_dien_thoai, 15, stdin);
            so_dien_thoai[wcscspn(so_dien_thoai, L"\n")] = 0;
            *cay_khach_hang = themKhachHangMoi(*cay_khach_hang, ma_khach_hang, ten, so_dien_thoai, ten_file_khach_hang);
            break;
        case 14:
            hienThiTatCaKhachHang(*cay_khach_hang);
            break;
        case 15:
            hienThiTatCaHoaDon(*cay_hoa_don);
            break;
        case 16:
            hienThiTatCaPhongHat(*cay_phong_hat);
            break;
        case 20:
            wprintf(L"Thoát chương trình.\n");
            break;
        default:
            wprintf(L"Lựa chọn không hợp lệ!\n");
        }
    } while (lua_chon != 20);
}

// Hàm chính
int main() {
    // Thiết lập console để hỗ trợ Unicode
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // Ensure proper locale setup for Unicode
    _wsetlocale(LC_ALL, L"vi-VN.UTF-8");

    // Setup console font
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 16;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, LF_FACESIZE, L"Consolas");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

    Node* cay_khach_hang = NULL;
    Node* cay_phong_hat = NULL;
    Node* cay_hoa_don = NULL;

    xuLyMenu(&cay_khach_hang, &cay_phong_hat, &cay_hoa_don);

    return 0;
}