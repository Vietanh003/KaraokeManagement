
#ifndef COMMON_H
#define COMMON_H

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

typedef struct Node {
    void* du_lieu;
    struct Node* left;
    struct Node* right;
    int chieu_cao;
} Node;

typedef int (*SoSanh)(const void*, const void*);

int layChieuCao(Node* node);
int layHeSoCanBang(Node* node);
Node* xoayPhai(Node* y);
Node* xoayTrai(Node* x);
Node* taoNode(void* du_lieu);
Node* chenNode(Node* goc, void* du_lieu, SoSanh so_sanh);
Node* timNode(Node* goc, void* du_lieu, SoSanh so_sanh);
void freeTree(Node* node);
void dinhDangVND(double so_tien, wchar_t* ket_qua, size_t kich_thuoc);
void taoMaTuDong(const wchar_t* tien_to, const wchar_t* ma_cuoi, wchar_t* ma_moi, size_t kich_thuoc);
void trimString(wchar_t* str);
Node* canBangLai(Node* node, SoSanh so_sanh);
#endif