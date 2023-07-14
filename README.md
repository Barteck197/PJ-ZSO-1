# PJ-ZSO-1
Projekt nr 1 w ramach przedmiotu ZSO (zaawansowane systemy operacyjne)

## Temat projektu
Problem świętego Mikołaja. 

Święty Mikołaj śpi w swojej chatce na biegunie północnym. 

Może go zbudzić jedynie: przybycie dziewięciu reniferów lub trzy spośród dziesięciu skrzatów, chcących poinformować Mikołaja o problemach z produkcją zabawek. 

Skrzaty i renifery przybywają osobno. Gdy zbiorą się wszystkie renifery, Mikołaj zaprzęga je do sań, dostarcza zabawki grzecznym dzieciom, wyprzęga je i pozwala odejść na spoczynek. 

Mikołaj zbudzony przez skrzaty wprowadza je do biura, udziela konsultacji a później żegna. 

Obsługa reniferów ma priorytet nad obsługą skrzatów. 

Snu Mikołaja nie może przerwać mniej niż dziewięć reniferów ani mniej niż trzy skrzaty!

## Wytyczne
- program ma być bezpieczny wielowątkowo (C/C++) i ma symulować rozwiązanie problemu przedstawionego scenariusza
- projekty piszemy w C/C++ (biblioteka `thread`) - preferowane C, a jeżeli w C++ to proszę unikać obiektowości, dziedziczenia, wszelki aspektów utrudniających weryfikację - skupiamy się na wątkach.
- program powinien dać się sparametryzować - robimy to na poziomie kompilacji przez `consty` oraz z wykorzystaniem `#ifdef` w przypadku wyświetlania informacji na ekranie i wprowadzeniu opóźnień. Generalnie program powinien dać się skompilować w dwóch wersjach bez wyświetlania informacji na ekranie i bez opóźnień (żadnych sleepów) oraz z informacjami i ze sleepami i do tego proszę użyć `#ifdef`. Ma to pokazać, ze program jest merytorycznie poprawny.
- program będzie testowany przy użyciu narzędzia Valgrind. Opis narzędzia jest w wykładzie 11. Mamy sprawdzić wycieki pamięci, blokowanie się wątków, warunki hazardu  itp... 
- aby program napisać poprawnie trzeba wykorzystać mechanizmy synchronizacji opisane w wykładach 1 – 6 czyli głównie muteksy i obowiązkowo zmienne decyzyjne. 
- zadanie trzeba obronić, to znaczy wstawić do gakko i umówić się na pogawędkę
- w zadaniach proszę unikać elementów losowości

Uwaga, program implementujemy w funkcji o nazwie `projekt_zso(...)` i ta funkcja powinna zawierać kompletne rozwiązanie problemu. Funkcja ta ma zostać uruchomiona 10 razy. Wszystkie wątki mają powstać w tej funkcji i zostać poprawnie zakończone.
