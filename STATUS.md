# Mario Party 6 recovery status

This is an evidence snapshot, not a completion claim. It was last verified on
2026-07-14 with the pinned project toolchain.

## Verification

- `ninja -j1`: `137 files OK` (the DOL and all configured REL outputs)
- `build/GP6E01/main.dol` SHA-1:
  `b897e6ade6b3a0cd2f9907689f38a3b19c327e70`
- DTK progress at that build: 6.87% code and 24.23% data overall; 33.87% code
  and 56.19% data in the DOL
- `src/` and `include/` contain no C or C++ `asm`/`__asm` blocks

The exact build result includes extracted original objects and explicit
standalone assembly fallbacks for owners that are not yet byte-identical C.
Those owners remain `NonMatching` in `configure.py`; fallback-linked code is
not counted as decompiled source.

## `game/` ownership

`game/` is not fully decompiled. The current `Game` library has 54 of 61
object owners configured as matching. The remaining owners are:

- `game/init.c`
- `game/jmp.c`
- `game/malloc.c`
- `game/hsfdraw.c`
- `game/hsfanim.c`
- `game/kerent.c`
- `board/board.c`

`game/mic.c` is matching and is linked from recovered C.

## Assembly fallback boundary

Inline assembly is forbidden in C and C++ sources. Architecture-specific code
that cannot be expressed by a verified MWCC intrinsic is isolated from the
clean source owners:

- the main and three REL runtime variants use explicit standalone `.s`
  fallbacks under `asm/`
- low-level context, interrupt, cache, startup, and timebase routines use DTK's
  extracted original-object fallback while their owners remain `NonMatching`
- THP paired-single decoder routines use extracted original-object fallback
- recovered scalar matrix, vector, and quaternion SDK implementations compile
  as clean C but remain `NonMatching` where the target used paired-single code

Promotion requires a compiled source object with matching bytes and
relocations. A successful container checksum alone does not promote a source
owner.
