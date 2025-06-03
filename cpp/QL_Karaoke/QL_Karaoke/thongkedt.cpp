#include "thongkedt.h"
#include "common.h"
#include <time.h>
#include <stdio.h>

double thongKeDoanhThu(Node* goc, struct tm bat_dau_tm, struct tm ket_thuc_tm) {
    if (!goc) return 0;

    time_t bat_dau = mktime(&bat_dau_tm);
    time_t ket_thuc = mktime(&ket_thuc_tm);

    if (bat_dau == (time_t)-1 || ket_thuc == (time_t)-1 || bat_dau > ket_thuc)
        return 0;

    double tong = 0;

    if (goc->du_lieu != NULL) {
        HoaDon* hd = (HoaDon*)goc->du_lieu;

        if (hd->ngay_thue >= bat_dau && hd->ngay_thue <= ket_thuc) {
            tong += hd->tong_tien;
        }
    }

    tong += thongKeDoanhThu(goc->left, bat_dau_tm, ket_thuc_tm);
    tong += thongKeDoanhThu(goc->right, bat_dau_tm, ket_thuc_tm);

    return tong;
}


double thongKeDoanhThuTheoNgay(Node* goc, int year, int month, int day) {
    struct tm bat_dau_tm = { 0 };
    struct tm ket_thuc_tm = { 0 };

    bat_dau_tm.tm_year = year - 1900;
    bat_dau_tm.tm_mon = month - 1;
    bat_dau_tm.tm_mday = day;

    ket_thuc_tm = bat_dau_tm;
    ket_thuc_tm.tm_hour = 23;
    ket_thuc_tm.tm_min = 59;
    ket_thuc_tm.tm_sec = 59;

    return thongKeDoanhThu(goc, bat_dau_tm, ket_thuc_tm);
}

double thongKeDoanhThuTheoThang(Node* goc, int year, int month) {
    struct tm bat_dau_tm = { 0 };
    struct tm ket_thuc_tm = { 0 };

    bat_dau_tm.tm_year = year - 1900;
    bat_dau_tm.tm_mon = month - 1;
    bat_dau_tm.tm_mday = 1;

    // Ngày bắt đầu tháng kế tiếp
    ket_thuc_tm.tm_year = year - 1900;
    ket_thuc_tm.tm_mon = month;
    ket_thuc_tm.tm_mday = 1;

    time_t t = mktime(&ket_thuc_tm);
    t -= 1; 
    localtime_s(&ket_thuc_tm, &t);

    return thongKeDoanhThu(goc, bat_dau_tm, ket_thuc_tm);
}

double thongKeDoanhThuTheoQuy(Node* goc, int year, int quarter) {
    if (quarter < 1 || quarter > 4) return 0;

    int start_month = (quarter - 1) * 3 + 1;

    struct tm bat_dau_tm = { 0 };
    struct tm ket_thuc_tm = { 0 };

    bat_dau_tm.tm_year = year - 1900;
    bat_dau_tm.tm_mon = start_month - 1;
    bat_dau_tm.tm_mday = 1;

    ket_thuc_tm.tm_year = year - 1900;
    ket_thuc_tm.tm_mon = start_month + 2;
    ket_thuc_tm.tm_mday = 1;

    time_t t = mktime(&ket_thuc_tm);
    t -= 1;
    localtime_s(&ket_thuc_tm, &t);

    return thongKeDoanhThu(goc, bat_dau_tm, ket_thuc_tm);
}

double thongKeDoanhThuTheoNam(Node* goc, int year) {
    struct tm bat_dau_tm = { 0 };
    struct tm ket_thuc_tm = { 0 };

    bat_dau_tm.tm_year = year - 1900;
    bat_dau_tm.tm_mon = 0;
    bat_dau_tm.tm_mday = 1;

    ket_thuc_tm.tm_year = year - 1900;
    ket_thuc_tm.tm_mon = 11;
    ket_thuc_tm.tm_mday = 31;
    ket_thuc_tm.tm_hour = 23;
    ket_thuc_tm.tm_min = 59;
    ket_thuc_tm.tm_sec = 59;

    return thongKeDoanhThu(goc, bat_dau_tm, ket_thuc_tm);
}
