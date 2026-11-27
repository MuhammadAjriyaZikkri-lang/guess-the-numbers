#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>

using namespace std;

struct Pemain {
    string username;
    string password;
    int totalScore;
};

vector<Pemain> daftarPemain;
bool warnaAktif = false;

void initWarna() {
    if (has_colors()) {
        start_color();
        use_default_colors();
        init_pair(1, COLOR_CYAN, -1);
        init_pair(2, COLOR_GREEN, -1);
        init_pair(3, COLOR_RED, -1);
        init_pair(4, COLOR_YELLOW, -1);
        init_pair(5, COLOR_MAGENTA, -1);
        warnaAktif = true;
    }
}

void pasangWarna(int pair, int ekstraAttr = A_NORMAL) {
    if (warnaAktif) {
        attron(COLOR_PAIR(pair) | ekstraAttr);
    } else if (ekstraAttr != A_NORMAL) {
        attron(ekstraAttr);
    }
}

void lepasWarna(int pair, int ekstraAttr = A_NORMAL) {
    if (warnaAktif) {
        attroff(COLOR_PAIR(pair) | ekstraAttr);
    } else if (ekstraAttr != A_NORMAL) {
        attroff(ekstraAttr);
    }
}

void loadUsers() {
    daftarPemain.clear();
    ifstream file("users.txt");
    if (!file.is_open()) {
        return;
    }

    Pemain p;
    while (file >> p.username >> p.password >> p.totalScore) {
        daftarPemain.push_back(p);
    }
}

void saveUsers() {
    ofstream file("users.txt");
    if (!file.is_open()) {
        return;
    }

    for (const auto& pemain : daftarPemain) {
        file << pemain.username << " " << pemain.password << " " << pemain.totalScore << "\n";
    }
}

int cariPemain(const string& username) {
    for (size_t i = 0; i < daftarPemain.size(); i++) {
        if (daftarPemain[i].username == username) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void simpanRiwayat(const string& username, int point, const string& hasil) {
    ofstream file("riwayat.txt", ios::app);
    if (!file.is_open()) {
        return;
    }
    file << username << " " << point << " " << hasil << "\n";
}

bool adaSpasi(const string& teks) {
    return teks.find_first_of(" \t\r\n") != string::npos;
}

string inputText(int baris, int kolom, const string& label) {
    char buffer[100] = "";

    move(baris, kolom);
    clrtoeol();
    mvprintw(baris, kolom, "%s: ", label.c_str());
    refresh();

    echo();
    curs_set(1);
    getnstr(buffer, 99);
    noecho();
    curs_set(0);

    return string(buffer);
}

void tunggu() {
    pasangWarna(1);
    mvprintw(LINES - 2, 2, "Tekan tombol apa saja untuk melanjutkan...");
    lepasWarna(1);
    refresh();
    getch();
}

void registerPemain() {
    clear();
    box(stdscr, 0, 0);

    pasangWarna(1, A_BOLD);
    mvprintw(2, 4, "=== REGISTRASI AKUN BARU ===");
    lepasWarna(1, A_BOLD);

    string username = inputText(4, 4, "Masukkan username (tanpa spasi)");
    string password = inputText(5, 4, "Masukkan password (tanpa spasi)");

    if (username.empty() || password.empty()) {
        pasangWarna(3);
        mvprintw(7, 4, "Username dan password tidak boleh kosong!");
        lepasWarna(3);
        tunggu();
        return;
    }

    if (adaSpasi(username) || adaSpasi(password)) {
        pasangWarna(3);
        mvprintw(7, 4, "Username dan password tidak boleh mengandung spasi!");
        lepasWarna(3);
        tunggu();
        return;
    }

    if (cariPemain(username) != -1) {
        pasangWarna(3);
        mvprintw(7, 4, "Username sudah terdaftar!");
        lepasWarna(3);
        tunggu();
        return;
    }

    daftarPemain.push_back({username, password, 0});
    saveUsers();

    pasangWarna(2, A_BOLD);
    mvprintw(7, 4, "Pendaftaran berhasil! Silakan login.");
    lepasWarna(2, A_BOLD);
    tunggu();
}

int login() {
    while (true) {
        clear();
        box(stdscr, 0, 0);

        pasangWarna(1, A_BOLD);
        mvprintw(2, 4, "=== GUESS THE NUMBERS - LOGIN ===");
        lepasWarna(1, A_BOLD);

        mvprintw(4, 4, "1. Login");
        mvprintw(5, 4, "2. Daftar Akun Baru");
        mvprintw(6, 4, "3. Keluar");
        mvprintw(8, 4, "Pilih [1-3]: ");
        refresh();

        char menu = getch();
        if (menu == '2') {
            registerPemain();
            continue;
        }
        if (menu == '3') {
            return -1;
        }
        if (menu != '1') {
            continue;
        }

        clear();
        box(stdscr, 0, 0);

        pasangWarna(1, A_BOLD);
        mvprintw(2, 4, "=== LOGIN PENGGUNA ===");
        lepasWarna(1, A_BOLD);

        string username = inputText(4, 4, "Masukkan username");
        string password = inputText(5, 4, "Masukkan password");

        int index = cariPemain(username);
        if (index != -1 && daftarPemain[index].password == password) {
            pasangWarna(2, A_BOLD);
            mvprintw(7, 4, "Login berhasil! Selamat datang, %s.", daftarPemain[index].username.c_str());
            lepasWarna(2, A_BOLD);
            tunggu();
            return index;
        }

        pasangWarna(3);
        mvprintw(7, 4, "Username atau password salah!");
        lepasWarna(3);
        tunggu();
    }
}

int hitungPoint(int nyawa, bool menang) {
    if (!menang) {
        return 0;
    }
    switch (nyawa) {
        case 5: return 1000;
        case 4: return 900;
        case 3: return 800;
        case 2: return 700;
        case 1: return 600;
        default: return 0;
    }
}

void tampilkanLeaderboard() {
    clear();
    box(stdscr, 0, 0);

    vector<Pemain> ranking = daftarPemain;
    sort(ranking.begin(), ranking.end(), [](const Pemain& a, const Pemain& b) {
        if (a.totalScore != b.totalScore) {
            return a.totalScore > b.totalScore;
        }
        return a.username < b.username;
    });

    pasangWarna(1, A_BOLD);
    mvprintw(2, 5, "========== LEADERBOARD ==========");
    mvprintw(4, 5, "RANK");
    mvprintw(4, 15, "USERNAME");
    mvprintw(4, 35, "TOTAL SCORE");
    lepasWarna(1, A_BOLD);

    if (ranking.empty()) {
        pasangWarna(4);
        mvprintw(6, 5, "Belum ada pemain yang terdaftar.");
        lepasWarna(4);
        tunggu();
        return;
    }

    int baris = 6;
    for (size_t i = 0; i < ranking.size(); i++) {
        int pair = (i == 0) ? 5 : (i == 1 ? 2 : 1);
        pasangWarna(pair);
        mvprintw(baris, 5, "%zu", i + 1);
        mvprintw(baris, 15, "%s", ranking[i].username.c_str());
        mvprintw(baris, 35, "%d", ranking[i].totalScore);
        lepasWarna(pair);

        baris++;
        if (baris >= LINES - 3) {
            tunggu();
            clear();
            box(stdscr, 0, 0);
            baris = 3;
        }
    }

    tunggu();
}

void tampilkanRiwayat(const string& username) {
    clear();
    box(stdscr, 0, 0);

    pasangWarna(1, A_BOLD);
    mvprintw(2, 5, "========== RIWAYAT PERMAINAN (%s) ==========", username.c_str());
    mvprintw(4, 5, "POINT");
    mvprintw(4, 18, "HASIL");
    lepasWarna(1, A_BOLD);

    ifstream file("riwayat.txt");
    string nama, hasil;
    int point;
    int baris = 6;
    bool ditemukan = false;

    while (file.is_open() && (file >> nama >> point >> hasil)) {
        if (nama == username) {
            int pair = (hasil == "MENANG") ? 2 : 3;
            pasangWarna(pair);
            mvprintw(baris, 5, "+%d", point);
            mvprintw(baris, 18, "%s", hasil.c_str());
            lepasWarna(pair);

            baris++;
            ditemukan = true;
            if (baris >= LINES - 3) {
                tunggu();
                clear();
                box(stdscr, 0, 0);
                baris = 3;
            }
        }
    }

    if (!ditemukan) {
        pasangWarna(4);
        mvprintw(6, 5, "Belum ada riwayat permainan untuk akun ini.");
        lepasWarna(4);
    }

    tunggu();
}

void mainkanRonde(int userIndex) {
    clear();
    box(stdscr, 0, 0);

    int angkaRahasia = (rand() % 100) + 1;
    int nyawa = 5;
    bool menang = false;

    pasangWarna(1, A_BOLD);
    mvprintw(2, 4, "Tebak angka 1 sampai 100! (Pemain: %s)", daftarPemain[userIndex].username.c_str());
    lepasWarna(1, A_BOLD);

    while (nyawa > 0) {
        int pairNyawa = (nyawa == 1) ? 3 : (nyawa <= 2 ? 4 : 2);
        pasangWarna(pairNyawa, A_BOLD);
        mvprintw(4, 4, "Nyawa tersisa: ");
        for (int i = 0; i < nyawa; i++) {
            printw("<3 ");
        }
        lepasWarna(pairNyawa, A_BOLD);

        string rawInput = inputText(6, 4, "Masukkan tebakan (1-100)");
        int tebakan = 0;

        try {
            size_t idx = 0;
            tebakan = stoi(rawInput, &idx);
            if (idx != rawInput.size()) {
                throw invalid_argument("Bukan angka penuh");
            }
        } catch (...) {
            move(8, 4);
            clrtoeol();
            move(9, 4);
            clrtoeol();
            pasangWarna(3);
            mvprintw(8, 4, "Input tidak valid! Masukkan angka bulat 1 sampai 100.");
            lepasWarna(3);
            refresh();
            continue;
        }

        if (tebakan < 1 || tebakan > 100) {
            move(8, 4);
            clrtoeol();
            move(9, 4);
            clrtoeol();
            pasangWarna(4);
            mvprintw(8, 4, "Angka harus berada di rentang 1 sampai 100!");
            lepasWarna(4);
            refresh();
            continue;
        }

        if (tebakan == angkaRahasia) {
            menang = true;
            break;
        }

        nyawa--;
        int selisih = abs(tebakan - angkaRahasia);

        move(8, 4);
        clrtoeol();
        move(9, 4);
        clrtoeol();

        if (tebakan > angkaRahasia) {
            pasangWarna(4);
            mvprintw(8, 4, "Tebakan terlalu besar!");
            lepasWarna(4);
        } else {
            pasangWarna(4);
            mvprintw(8, 4, "Tebakan terlalu kecil!");
            lepasWarna(4);
        }

        if (selisih <= 10) {
            pasangWarna(2);
            mvprintw(9, 4, "Info: Sudah cukup dekat!");
            lepasWarna(2);
        } else {
            pasangWarna(3);
            mvprintw(9, 4, "Info: Masih cukup jauh!");
            lepasWarna(3);
        }

        refresh();
    }

    int point = hitungPoint(nyawa, menang);
    daftarPemain[userIndex].totalScore += point;
    saveUsers();

    string statusHasil = menang ? "MENANG" : "KALAH";
    simpanRiwayat(daftarPemain[userIndex].username, point, statusHasil);

    if (menang) {
        pasangWarna(2, A_BOLD);
        mvprintw(11, 4, "MENANG!! Angkanya: %d", angkaRahasia);
        mvprintw(12, 4, "Point didapat: +%d", point);
        lepasWarna(2, A_BOLD);
    } else {
        pasangWarna(3, A_BOLD);
        mvprintw(11, 4, "COBA LAGI! Angkanya: %d", angkaRahasia);
        mvprintw(12, 4, "Point didapat: +%d", point);
        lepasWarna(3, A_BOLD);
    }

    pasangWarna(1, A_BOLD);
    mvprintw(13, 4, "Total score kamu: %d", daftarPemain[userIndex].totalScore);
    lepasWarna(1, A_BOLD);

    mvprintw(15, 4, "Tekan tombol apa saja untuk kembali ke menu...");
    refresh();
    getch();
}

int main() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    initWarna();

    srand(static_cast<unsigned int>(time(nullptr)));
    loadUsers();

    int currentUser = login();
    if (currentUser == -1) {
        endwin();
        return 0;
    }

    char pilihan;
    do {
        clear();
        box(stdscr, 0, 0);

        pasangWarna(1, A_BOLD);
        mvprintw(2, 4, "=== GUESS THE NUMBERS ===");
        lepasWarna(1, A_BOLD);

        mvprintw(3, 4, "Pemain: %s | Total Score: %d",
                 daftarPemain[currentUser].username.c_str(),
                 daftarPemain[currentUser].totalScore);

        mvprintw(5, 4, "1. Mulai Main");
        mvprintw(6, 4, "2. Lihat Leaderboard");
        mvprintw(7, 4, "3. Lihat Riwayat Permainan");
        mvprintw(8, 4, "4. Ganti Akun");
        mvprintw(9, 4, "5. Keluar");
        mvprintw(11, 4, "Pilih [1-5]: ");
        refresh();

        pilihan = getch();

        switch (pilihan) {
            case '1':
                mainkanRonde(currentUser);
                break;
            case '2':
                tampilkanLeaderboard();
                break;
            case '3':
                tampilkanRiwayat(daftarPemain[currentUser].username);
                break;
            case '4':
                currentUser = login();
                if (currentUser == -1) {
                    pilihan = '5';
                }
                break;
            default:
                break;
        }
    } while (pilihan != '5');

    endwin();
    return 0;
}