"""Turok 2 (N64, US) asset tools (Phase 6). Never commit their output: it is game data.

    romid.py      identify the ROM (.z64/.v64/.n64 -> z64, SHA1 gate), asset roots
    romfs.py      the ROM filesystem: regions, cartridge index tree, load units, coverage
    propack.py    RNC ProPack methods 1 and 2 (ProPackDecompress)
    cache_calls.py  the CCache__Request call sites in the C (descriptions, callbacks)
    textures.py   texture banks + graphic sets (HUD, fonts, screens) -> PNG
    models.py     object geometry -> OBJ / glTF / PNG (posed through anim.py with --pose)
    anim.py       skeletons + animations -> JSON / glTF / PNG
    levels.py     levels (collision, grid sections, instances, paths, ...) -> JSON + maps
    cinemas.py    cinemas -> JSON
    audio.py      libaudio banks, sound tree, sequences, adec streams -> WAV / MIDI / JSON
    extract_all.py  everything above in one run, with a summary and a coverage table

Every module also works as a script (python tools/assets/<name>.py ROM --out DIR); they
import each other by plain name (each puts its own folder on sys.path), so the folder does
not need to be installed as a package.
"""
