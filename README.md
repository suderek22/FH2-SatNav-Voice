# SatNav Voice - Google Maps PL v1.0.0

FH2 Mod Platform mod that replaces all 122 Polish SatNav voice lines with a Polish voice pack (OG Google Maps style).

## Install

1. Copy the contents of `fh2-mods` into your game's `fh2-mods` folder (`SatNavVoice.dll`, `SatNavVoice.ini`, `SatNav\HZ2_DX_SatNav_PL.bank`).
2. Delete the old `SatNavCustomVoice.dll` and `SatNavCustomVoice.ini` if you have them.
3. Set the game's speech language to **Polish**.
4. The option **SatNav Voice - PL** is under Pause > Settings > Mods (on by default). Changing it needs a game restart.

## What is replaced

Every line of `HZ2_DX_SatNav_PL.bank`: turns, bear/keep, exits, U-turn, roundabouts (1st-6th exit), recalculating, destination reached (near/far/extended) and the "destination set" lines (autoshow, exhibition, festival, race, paint shop, workshop...). Imperial lines use the matching metric recordings.

## How it works

The stock bank is an FMOD FEV file with an embedded FSB5 container of XMA2 audio. `tools\build_bank.py` decodes every line, replaces it with the file assigned in `tools\mapping.csv` (from `source\`), and rewrites the container as PCM16 (about 34 MB). Each replacement is converted to mono at the bank's sample rate, trimmed of silence, and loudness-matched to the stock line. The DLL serves the bank through the platform's `set_file` at `audio/fmodanthem/durango/HZ2_DX_SatNav_PL.bank`.

---

# SatNav Voice - Google Maps PL v1.0.0 (po polsku)

Mod do FH2 Mod Platform, który zastępuje wszystkie 122 polskie komunikaty nawigacji SatNav polskim pakietem głosowym (w stylu oryginalnego Google Maps).

## Instalacja

1. Skopiuj zawartość folderu `fh2-mods` do folderu `fh2-mods` w katalogu gry (`SatNavVoice.dll`, `SatNavVoice.ini`, `SatNav\HZ2_DX_SatNav_PL.bank`).
2. Usuń stare pliki `SatNavCustomVoice.dll` i `SatNavCustomVoice.ini`, jeśli je masz.
3. Ustaw język mowy w grze na **polski**.
4. Opcja **SatNav Voice - PL** znajduje się w Pauza > Ustawienia > Mody (domyślnie włączona). Zmiana wymaga ponownego uruchomienia gry.

## Co jest zastępowane

Każdy komunikat z `HZ2_DX_SatNav_PL.bank`: skręty, łagodne skręty/trzymanie się strony, zjazdy, zawracanie, ronda (1.–6. zjazd), przeliczanie trasy, dotarcie do celu (blisko/daleko/rozszerzony) oraz komunikaty o ustawieniu celu (salon samochodowy, wystawa, festiwal, wyścig, lakiernia, warsztat itd.). Komunikaty w jednostkach imperialnych używają odpowiadających im nagrań metrycznych.

## Jak to działa

Oryginalny bank to plik FMOD FEV z osadzonym kontenerem FSB5 z dźwiękiem XMA2. `tools\build_bank.py` dekoduje każdy komunikat, zastępuje go plikiem przypisanym w `tools\mapping.csv` (z folderu `source\`) i zapisuje kontener ponownie jako PCM16 (ok. 34 MB). Każde nagranie jest konwertowane do mono w częstotliwości próbkowania banku, pozbawiane ciszy na początku i końcu oraz dopasowywane głośnością do oryginalnego komunikatu. DLL podaje bank grze przez `set_file` platformy pod ścieżką `audio/fmodanthem/durango/HZ2_DX_SatNav_PL.bank`.