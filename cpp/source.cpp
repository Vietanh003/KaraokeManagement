#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct PhongHat {
    char maPhong[10];
    char tenPhong[50];
    int soLanThue;
    struct PhongHat* next;
} PhongHat;

typedef struct {
    PhongHat* head;
} DanhSachPhongHat;

typedef struct Node {
    PhongHat* phong;
    struct Node* left;
    struct Node* right;
} Node;

// Tạo phòng hát
PhongHat* taoPhongHat(char* ma, char* ten, int soLan) {
    PhongHat* p = (PhongHat*)malloc(sizeof(PhongHat));
    strcpy(p->maPhong, ma);
    strcpy(p->tenPhong, ten);
    p->soLanThue = soLan;
    p->next = NULL;
    return p;
}

// Thêm vào đầu danh sách
void themPhongHat(DanhSachPhongHat* ds, PhongHat* p) {
    p->next = ds->head;
    ds->head = p;
}

// Ghi danh sách vào file
void ghiFile(DanhSachPhongHat* ds, char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        printf("Khong mo duoc file de ghi!\n");
        return;
    }
    PhongHat* p = ds->head;
    while (p) {
        fprintf(f, "%s\n%s\n%d\n", p->maPhong, p->tenPhong, p->soLanThue);
        p = p->next;
    }
    fclose(f);
}

// Đọc danh sách từ file
void docFile(DanhSachPhongHat* ds, char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Khong mo duoc file de doc!\n");
        return;
    }
    char ma[10], ten[50];
    int soLan;
    while (fscanf(f, "%s\n%[^\n]\n%d\n", ma, ten, &soLan) != EOF) {
        PhongHat* p = taoPhongHat(ma, ten, soLan);
        themPhongHat(ds, p);
    }
    fclose(f);
}

// Hiển thị danh sách
void hienThiDanhSach(DanhSachPhongHat* ds) {
    PhongHat* p = ds->head;
    while (p) {
        printf("Ma phong: %s\nTen phong: %s\nSo lan thue: %d\n\n", p->maPhong, p->tenPhong, p->soLanThue);
        p = p->next;
    }
}
// Chèn phòng hát vào cây theo số lần thuê giảm dần
Node* chenNode(Node* root, PhongHat* p) {
    if (root == NULL) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->phong = p;
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    if (p->soLanThue > root->phong->soLanThue)
        root->left = chenNode(root->left, p);
    else
        root->right = chenNode(root->right, p);
    return root;
}

// Duyệt cây theo thứ tự giảm dần và in ra top 3
void duyetTop3(Node* root, int* count) {
    if (root == NULL || *count >= 3) return;
    duyetTop3(root->left, count);
    if (*count < 3) {
        printf("TOP %d: %s - %s (%d lan thue)\n", *count + 1, root->phong->maPhong, root->phong->tenPhong, root->phong->soLanThue);
        (*count)++;
    }
    duyetTop3(root->right, count);
}

void thongKeTop3(DanhSachPhongHat* ds) {
    Node* root = NULL;
    PhongHat* p = ds->head;
    while (p) {
        root = chenNode(root, p);
        p = p->next;
    }
    int count = 0;
    printf("\n--- TOP 3 PHONG THUE NHIEU NHAT ---\n");
    duyetTop3(root, &count);
}
void menu() {
    printf("\n===== MENU =====\n");
    printf("1. Them phong hat moi\n");
    printf("2. Doc danh sach tu file\n");
    printf("3. Ghi danh sach vao file\n");
    printf("4. Hien thi danh sach\n");
    printf("5. Thong ke top 3 phong thue nhieu nhat\n");
    printf("0. Thoat\n");
}

int main() {
    DanhSachPhongHat ds = { NULL };
    int ch;
    char ma[10], ten[50];
    int soLan;
    do {
        menu();
        printf("Nhap lua chon: ");
        scanf("%d", &ch);
        getchar(); // bỏ ký tự \n

        switch (ch) {
        case 1:
            printf("Nhap ma phong: "); gets(ma);
            printf("Nhap ten phong: "); gets(ten);
            printf("Nhap so lan thue: "); scanf("%d", &soLan); getchar();
            themPhongHat(&ds, taoPhongHat(ma, ten, soLan));
            break;
        case 2:
            docFile(&ds, "phonghat.txt");
            break;
        case 3:
            ghiFile(&ds, "phonghat.txt");
            break;
        case 4:
            hienThiDanhSach(&ds);
            break;
        case 5:
            thongKeTop3(&ds);
            break;
        }
    } while (ch != 0);

    return 0;
}