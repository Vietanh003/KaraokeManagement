#include "CustomerBST.h"

// Hàm tạo nút mới
BSTNode* createNode(const char* id, const char* name) {
    BSTNode* newNode = (BSTNode*)malloc(sizeof(BSTNode));
    if (newNode) {
        strcpy(newNode->data.id, id);
        strcpy(newNode->data.name, name);
        newNode->left = NULL;
        newNode->right = NULL;
    }
    return newNode;
}

// Hàm thêm khách hàng vào BST
BSTNode* insertCustomer(BSTNode* root, const char* id, const char* name) {
    if (root == NULL) {
        return createNode(id, name);
    }
    if (strcmp(id, root->data.id) < 0) {
        root->left = insertCustomer(root->left, id, name);
    } else if (strcmp(id, root->data.id) > 0) {
        root->right = insertCustomer(root->right, id, name);
    }
    return root;
}

// Hàm duyệt BST theo thứ tự
void inOrderTraversal(BSTNode* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printf("Customer ID: %s, Name: %s\n", root->data.id, root->data.name);
        inOrderTraversal(root->right);
    }
}

// Hàm giải phóng BST
void freeBST(BSTNode* root) {
    if (root != NULL) {
        freeBST(root->left);
        freeBST(root->right);
        free(root);
    }
}

// Hàm main để test BST
int main() {
    BSTNode* root = NULL;

    // Thêm một số khách hàng để test
    root = insertCustomer(root, "C001", "Nguyen Van A");
    root = insertCustomer(root, "C002", "Tran Thi B");
    root = insertCustomer(root, "C003", "Le Van C");
    root = insertCustomer(root, "C000", "Pham Thi D");

    // Duyệt và hiển thị danh sách khách hàng
    printf("List of customers (sorted by ID):\n");
    inOrderTraversal(root);

    // Giải phóng bộ nhớ
    freeBST(root);

    return 0;
}