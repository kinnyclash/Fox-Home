# Fox Home v0.11.18: TI-84 Plus CE (C version, open beta)

This version runs on a regular TI-84 Plus CE, no Python needed. It's written in C for the CE C/C++ toolchain. Because it's a C program, the calculator has to be **jailbroken** to run it (for example with arTIfiCE).

## What's in this folder
| Path | What it is |
|---|---|
| `src/` | The game's source code (`main.c`, plus `sprites.c` with Kinny's fox and clothing art). |
| `makefile` | Build settings for the CE toolchain. |
| `FOXBG.8xv` | The four living-room backgrounds, already converted. Send this to the calculator. |
| `art/` | Kinny's original art as PNGs, for reference. |
| `tools/mkbg.py` | Rebuilds `FOXBG.8xv` from the background PNGs. Only needed if you change them. |

## 1. Build the program (once, on a computer)
1. Install the **CE C/C++ toolchain** (CEdev) from github.com/CE-Programming/toolchain/releases, following its install guide.
2. Open a terminal in this folder and run:
   ```
   make
   ```
3. That creates `bin/FOXHOME.8xp`.

## 2. Put it on the calculator
Use **TI Connect CE** (or TiLP) to send these files:
- `bin/FOXHOME.8xp`: the game.
- `FOXBG.8xv`: the backgrounds. Optional, but without it the living room uses the built-in drawing.
- The **CE C libraries** (`clibs.8xg`), from github.com/CE-Programming/libraries/releases. C programs need them to run. Skip this if you already have them.

## 3. Run it
Start it the same way you run other C programs on your jailbroken calculator: through arTIfiCE or a shell like Cesium, or from the home screen with `Asm(prgmFOXHOME)` if your OS allows that.

Press `CLEAR` on the home screen to save and quit. Your save is the AppVar `FOXSAVE`. It gets archived when you quit, so it survives RAM clears, but anything since your last quit is lost if the calculator crashes. Don't delete `FOXSAVE` unless you want to start over.

## Differences from the Python version
- **Naming a fox:** a naming screen replaces Python's `input()`. Letter keys type letters the way they do with ALPHA held. `ALPHA` switches lowercase on and off, `0` types a space, and `DEL` erases.
- **Dev menu:** same secret code on the home screen (`(` `)` `(` `)`), now with its own number screen.
- **Holding a key** does nothing extra. Each press counts once.
- **Saves:** C saves go in `FOXSAVE`, not the Python version's lists, so progress doesn't carry over between the two versions.
- Everything else works the same: foxes, events, food, rooms, clothes, Simon Says, Sumo, Fishing, day and night.

## Adding art later
- **Clothes and fox:** stored as drawing data in `src/sprites.c`. Send new art in to have it converted.
- **Backgrounds:** need to be 160×55 PNGs (the game shows them at double size). Then run:
  ```
  python3 tools/mkbg.py FOXBG.8xv morning.png day.png evening.png night.png
  ```
  Right now only the living room has backgrounds. Backgrounds for other rooms would need a small code change.

Created with love by Kinny. CE C port for the open beta.
