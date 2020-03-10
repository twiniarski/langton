#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "allegro5/allegro_image.h"

#include <iostream>
using namespace std;

//
// Konfiguracja gry
//

const int sz = 80;	// szerokość planszy
const int wy = 60;	// wysokość planszy

const int k_sz = 10;	// szerokość (piksele) kafelka
const int k_wy = 10;	// wysokość (piksele) kafelka

const float FPS = 20;       // Frames Per Second

const int screen_w = sz*k_sz;   // szerokość ekranu (screen width)
const int screen_h = wy*k_wy;    // wysokość ekranu (screen height)

bool key[ALLEGRO_KEY_MAX];  // wciśnięte klawisze

bool wyjdz = false;

enum typ_kafelka
{
    pusty = 0,
    zwykla_podloga,
    zwykla,
    sciana,
    gracz,
    liczba_kafelkow
};

class kafelek
{
public:

    enum typ_kafelka typ;

};

const string plik_z_kafelkiem[liczba_kafelkow] =
{
    "kafelki/pusty.png",
    "kafelki/pavement.png",
    "kafelki/sciana_niezniszczalna.png",
    "kafelki/sciana.png",
    "kafelki/wskaznik.png",
};


class gracz
{
    int cx, cy;
    int ckr;
    bool cstart;

    public:
    gracz(){
    cx=sz/2;
    cy=wy/2;
    ckr=0;
    cstart=false;
    }
            int& x=cx;
            int& y=cy;
            int& kr=ckr;
            bool& start=cstart;
};

class plansza
{
    public:
    class kafelek podloga[sz][wy];
    class kafelek sciana[sz][wy];

    class gracz g;
};

//
// Zmienne
//

class plansza p;

ALLEGRO_BITMAP* bitmapa[liczba_kafelkow];

//
// Inicjalizacja
//

bool przygotuj_bitmapy()
{
    for (int i = 0; i < liczba_kafelkow; i++) {
        bitmapa[i] = al_load_bitmap(plik_z_kafelkiem[i].c_str());

        if (!bitmapa[i]) {
            cerr << "Błąd podczas wczytywania bitmapy " << plik_z_kafelkiem[i] << "." << endl;
            return false;
        }
    }

    return true;
}

void przygotuj_plansze()
{

    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < wy; j++) {
            p.podloga[i][j].typ = zwykla_podloga;

        }
    }

}

//
// Rysowanie planszy
//

void rysuj_plansze()
{
    al_clear_to_color(al_map_rgb(0,0,0));

    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < wy; j++) {
            // najpierw podłoga
            al_draw_bitmap_region(bitmapa[p.podloga[i][j].typ], 0,
                                  0, k_sz, k_wy, i*k_sz, j*k_sz, 0);


            // później ściany - mogą przykrywać obiekty
            al_draw_bitmap_region(bitmapa[p.sciana[i][j].typ], 0, 0, k_sz, k_wy, i*k_sz, j*k_sz, 0);

            // teraz gracze


                if (p.g.x == i && p.g.y == j) {
                    // k-ty gracz stoi na polu (i,j)

                    al_draw_bitmap_region(bitmapa[gracz], 0,
                                          0, k_sz, k_wy, i*k_sz, j*k_sz, 0);
                }
        }
    }
}



void wybor_startu()
{
    int x = p.g.x;
    int y = p.g.y;

    if (key[ALLEGRO_KEY_UP]) {
                    y = (y-1+wy)%wy;
    }
    else if (key[ALLEGRO_KEY_LEFT]) {
                    x = (x-1+sz)%sz;
    }
    else if (key[ALLEGRO_KEY_RIGHT]){
                    x = (x+1+sz)%sz;
    }
    else if (key[ALLEGRO_KEY_DOWN]) {
                    y = (y+1+sz)%sz;
    }
    p.g.x = x;
    p.g.y = y;
    if (key[ALLEGRO_KEY_ENTER]) p.g.start= true;
}


void ruchy_graczy(int energia)
{
    while(energia>0){
    int x = p.g.x;
    int y = p.g.y;
    int kr= p.g.kr;
    if(p.g.start){

     if (p.g.kr == 3 && p.sciana[x][y].typ == pusty) {
                    p.sciana[x][y].typ = zwykla;
                    kr = 0;
                    y = (y-1+wy)%wy;
    }
    else if (p.g.kr == 3 && p.sciana[x][y].typ == zwykla){
                    p.sciana[x][y].typ = pusty;
                    kr= 2;
                    y = (y+1+wy)%wy;
    }
    else if (p.g.kr == 1 && p.sciana[x][y].typ == pusty) {
                    p.sciana[x][y].typ = zwykla;
                    kr = 2;
                    y = (y+1+wy)%wy;
    }
    else if (p.g.kr == 1 && p.sciana[x][y].typ == zwykla){
                    p.sciana[x][y].typ = pusty;
                    kr= 0;
                    y = (y-1+wy)%wy;
    }
    else if (p.g.kr == 2 && p.sciana[x][y].typ == pusty) {
                    p.sciana[x][y].typ = zwykla;
                    kr = 3;
                    x = (x-1+sz)%sz;
    }
    else if (p.g.kr == 2 && p.sciana[x][y].typ == zwykla){
                    p.sciana[x][y].typ = pusty;
                    kr= 1;
                    x = (x+1+sz)%sz;
    }
    else if (p.g.kr == 0 && p.sciana[x][y].typ == pusty) {
                    p.sciana[x][y].typ = zwykla;
                    kr = 1;
                    x = (x+1+sz)%sz;
    }
    else if (p.g.kr == 0 && p.sciana[x][y].typ == zwykla){
                    p.sciana[x][y].typ = pusty;
                    kr= 3;
                    x = (x-1+sz)%sz;
    }
            }
    p.g.x = x;
    p.g.y = y;
    p.g.kr = kr;
    energia--;
    }
}

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;

int init()
{


    if(!al_init()) {
        cerr << "Błąd podczas inicjalizacji allegro." << endl;
        return -1;
    }

    if (!al_init_primitives_addon()) {
        cerr << "Błąd podczas inicjalizacji dodatku 'primitives'." << endl;
        return -1;
    }

    if(!al_init_image_addon()) {
        cerr << "Błąd podczas inicjalizacji dodatku 'image'." << endl;
        return -1;
    }

    if(!al_install_keyboard()) {
        cerr << "Błąd podczas inicjalizacji klawiatury." << endl;
        return -1;
    }

    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
        cerr << "Błąd podczas inicjalizacji zegara." << endl;
        return -1;
    }

    display = al_create_display(screen_w, screen_h);
    if(!display) {
        cerr << "Błąd podczas inicjalizacji ekranu." << endl;
        al_destroy_timer(timer);
        return -1;
    }

    event_queue = al_create_event_queue();
    if(!event_queue) {
        cerr << "Błąd podczas inicjalizacji kolejki zdarzeń." << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_clear_to_color(al_map_rgb(0,0,0));

    al_flip_display();


    return 0;
}

int main(int argc, char ** argv)
{
    if (init() != 0) {
        cerr << "Inicjalizacja nie powiodła się." << endl;
        return -1;
    }

    if (!przygotuj_bitmapy()) {
        return -1;
    }
    przygotuj_plansze();

    bool przerysuj = true;
    float energy=1;

    al_start_timer(timer);
    while(!wyjdz)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(p.g.start==false) wybor_startu();

        if(ev.keyboard.keycode ==ALLEGRO_KEY_Z)
                    {
                        if(energy > 1)energy=energy*2/3;
                        else energy = 0;

                    }
        if(ev.keyboard.keycode ==ALLEGRO_KEY_X)
                    {
                        if (energy < 1) energy=1;
                        else energy=energy*3/2;

                    }
        if(ev.keyboard.keycode ==ALLEGRO_KEY_SPACE)
                    { p.g.start=!p.g.start;
                    }
        if(ev.type == ALLEGRO_EVENT_TIMER) {

            przerysuj = true;

            ruchy_graczy(energy);

        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            key[ev.keyboard.keycode] = true;
        } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            key[ev.keyboard.keycode] = false;

            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                wyjdz = true;
            }
        }

        if(przerysuj && al_is_event_queue_empty(event_queue)) {
            przerysuj = false;

            rysuj_plansze();

            al_flip_display();
         }
    }
    return 0;
}
