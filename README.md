# Familiada - gra rozrywkowa

## Opis Projektu
Gra "Familiada" to desktopowa aplikacja będąca wiernym odwzorowaniem popularnego teleturnieju. Projekt został zrealizowany z wykorzystaniem paradygmatu programowania obiektowego w języku C++ (C++/CLI z użyciem biblioteki Windows Forms). Aplikacja kompleksowo zarządza logiką gry, punktacją, interfejsem graficznym oraz wczytywaniem bazy danych z zewnętrznych plików, co czyni ją świetnym przykładem strukturalnej architektury OOP.

## Funkcjonalności
* **Personalizacja drużyn:** Możliwość wyboru od 3 do 6 graczy w każdej drużynie oraz ustawienia własnych nazw i imion uczestników.
* **Dynamiczna baza pytań:** Pytania oraz punktowane odpowiedzi są na bieżąco wczytywane i parsowane z pliku w formacie JSON (`familiada_pytania.json`).
* **System buzzerów:** Zintegrowana obsługa fizycznej rywalizacji na klawiaturze, z blokadą wejścia po wciśnięciu pierwszego przycisku.
* **Mechanika teleturnieju:** Pełna obsługa 3 błędów (krzyżyków), mechanizmu przejmowania rundy przez drużynę przeciwną i sumowania punktów na środku tablicy.
* **Rozbudowany Finał:** Unikalna runda finałowa dla dwóch najlepszych graczy zwycięskiej drużyny. Posiada wbudowany odliczający timer oraz weryfikację duplikatów – drugi gracz nie może powtórzyć odpowiedzi podanej przez pierwszego.

## Uruchomienie
Aby uruchomić grę "Familiada" należy włączyć plik  **`.exe`**  znajdujący się w folderze `Familiada/x64/Release`

## Wymagania sprzętowe:

#### **Minimum:**
* **OS:** Dowolny
* **Procesor:** Zalecany
* **Pamięć:** 1 MB RAM
* **Grafika:** To tylko jest tekst...
* **Miejsce na dysku:** 5 MB miejsca na dysku 
* **Karta dźwiękowa:** Dowolna
* **Dodatkowe uwagi:** Pójdzie na ziemniaku.

---

#### **Zalecane:**
* **OS:** Windows 90000000
* **Procesor:** Bardzo szybki procesor obsługujący 9000 pytań na sekundę
* **Pamięć:** 999 GB RAM
* **Grafika:** Karta graficzna z obsługą 3D (dla efektów wizualnych)
* **Miejsce na dysku:** 5 MB miejsca na dysku 
* **Karta dźwiękowa:** Karta symulująca orkiestrę na żywo
* **Dodatkowe uwagi:** Naprawdę nie jest wymagająca gra...

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
