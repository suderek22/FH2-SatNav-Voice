# SatNav Voice

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
durango/HZ2_DX_SatNav_PL.bank`.
