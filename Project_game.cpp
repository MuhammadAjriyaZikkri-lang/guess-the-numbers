#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <cmath>

using namespace std;

int main() {
    // Inisialisasi ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();

    // Inisialisasi Palet Warna
    init_pair(1, COLOR_CYAN, COLOR_BLACK);    // Judul & Border Utama
    init_pair(2, COLOR_GREEN, COLOR_BLACK);   // Menang / Sangat Dekat
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);  // Lumayan Dekat
    init_pair(4, COLOR_RED, COLOR_BLACK);     // Kalah / Jauh / Nyawa
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK); // Info / Animasi

    srand(clock());
    int pilihan;

    do {
        clear();
        box(stdscr, 0, 0);

        attron(A_BOLD | COLOR_PAIR(1));
        mvprintw(2, 4, "=== GAME TEBAK ANGKA (1 - 100) ===");
        attroff(A_BOLD | COLOR_PAIR(1));

        mvprintw(4, 4, "[1] Mulai Main");
        mvprintw(5, 4, "[2] Keluar");
        mvprintw(7, 4, "Pilihanmu [1-2]: ");
        refresh();

        pilihan = getch();

        if (pilihan == '1') {
            clear();
            box(stdscr, 0, 0);

            int angkaRahasia = (rand() % 100) + 1;
            int tebakan = 0;
            int nyawa = 5;
            bool menang = false;
            char pesanHint[60] = "Belum ada tebakan. Silakan mulai!";

            attron(A_BOLD | COLOR_PAIR(5));
            mvprintw(2, 4, "Tebak angka rahasia antara 1 sampai 100!");
            attroff(A_BOLD | COLOR_PAIR(5));

            while (nyawa > 0) {
                // Tampilkan sisa nyawa
                attron(COLOR_PAIR(4));
                mvprintw(4, 4, "Nyawa Tersisa: ");
                for (int i = 0; i < nyawa; i++) addstr("<3 ");
                addstr("          ");
                attroff(COLOR_PAIR(4));

            
                mvprintw(6, 4, "                                                              ");
                mvprintw(6, 4, "Petunjuk: ");
            
                if (string(pesanHint).find("SANGAT DEKAT") != string::npos) {
                    attron(A_BOLD | COLOR_PAIR(2));
                } else if (string(pesanHint).find("Lumayan") != string::npos) {
                    attron(COLOR_PAIR(3));
                } else {
                    attron(COLOR_PAIR(4));
                }
                printw("%s", pesanHint);
                attroff(COLOR_PAIR(2) | COLOR_PAIR(3) | COLOR_PAIR(4));
            
                mvprintw(8, 4, "                                                              ");
                mvprintw(8, 4, "Masukkan tebakan :       ");
                move(8, 22);

                echo();
                scanw("%d", &tebakan);
                noecho();

            
                attron(COLOR_PAIR(5));
                mvprintw(10, 4, "Memproses tebakan .   ");
                refresh();
                napms(100);
                mvprintw(10, 4, "Memproses tebakan ..  ");
                refresh();
                napms(100);
                mvprintw(10, 4, "Memproses tebakan ... ");
                refresh();
                napms(100);
            
                mvprintw(10, 4, "                                                        ");
                attroff(COLOR_PAIR(5));

                if (tebakan == angkaRahasia) {
                    menang = true;
                    break;
                } else {
                    nyawa--;

                
                    int selisih = abs(tebakan - angkaRahasia);

                    if (selisih <= 5) {
                        sprintf(pesanHint, "SANGAT DEKAT! Dikit lagi tembus!");
                    } else if (selisih <= 15) {
                        if (tebakan > angkaRahasia) {
                            sprintf(pesanHint, "Lumayan dekat, tapi terlalu BESAR!");
                        } else {
                            sprintf(pesanHint, "Lumayan dekat, tapi terlalu KECIL!");
                        }
                    } else {
                        if (tebakan > angkaRahasia) {
                            sprintf(pesanHint, "Terlalu BESAR & masih JAUH!");
                        } else {
                            sprintf(pesanHint, "Terlalu KECIL & masih JAUH!");
                        }
                    }
                }
                refresh();
            }

        
            mvprintw(12, 4, "---------------------------------------------");
            if (menang) {
                attron(A_BOLD | A_BLINK | COLOR_PAIR(2));
                mvprintw(14, 4, "SELAMAT! Kamu Menang! Angkanya: %d", angkaRahasia);
                attroff(A_BOLD | A_BLINK | COLOR_PAIR(2));
            } else {
                attron(A_BOLD | COLOR_PAIR(4));
                mvprintw(14, 4, "GAME OVER! Angka yang benar: %d", angkaRahasia);
                attroff(A_BOLD | COLOR_PAIR(4));
            }

            attron(COLOR_PAIR(1));
            mvprintw(16, 4, "Tekan tombol apa saja untuk kembali ke menu...");
            attroff(COLOR_PAIR(1));
            
            refresh();
            getch();

        } else if (pilihan == '2') {
            break;
        }

    } while (pilihan != '2');

    endwin();
    return 0;
}