#include "common.h"
#include "khachhang.h"
#include "phonghat.h"
#include "hoadon.h"
#include "hanghoa.h"
#include "thongkedt.h"
#include "chitiethoadon.h"
#include <windows.h>
#include <stdio.h>
#include <wchar.h>

void hienThiMenu() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // Set green color for the title
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    // Top border and title
    wprintf(L"╔════════════════════════════════════╗\n");
    wprintf(L"║      HỆ THỐNG QUẢN LÝ KARAOKE      ║\n");
    wprintf(L"╚════════════════════════════════════╝\n");

    // Reset to default color (white text)
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    // Menu options with consistent spacing
    wprintf(L"║  1. Thêm phòng hát mới             ║\n");
    wprintf(L"║  2. Thêm một khách hàng mới        ║\n");
    wprintf(L"║  3. Thêm hàng hóa mới              ║\n");
    wprintf(L"║  4. Thêm hóa đơn và chi tiết hóa đơn║\n");
    wprintf(L"║  5. Hiển thị tất cả khách hàng     ║\n");
    wprintf(L"║  6. Hiển thị tất cả phòng hát      ║\n");
    wprintf(L"║  7. Hiển thị tất cả hàng hóa       ║\n");
    wprintf(L"║  8. Hiển thị tất cả hóa đơn        ║\n");
    wprintf(L"║  9. Chỉnh sửa hàng hóa             ║\n");
    wprintf(L"║ 10. Xóa hàng hóa                   ║\n");
    wprintf(L"║ 11. Sắp xếp hóa đơn theo tổng tiền ║\n");
    wprintf(L"║     (giảm dần)                     ║\n");
    wprintf(L"║ 12. Sắp xếp hóa đơn theo mã khách  ║\n");
    wprintf(L"║     hàng (tăng dần)                ║\n");
    wprintf(L"║ 13. Thống kê top 3 phòng thuê nhiều║\n");
    wprintf(L"║     nhất                           ║\n");
    wprintf(L"║ 14. Tìm kiếm hóa đơn theo số hóa   ║\n");
    wprintf(L"║     đơn                            ║\n");
    wprintf(L"║ 15. Tìm kiếm và liệt kê hóa đơn    ║\n");
    wprintf(L"║    theo mã khách hàng              ║\n");
    wprintf(L"║ 16. Thống kê doanh thu theo khoảng ║\n");
    wprintf(L"║ 17. Thống kê doanh thu ngày        ║\n");
    wprintf(L"║ 18. Thống kê doanh thu tháng       ║\n");
    wprintf(L"║ 19. Thống kê doanh thu quý         ║\n");
    wprintf(L"║ 20. Thống kê doanh thu năm         ║\n");
    wprintf(L"║     thời gian                      ║\n");
    wprintf(L"║ 21. Thoát                          ║\n");
    wprintf(L"╚════════════════════════════════════╝\n");

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
    wprintf(L"\nNhập lựa chọn (1-21): ");

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
time_t normalizeToDayStart(time_t t) {
    struct tm tm;
    localtime_s(&tm, &t); 
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    return mktime(&tm); 
}
void xuLyMenu(Node** cay_khach_hang, Node** cay_phong_hat, Node** cay_hoa_don, Node** cay_hang_hoa, Node** cay_chi_tiet_hoa_don) {
    int lua_chon;
    wchar_t ma_khach_hang[MAX_ID], ten[MAX_NAME], so_dien_thoai[15];
    wchar_t ma_hoa_don[MAX_ID], ma_phong[MAX_ID];
    wchar_t ma_hang[MAX_ID];
    float tong_tien;
    time_t ngay_thue;
    wchar_t bat_dau[20], ket_thuc[20];
    wchar_t loai_phong[20];
    wchar_t ten_hang[MAX_NAME];
    double gia_tien, giam_gia;
    const wchar_t* ten_file_khach_hang = L"khachhang.txt";
    const wchar_t* ten_file_phong_hat = L"phonghat.txt";
    const wchar_t* ten_file_hoa_don = L"haodon.txt";
    const wchar_t* ten_file_hang_hoa = L"hanghoa.txt";
    const wchar_t* ten_file_chi_tiet = L"chitiethoadon.txt";

    wprintf(L"Đang đọc dữ liệu từ các file...\n");
    *cay_khach_hang = docKhachHangTuFile(ten_file_khach_hang);
    wprintf(L"Đã đọc dữ liệu khách hàng từ file %ls\n", ten_file_khach_hang);
    *cay_phong_hat = docPhongHatTuFile(ten_file_phong_hat);
    wprintf(L"Đã đọc dữ liệu phòng hát từ file %ls\n", ten_file_phong_hat);
    *cay_hoa_don = docHoaDonTuFile(ten_file_hoa_don);
    wprintf(L"Đã đọc dữ liệu hóa đơn từ file %ls\n", ten_file_hoa_don);
    *cay_hang_hoa = docHangHoaTuFile(ten_file_hang_hoa);
    wprintf(L"Đã đọc dữ liệu hàng hóa từ file %ls\n", ten_file_hang_hoa);
    *cay_chi_tiet_hoa_don = docChiTietHoaDonTuFile(ten_file_chi_tiet);
    wprintf(L"Đã đọc dữ liệu chi tiết hóa đơn từ file %ls\n", ten_file_chi_tiet);

    do {
        hienThiMenu();
        wchar_t input[10];
        fgetws(input, 10, stdin);
        if (swscanf_s(input, L"%d", &lua_chon) == 1) {
            switch (lua_chon) {
            case 1: // Thêm phòng hát mới
            {
                wprintf(L"Nhập mã phòng: ");
                fgetws(ma_phong, MAX_ID, stdin);
                ma_phong[wcscspn(ma_phong, L"\n")] = 0;
                if (wcslen(ma_phong) == 0) {
                    wprintf(L"Mã phòng không được để trống!\n");
                    break;
                }
                wprintf(L"Chọn loại phòng:\n");
                wprintf(L"1. VIP\n");
                wprintf(L"2. Thường\n");
                wprintf(L"Nhập lựa chọn (1 hoặc 2): ");
                wchar_t input_loai_phong[10];
                int loai_phong_choice;
                fgetws(input_loai_phong, 10, stdin);
                if (swscanf_s(input_loai_phong, L"%d", &loai_phong_choice) != 1 || (loai_phong_choice != 1 && loai_phong_choice != 2)) {
                    wprintf(L"Lựa chọn không hợp lệ!\n");
                    break;
                }
                wcscpy_s(loai_phong, 20, loai_phong_choice == 1 ? L"VIP" : L"Thường");
                themPhongHatMoi(cay_phong_hat, ma_phong, loai_phong, ten_file_phong_hat);
                ghiPhongHatRaFile(*cay_phong_hat, ten_file_phong_hat);
                wprintf(L"Đã thêm phòng hát mới.\n");
            }
            break;

            case 2: // Thêm một khách hàng mới
            {
                wchar_t ma_khach_hang[MAX_ID] = L"";
                wprintf(L"Nhập tên khách hàng: ");
                fgetws(ten, MAX_NAME, stdin);
                trimString(ten);
                wprintf(L"Nhập số điện thoại: ");
                fgetws(so_dien_thoai, 15, stdin);
                trimString(so_dien_thoai);
                *cay_khach_hang = themKhachHangMoi(*cay_khach_hang, ma_khach_hang, ten, so_dien_thoai, ten_file_khach_hang);
            }
            break;

            case 3: // Thêm hàng hóa mới
            {
                wprintf(L"Nhập tên hàng: ");
                if (fgetws(ten_hang, MAX_NAME, stdin) == NULL) {
                    wprintf(L"Lỗi đọc tên hàng!\n");
                    break;
                }
                trimString(ten_hang);
                if (wcslen(ten_hang) == 0) {
                    wprintf(L"Tên hàng không được để trống!\n");
                    break;
                }

                wchar_t price_input[20];
                int valid_price = 0;
                do {
                    wprintf(L"Nhập giá tiền: ");
                    if (fgetws(price_input, 20, stdin) == NULL) {
                        wprintf(L"Lỗi đọc giá tiền! Nhập lại.\n");
                        continue;
                    }
                    trimString(price_input);
                    if (wcslen(price_input) == 0) {
                        wprintf(L"Giá tiền không được để trống! Nhập lại.\n");
                        continue;
                    }
                    if (swscanf_s(price_input, L"%lf", &gia_tien) == 1 && gia_tien >= 0) {
                        valid_price = 1;
                    }
                    else {
                        wprintf(L"Giá tiền không hợp lệ! Nhập lại.\n");
                    }
                } while (!valid_price);

                wchar_t discount_input[20];
                int valid_discount = 0;
                do {
                    wprintf(L"Nhập giảm giá: ");
                    if (fgetws(discount_input, 20, stdin) == NULL) {
                        wprintf(L"Lỗi đọc giảm giá! Nhập lại.\n");
                        continue;
                    }
                    trimString(discount_input);
                    if (wcslen(discount_input) == 0) {
                        wprintf(L"Giảm giá không được để trống! Nhập lại.\n");
                        continue;
                    }
                    if (swscanf_s(discount_input, L"%lf", &giam_gia) == 1 && giam_gia >= 0 && giam_gia <= 100) {
                        valid_discount = 1;
                    }
                    else {
                        wprintf(L"Giảm giá không hợp lệ (0-100%%)! Nhập lại.\n");
                    }
                } while (!valid_discount);

                themHangHoaMoi(cay_hang_hoa, ten_hang, gia_tien, giam_gia, ten_file_hang_hoa);
            }
            break;

            case 4: // Thêm hóa đơn và chi tiết hóa đơn
            {
                themHoaDon(cay_hoa_don, *cay_khach_hang, *cay_phong_hat, *cay_hang_hoa, cay_chi_tiet_hoa_don, ten_file_hoa_don, ten_file_chi_tiet);
                *cay_khach_hang = docKhachHangTuFile(ten_file_khach_hang); // Reload to update customer tree
            }
            break;

            case 5: // Hiển thị tất cả khách hàng
            {
                hienThiTatCaKhachHang(*cay_khach_hang);
            }
            break;

            case 6: // Hiển thị tất cả phòng hát
            {
                hienThiTatCaPhongHat(*cay_phong_hat);
            }
            break;

            case 7: // Hiển thị tất cả hàng hóa
            {
                hienThiTatCaHangHoa(*cay_hang_hoa);
            }
            break;

            case 8: // Hiển thị tất cả hóa đơn
            {
                hienThiTatCaHoaDon(*cay_hoa_don, *cay_chi_tiet_hoa_don);
            }
            break;

            case 9: // Chỉnh sửa hàng hóa
            {
                wprintf(L"Nhập mã hàng hóa cần chỉnh sửa: ");
                if (fgetws(ma_hang, MAX_ID, stdin)) {
                    ma_hang[wcscspn(ma_hang, L"\n")] = 0;
                    if (wcslen(ma_hang) == 0) {
                        wprintf(L"Mã hàng không được để trống!\n");
                        break;
                    }
                    chinhSuaHangHoa(*cay_hang_hoa, ma_hang);
                    ghiHangHoaRaFile(*cay_hang_hoa, ten_file_hang_hoa);
                }
            }
            break;

            case 10: // Xóa hàng hóa
            {
                wprintf(L"Nhập mã hàng hóa cần xóa: ");
                if (fgetws(ma_hang, MAX_ID, stdin)) {
                    ma_hang[wcscspn(ma_hang, L"\n")] = 0;
                    if (wcslen(ma_hang) == 0) {
                        wprintf(L"Mã hàng không được để trống!\n");
                        break;
                    }
                    int da_xoa = 0;
                    *cay_hang_hoa = xoaHangHoa(*cay_hang_hoa, ma_hang, &da_xoa);
                    if (da_xoa) {
                        wprintf(L"Đã xóa hàng hóa mã %ls thành công!\n", ma_hang);
                        ghiHangHoaRaFile(*cay_hang_hoa, ten_file_hang_hoa);
                    }
                    else {
                        wprintf(L"Không tìm thấy hàng hóa mã %ls để xóa!\n", ma_hang);
                    }
                }
            }
            break;

            case 11: // Sắp xếp hóa đơn theo tổng tiền (giảm dần)
            {
                sapXepHoaDonTheoTongTien(*cay_hoa_don, *cay_chi_tiet_hoa_don);
            }
            break;

            case 12: // Sắp xếp hóa đơn theo mã khách hàng (tăng dần)
            {
                sapXepHoaDonTheoMaKhachHang(*cay_hoa_don, *cay_chi_tiet_hoa_don);
            }
            break;

            case 13: // Thống kê top 3 phòng thuê nhiều nhất
            {
                // TODO: Implement thongKeTop3Phong
               
                wprintf(L"Chức năng thống kê top 3 phòng thuê nhiều nhất chưa được triển khai!\n");
            }
            break;

            case 14: // Tìm kiếm hóa đơn theo số hóa đơn
            {
                wprintf(L"Nhập mã hóa đơn: ");
                fgetws(ma_hoa_don, MAX_ID, stdin);
                ma_hoa_don[wcscspn(ma_hoa_don, L"\n")] = 0;
               /* timHoaDonTheoMa(*cay_hoa_don, ma_hoa_don);*/
            }
            break;

            case 15: // Tìm kiếm và liệt kê hóa đơn theo mã khách hàng
            {
                wprintf(L"Nhập mã khách hàng: ");
                fgetws(ma_khach_hang, MAX_ID, stdin);
                ma_khach_hang[wcscspn(ma_khach_hang, L"\n")] = 0;
                lietKeHoaDonTheoKhachHang(*cay_khach_hang, ma_khach_hang);
            }
            break;
            case 16: // Thống kê doanh thu theo khoảng thời gian
            {
                wchar_t bat_dau[20], ket_thuc[20];
                wprintf(L"Nhập ngày bắt đầu (YYYY-MM-DD): ");
                fgetws(bat_dau, 20, stdin);
                bat_dau[wcslen(bat_dau) - 1] = L'\0';

                wprintf(L"Nhập ngày kết thúc (YYYY-MM-DD): ");
                fgetws(ket_thuc, 20, stdin);
                ket_thuc[wcslen(ket_thuc) - 1] = L'\0';

                struct tm tm_bat_dau = { 0 }, tm_ket_thuc = { 0 };
                swscanf_s(bat_dau, L"%d-%d-%d", &tm_bat_dau.tm_year, &tm_bat_dau.tm_mon, &tm_bat_dau.tm_mday);
                swscanf_s(ket_thuc, L"%d-%d-%d", &tm_ket_thuc.tm_year, &tm_ket_thuc.tm_mon, &tm_ket_thuc.tm_mday);
                tm_bat_dau.tm_year -= 1900;
                tm_bat_dau.tm_mon -= 1;
                tm_ket_thuc.tm_year -= 1900;
                tm_ket_thuc.tm_mon -= 1;

                double doanh_thu = thongKeDoanhThu(*cay_hoa_don, tm_bat_dau, tm_ket_thuc);

                // Định dạng tiền tệ
                wchar_t ket_qua[64];
                dinhDangVND(doanh_thu, ket_qua, sizeof(ket_qua) / sizeof(wchar_t));

                wprintf(L"Tổng doanh thu: %ls\n", ket_qua);
                break;
            }

            case 17:
            {
                wchar_t ngay[20];
                wprintf(L"Nhập ngày (YYYY-MM-DD): ");
                fgetws(ngay, _countof(ngay), stdin); 
                trimString(ngay);

                int year, month, day;
                if (swscanf_s(ngay, L"%d-%d-%d", &year, &month, &day) != 3) {
                    wprintf(L"Định dạng ngày không hợp lệ! Vui lòng nhập theo YYYY-MM-DD.\n");
                    break;
                }

                if (month < 1 || month > 12 || day < 1 || day > 31 || year < 1900) {
                    wprintf(L"Ngày không hợp lệ!\n");
                    break;
                }

                double doanh_thu = thongKeDoanhThuTheoNgay(*cay_hoa_don, year, month, day);

                wchar_t ket_qua[64]; 
                dinhDangVND(doanh_thu, ket_qua, _countof(ket_qua)); 

                wprintf(L"Doanh thu ngày %ls: %ls\n", ngay, ket_qua);
            }
            break;

            case 18: // Thống kê doanh thu theo tháng
            {
                wchar_t thang[20];
                wprintf(L"Nhập tháng (YYYY-MM): ");
                fgetws(thang, 20, stdin);
                thang[wcslen(thang) - 1] = L'\0';

                int year, month;
                if (swscanf_s(thang, L"%d-%d", &year, &month) != 2 || month < 1 || month > 12) {
                    wprintf(L"Định dạng tháng không hợp lệ! Vui lòng nhập theo YYYY-MM.\n");
                    break;
                }

                double doanh_thu = thongKeDoanhThuTheoThang(*cay_hoa_don, year, month);
                wchar_t ket_qua[64];
                dinhDangVND(doanh_thu, ket_qua, sizeof(ket_qua) / sizeof(wchar_t));
                wprintf(L"Doanh thu tháng %ls: %ls\n", thang, ket_qua);
            }
            break;

            case 19: // Thống kê doanh thu theo quý
            {
                wchar_t quy[20];
                wprintf(L"Nhập quý (YYYY-Q): ");
                fgetws(quy, 20, stdin);
                quy[wcslen(quy) - 1] = L'\0';

                int year, quarter;
                if (swscanf_s(quy, L"%d-%d", &year, &quarter) != 2 || quarter < 1 || quarter > 4) {
                    wprintf(L"Định dạng quý không hợp lệ! Vui lòng nhập theo YYYY-Q (Q từ 1 đến 4).\n");
                    break;
                }

                double doanh_thu = thongKeDoanhThuTheoQuy(*cay_hoa_don, year, quarter);
                wchar_t ket_qua[64];
                dinhDangVND(doanh_thu, ket_qua, sizeof(ket_qua) / sizeof(wchar_t));
                wprintf(L"Doanh thu quý %ls: %ls\n", quy, ket_qua);
            }
            break;

            case 20: // Thống kê doanh thu theo năm
            {
                wchar_t nam[20];
                wprintf(L"Nhập năm (YYYY): ");
                fgetws(nam, 20, stdin);
                nam[wcslen(nam) - 1] = L'\0';

                int year;
                if (swscanf_s(nam, L"%d", &year) != 1) {
                    wprintf(L"Định dạng năm không hợp lệ! Vui lòng nhập theo YYYY.\n");
                    break;
                }

                double doanh_thu = thongKeDoanhThuTheoNam(*cay_hoa_don, year);
                wchar_t ket_qua[64];
                dinhDangVND(doanh_thu, ket_qua, sizeof(ket_qua) / sizeof(wchar_t));
                wprintf(L"Doanh thu năm %ls: %ls\n", nam, ket_qua);
            }
            break;

            case 21: // Thoát
            {
                wprintf(L"Thoát chương trình.\n");
            }
            break;

            default:
            {
                wprintf(L"Lựa chọn không hợp lệ!\n");
            }
            break;
            }
        }
        else {
            wprintf(L"Lựa chọn không hợp lệ!\n");
            while (getwchar() != L'\n');
        }
    } while (lua_chon != 17);
}
int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    _wsetlocale(LC_ALL, L"vi-VN.UTF-8");

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
    Node* cay_hang_hoa = NULL;
    Node* cay_chi_tiet_hoa_don = NULL;

    xuLyMenu(&cay_khach_hang, &cay_phong_hat, &cay_hoa_don, &cay_hang_hoa, &cay_chi_tiet_hoa_don);

    freeTree(cay_khach_hang);
    freeTree(cay_phong_hat);
    freeTree(cay_hoa_don);
    freeTree(cay_hang_hoa);
    freeTree(cay_chi_tiet_hoa_don);

    return 0;
}
