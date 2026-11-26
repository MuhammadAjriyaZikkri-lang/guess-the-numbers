#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

struct Pemain {
    string username;
    string pasword;
    int totalScore;
};

vector<Pemain> daftarPemain;

void loadUsers() {
    daftarPemain.clear();

    ifstream file("users.txt");
    Pemain p;

    while (file >> p.username >> p.pasword >> p.totalScore) {
        daftarPemain.push_back(p);
    }
    file.close();
}

void saveUsers() {
    ofstream file("users.txt");

    for (const auto& pemain : daftarPemain) {
        file << pemain.username << " " << pemain.pasword << " " << pemain.totalScore << endl;
    }
    file.close();
}
int cariPemain(const string& username) {
    for (int i = 0; i < daftarPemain.size(); i++) {
        if (daftarPemain[i].username == username) {
            return i;
        }
    }
    return -1;
}

void simpanRiwayat(const string username, int point, string hasil) {
    ofstream file ("riwayat.txt", ios::app);
    file << username << " " << " " << point << " " << hasil << endl;
    file.close();
}

string inputText(int baris, int kolom, string label) {
    char buffer[100] = "";

    mvprintw(baris, kolom, "%s: ", label.c_str());
    echo();
    getnstr(buffer, 99);
    noecho();
    
    return string(buffer);
}

void tunggu() {
    mvprintw(LINES -2, 2, "Tekan tombol apa saja untuk melanjutkan...");
    refresh();
    getch();
}

void registerPemain() {
    clear();

    string username = inputText(2, 4, "masukan username: ");
    string password = inputText(3, 4, "masukan password: ");

    if (cariPemain(username) != -1) {
        mvprintw (5, 4, "Username sudah terdaftar!");
        tunggu();
    }

<<<<<<< HEAD
    daftarPemain.push_back({username, password, 0});
    saveUsers();

    mvprintw(5, 4, "pendaftaran berhasil!");
    tunggu();
}
=======
    mvwprintw(loginWin, 1, 17, "LOGIN PENGGUNA");
    mvwprintw(loginWin, 3, 5, "masukan username:");
    
    char username[30] = "";
    wrefresh(loginWin);

    echo();
    curs_set(1);

    mvwgetnstr(loginWin, 5, 5, username, 29);

    noecho();
    curs_set(0);

    namaPengguna = username;

    mvwprintw(loginWin, 7, 5, "Login berhasil!!");
    mvwprintw(loginWin, 8, 5, "welcomee! %s", username);

    wrefresh(loginWin);

    getch();

    delwin(loginWin);
>>>>>>> 197a7cf (Perbaiki login dan tambah leaderboard)

    int login() {
    clear();
    mvprintw(2, 4, "LOGIN");

    string username = inputText(6, 4, "masukan username: ");
    string password = inputText(7, 5, "masukan password: ");

    int index = cariPemain(username);

    if (index != -1) {
        if (daftarPemain[index].pasword == password) {
            mvprintw(9, 4, "Login berhasil!");
            tunggu();
            return index;
        }
    }

return -1;
}

int hitungPoint(int nyawa) {
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

    vector<Pemain> ranking = daftarPemain;
    sort(ranking.begin(), ranking.end(), [](const Pemain& a, const Pemain& b) {
        return a.totalScore > b.totalScore;
    });

 mvprintw (2, 5, "========== LEADERBOARD ==========");
 mvprintw (4, 5, "RANK");
 mvprintw (4, 15, "USERNAME");
 mvprintw (4, 35, "TOTAL SCORE");

 int baris = 6;

 for(int i = 0; i < ranking.size(); i++) {
    mvprintw(baris, 5, "%d", i + 1);
    mvprintw(baris, 15, "%s", ranking[i].username.c_str());
    mvprintw(baris, 35, "%d", ranking[i].totalScore);

    baris++;

    if (baris >= LINES - 3) {
        tunggu();
        clear();
        baris = 3;
        }
    }

    if (ranking.empty()) {
        mvprintw(6, 5, "Belum ada pemain yang terdaftar.");
    }

    tunggu();
 }

 void tampilkanRiwayat(string username) {
    clear();

    mvprintw(2, 5, "========== RIWAYAT PERMAINAN ==========");

    ifstream file("history.txt");

    string nama, hasil;
    int level, point;
    int baris = 5;
    bool ditemukan = false;

    mvprintw(4, 5, "LEVEL");
    mvprintw(4, 15, "POINT");
    mvprintw(4, 30, "HASIL");

    while (file >> nama >> level >> point >> hasil) {
        if (nama == username) {
            mvprintw(baris, 5, "%d", level);
            mvprintw(baris, 15, "%d", point);
            mvprintw(baris, 30, "%s", hasil.c_str());

            baris++;
            ditemukan = true;
            if (baris >= LINES -3) {
                tunggu(); 
                clear();
                baris = 4;
            }
        }
    }

    file.close();

    if(!ditemukan) {
        mvprintw(6, 5, "Belum ada riwayat permainan.");
    }

    tunggu();
 }

 int main() {
    initscr();
    cbreak();
    noecho();
    start_color();
    int score = 0;
    int point = 0;

    login();

    loadUsers();
    tampilkanLeaderboard();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);

    srand(clock());
    char pilihan;

    do {
        clear();
        box(stdscr, 0, 0);

        attron(COLOR_PAIR(1));
        mvprintw(2, 4, "GUESS THE NUMBERS");
        mvprintw(4, 4, "1. Mulai Main");
        mvprintw(5, 4, "2. Keluar");
        mvprintw(7, 4, "Pilih [1-2]: ");
        attroff(COLOR_PAIR(1));
        refresh();
    
        pilihan = getch();

        if (pilihan == '1') {
            clear();
            box(stdscr, 0, 0);

            int angkaRahasia = (rand() % 100) + 1;
            int tebakan = 0;
            int nyawa = 5;
            bool menang = false;
            
            mvprintw(2, 4, "Tebak angka 1 sampai 100!");

            while (nyawa > 0) {
                mvprintw(4, 4, "Nyawa tersisa: %d   ", nyawa);
                mvprintw(6, 4, "Masukkan tebakan: ");
                refresh();

                echo();
                scanw("%d", &tebakan);
                noecho();

                if (tebakan == angkaRahasia) {
                    menang = true;
                    break;
                } else {
                    nyawa--;
                    int selisih = abs(tebakan - angkaRahasia);

                    if (tebakan > angkaRahasia) {
                        mvprintw(8, 4, "Tebakan terlalu besar!.     ");
                    } else {
                        mvprintw(8, 4, "Tebakan terlalu kecil!.     ");
                    }

                    if (selisih <= 10) {
                        mvprintw(9, 4, "Info: Sudah cukup dekat!.   ");
                    } else {
                        mvprintw(9, 4, "Info: Masih cukup jauh!.    ");
                    }
                }
                refresh();
            }

            if (menang) {

                point = hitungPoint(nyawa);
                score = score + point;

                attron(COLOR_PAIR(2) | A_BOLD);
                mvprintw(11, 4, "MENANG!! Angkanya: %d", angkaRahasia);
                mvprintw(12, 4, "Point: +%d", point);
                mvprintw(13, 4, "total score: %d", score);
                attroff(COLOR_PAIR(2) | A_BOLD);

            } else {

                point = hitungPoint(nyawa);
                score = score + point;

                attron(COLOR_PAIR(3) | A_BOLD);
                mvprintw(11, 4, "COBA LAGI!, Angkanya: %d", angkaRahasia);
                mvprintw(12, 4, "Point yang di dapat: + %d", point);
                mvprintw(13, 4, "total score: %d", score);
                attroff(COLOR_PAIR(3) | A_BOLD);
            }
            
            mvprintw(13, 4, "Tekan tombol apa saja untuk kembali...");
            refresh();
            getch();
        }

    } while (pilihan != '2');

    endwin();
    return 0;
}
