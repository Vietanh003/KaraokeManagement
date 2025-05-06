#ifndef CUSTOMER_BST_H
#define CUSTOMER_BST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cấu trúc khách hàng
typedef struct {
    char id[10]; // Mã khách hàng (ví dụ: C001)
    char name[50]; // Tên khách hàng
} Customer;

// Cấu trúc nút BST
typedef struct BSTNode {
    Customer data;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

// Hàm thêm khách hàng vào BST
BSTNode* insertCustomer(BSTNode* root, const char* id, const char* name);

// Hàm duyệt BST theo thứ tự (inorder)
void inOrderTraversal(BSTNode* root);

// Hàm giải phóng BST
void freeBST(BSTNode* root);

#endif