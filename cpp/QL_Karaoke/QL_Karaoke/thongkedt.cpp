#include "thongkedt.h"
#include "common.h"
#include <time.h>
#include <wchar.h>
#include <stdio.h>

double thongKeDoanhThu(Node* goc, struct tm bat_dau_tm, struct tm ket_thuc_tm) {
    if (!goc) return 0;

    time_t bat_dau = mktime(&bat_dau_tm);
    time_t ket_thuc = mktime(&ket_thuc_tm);

    if (bat_dau == (time_t)-1 || ket_thuc == (time_t)-1 || bat_dau > ket_thuc)
        return 0;

    double tong = 0;
    HoaDon* hd = (HoaDon*)goc->du_lieu;

    if (hd->ngay_thue >= bat_dau && hd->ngay_thue <= ket_thuc) {
        tong += hd->tong_tien;
    }

    tong += thongKeDoanhThu(goc->left, bat_dau_tm, ket_thuc_tm);
    tong += thongKeDoanhThu(goc->right, bat_dau_tm, ket_thuc_tm);

    return tong;
}
