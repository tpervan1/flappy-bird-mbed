#include "mbed.h"
#include "stm32f413h_discovery_lcd.h"
#include "string"
#include "time.h"

InterruptIn taster1(p8);

Ticker skakutanje;
Ticker jelpukla;
//globalne varijable
int x=30, y=110, x1=240, xs1=240, x2=1000, xs2=1000, x3=1000, xs3=1000;
int x11=50, xs11=50, x22=50, xs22=50, x33=50, xs33=50;
int bodovi=0;
int HighScore=0;
bool igra=0;
bool pocetak=1;
int ispisbodova=0;
bool pocetna_poruka=1;
bool krajnja_poruka=0;
bool kraj=0;

//neki stringovi
std::string wellcome1 = "Welcome to Flappy Bird!";
std::string wellcome2= "Touch to play";
std::string endgame1="Game over!";
//prototipi nekih funkcija
void CrtajPrepreke();
void JeLiUdarila(int, int, int);
void koordinate();
void ObrisiPrepreke();

void Reset()//funkcija koja resetuje globalne varijable
{
bodovi=0;
x=30;
y=110;
x1=240;
xs1=240;
xs2=1000;
xs3=1000;
x2=1000;
x3=1000;
}

void CrtajPticu()//funkcija koja crta pticu
{
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawCircle(x, y, 10);//crna kruznica oko tijela
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    BSP_LCD_FillCircle(x, y, 9);  //tijelo
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawCircle(x+5, y-5, 5);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);//oko
    BSP_LCD_FillCircle(x+5,y-5,4);
    BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
    BSP_LCD_FillEllipse(x+6,y+4,7,4); //usta
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillCircle(x+5,y-5,2);//oko
    BSP_LCD_DrawHLine(x-1,y+4,15); //usne
     //krila
    if(y%40==0){
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);//kruznica oko krila
    BSP_LCD_DrawCircle(x-8, y-5, 6);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);//krila
    BSP_LCD_FillCircle(x-8,y-5,5);
    }
    if(y%40==10){
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawCircle(x-8, y-3, 6);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_FillCircle(x-8,y-3,5);
    }
     if(y%40==20){
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawCircle(x-8, y, 6);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_FillCircle(x-8,y,5);
    }
    else if(y%40==30){
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawCircle(x-8, y+4, 6);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_FillCircle(x-8,y+4,5);
    }
}

void ObrisiPticu()//funkcija koja brise pticu
{
    BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
    BSP_LCD_DrawCircle(x, y, 10);
    BSP_LCD_FillCircle(x, y, 9);
    BSP_LCD_DrawCircle(x+5, y-5, 5);
    BSP_LCD_FillCircle(x+5,y-5,4);
    BSP_LCD_FillEllipse(x+6,y+4,7,4);
    BSP_LCD_FillCircle(x+5,y-5,2);
    BSP_LCD_DrawHLine(x-1,y+4,15);

    if(y%40==0){
    BSP_LCD_DrawCircle(x-8, y-5, 6);
    BSP_LCD_FillCircle(x-8,y-5,5);
    }
    if(y%40==10){
    BSP_LCD_DrawCircle(x-8, y-3, 6);
    BSP_LCD_FillCircle(x-8,y-3,5);
    }
     if(y%40==20){
    BSP_LCD_DrawCircle(x-8, y, 6);
    BSP_LCD_FillCircle(x-8,y,5);
    }
    else if(y%40==30){
    BSP_LCD_DrawCircle(x-8, y+4, 6);
    BSP_LCD_FillCircle(x-8,y+4,5);
    }
}
//funkcija koja crta dinamicku pozadinu(oblake i zgrade)
void Dinamicka_Pozadina(){
    //oblaci
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_FillRect(x1+55,30,38,10);
    BSP_LCD_FillRect(x1+64,20,20,10);

    BSP_LCD_FillRect(x2+55,40,38,10);
    BSP_LCD_FillRect(x2+64,30,20,10);

    BSP_LCD_FillRect(x3+55,30,38,10);
    BSP_LCD_FillRect(x3+64,20,20,10);
    //zgrade
    BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
    BSP_LCD_FillRect(x1+60,200,20,35);
    BSP_LCD_FillRect(x1+90,190,10,45);

    BSP_LCD_FillRect(x2+60,190,20,45);
    BSP_LCD_FillRect(x2+90,215,10,20);

    BSP_LCD_FillRect(x3+60,210,20,25);
    BSP_LCD_FillRect(x3+90,185,10,50);
    //prozori na zgradama
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    //zadnji sprat
    BSP_LCD_FillRect(x1+63,203,5,5);//1.zgrada
    BSP_LCD_FillRect(x1+70,203,5,5);//1.zgrada
    BSP_LCD_FillRect(x1+93,193,5,5);//2.zgrada

    BSP_LCD_FillRect(x2+63,193,5,5);
    BSP_LCD_FillRect(x2+70,193,5,5);
    BSP_LCD_FillRect(x2+93,218,5,5);

    BSP_LCD_FillRect(x3+63,213,5,5);
    BSP_LCD_FillRect(x3+70,213,5,5);
    BSP_LCD_FillRect(x3+93,188,5,5);
    //predzadnji sprat
    BSP_LCD_FillRect(x1+63,211,5,5);
    BSP_LCD_FillRect(x1+70,211,5,5);
    BSP_LCD_FillRect(x1+93,201,5,5);

    BSP_LCD_FillRect(x2+63,201,5,5);
    BSP_LCD_FillRect(x2+70,201,5,5);
    BSP_LCD_FillRect(x2+93,226,5,5);

    BSP_LCD_FillRect(x3+63,221,5,5);
    BSP_LCD_FillRect(x3+70,221,5,5);
    BSP_LCD_FillRect(x3+93,196,5,5);
    //sprat
    BSP_LCD_FillRect(x1+63,219,5,5);
    BSP_LCD_FillRect(x1+70,219,5,5);
    BSP_LCD_FillRect(x1+93,209,5,5);

    BSP_LCD_FillRect(x2+63,209,5,5);
    BSP_LCD_FillRect(x2+70,209,5,5);

    BSP_LCD_FillRect(x3+93,204,5,5);
    //sprat
    BSP_LCD_FillRect(x1+63,228,5,5);
    BSP_LCD_FillRect(x1+70,228,5,5);
    BSP_LCD_FillRect(x1+93,217,5,5);

    BSP_LCD_FillRect(x2+63,218,5,5);
    BSP_LCD_FillRect(x2+70,218,5,5);

    BSP_LCD_FillRect(x3+93,212,5,5);
    //prizemlje
    BSP_LCD_FillRect(x1+93,225,5,5);

    BSP_LCD_FillRect(x3+93,220,5,5);
    //zemlja
    BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGREEN);
    BSP_LCD_FillRect(0,235,240,5);
}
//funkcija koja brise dinamicku pozadinu
void Obrisi_Dinamicku_Pozadinu(){
    //brisem oblake
    BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
    BSP_LCD_FillRect(x1+55,30,38,10);
    BSP_LCD_FillRect(x1+64,20,20,10);

    BSP_LCD_FillRect(x2+55,40,38,10);
    BSP_LCD_FillRect(x2+64,30,20,10);

    BSP_LCD_FillRect(x3+55,30,38,10);
    BSP_LCD_FillRect(x3+64,20,20,10);
    //brisem zgrade

    BSP_LCD_FillRect(x1+60,200,20,35);
    BSP_LCD_FillRect(x1+90,190,10,45);

    BSP_LCD_FillRect(x2+60,190,20,45);
    BSP_LCD_FillRect(x2+90,215,10,20);

    BSP_LCD_FillRect(x3+60,210,20,25);
    BSP_LCD_FillRect(x3+90,185,10,50);
    //brisem prozore
    //zadnji sprat
    BSP_LCD_FillRect(x1+63,203,5,5);//1.zgrada
    BSP_LCD_FillRect(x1+70,203,5,5);//1.zgrada
    BSP_LCD_FillRect(x1+93,193,5,5);//2.zgrada

    BSP_LCD_FillRect(x2+63,193,5,5);
    BSP_LCD_FillRect(x2+70,193,5,5);
    BSP_LCD_FillRect(x2+93,218,5,5);

    BSP_LCD_FillRect(x3+63,213,5,5);
    BSP_LCD_FillRect(x3+70,213,5,5);
    BSP_LCD_FillRect(x3+93,188,5,5);
    //predzadnji sprat
    BSP_LCD_FillRect(x1+63,211,5,5);
    BSP_LCD_FillRect(x1+70,211,5,5);
    BSP_LCD_FillRect(x1+93,201,5,5);

    BSP_LCD_FillRect(x2+63,201,5,5);
    BSP_LCD_FillRect(x2+70,201,5,5);
    BSP_LCD_FillRect(x2+93,226,5,5);

    BSP_LCD_FillRect(x3+63,221,5,5);
    BSP_LCD_FillRect(x3+70,221,5,5);
    BSP_LCD_FillRect(x3+93,196,5,5);
    //sprat
    BSP_LCD_FillRect(x1+63,219,5,5);
    BSP_LCD_FillRect(x1+70,219,5,5);
    BSP_LCD_FillRect(x1+93,209,5,5);

    BSP_LCD_FillRect(x2+63,209,5,5);
    BSP_LCD_FillRect(x2+70,209,5,5);

    BSP_LCD_FillRect(x3+93,204,5,5);
    //sprat
    BSP_LCD_FillRect(x1+63,228,5,5);
    BSP_LCD_FillRect(x1+70,228,5,5);
    BSP_LCD_FillRect(x1+93,217,5,5);

    BSP_LCD_FillRect(x2+63,218,5,5);
    BSP_LCD_FillRect(x2+70,218,5,5);

    BSP_LCD_FillRect(x3+93,212,5,5);
    //prizemlje
    BSP_LCD_FillRect(x1+93,225,5,5);

    BSP_LCD_FillRect(x3+93,220,5,5);
}

void IspisiBodove()//ispisi bodove u desno cose
{
    std::string s=std::to_string(bodovi);//bodovi u string
    unsigned char *val1=new unsigned char[s.length()+1];
    strcpy((char *)val1,s.c_str());
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(0, 15, val1, RIGHT_MODE);
    delete [] val1;
}

//poruka koja se prikazuje samo jednom, prilikom pokretanja
void PocetnaPoruka()
{
    BSP_LCD_Init();
            unsigned char *val2=new unsigned char[wellcome1.length()+1];
            strcpy((char *)val2,wellcome1.c_str());
            unsigned char *val3=new unsigned char[wellcome2.length()+1];
            strcpy((char *)val3,wellcome2.c_str());
            BSP_LCD_SetFont(&Font12);
    BSP_LCD_DisplayStringAt(0, 100, val2, CENTER_MODE);//Welcome to Flappy Bird
    BSP_LCD_DisplayStringAt(0, 130, val3, CENTER_MODE);//Touch to play
    delete[] val2;delete[] val3;
}
//poruka koja se prikazuje kada izgubimo
void PorukaZaKraj()
{
    if(bodovi>HighScore) HighScore=bodovi;//Postavi novi highscore
        BSP_LCD_Clear(LCD_COLOR_WHITE);
        BSP_LCD_Init();
        unsigned char *val4=new unsigned char[endgame1.length()+1];
        strcpy((char *)val4,endgame1.c_str());
        BSP_LCD_DisplayStringAt(0, 70, val4, CENTER_MODE);//game over
        std::string endgame2="Your score:" + std::to_string(ispisbodova);
        std::string endgame3="Highscore:" + std::to_string(HighScore);
        unsigned char *val5=new unsigned char[endgame2.length()+1];
        strcpy((char *)val5,endgame2.c_str());
        unsigned char *val6=new unsigned char[endgame3.length()+1];
        strcpy((char *)val6,endgame3.c_str());
        unsigned char *val7=new unsigned char[wellcome2.length()+1];
        strcpy((char *)val7,wellcome2.c_str());
        BSP_LCD_DisplayStringAt(0, 100, val5, CENTER_MODE);//skor
        BSP_LCD_DisplayStringAt(0, 130, val6, CENTER_MODE);//haaiskor
        BSP_LCD_DisplayStringAt(0, 160, val7, CENTER_MODE);//pokreni
        Reset();
        krajnja_poruka=1;
        delete[] val4;delete[] val5;delete[] val6;delete[] val7;
}
/*funkcija koja poziva funkcije za brisanje ekrana, azuriranje koordinata, a
zatim crta azurirane verzije*/
void ptica(){
    if(krajnja_poruka==1) igra=0;
    if(igra==1){
        BSP_LCD_Init();
        ObrisiPrepreke();
        ObrisiPticu();
        Obrisi_Dinamicku_Pozadinu();
        koordinate();
            if(igra) {
                CrtajPrepreke();
                CrtajPticu();
                Dinamicka_Pozadina();
                IspisiBodove();
                ispisbodova=bodovi;}
            else { PorukaZaKraj(); kraj=1;}
        BSP_LCD_Init();
    }
    else{
        if(pocetak==1) PocetnaPoruka();
        else  { PorukaZaKraj();kraj=1;}

    }
}
//funkcija koja azurira koordinate u igri
void koordinate(){
    if(igra==1) {
        if(y<=0) y=10;
        if(y>=230) y=230;
        y+=10;
        x1-=10;
        xs1=x1;
        xs22=x22;
        if(x1==-100) x1=1000; //stavi na 1000 jer nece dostici 240 neko vrijeme
        if(x1==160){//kad je 160 izmijeni sljedecu-x2
            srand (time(NULL));
            x22=10*(rand() % 10 + 1);
            x2=260+x22;
        }
        x2-=10;
        xs2=x2;
        xs33=x33;
        if(x2==-100) x2=1000;
        if(x2==160){
            srand (time(NULL));
            x33=10*(rand() % 10 + 1);
            x3=260+x33;
        }
        x3-=10;
        xs3=x3;
        xs11=x11;
        if(x3==-100) x3=1000;
        if(x3==160){
            srand (time(NULL));
            x11=10*(rand() % 10 + 1);
            x1=260+x11;
        }
        if(y<=10 || y>=230) { igra=0; return;}
        if(x1==10 || x2==10 || x3==10) bodovi++;
    }
}
//funkcija koja provjerava da li je ptica udarila u neku od prepreka
void JeLiUdarilaUPrepreku()
{
     JeLiUdarila(y,x1,x11);
        JeLiUdarila(y,x2,x22);
        JeLiUdarila(y,x3,x33);
}
//funkcija koja gleda je li ptica udarila u pojedinu prepreku, sastavni dio gornje fje
void JeLiUdarila(int y, int x1, int x11)
{
    if((x>=x1-10 && x<=x1+50 && (y<20+x11+10  || y>100+x11-10))|| y<=10 || y>=230) { igra=0; krajnja_poruka=1;kraj=1;}
}
//funkcija koja registruje klik
void kliknuto(){
    if(pocetak==1) {pocetak=0;BSP_LCD_Clear(LCD_COLOR_CYAN); igra=1;}
    if(kraj==0 && igra==0) return;
    if(kraj==1 && krajnja_poruka) {kraj=0;krajnja_poruka=0;BSP_LCD_Clear(LCD_COLOR_CYAN);igra=1; }
    if(igra) {ObrisiPticu(); y=y-30; CrtajPticu();}//raste po 20(koord sistem je takav da je 0 po y osi gore)
}
//crtaj prepreku
void prepreka1(int p){
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillRect(p,0,10,20+x11);//gornja
     BSP_LCD_FillRect(p,100+x11,10,135-x11);//donja
}
void prepreka2(int p){
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillRect(p,0,10,20+x22);
    BSP_LCD_FillRect(p,100+x22,10,135-x22);
}
void prepreka3(int p){
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillRect(p,0,10,20+x33);
    BSP_LCD_FillRect(p,100+x33,10,135-x33);
}
//brisi prepreku
void prepreka11(int p, int xs){
    BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
    BSP_LCD_FillRect(p+30,0,10,20+xs);//gornja
     BSP_LCD_FillRect(p+30,100+xs,10,135-xs);//donja
}
void prepreka22(int p, int xs){
    BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
    BSP_LCD_FillRect(p+30,0,10,20+xs);
    BSP_LCD_FillRect(p+30,100+xs,10,135-xs);
}
void prepreka33(int p, int xs){
    BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
    BSP_LCD_FillRect(p+30,0,10,20+xs);
    BSP_LCD_FillRect(p+30,100+xs,10,135-xs);
}
//crtaj sve prepreke
void CrtajPrepreke(){
    prepreka1(x1);
    prepreka2(x2);
    prepreka3(x3);
}
//obrisi sve prepreke
void ObrisiPrepreke(){
    prepreka11(xs1, xs11);
    prepreka22(xs2, xs22);
    prepreka33(xs3, xs33);
}

int main() {
    skakutanje.attach(&ptica,0.05); //crta pticu u ovisnosti od x i y
    jelpukla.attach(&JeLiUdarilaUPrepreku, 0.01);
    taster1.rise(&kliknuto);//dize pticu na klik


    while (1) {
            wait_ms(1);
        }
    }
