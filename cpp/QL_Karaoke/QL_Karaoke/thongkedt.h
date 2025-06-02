#ifndef THONGKEDT_H
#define THONGKEDT_H

#include "common.h"
#include "hoadon.h"

double thongKeDoanhThu(Node* goc, struct tm bat_dau, struct tm ket_thuc);

double thongKeDoanhThuTheoNgay(Node* goc, int year, int month, int day);

double thongKeDoanhThuTheoThang(Node* goc, int year, int month);

double thongKeDoanhThuTheoQuy(Node* goc, int year, int quarter);

double thongKeDoanhThuTheoNam(Node* goc, int year);

#endif