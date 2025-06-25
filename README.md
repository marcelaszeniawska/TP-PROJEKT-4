# TP-PROJEKT-4
WYKONAŁY: Marcela Szeniawska 203272, Aleksandra Sala 203559;
ABY URUCHOMIĆ PROGRAM NALEŻY MIEĆ USTAWIONY PODSYSTEM NA WINDOWS

➜ Symulator Windy — Projekt C++

 ➜ Opis projektu

Projekt to prosty symulator działania windy napisany w C++ z użyciem WinAPI i biblioteki GDI+ do grafiki.  
Aplikacja pozwala na:

- Symulację ruchu windy między 6 piętrami
- Obsługę przycisków wyboru piętra
- Wyświetlanie aktualnej masy pasażerów oraz liczby osób w kabinie
- Definiowanie liczby osób wsiadających („IN”) i wysiadających („OUT”) na każdym piętrze
- Kontrolę maksymalnego udźwigu windy (600 kg)
- Automatyczny powrót windy na parter, jeśli jest pusta.

➜ Funkcjonalności

- Ruch windy: Winda porusza się w górę lub w dół z określoną prędkością, zatrzymując się na wybranych piętrach.
- Sterowanie: Zawiera przyciski do wyboru piętra oraz pola „IN” i „OUT” jako rozwijane listy umożliwiające wybór liczby osób wsiadających i wysiadających.
- Kontrola udźwigu: Program pilnuje, aby masa pasażerów nie przekroczyła limitu 600 kg (przy założeniu średniej wagi 70 kg na osobę).
- Wyświetlanie danych: Aktualna masa i liczba osób w windzie są wyświetlane w lewym górnym rogu okna.
- Estetyka: Kabina windy jest rysowana jako prostokąt w jednolitym zielonym kolorze, a interfejs jest czytelny i prosty.

➜ Struktura programu

- WinAPI— obsługa okna, przycisków, timerów, komunikacji z systemem
- GDI+ — rysowanie windy, linii pięter oraz wyświetlanie tekstu
- Logika symulacji — zarządzanie ruchem windy, zmianą liczby pasażerów, kontrola udźwigu
- Interfejs użytkownika — przyciski wyboru pięter, pola „IN” i „OUT” jako rozwijane listy umożliwiające wybór liczby osób wsiadających i wysiadających.

➜ Jak uruchomić

1. Skopiuj cały kod źródłowy do pliku .cpp.
2. Skonfiguruj projekt w Visual Studio lub innym środowisku obsługującym WinAPI i GDI+.
3. Skompiluj i uruchom program.
4. Używaj przycisków po prawej stronie, by wybierać piętra.
5. Ustaw liczby osób wsiadających i wysiadających w rozwijanych listach „IN” i „OUT”.
6. Obserwuj ruch windy oraz aktualizację masy i liczby pasażerów.

➜ Uwagi

- Program zakłada średnią wagę pasażera 70 kg.
- Maksymalna masa windy to 600 kg.
- Jeśli winda jest pusta i znajduje się na piętrze innym niż parter, po 5 sekundach automatycznie wraca na parter.
