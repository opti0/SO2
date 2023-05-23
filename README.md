# TODO:

## - Działanie power pelletu
	Jest dodana podstawowa funkcjonalność. W momencie zjedzienia pelletu pacman i pellet na chwilę znikają. 
      
## - Duchy:
	Duchy czasem znikają z planszy (widoczne zwłaszcza na "pustej mapie")
	Po zjedzeniu ducha on sie pojawia w tym samym miejscu w kolejnej iteracji swojego wątku

## - Mutexy:
    W jednym miejscu trzeba jeszcze zmienić "board mutex" na nasz semafor

## - Język kodu:
    Zmienić nazwy funkcji w pliku "semafor.cpp" na angielski,
    tak, aby zachować spójność całego projektu
    i być zgodnym z warunkami z prezentacji.

## - Komentarze w kodzie:
    Brakuje komentarzy w kodzie (w języku angielskim,
    łumaczących co z grubsza robi każda funkcja.

---
# SO2 - Pacman

## O projekcie
Założeniem projektu jest stworzenie gry typu Pacman, wyświetlającej się w terminalu i wykorzystującej do działania mechanizm wielowątkowości.

Gracz ma do dyspozycji różne mapy, losowo przydzielane podczas rozpoczęcia rozgrywki.

## Twórcy
inż. Maciej Radecki

Jacek Myjkowski

---
## Funkcjonalność gry
Podstawowa funkcjonalność gry opiera się na chodzeniu Pacmanem po mapie i zbieraniu punkcików. Gracz musi unikać duchów poruszających się losowo po mapie, nie gonią one Pacmana.

Gracz wygrywa, gdy zbierze z mapy wszystkie punkty.

Gracz przegrywa, gdy dotknie ducha.

## Dodatkowa funkcjonalność
Pacman ma możliwość zjedzenia ducha. Aby to zrobić, potrzebuje podnieść "power pellet", który na określony czas da mu taką umiejętność. W tym momencie ikonka Pacmana zmieni kolor z żółtego na zielony. Gdy czas działania power pelletu dobiegnie końca, Pacman z powrotem znowu stanie się żółty.

---
## Ekran gry
![Example board](pacman_board.png)

Aby nie rozpraszać uwagi gracza, wyświetlane są tylko najpotrzebniejsze informacje. Dlatego poza samą planszą wyświetlane są tylko punkty zdobyte przez gracza oraz ilość wykonanych przez niego ruchów.

### Poruszanie się po mapie:
Gracz do poruszania się po mapie używa strzałek na klawiaturze. Poruszanie się za pomocą klawiszy "WSAD" nie jest możliwe.

### Oznaczenia na planszy:
* Kolorem żółtym zaznaczony jest Pacman.
* Kolorem czerwonym oznaczone są duchy.
* Kolorem zielonym oznaczony jest power pellet oraz Pacman w czasie działania power pelletu.
* Kolorem białym oznaczone są punkciki do zebrania przez Pacmana.
* Na niebiesko są ściany oraz krawędzie mapy.

---

## Wątki w programie:
### 1. Pacman
co reprezentuje ten wątek
### 2. Duchy
co reprezentuje ten wątek

## Sekcje krytyczne:
(typ, np muteks/semafor)




