#define UNICODE
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <sstream>
#include <iomanip>

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;
using namespace std;

const int LICZBA_PIETER = 6;
const double WAGA_OSOBY = 70.0;
const double MAKS_WAGA = 600.0;
const int TIMER_ID = 1;
const int TIMER_POSTOJ_ID = 2;
const int TIMER_POWROT_ID = 3;
const int PREDKOSC_PX = 4;
const int TIMER_MS = 20;

HINSTANCE hInst;
HWND hwndMain;
HWND poleMasa, poleLiczba;
HWND przyciski[LICZBA_PIETER];
HWND polaWejscie[LICZBA_PIETER];
HWND polaWyjscie[LICZBA_PIETER];
HWND etykietaIN, etykietaOUT;
ULONG_PTR gdiToken;

int wysokoscPietra = 100;
int liczbaOsob = 0;
double masa = 0.0;
int aktualnePietro = 0;
int docelowePietro = 0;
bool jedzie = false;
bool postojAktywny = false;
bool kierunekWGore = true;
double yPozycja = 0;

int PietroNaY(int pietro) {
    return (LICZBA_PIETER - 1 - pietro) * wysokoscPietra + 10;
}

int PobierzLiczbe(HWND combo) {
    int sel = (int)SendMessage(combo, CB_GETCURSEL, 0, 0);
    if (sel == CB_ERR) return 0;
    return sel;
}

void AktualizujEtykiety() {
    wstringstream ss;
    ss << L"Masa: " << fixed << setprecision(1) << masa << L" kg";
    SetWindowText(poleMasa, ss.str().c_str());

    wstring txt = L"Osoby: " + to_wstring(liczbaOsob);
    SetWindowText(poleLiczba, txt.c_str());
}

void RysujWinde(HDC hdc) {
    Graphics g(hdc);

    SolidBrush tlo(Color(240, 240, 240));
    g.FillRectangle(&tlo, 0, 0, 480, 600);

    Pen linia(Color(100, 100, 100), 1);
    for (int i = 0; i <= LICZBA_PIETER; ++i) {
        int y = i * wysokoscPietra + 10;
        g.DrawLine(&linia, 0, y, 280, y);
    }

    int kabinaX = 200;
    int kabinaY = (int)yPozycja;

    SolidBrush kabina(Color(0, 128, 0));
    

    g.FillRectangle(&kabina, kabinaX, kabinaY, 80, 80);

    Pen kontur(Color(0, 50, 0), 3);
    g.DrawRectangle(&kontur, kabinaX, kabinaY, 80, 80);

    SolidBrush ledBg(Color(30, 30, 30));
    Rect ledRect(kabinaX + 10, kabinaY + 25, 60, 30);
    g.FillRectangle(&ledBg, ledRect);

    FontFamily ff(L"Consolas");
    Font font(&ff, 22, FontStyleBold, UnitPixel);
    SolidBrush ledTxt(Color(0, 255, 0));

    wstring tekstPietro = L" " + to_wstring(aktualnePietro) + L" ";
    g.DrawString(tekstPietro.c_str(), -1, &font, PointF((float)(kabinaX + 20), (float)(kabinaY + 28)), &ledTxt);
}

void PoDotarciuNaPietro(int pietro) {
    aktualnePietro = pietro;
    jedzie = false;
    KillTimer(hwndMain, TIMER_ID);

    int wysiada = PobierzLiczbe(polaWyjscie[pietro]);
    wysiada = min(wysiada, liczbaOsob);
    liczbaOsob -= wysiada;

    int wsiada = PobierzLiczbe(polaWejscie[pietro]);
    double nowaMasa = (liczbaOsob + wsiada) * WAGA_OSOBY;

    if (nowaMasa <= MAKS_WAGA) {
        liczbaOsob += wsiada;
    }
    else {
        MessageBox(hwndMain, L"Przeciążenie! Osoby nie mogą wejść.", L"Ostrzeżenie", MB_ICONWARNING);
    }

    masa = liczbaOsob * WAGA_OSOBY;
    AktualizujEtykiety();

    KillTimer(hwndMain, TIMER_POSTOJ_ID);
    KillTimer(hwndMain, TIMER_POWROT_ID);

    if (liczbaOsob == 0 && pietro != 0) {
        postojAktywny = false;
        SetTimer(hwndMain, TIMER_POWROT_ID, 5000, NULL);
    }
    else {
        postojAktywny = true;
        SetTimer(hwndMain, TIMER_POSTOJ_ID, 5000, NULL);
    }
}

void JedzNaPietro(int pietro) {
    if (jedzie || pietro == aktualnePietro) return;
    docelowePietro = pietro;
    kierunekWGore = (docelowePietro > aktualnePietro);
    jedzie = true;
    SetTimer(hwndMain, TIMER_ID, TIMER_MS, NULL);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        hwndMain = hwnd;
        RECT rc; GetClientRect(hwnd, &rc);
        wysokoscPietra = (rc.bottom - rc.top - 20) / LICZBA_PIETER;
        yPozycja = PietroNaY(0);

        for (int i = LICZBA_PIETER - 1; i >= 0; --i) {
            int y = 10 + (LICZBA_PIETER - 1 - i) * wysokoscPietra + (wysokoscPietra - 25) / 2;

            wstring txt = L"P. " + to_wstring(i);
            przyciski[i] = CreateWindow(L"BUTTON", txt.c_str(), WS_CHILD | WS_VISIBLE,
                320, y, 50, 25, hwnd, (HMENU)(100 + i), hInst, nullptr);

            polaWejscie[i] = CreateWindow(L"COMBOBOX", NULL,
                WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST,
                375, y, 40, 200, hwnd, NULL, hInst, nullptr);

            polaWyjscie[i] = CreateWindow(L"COMBOBOX", NULL,
                WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST,
                420, y, 40, 200, hwnd, NULL, hInst, nullptr);

            for (int v = 0; v <= 8; v++) {
                wstring s = to_wstring(v);
                SendMessage(polaWejscie[i], CB_ADDSTRING, 0, (LPARAM)s.c_str());
                SendMessage(polaWyjscie[i], CB_ADDSTRING, 0, (LPARAM)s.c_str());
            }

            SendMessage(polaWejscie[i], CB_SETCURSEL, 0, 0);
            SendMessage(polaWyjscie[i], CB_SETCURSEL, 0, 0);
        }

        etykietaIN = CreateWindow(L"STATIC", L"IN", WS_CHILD | WS_VISIBLE | SS_CENTER,
            375, 5, 40, 20, hwnd, NULL, hInst, nullptr);
        etykietaOUT = CreateWindow(L"STATIC", L"OUT", WS_CHILD | WS_VISIBLE | SS_CENTER,
            420, 5, 40, 20, hwnd, NULL, hInst, nullptr);

        poleMasa = CreateWindow(L"STATIC", L"Masa: 0 kg", WS_CHILD | WS_VISIBLE | SS_CENTER,
            10, 2, 100, 25, hwnd, NULL, hInst, nullptr);
        poleLiczba = CreateWindow(L"STATIC", L"Osoby: 0", WS_CHILD | WS_VISIBLE | SS_CENTER,
            10, 28, 100, 25, hwnd, NULL, hInst, nullptr);
        break;
    }

    case WM_COMMAND: {
        int id = LOWORD(wParam);
        if (id >= 100 && id < 100 + LICZBA_PIETER) {
            JedzNaPietro(id - 100);
        }
        break;
    }

    case WM_TIMER:
        if (wParam == TIMER_ID && jedzie) {
            int yDoc = PietroNaY(docelowePietro);
            if (kierunekWGore) {
                yPozycja -= PREDKOSC_PX;
                if (yPozycja <= yDoc) {
                    yPozycja = yDoc;
                    PoDotarciuNaPietro(docelowePietro);
                }
            }
            else {
                yPozycja += PREDKOSC_PX;
                if (yPozycja >= yDoc) {
                    yPozycja = yDoc;
                    PoDotarciuNaPietro(docelowePietro);
                }
            }
            InvalidateRect(hwnd, NULL, TRUE);
        }
        else if (wParam == TIMER_POSTOJ_ID) {
            KillTimer(hwnd, TIMER_POSTOJ_ID);
            postojAktywny = false;
        }
        else if (wParam == TIMER_POWROT_ID) {
            KillTimer(hwnd, TIMER_POWROT_ID);
            if (liczbaOsob == 0 && !jedzie && !postojAktywny) {
                JedzNaPietro(0);
            }
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RysujWinde(hdc);
        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    hInst = hInstance;

    GdiplusStartupInput gsi;
    GdiplusStartup(&gdiToken, &gsi, nullptr);

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"WindaFinal";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(L"WindaFinal", L"Symulator Windy - wersja prostokątna",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 480, 600,
        nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiToken);
    return 0;
}
