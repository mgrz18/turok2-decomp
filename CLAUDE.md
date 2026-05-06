# Turok 2 Decomp — workspace operacional

Este workspace existe como **stepping stone** para portar nativamente **Armorines: Project S.W.A.R.M. (N64)** a Mac (Apple Silicon) y Android. Turok 2 va primero porque comparten engine (Acclaim Studios London), y Turok 2 tiene una base de community/romhacking ligeramente más establecida.

## Identidad y rol

Eres Claude trabajando con Miguel Garcia (admin Lasec, también dev hobbyist de N64 native ports) en su Mac (M-series, macOS 14+).

Workspace: `~/Documents/Proyectos/turok2_decomp/` (donde estás ahora).

Proyecto hermano: `~/Documents/Proyectos/1964/GoldenRecomp/` — referencia constante. Toda decisión de tooling, estructura y estilo debería seguir lo que ya está validado ahí.

## Objetivo de fondo

1. **Corto plazo**: Decompilar Turok 2: Seeds of Evil (N64) lo suficiente para usar N64Recomp y producir un binario nativo macOS que arranque.
2. **Mediano plazo**: Caracterizar el microcode RSP que usa el engine Acclaim London (probablemente F3DEX modificado), escribir handlers en RT64 si hace falta.
3. **Largo plazo**: Aplicar el mismo pipeline a Armorines (mismo engine), reusando ~70% del trabajo.
4. **Stretch**: Build Android (RT64 ya tiene Vulkan backend; falta el wrap nativo NDK + input touch).

**Why this path vs un emulador**: el usuario ya sabe que un emulador es 100x menos trabajo y suficiente para "solo jugarlo". Si está en este workspace es porque quiere modding (HD textures, widescreen real, 60 FPS), preservación, o aprender ingeniería inversa N64 a profundidad. **No le repitas el pitch del emulador a menos que él lo mencione**.

## Estado actual

🔴 **CERO. Workspace recién creado.** No hay código, ni decomp, ni splat config, ni ROM dumps. Empezando desde scratch.

| Componente | Estado |
|---|---|
| ROM Turok 2 (US/EU/JP) | ❌ no presente — verificar legalidad de obtención |
| splat config (`splat.yaml`) | ❌ |
| Disassembly inicial | ❌ |
| Funciones identificadas (matching) | ❌ 0 |
| N64Recomp config (`*.toml`) | ❌ |
| Build de algún ELF | ❌ |
| RT64 + N64ModernRuntime stack | ⚠️ disponible en GoldenRecomp como referencia |

## Engine y microcode

**Acclaim Studios London engine** — usado en:
- Turok 2: Seeds of Evil (1998)
- Turok: Rage Wars (1999)
- Turok 3: Shadow of Oblivion (2000)
- **Armorines: Project S.W.A.R.M.** (1999) ← objetivo final
- Re-Volt, Forsaken, NBA Jam (parcialmente)

### Microcode (a investigar)

- **Stock F3DEX** modificado (probable, NO confirmado al 100%)
- Audio: ucode propio probablemente (Acclaim tenía custom audio en varios títulos)
- VS GoldenEye (F3D_Gold custom): el microcode de Acclaim **debería** ser más cercano al stock → menos handlers que escribir en RT64

**Tarea inicial crítica**: extraer el RSP ucode del ROM y compararlo byte-a-byte con F3DEX stock. Si las diferencias son <20% del código, RT64 puede manejarlo con handlers menores. Si son fundamentales (como F3D_Gold), es proyecto de meses.

## Tooling de referencia (todo ya usado en GoldenRecomp)

- **splat** — disassembler/segment splitter <https://github.com/ethteck/splat>
- **N64Recomp** — static recomp Mr-Wiseguy <https://github.com/Mr-Wiseguy/N64Recomp>
- **N64ModernRuntime** — runtime base <https://github.com/Mr-Wiseguy/N64ModernRuntime>
- **RT64** — renderer <https://github.com/rt64/rt64>
- **m2c** — MIPS-to-C decompiler para acelerar matching
- **decomp.me** — comparativa visual del matching
- **mips_to_c** — predecesor de m2c
- **IDO 5.3 recomp** — el compilador histórico que SGI usó; necesario para byte-exact matching

GoldenRecomp ya tiene `tools/ido5.3_recomp/` instalado. Reusar.

## Roadmap inicial (próximas 4-8 semanas)

### Fase 1 — Bootstrap (semana 1)

- [ ] Confirmar/dump de ROM Turok 2 US (sha1 conocido)
- [ ] Inicializar repo git en este folder
- [ ] Setup splat.yaml con segments mínimos: header, boot, main, audio, gfx
- [ ] Primer disasm: ¿cuánto código hay y dónde están los entry points?
- [ ] Identificar microcode RSP: dump de `ucode_text` y `ucode_data`, comparar contra F3DEX

### Fase 2 — Microcode triage (semana 2)

- [ ] Diff RSP ucode vs F3DEX stock — ¿cuántas instrucciones cambiaron?
- [ ] Identificar nuevos opcodes graphics (G_*)
- [ ] Decisión go/no-go: si el delta es manejable → continuar; si es F3D_Gold-tier → reconsiderar el approach

### Fase 3 — Decomp inicial (semanas 3-6)

- [ ] m2c sobre boot, main, gfx_init
- [ ] Identificar libultra functions (osCreateThread, osSetEventMesg, etc.) por patrón
- [ ] Matching de las primeras 50-100 funciones críticas
- [ ] CMakeLists.txt para producir un .z64 byte-exact con el original

### Fase 4 — N64Recomp (semanas 7-8)

- [ ] Aplicar N64Recomp sobre el ELF generado
- [ ] Stub mínimo en C++ para que linkee
- [ ] Bring up RT64 con microcode handler
- [ ] Primera ventana abierta, aunque no se vea nada

## Lessons learned de GoldenRecomp (no repetir)

Leídas en `/Users/miguel_garcia/Documents/Proyectos/1964/GoldenRecomp/README.md` y `docs/INVESTIGATION.md`:

1. **El microcode es el bottleneck real**, no el recomp en sí. N64Recomp funciona; lo que falla es entender qué hace el RSP.
2. **No-determinismo en runs**: GoldenRecomp produce 250k pixels en 40-50% de runs. Síntoma de race conditions en el setup del DL pipeline. Si vemos esto en Turok 2, atacar `fbPair`/`fullSync` primero.
3. **Validar la pipeline antes que el output**: usar shaders diagnósticos (force-magenta) para verificar que el render path funciona ANTES de buscar texturas correctas.
4. **Texture loading desde direcciones corruptas** ("low-bits-0xB pattern" en GE): suele indicar que la TLB o segment mapping está mal, NO que el texture decoder esté roto.
5. **Combiner alpha cascade = 0 → coverage discard**: típico fallo silencioso. Logear el combiner mode antes que el pixel output.

## Preferencias del usuario (heredadas de proyectos previos)

- Idioma: **español mexicano neutral** (tú, no vos). Tecnicismos en inglés (build, commit, microcode, recomp, etc.).
- Actuar rápido sin cuestionarios largos.
- Nunca cerrar/destruir proyectos sin consentimiento explícito.
- Plan: Claude Max compartido (no API). No proponer soluciones que requieran API key Anthropic.
- Cambios destructivos (rm -rf, drop tables, etc.): **confirmar antes**.
- Modo Claude Code: `bypassPermissions` global cuando esté en su Mac.

## Cómo trabajar en este workspace

- `git init` apenas haya algo concreto que versionar.
- Estructura propuesta (mirroring GoldenRecomp):
  ```
  turok2_decomp/
  ├── assets/         # extracts de ROM (texturas, audio, geo)
  ├── bin/            # boot/main code raw
  ├── build/          # output de make/cmake
  ├── include/        # headers reverse-engineered
  ├── src/            # C reverse-engineered
  ├── rsp/            # microcode disasm
  ├── tools/          # splat, ido, etc. (symlinks a GoldenRecomp si aplica)
  ├── splat.yaml      # config inicial
  ├── undefined_syms.txt
  ├── undefined_funcs.txt
  └── Makefile
  ```
- ROM dump: dejarlo como `baserom.us.z64` con sha1 conocido (verificar legalidad — debe ser dump de un cartucho del usuario).
- Para el primer commit, idea: solo `splat.yaml` mínimo + Makefile + README explicando el goal.

## TODO de discovery (lo primero que debe hacer una sesión Claude nueva aquí)

1. Verificar si hay ROM en este folder. Si no, el usuario debe proveerla.
2. Buscar disasm públicos de Turok 2 en GitHub: `n64decomp/turok2`, `Turok2-Decomp`, `acclaim-london-engine`, etc.
3. Si encuentra base existente, **clonarla y construir sobre ella** en vez de empezar from scratch.
4. Confirmar versión target con el usuario: US 1.0 (más documentada), o EU/JP.

## Referencias externas útiles

- N64 microcode reference (Mr-Wiseguy): <https://github.com/Mr-Wiseguy/N64ModernRuntime/wiki>
- RT64 architecture docs: <https://github.com/rt64/rt64/wiki>
- splat tutorials: <https://github.com/ethteck/splat/blob/main/docs/tutorial.md>
- decomp.me (sandbox de matching): <https://decomp.me>
- N64Brew Wiki (microcode/RSP/RDP): <https://n64brew.dev/wiki>
- Turok 2 ROM hacking notes (community): <https://gametechwiki.com> (buscar Turok 2)

## Pendientes operativos

- [ ] El usuario confirma si ya tiene ROM Turok 2 dumpeada y dónde
- [ ] Decisión: empezar por Turok 2 (más comunidad) o saltar directo a Armorines
- [ ] Investigar si existe ya algún disasm público de Turok 2 en GitHub
- [ ] Ver si conviene reusar `tools/ido5.3_recomp/` de GoldenRecomp via symlink o copia
