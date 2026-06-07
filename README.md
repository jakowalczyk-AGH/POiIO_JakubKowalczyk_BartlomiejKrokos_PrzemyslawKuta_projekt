# Familiada - C++ Windows Forms Game

## Opis Projektu
Gra "Familiada" to desktopowa aplikacja będąca wiernym odwzorowaniem popularnego teleturnieju. Projekt został zrealizowany z wykorzystaniem paradygmatu programowania obiektowego w języku C++ (C++/CLI z użyciem biblioteki Windows Forms). Aplikacja kompleksowo zarządza logiką gry, punktacją, interfejsem graficznym oraz wczytywaniem bazy danych z zewnętrznych plików, co czyni ją świetnym przykładem strukturalnej architektury OOP.

## Funkcjonalności
* **Personalizacja drużyn:** Możliwość wyboru od 3 do 6 graczy w każdej drużynie oraz ustawienia własnych nazw i imion uczestników.
* **Dynamiczna baza pytań:** Pytania oraz punktowane odpowiedzi są na bieżąco wczytywane i parsowane z pliku w formacie JSON (`familiada_pytania.json`).
* **System buzzerów:** Zintegrowana obsługa fizycznej rywalizacji na klawiaturze, z blokadą wejścia po wciśnięciu pierwszego przycisku.
* **Mechanika teleturnieju:** Pełna obsługa 3 błędów (krzyżyków), mechanizmu przejmowania rundy przez drużynę przeciwną i sumowania punktów na środku tablicy.
* **Rozbudowany Finał:** Unikalna runda finałowa dla dwóch najlepszych graczy zwycięskiej drużyny. Posiada wbudowany odliczający timer oraz weryfikację duplikatów – drugi gracz nie może powtórzyć odpowiedzi podanej przez pierwszego.

## Wymagania i Uruchomienie
Aby poprawnie skompilować i uruchomić projekt, upewnij się, że posiadasz środowisko obsługujące C++/CLI (np. Visual Studio z zainstalowanymi komponentami ".NET desktop development" oraz "C++/CLI support").

1. Sklonuj repozytorium na swój dysk.
2. Otwórz plik rozwiązania (Solution - `.slnx`) w Visual Studio.
3. **Ważne:** Upewnij się, że w folderze wyjściowym z wygenerowanym plikiem wykonywalnym (`.exe`) (zazwyczaj folder `Debug` lub `Release`) znajdują się niezbędne zasoby aplikacji:
   * Plik z czcionką: `familiada.ttf`
   * Plik z pytaniami: `familiada_pytania.json`
   * Katalog `Assets/` zawierający grafiki buzzerów:
     * `buzzer-dark-red.png` / `buzzer-light-red.png`
     * `buzzer-dark-green.png` / `buzzer-light-green.png`
4. Skompiluj projekt (Build) i uruchom grę.

## Zasady Gry
Rozgrywka dzieli się na rundy zasadnicze (ich liczba zależy od liczby członków drużyny) oraz finał.

**Ważne:** Odpowiedzi wpisujemy z małej litery, bez polskich znaków, w liczbie pojedyncznej, a czasowniki w formie bezosobowej, np. latanie, sprzatanie, myslenie.

**Rundy zasadnicze:**
1. Na początku każdej rundy reprezentant z każdej drużyny rywalizuje o prawo głosu, starając się jak najszybciej wcisnąć buzzer.
2. Drużyna, która pierwsza wciśnie przycisk, przejmuje rundę i jej zadaniem jest odgadnięcie wszystkich ukrytych na tablicy odpowiedzi.
3. Za każdą błędną odpowiedź lub odpowiedź spoza tablicy drużyna otrzymuje błąd (tzw. "krzyżyk").
4. Po zebraniu 3 błędów, drużyna przeciwna ma prawo do narady i podania jednej odpowiedzi. Jeśli trafią w nieodkryte hasło – kradną całą pulę punktów zebraną w danej rundzie. Jeśli spudłują – punkty pozostają u pierwotnej drużyny.

**Finał:**
1. Do finału awansuje drużyna, która zgromadzi najwięcej punktów. Gra automatycznie wybiera dwóch pierwszych zawodników ze zwycięskiej ekipy.
2. Gracze odpowiadają na 5 pytań pod dużą presją czasu.
3. Odpowiedzi drugiego gracza są walidowane – aplikacja poinformuje o błędzie (BZZZZ!), jeśli gracz wpisze hasło podane wcześniej przez pierwszego zawodnika.
4. Zdobycie minimum 200 punktów z obu tur finałowych gwarantuje wygraną w programie.

## Sterowanie
Aplikacja wymaga połączenia korzystania z myszy (nawigacja, klikanie w pola tekstowe w celu wpisania odpowiedzi) oraz klawiatury podczas samej rozgrywki:
* **`Q`** – Wciśnięcie buzzera przez LEWĄ drużynę na ekranie startowym rundy.
* **`P`** – Wciśnięcie buzzera przez PRAWĄ drużynę na ekranie startowym rundy.
* **`Enter`** – Ostateczne zatwierdzenie wpisanej odpowiedzi w polu tekstowym (zarówno w rundach zasadniczych, jak i w finale). Przycisk ten tłumi również standardowy systemowy dźwięk, zapewniając płynne przejścia w grze.
