#include <iostream>
#include <string>
#include <vector>
#include <cctype>   // Thư viện xử lý ký tự
#include <iomanip> 

using namespace std;

// ====================================================
// 1. CẤU TRÚC DỮ LIỆU
// ====================================================

struct Node {
    string tu_tieng_anh;     // Key (Từ khóa)
    string nghia_tieng_viet; // Value (Nghĩa)
    Node* next;              // Con trỏ liên kết
};

// Bảng băm gồm 26 phần tử (tương ứng A -> Z)
// Mỗi phần tử là một "đầu tàu" quản lý danh sách từ của chữ cái đó
const int TABLE_SIZE = 26;
Node* hashTable[TABLE_SIZE];

// ====================================================
// 2. CÁC HÀM HỖ TRỢ (UTILITIES)
// ====================================================

// Khởi tạo bảng băm rỗng
void khoiTao() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
}

// Hàm Băm: Chuyển ký tự đầu thành index (0-25)
// Ví dụ: 'A' hoặc 'a' -> 0, 'B' -> 1...
int hashFunction(string key) {
    if (key.empty()) return -1;
    char firstChar = tolower(key[0]); // Chuyển về chữ thường để đồng nhất
    if (firstChar >= 'a' && firstChar <= 'z') {
        return firstChar - 'a';
    }
    return -1; // Ký tự không hợp lệ (số, ký tự đặc biệt...)
}

// ====================================================
// 3. CÁC CHỨC NĂNG CHÍNH (CORE FEATURES)
// ====================================================

// --- CHỨC NĂNG 1: THÊM TỪ (CÓ SẮP XẾP TĂNG DẦN) ---
void themTu() {
    string key, meaning;
    cout << "\n>> Nhap tu Tieng Anh: ";
    getline(cin, key);
    
    // 1. Validation (Kiểm tra dữ liệu)
    int index = hashFunction(key);
    if (index == -1) {
        cout << "(!) LOI: Tu phai bat dau bang chu cai (A-Z)." << endl;
        return;
    }
    
    cout << ">> Nhap nghia Tieng Viet: ";
    getline(cin, meaning);

    // 2. Tạo Node mới
    Node* newNode = new Node;
    newNode->tu_tieng_anh = key;
    newNode->nghia_tieng_viet = meaning;
    newNode->next = NULL;

    // 3. Xử lý chèn vào danh sách (Sorted Insertion)
    // Tại đây, ta chỉ quan tâm đến danh sách ở hashTable[index] (Ví dụ nhóm A)
    // KHÔNG liên quan gì đến các nhóm khác.

    // TH1: Chèn vào đầu danh sách (Head)
    // Xảy ra khi: Danh sách rỗng HOẶC Từ mới nhỏ hơn từ đầu tiên (Ví dụ: thêm "Ant" khi đang có "Apple")
    if (hashTable[index] == NULL || key < hashTable[index]->tu_tieng_anh) {
        newNode->next = hashTable[index];
        hashTable[index] = newNode;
    } 
    // TH2: Chèn vào giữa hoặc cuối
    else {
        Node* current = hashTable[index];
        
        // Kiểm tra trùng lặp với từ đầu tiên
        if (current->tu_tieng_anh == key) {
            cout << "(!) LOI: Tu '" << key << "' da ton tai!" << endl;
            delete newNode; return;
        }

        // Duyệt tìm vị trí thích hợp:
        // Chạy đến khi gặp từ lớn hơn từ mới, hoặc hết danh sách.
        // Ví dụ: Đang có [Ant] -> [Apple]. Thêm "Ace".
        // "Ace" > "Ant" -> đi tiếp. "Ace" < "Apple" -> dừng lại chèn vào giữa.
        while (current->next != NULL && current->next->tu_tieng_anh < key) {
            current = current->next;
        }

        // Kiểm tra trùng lặp (với phần tử tiếp theo nếu có)
        if (current->next != NULL && current->next->tu_tieng_anh == key) {
            cout << "(!) LOI: Tu '" << key << "' da ton tai!" << endl;
            delete newNode; return;
        }
        
        // Chèn Node mới vào sau Node current
        newNode->next = current->next;
        current->next = newNode;
    }
    cout << "(v) Da them thanh cong: " << key << endl;
}

// --- CHỨC NĂNG 2: TRA CỨU TỪ ---
void traCuuTu() {
    string key;
    cout << "\n>> Nhap tu can tra cuu: ";
    getline(cin, key);

    int index = hashFunction(key);
    if (index == -1) {
        cout << "(!) Tu khong hop le." << endl;
        return;
    }

    Node* p = hashTable[index];
    bool found = false;
    
    // Duyệt trong danh sách của nhóm tương ứng
    while (p != NULL) {
        if (p->tu_tieng_anh == key) {
            cout << "---------------------------------" << endl;
            cout << "Tieng Anh : " << p->tu_tieng_anh << endl;
            cout << "Tieng Viet: " << p->nghia_tieng_viet << endl;
            cout << "---------------------------------" << endl;
            found = true;
            break;
        }
        
        // TỐI ƯU HÓA TÌM KIẾM (OPTIONAL)
        // Vì danh sách đã sắp xếp, nếu gặp từ lớn hơn từ cần tìm -> Dừng luôn.
        // Ví dụ: Tìm "Ant". Gặp "Apple" (> Ant) -> Biết ngay là không có Ant phía sau.
        if (p->tu_tieng_anh > key) {
            break;
        }

        p = p->next;
    }

    if (!found) {
        cout << "(!) Khong tim thay tu '" << key << "' trong tu dien." << endl;
    }
}

// --- CHỨC NĂNG 3: SỬA NGHĨA CỦA TỪ ---
void suaTu() {
    string key;
    cout << "\n>> Nhap tu Tieng Anh can sua: ";
    getline(cin, key);

    int index = hashFunction(key);
    if (index == -1) return;

    Node* p = hashTable[index];
    while (p != NULL) {
        if (p->tu_tieng_anh == key) {
            cout << ">> Tu hien tai: " << p->tu_tieng_anh << " = " << p->nghia_tieng_viet << endl;
            cout << ">> Nhap nghia moi: ";
            string newMeaning;
            getline(cin, newMeaning);
            p->nghia_tieng_viet = newMeaning;
            cout << "(v) Da cap nhat nghia thanh cong." << endl;
            return;
        }
        // Tối ưu tương tự tra cứu
        if (p->tu_tieng_anh > key) break;
        
        p = p->next;
    }
    cout << "(!) Khong tim thay tu de sua." << endl;
}

// --- CHỨC NĂNG 4: XÓA TỪ ---
void xoaTu() {
    string key;
    cout << "\n>> Nhap tu can xoa: ";
    getline(cin, key);

    int index = hashFunction(key);
    if (index == -1) return;

    Node* current = hashTable[index];
    Node* prev = NULL;

    // Duyệt danh sách để tìm node cần xóa
    while (current != NULL && current->tu_tieng_anh != key) {
        // Tối ưu: Nếu gặp từ lớn hơn key -> Dừng luôn vì không thể tìm thấy phía sau
        if (current->tu_tieng_anh > key) {
            current = NULL; // Đánh dấu là không tìm thấy
            break;
        }
        prev = current;
        current = current->next;
    }

    // Nếu không tìm thấy
    if (current == NULL) {
        cout << "(!) Khong tim thay tu '" << key << "' de xoa." << endl;
        return;
    }

    // Nếu tìm thấy, thực hiện xóa
    if (prev == NULL) {
        // Trường hợp xóa phần tử đầu danh sách (Head)
        hashTable[index] = current->next;
    } else {
        // Trường hợp xóa ở giữa hoặc cuối
        prev->next = current->next;
    }
    
    delete current; // Giải phóng bộ nhớ
    cout << "(v) Da xoa thanh cong tu '" << key << "'." << endl;
}

// --- CHỨC NĂNG 5: HIỂN THỊ DANH SÁCH ---
void hienThi(int mode) {
    // Mode 1: Hiển thị tất cả
    // Mode 2: Hiển thị theo nhóm chữ cái
    
    int start = 0, end = 25;
    
    if (mode == 2) {
        char c;
        cout << ">> Nhap chu cai nhom can xem (A-Z): ";
        cin >> c;
        cin.ignore(); // Xóa bộ nhớ đệm sau khi nhập char
        
        int idx = hashFunction(string(1, c));
        if (idx == -1) {
            cout << "(!) Ky tu khong hop le." << endl; 
            return;
        }
        start = idx;
        end = idx;
    }

    cout << "\n========== NOI DUNG TU DIEN ==========" << endl;
    bool isEmptyDict = true;

    for (int i = start; i <= end; i++) {
        if (hashTable[i] != NULL) {
            isEmptyDict = false;
            cout << "\n[ Nhom " << (char)('A' + i) << " ]" << endl;
            Node* p = hashTable[i];
            while (p != NULL) {
                cout << "  + " << left << setw(15) << p->tu_tieng_anh 
                     << ": " << p->nghia_tieng_viet << endl;
                p = p->next;
            }
        }
    }
    
    if (isEmptyDict) {
        cout << "(Tu dien dang trong hoac nhom nay khong co tu)" << endl;
    }
    cout << "======================================" << endl;
}

// ====================================================
// 4. CHƯƠNG TRÌNH CHÍNH (MAIN MENU)
// ====================================================

int main() {
    khoiTao();
    
    // Thêm dữ liệu mẫu (Cố tình thêm lộn xộn để test tính năng sắp xếp)
    // Nhóm A
    Node* n_algo = new Node{"Algorithm", "Giai thuat", NULL}; 
    Node* n_apple = new Node{"Apple", "Qua tao", NULL};
    // Sắp xếp tay: Algorithm -> Apple
    hashTable[0] = n_algo;
    n_algo->next = n_apple;

    // Nhóm C
    Node* n_comp = new Node{"Computer", "May tinh", NULL};
    hashTable['c'-'a'] = n_comp;
    
    // Nhóm B (Thêm lẻ loi)
    Node* n_bug = new Node{"Bug", "Loi chuong trinh", NULL}; 
    hashTable['b'-'a'] = n_bug;


    int choice;
    while (true) {
        cout << "\n----------- MENU TU DIEN -----------" << endl;
        cout << "1. Them tu moi (Add)" << endl;
        cout << "2. Tra cuu tu (Search)" << endl;
        cout << "3. Xoa tu (Delete)" << endl;
        cout << "4. Sua nghia cua tu (Edit)" << endl;
        cout << "5. Hien thi toan bo (Show All)" << endl;
        cout << "6. Hien thi theo nhom (Show Group)" << endl;
        cout << "0. Thoat (Exit)" << endl;
        cout << "------------------------------------" << endl;
        cout << "Lua chon cua ban: ";
        cin >> choice;
        cin.ignore(); // CỰC KỲ QUAN TRỌNG: Xóa ký tự Enter thừa

        switch (choice) {
            case 1: themTu(); break;
            case 2: traCuuTu(); break;
            case 3: xoaTu(); break;
            case 4: suaTu(); break;
            case 5: hienThi(1); break;
            case 6: hienThi(2); break;
            case 0: 
                cout << "Tam biet!";
                return 0;
            default: 
                cout << "Lua chon khong hop le!" << endl;
        }
    }
    return 0;
}