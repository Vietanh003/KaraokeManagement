#include "hoadon.h"
#include "chitiethoadon.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <string.h>
#include "khachhang.h"
#include "common.h"
#include "phonghat.h"


int soSanhHoaDon(const void* a, const void* b) {
    return wcscmp(((HoaDon*)a)->ma_hoa_don, ((HoaDon*)b)->ma_hoa_don);
}

Node* docHoaDonTuFile(const wchar_t* ten_file) {
    Node* goc = NULL;
    FILE* file;
    errno_t err = _wfopen_s(&file, ten_file, L"r,ccs=UTF-8");
    if (err != 0 || !file) {
        wprintf(L"Không mở được file %ls, tạo file mới...\n", ten_file);
        err = _wfopen_s(&file, ten_file, L"w,ccs=UTF-8");
        if (err == 0 && file) fclose(file);
        return NULL;
    }

    wchar_t ma_hoa_don[MAX_ID], ma_khach_hang[MAX_ID], ma_phong[MAX_ID];
    float tong_tien;
    wchar_t ngay[26], gio_thue[20], gio_ra[20];
    while (fwscanf_s(file, L"%ls %ls %ls %f %ls %ls %ls",
        ma_hoa_don, (unsigned)_countof(ma_hoa_don),
        ma_khach_hang, (unsigned)_countof(ma_khach_hang),
        ma_phong, (unsigned)_countof(ma_phong),
        &tong_tien,
        ngay, (unsigned)_countof(ngay),
        gio_thue, (unsigned)_countof(gio_thue),
        gio_ra, (unsigned)_countof(gio_ra)) == 7) {
        HoaDon* hd = (HoaDon*)malloc(sizeof(HoaDon));
        if (hd) {
            wcscpy_s(hd->ma_hoa_don, MAX_ID, ma_hoa_don);
            wcscpy_s(hd->ma_khach_hang, MAX_ID, ma_khach_hang);
            wcscpy_s(hd->ma_phong, MAX_ID, ma_phong);
            hd->tong_tien = tong_tien;

            struct tm tm = { 0 };
            swscanf_s(ngay, L"%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday);
            tm.tm_year -= 1900;
            tm.tm_mon -= 1;
            hd->ngay_thue = mktime(&tm);

            swscanf_s(gio_thue, L"%d:%d:%d", &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
            hd->gio_thue = mktime(&tm);

            swscanf_s(gio_ra, L"%d:%d:%d", &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
            hd->gio_ra = mktime(&tm);

            goc = chenNode(goc, hd, soSanhHoaDon);
        }
    }

    fclose(file);
    return goc;
}
void themHoaDon(Node** goc_hoa_don, Node* cay_khach_hang, Node* cay_phong_hat, Node* cay_hang_hoa, Node** cay_chi_tiet_hoa_don, const wchar_t* ten_file_hoa_don, const wchar_t* ten_file_chi_tiet) {
    // Generate automatic invoice ID
    wchar_t ma_hoa_don[MAX_ID];
    wchar_t so_dien_thoai[15];
    wchar_t ma_khach_hang[MAX_ID];
    Node* last_node = *goc_hoa_don;
    while (last_node && last_node->right) last_node = last_node->right;
    taoMaTuDong(L"HD", last_node ? ((HoaDon*)last_node->du_lieu)->ma_hoa_don : NULL, ma_hoa_don, MAX_ID);

    wprintf(L"Nhập số điện thoại khách hàng: ");
    fgetws(so_dien_thoai, 15, stdin);
    trimString(so_dien_thoai);
    if (wcslen(so_dien_thoai) == 0) {
        wprintf(L"Số điện thoại không được để trống!\n");
        return;
    }

    const wchar_t* ten_file_khach_hang = L"khachhang.txt";

    // Tìm khách hàng
    const wchar_t* ma_khach_tim_thay = timMaKhachHangTheoSoDienThoai(so_dien_thoai, ten_file_khach_hang);
    if (ma_khach_tim_thay) {
        wcscpy_s(ma_khach_hang, MAX_ID, ma_khach_tim_thay);

        // Gọi hàm thêm khách hàng
        Node* new_cay_khach_hang = themKhachHangMoi(cay_khach_hang, ma_khach_hang, L"", so_dien_thoai, ten_file_khach_hang);
        if (new_cay_khach_hang != cay_khach_hang) {
            cay_khach_hang = new_cay_khach_hang;
        }
    }
    else {
       
        wprintf(L"Không tìm thấy khách hàng với số điện thoại %ls. Thêm khách hàng mới...\n", so_dien_thoai);
        wchar_t ten[MAX_NAME];
        wprintf(L"Nhập tên khách hàng: ");
        fgetws(ten, MAX_NAME, stdin);
        trimString(ten);
        if (wcslen(ten) == 0) {
            wprintf(L"Tên khách hàng không được để trống!\n");
            return;
        }

        // Tạo mã khách hàng mới
        Node* last_kh = cay_khach_hang;
        while (last_kh && last_kh->right) last_kh = last_kh->right;
        taoMaTuDong(L"KH", last_kh ? ((KhachHang*)last_kh->du_lieu)->ma_khach_hang : NULL, ma_khach_hang, MAX_ID);

        // Thêm khách hàng mới vào cây
        Node* new_cay_khach_hang = themKhachHangMoi(cay_khach_hang, ma_khach_hang, ten, so_dien_thoai, ten_file_khach_hang);
        if (new_cay_khach_hang != cay_khach_hang) {
            cay_khach_hang = new_cay_khach_hang;
        }
        else {
            wprintf(L"Thêm khách hàng thất bại!\n");
            return;
        }
    }

    wchar_t ma_phong[MAX_ID];
    wprintf(L"Nhập mã phòng: ");
    fgetws(ma_phong, MAX_ID, stdin);
    trimString(ma_phong);
    PhongHat ph_tim = { 0 };
    wcscpy_s(ph_tim.ma_phong, MAX_ID, ma_phong);
    Node* node_ph = timNode(cay_phong_hat, &ph_tim, soSanhPhongHat);
    if (!node_ph) {
        wprintf(L"Mã phòng %ls không tồn tại!\n", ma_phong);
        return;
    }
    PhongHat* ph = (PhongHat*)node_ph->du_lieu;

    // Nhập ngày thuê và giờ thuê
    wchar_t ngay_thue_str[20], gio_thue_str[20];
    int year, month, day, hh, mm, ss;

    wprintf(L"Nhập ngày thuê (YYYY-MM-DD): ");
    fgetws(ngay_thue_str, 20, stdin);
    trimString(ngay_thue_str);
    if (swscanf_s(ngay_thue_str, L"%d-%d-%d", &year, &month, &day) != 3 ||
        year < 1900 || month < 1 || month > 12 || day < 1 || day > 31) {
        wprintf(L"Ngày thuê không hợp lệ!\n");
        return;
    }

    wprintf(L"Nhập giờ thuê (HH:MM:SS): ");
    fgetws(gio_thue_str, 20, stdin);
    trimString(gio_thue_str);
    if (swscanf_s(gio_thue_str, L"%d:%d:%d", &hh, &mm, &ss) != 3 ||
        hh < 0 || hh > 23 || mm < 0 || mm > 59 || ss < 0 || ss > 59) {
        wprintf(L"Giờ thuê không hợp lệ!\n");
        return;
    }

    // Tạo struct tm từ ngày + giờ
    struct tm tm_gio_thue = { 0 };
    tm_gio_thue.tm_year = year - 1900;
    tm_gio_thue.tm_mon = month - 1;
    tm_gio_thue.tm_mday = day;
    tm_gio_thue.tm_hour = hh;
    tm_gio_thue.tm_min = mm;
    tm_gio_thue.tm_sec = ss;

    time_t gio_thue = mktime(&tm_gio_thue);
    time_t gio_ra = time(NULL); // giờ ra là thời điểm hiện tại (06:43 PM on May 25, 2025)

    if (gio_ra <= gio_thue) {
        wprintf(L"Giờ ra phải sau giờ thuê! Thời gian hiện tại: %ls\n", _wctime(&gio_ra));
        return;
    }
    if (difftime(gio_ra, gio_thue) <= 0) {
        wprintf(L"❌ Giờ thuê phải trước thời điểm hiện tại!\n");
        return;
    }

    float hours = (float)(difftime(gio_ra, gio_thue) / 3600.0);
    float room_cost = 0;
    int is_after_18 = tm_gio_thue.tm_hour >= 18;
    if (wcscmp(ph->loai_phong, L"VIP") == 0) {
        room_cost = hours * (is_after_18 ? 499000.0f : 299000.0f);
    }
    else {
        room_cost = hours * (is_after_18 ? 299000.0f : 199000.0f);
    }

    // Add invoice details
    float tong_tien = room_cost;
    wprintf(L"\nDanh sách hàng hóa:\n");
    hienThiTatCaHangHoa(cay_hang_hoa);
    wprintf(L"\nNhập chi tiết hóa đơn (nhập mã hàng '0' để kết thúc):\n");
    while (1) {
        wchar_t ma_hang[MAX_ID];
        wprintf(L"Mã hàng: ");
        fgetws(ma_hang, MAX_ID, stdin);
        trimString(ma_hang);
        if (wcscmp(ma_hang, L"0") == 0) break;

        HangHoa hh_tim = { 0 };
        wcscpy_s(hh_tim.ma_hang, MAX_ID, ma_hang);
        Node* node_hh = timNode(cay_hang_hoa, &hh_tim, soSanhHangHoa);
        if (!node_hh) {
            wprintf(L"Mã hàng %ls không tồn tại!\n", ma_hang);
            continue;
        }
        HangHoa* hh = (HangHoa*)node_hh->du_lieu;
        if (!hh) {
            wprintf(L"Dữ liệu hàng hóa không hợp lệ cho mã %ls!\n", ma_hang);
            continue;
        }

        int so_luong;
        wprintf(L"Số lượng: ");
        wchar_t input[10];
        fgetws(input, 10, stdin);
        if (swscanf_s(input, L"%d", &so_luong) != 1 || so_luong <= 0) {
            wprintf(L"Số lượng không hợp lệ!\n");
            continue;
        }

        float don_gia = hh->gia_tien - hh->giam_gia;
        if (don_gia < 0) don_gia = 0;
        themChiTietHoaDonMoi(cay_chi_tiet_hoa_don, ma_hoa_don, ma_hang, so_luong, don_gia, ten_file_chi_tiet);
        tong_tien += so_luong * don_gia;
    }

    // Tạo hóa đơn
    HoaDon* hd = (HoaDon*)malloc(sizeof(HoaDon));
    if (!hd) {
        wprintf(L"Lỗi cấp phát bộ nhớ!\n");
        return;
    }

    wcscpy_s(hd->ma_hoa_don, MAX_ID, ma_hoa_don);
    wcscpy_s(hd->ma_khach_hang, MAX_ID, ma_khach_hang);
    wcscpy_s(hd->ma_phong, MAX_ID, ma_phong);
    hd->tong_tien = tong_tien;
    hd->gio_thue = gio_thue;
    hd->gio_ra = gio_ra;

    *goc_hoa_don = chenNode(*goc_hoa_don, hd, soSanhHoaDon);

    // Save to file
    FILE* file;
    errno_t err = _wfopen_s(&file, ten_file_hoa_don, L"a,ccs=UTF-8");
    if (err != 0 || !file) {
        wprintf(L"Không mở được file %ls để ghi!\n", ten_file_hoa_don);
    }
    else {
        struct tm tm_thue, tm_ra;
        localtime_s(&tm_thue, &gio_thue);
        localtime_s(&tm_ra, &gio_ra);
        wchar_t ngay[26], gio_thue_str[20], gio_ra_str[20];
        wcsftime(ngay, sizeof(ngay) / sizeof(wchar_t), L"%Y-%m-%d", &tm_thue);
        wcsftime(gio_thue_str, sizeof(gio_thue_str) / sizeof(wchar_t), L"%H:%M:%S", &tm_thue);
        wcsftime(gio_ra_str, sizeof(gio_ra_str) / sizeof(wchar_t), L"%H:%M:%S", &tm_ra);
        fwprintf(file, L"%ls %ls %ls %.2f %ls %ls %ls\n",
            ma_hoa_don, ma_khach_hang, ma_phong, tong_tien,
            ngay, gio_thue_str, gio_ra_str);
        fclose(file);
    }

    wprintf(L"Đã thêm hóa đơn %ls thành công!\n", ma_hoa_don);
}


//void timHoaDonTheoMa(Node* goc, const wchar_t* ma_hoa_don) {
//    HoaDon hd_tim = { 0 };
//    wcscpy_s(hd_tim.ma_hoa_don, MAX_ID, ma_hoa_don);
//    Node* node_hd = timNode(goc, &hd_tim, soSanhHoaDon);
//    if (node_hd) {
//        inHoaDon(node_hd, NULL);
//    }
//    else {
//        wprintf(L"Không tìm thấy hóa đơn %ls!\n", ma_hoa_don);
//    }
//}

int soSanhHoaDonTheoTongTien(const void* a, const void* b) {
    const HoaDon* hd1 = (const HoaDon*)a;
    const HoaDon* hd2 = (const HoaDon*)b;
    if (hd1->tong_tien > hd2->tong_tien) return -1;
    if (hd1->tong_tien < hd2->tong_tien) return 1;
    return wcscmp(hd1->ma_hoa_don, hd2->ma_hoa_don);
}
void themVaoCayTheoTongTien(Node* goc, Node** cay_theo_tong_tien) {
    if (!goc) return;

    themVaoCayTheoTongTien(goc->left, cay_theo_tong_tien);

    HoaDon* hd = (HoaDon*)malloc(sizeof(HoaDon));
    if (!hd) {
        wprintf(L"Lỗi cấp phát bộ nhớ cho hóa đơn!\n");
        return;
    }
    memcpy(hd, goc->du_lieu, sizeof(HoaDon));

    *cay_theo_tong_tien = chenNode(*cay_theo_tong_tien, hd, soSanhHoaDonTheoTongTien);

    themVaoCayTheoTongTien(goc->right, cay_theo_tong_tien);
}

void inCayTheoTongTien(Node* goc, Node* cay_chi_tiet_hoa_don) {
    if (!goc) return;
    inCayTheoTongTien(goc->left, cay_chi_tiet_hoa_don);
    inThongTinHoaDon(goc, cay_chi_tiet_hoa_don);
    inCayTheoTongTien(goc->right, cay_chi_tiet_hoa_don);
}

void sapXepHoaDonTheoTongTien(Node* goc, Node* cay_chi_tiet_hoa_don) {
    if (!goc) {
        wprintf(L"Không có hóa đơn.\n");
        return;
    }

    wprintf(L"Danh sách hóa đơn trước khi sắp xếp:\n");
    hienThiTatCaHoaDon(goc, cay_chi_tiet_hoa_don);

    Node* cay_theo_tong_tien = NULL;
    themVaoCayTheoTongTien(goc, &cay_theo_tong_tien);

    wprintf(L"Danh sách hóa đơn sắp xếp theo tổng tiền (giảm dần):\n");
    inCayTheoTongTien(cay_theo_tong_tien, cay_chi_tiet_hoa_don);
}

void themVaoCayTheoMaKH(Node* goc, Node** cay_theo_ma_kh) {
    if (!goc) return;

    themVaoCayTheoMaKH(goc->left, cay_theo_ma_kh);


    HoaDon* hd_cu = (HoaDon*)goc->du_lieu;
    HoaDon* hd_moi = (HoaDon*)malloc(sizeof(HoaDon));
    if (!hd_moi) {
        wprintf(L"Lỗi cấp phát bộ nhớ!\n");
        return;
    }
    memcpy(hd_moi, hd_cu, sizeof(HoaDon));

    *cay_theo_ma_kh = chenNode(*cay_theo_ma_kh, hd_moi, soSanhHoaDonTheoMaKhachHang);

    themVaoCayTheoMaKH(goc->right, cay_theo_ma_kh);
}


int soSanhHoaDonTheoMaKhachHang(const void* a, const void* b) {
    const HoaDon* hd1 = (const HoaDon*)a;
    const HoaDon* hd2 = (const HoaDon*)b;
    int cmp = wcscmp(hd1->ma_khach_hang, hd2->ma_khach_hang);
    if (cmp != 0) return cmp;
    return wcscmp(hd1->ma_hoa_don, hd2->ma_hoa_don);
}

void inCayTheoMaKH(Node* goc, Node* cay_chi_tiet_hoa_don) {
    if (!goc) return;
    inCayTheoMaKH(goc->left, cay_chi_tiet_hoa_don);
    inThongTinHoaDon(goc, cay_chi_tiet_hoa_don);
    inCayTheoMaKH(goc->right, cay_chi_tiet_hoa_don);
}

void sapXepHoaDonTheoMaKhachHang(Node* goc, Node* cay_chi_tiet_hoa_don) {
    if (!goc) {
        wprintf(L"Không có hóa đơn.\n");
        return;
    }

    wprintf(L"Danh sách hóa đơn trước khi sắp xếp:\n");
    hienThiTatCaHoaDon(goc, cay_chi_tiet_hoa_don);

    Node* cay_theo_ma_kh = NULL;
    themVaoCayTheoMaKH(goc, &cay_theo_ma_kh);

    wprintf(L"Danh sách hóa đơn sắp xếp theo mã khách hàng (tăng dần):\n");
    inCayTheoMaKH(cay_theo_ma_kh, cay_chi_tiet_hoa_don);

    freeTree(cay_theo_ma_kh);
}
void hienThiTatCaHoaDon_DeQuy(Node* node, Node* cay_chi_tiet_hoa_don) {
    if (!node) return;

    hienThiTatCaHoaDon_DeQuy(node->left, cay_chi_tiet_hoa_don);

    inThongTinHoaDon(node, cay_chi_tiet_hoa_don);

    hienThiTatCaHoaDon_DeQuy(node->right, cay_chi_tiet_hoa_don);
}

void hienThiTatCaHoaDon(Node* goc, Node* cay_chi_tiet_hoa_don) {
    if (!goc) {
        wprintf(L"Danh sách hóa đơn trống!\n");
        return;
    }

    wprintf(L"\n===== DANH SÁCH TẤT CẢ HÓA ĐƠN =====\n");
    hienThiTatCaHoaDon_DeQuy(goc, cay_chi_tiet_hoa_don);
    wprintf(L"====================================\n");
}


void inThongTinHoaDon(Node* node, Node* cay_chi_tiet_hoa_don) {
    HoaDon* hd = (HoaDon*)node->du_lieu;

    if (hd->gio_thue == (time_t)-1 || hd->gio_ra == (time_t)-1) {
        wprintf(L"Hóa đơn %ls: Thời gian không hợp lệ!\n", hd->ma_hoa_don);
        return;
    }
    wprintf(L"[DEBUG] Mã hóa đơn: %ls\n", hd->ma_hoa_don);

    struct tm tm_thue, tm_ra;
    errno_t err1 = localtime_s(&tm_thue, &hd->gio_thue);
    errno_t err2 = localtime_s(&tm_ra, &hd->gio_ra);

    if (err1 != 0 || err2 != 0 ||
        tm_thue.tm_mon < 0 || tm_thue.tm_mon > 11 ||
        tm_ra.tm_mon < 0 || tm_ra.tm_mon > 11) {
        wprintf(L"Hóa đơn %ls: Thời gian không hợp lệ hoặc không thể chuyển đổi!\n", hd->ma_hoa_don);
        return;
    }

    wchar_t ngay[26], gio_thue_str[20], gio_ra_str[20], tong_tien_vnd[64];
    wcsftime(ngay, sizeof(ngay) / sizeof(wchar_t), L"%Y-%m-%d", &tm_thue);
    wcsftime(gio_thue_str, sizeof(gio_thue_str) / sizeof(wchar_t), L"%H:%M:%S", &tm_thue);
    wcsftime(gio_ra_str, sizeof(gio_ra_str) / sizeof(wchar_t), L"%H:%M:%S", &tm_ra);
    dinhDangVND(hd->tong_tien, tong_tien_vnd, sizeof(tong_tien_vnd) / sizeof(wchar_t));

    wprintf(L"Hóa đơn: %ls, KH: %ls, Phòng: %ls, Tổng tiền: %ls, Ngày: %ls, Giờ thuê: %ls, Giờ ra: %ls\n",
        hd->ma_hoa_don, hd->ma_khach_hang, hd->ma_phong, tong_tien_vnd, ngay, gio_thue_str, gio_ra_str);

    if (cay_chi_tiet_hoa_don) {
        wprintf(L"  Chi tiết hóa đơn:\n");
        hienThiChiTietHoaDonTheoMaHoaDon(cay_chi_tiet_hoa_don, hd->ma_hoa_don);
    }
}
