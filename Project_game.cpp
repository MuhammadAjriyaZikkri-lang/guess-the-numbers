#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <fstream>

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

void simpanRiwayat(const string username, int level, int point, string hasil) {
    ofstream file ("riwayat.txt", ios::app);
    file << username << " " << level << " " << point << " " << hasil << endl;
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

void login() {
    clear();

    int height = 10;
    int width = 50;
    int startY = 5;
    int startX = 15;
    string namaPengguna;

    WINDOW *loginWin = newwin(height, width, startY, startX);
    if (loginWin == NULL) {
        return;
    }
    box(loginWin, 0, 0);

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

    clear();
    refresh();
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

int main() {
    initscr();
    cbreak();
    noecho();
    start_color();
    int score = 0;
    int point = 0;

    login();

    loadUsers();

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
