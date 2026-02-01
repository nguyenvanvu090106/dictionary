#include <iostream>
#include <string>
#include <cctype>
#include <iomanip>

using namespace std;

// 1. CAU TRUC DU LIEU

struct Node {
    string tu_tieng_anh; 		//key 
    string nghia_tieng_viet;	//value 
    Node* next;		// noi cac node lai
};

const int TABLE_SIZE = 26;		//khai bao kich thuoc ban bam 
Node* hashTable[TABLE_SIZE];

// Khoi tao bang bam
void khoiTao() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
}

// Chuan hoa chu thuong 
string toLowerCase(string s) {
    for (int i = 0; i < s.length(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}

// Ham bam
int hashFunction(string key) {
    if (key.empty()) return -1; //ktra du lieu rong 

    key = toLowerCase(key);
    char c = key[0]; // lay kí tu dau tien 

    if (c >= 'a' && c <= 'z') {	//ktra ki tu dau co phai la chu hay ko 
        return c - 'a';
    }
    return -1;
}

// 3. CAC CHUC NANG CHINH

// THEM TU
void themTu() {
    string key, meaning;
    cout << "\n>> Nhap tu Tieng Anh: ";
    getline(cin, key);
    key = toLowerCase(key);

    int index = hashFunction(key); //xac dinh cac vi tri bang bam 
    if (index == -1) {
        cout << "(!) Tu khong hop le.\n";
        return;
    }

    cout << ">> Nhap nghia Tieng Viet: ";
    getline(cin, meaning);

    Node* newNode = new Node; // tao node moi 
    newNode->tu_tieng_anh = key;
    newNode->nghia_tieng_viet = meaning;
    newNode->next = NULL;

    // Chen co sap xep
    if (hashTable[index] == NULL || key < hashTable[index]->tu_tieng_anh) {		//chen dau dsach 
        newNode->next = hashTable[index];
        hashTable[index] = newNode;
    } else {
        Node* cur = hashTable[index];		//chen giua hoac cuoi 

        if (cur->tu_tieng_anh == key) {		//ktra ptu trung 
            cout << "(!) Tu da ton tai.\n";
            delete newNode;
            return;
        }

        while (cur->next != NULL && cur->next->tu_tieng_anh < key) {	//tim vi tri chen 
            cur = cur->next;
        }

        if (cur->next != NULL && cur->next->tu_tieng_anh == key) {
            cout << "(!) Tu da ton tai.\n";
            delete newNode;
            return;
        }

        newNode->next = cur->next;
        cur->next = newNode;
    }

    cout << "(v) Da them tu thanh cong.\n";
}

// TRA CUU TU
void traCuuTu() {
    string key;
    cout << "\n>> Nhap tu can tra cuu: ";
    getline(cin, key);
    key = toLowerCase(key);

    int index = hashFunction(key);		//xdinh vi tri trong hash 
    if (index == -1) {
        cout << "(!) Tu khong hop le.\n";
        return;
    }

    Node* p = hashTable[index];		//duyet dslk 
    while (p != NULL) {
        if (p->tu_tieng_anh == key) {
            cout << "-----------------------------\n";
            cout << "Tieng Anh : " << p->tu_tieng_anh << endl;
            cout << "Tieng Viet: " << p->nghia_tieng_viet << endl;
            cout << "-----------------------------\n";
            return;
        }
        if (p->tu_tieng_anh > key) break;
        p = p->next;
    }

    cout << "(!) Khong tim thay tu.\n";
}

// SUA TU
void suaTu() {
    string key;
    cout << "\n>> Nhap tu can sua: ";
    getline(cin, key);
    key = toLowerCase(key);

    int index = hashFunction(key);
    if (index == -1) return;

    Node* p = hashTable[index];
    while (p != NULL) {
        if (p->tu_tieng_anh == key) {
            cout << ">> Nghia cu: " << p->nghia_tieng_viet << endl;
            cout << ">> Nhap nghia moi: ";
            getline(cin, p->nghia_tieng_viet);
            cout << "(v) Da cap nhat.\n";
            return;
        }
        if (p->tu_tieng_anh > key) break;
        p = p->next;
    }
    cout << "(!) Khong tim thay tu.\n";
}

// XOA TU
void xoaTu() {
    string key;
    cout << "\n>> Nhap tu can xoa: ";
    getline(cin, key);
    key = toLowerCase(key);

    int index = hashFunction(key);
    if (index == -1) return;

    Node* cur = hashTable[index];		//node hien thanh 
    Node* prev = NULL;					//con tro trc node cur 

    while (cur != NULL && cur->tu_tieng_anh != key) {
        if (cur->tu_tieng_anh > key) {
            cur = NULL;
            break;
        }
        prev = cur;
        cur = cur->next;
    }

    if (cur == NULL) {
        cout << "(!) Khong tim thay tu.\n";
        return;
    }

    if (prev == NULL)
        hashTable[index] = cur->next;
    else
        prev->next = cur->next;
        
    delete cur;
    cout << "(v) Da xoa tu thanh cong.\n";
}

// HIEN THI
void hienThi(int mode) {
    int start = 0, end = 25;

    if (mode == 2) {
        char c;
        cout << ">> Nhap chu cai (A-Z): ";
        cin >> c;
        cin.ignore();
        c = tolower(c);

        int idx = hashFunction(string(1, c));	// tim vi tri trong hash table 
        if (idx == -1) {
            cout << "(!) Khong hop le.\n";
            return;
        }
        start = end = idx;
    }

    cout << "\n========== TU DIEN ==========\n";
    bool empty = true;

    for (int i = start; i <= end; i++) {	// duyet hash table 
        if (hashTable[i] != NULL) {
            empty = false;
            cout << "\n[ " << char('A' + i) << " ]\n";
            Node* p = hashTable[i];
            while (p != NULL) {
                cout << "  + " << left << setw(15)
                     << p->tu_tieng_anh
                     << ": " << p->nghia_tieng_viet << endl;
                p = p->next;
            }
        }
    }

    if (empty)
        cout << "(Tu dien dang trong)\n";

    cout << "==============================\n";
}

// 4. MAIN

int main() {
    khoiTao();

    // Du lieu mau
	// A
	Node* a1 = new Node{"algorithm", "Giai thuat", NULL};
	Node* a2 = new Node{"apple", "Qua tao", NULL};
	Node* a3 = new Node{"array", "Mang", NULL};
	a1->next = a2;
	a2->next = a3;
	hashTable['a' - 'a'] = a1;
	
	// B
	Node* b1 = new Node{"bug", "Loi chuong trinh", NULL};
	Node* b2 = new Node{"binary", "Nhi phan", NULL};
	b1->next = b2;
	hashTable['b' - 'a'] = b1;
	
	// C
	Node* c1 = new Node{"computer", "May tinh", NULL};
	Node* c2 = new Node{"code", "Ma nguon", NULL};
	Node* c3 = new Node{"compiler", "Trinh bien dich", NULL};
	c1->next = c2;
	c2->next = c3;
	hashTable['c' - 'a'] = c1;
	
	// D
	Node* d1 = new Node{"data", "Du lieu", NULL};
	Node* d2 = new Node{"database", "Co so du lieu", NULL};
	d1->next = d2;
	hashTable['d' - 'a'] = d1;
	
	// E
	hashTable['e' - 'a'] = new Node{"error", "Loi", NULL};
	
	// F
	Node* f1 = new Node{"function", "Ham", NULL};
	Node* f2 = new Node{"file", "Tap tin", NULL};
	f1->next = f2;
	hashTable['f' - 'a'] = f1;
	
	// G
	hashTable['g' - 'a'] = new Node{"graph", "Do thi", NULL};
	
	// H
	hashTable['h' - 'a'] = new Node{"hash", "Bam", NULL};
	
	// I
	hashTable['i' - 'a'] = new Node{"integer", "So nguyen", NULL};
	
	// L
	hashTable['l' - 'a'] = new Node{"loop", "Vong lap", NULL};
	
	// M
	hashTable['m' - 'a'] = new Node{"memory", "Bo nho", NULL};


    int choice;
    while (true) {
        cout << "\n----- MENU TU DIEN -----\n";
        cout << "1. Them tu\n";
        cout << "2. Tra cuu\n";
        cout << "3. Xoa tu\n";
        cout << "4. Sua tu\n";
        cout << "5. Hien thi tat ca\n";
        cout << "6. Hien thi theo nhom\n";
        cout << "0. Thoat\n";
        cout << "Lua chon: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: themTu(); break;
            case 2: traCuuTu(); break;
            case 3: xoaTu(); break;
            case 4: suaTu(); break;
            case 5: hienThi(1); break;
            case 6: hienThi(2); break;
            case 0: return 0;
            default: cout << "Lua chon sai!\n";
        }
    }
}
