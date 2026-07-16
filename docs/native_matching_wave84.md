# Native matching Wave 84: recover `mdparty` setup and transitions

Wave 84 materializes 24 target-backed functions and `0x3A9C` target text
bytes in `REL/mdpartydll/mdparty.c` from frozen `fork/main` commit
`8ab8d17db6e6b15eeacf4c9e5ae6c437bc6dd973`. Relocation-aware objdiff proves
23 functions and `0x3764` bytes exact. `fn_1_2EBEC` remains an explicitly
non-exact WIP residue. The incomplete owner remains fallback-linked and
`NonMatching`; none of its source is counted in the conservative source-linked
REL total.

No assembly, byte packet, volatile/register force, fake local, source padding,
or guessed symbol was introduced.

## Materialized spans

The independently recovered spans are:

| Span | Represented functions | Target text | Exact text | Recovered responsibility |
| --- | ---: | ---: | ---: | --- |
| `fn_1_26FC`, `fn_1_275C`, `fn_1_39AC`, `fn_1_A5D8`, `fn_1_B4C8`, `fn_1_BB30` | 6 | `0x6CC` | `0x6CC` | window setup, player-state setup, selection flow, and state scanning |
| `fn_1_12B18`, `fn_1_12D7C`, `fn_1_12EC8`, `fn_1_14344`, `fn_1_14F64`, `fn_1_1733C`, `fn_1_17E10`, `fn_1_1807C` | 8 | `0x108C` | `0x108C` | character-model presentation, animation transitions, and per-player object state |
| `fn_1_21628`, `fn_1_21C9C`, `fn_1_22EB8`, `fn_1_2C874`, `fn_1_2CE00`, `fn_1_2EBEC`, `fn_1_2EF24` | 7 | `0x18F4` | `0x15BC` | party-entry selection, sprite updates, mode initialization, and late flow state |
| `fn_1_D3E0`, `fn_1_16A8C`, `fn_1_28044` | 3 | `0xA50` | `0xA50` | model-transition helper, character-model construction, and four-player callback setup |
| Wave 84 total | 24 | `0x3A9C` | `0x3764` | 23 exact functions and one retained WIP residue |

## Recovered storage and source shape

Target offsets recover the `0x38`-byte character-model entry used by
`lbl_1_bss_288`:

```c
typedef struct LBL_1_BSS_288_ENTRY {
    HU3D_MODELID modelId;
    HU3D_ANIMID animId[4];
    s16 unk_A;
    HuVecF pos;
    HuVecF rot;
    HuVecF scale;
    s16 unk_30;
    u8 unk_32[6];
} LBL_1_BSS_288_ENTRY;
```

The recovered functions use the vector fields directly. `fn_1_16A8C` also
proves that the player character selector is held in a 32-bit local and
narrowed at the signed-16-bit model boundary. This produces the target entry
address calculation and register lifetime without a forced register.

The independently exact `fn_1_D3E0(s16 modelNo, float angle, s16 time,
s16 duration)` owns the model-rotation transition and calls the already exact
`fn_1_1868`. Its trailing inline redeclaration reproduces both expansions in
`fn_1_1807C`; the caller passes `i + 11` through the helper's signed-16-bit
boundary instead of duplicating the emitted instructions.

Other exact source-shape discriminators include:

- explicit signed-16-bit character-selector conversions at both argument
  positions of the two inlined `fn_1_C158` calls in `fn_1_12EC8`;
- the chained assignment in `fn_1_1733C`, which stores the helper result to
  both vector components before the target reload;
- separate increment/decrement statements in `fn_1_21628`, preserving the
  target reload of the global before its comparison;
- the exact inline `fn_1_9B90` helper call in `fn_1_22EB8`, replacing a
  hand-expanded sprite-bank operation; and
- `fn_1_A880` as a wrapper around its byte-identical twin `fn_1_9A24`.
  Nested inlining reproduces the target nonvolatile-register allocation in
  `fn_1_2C874`; both standalone helpers remain independently exact.

These are compiler- and target-backed call boundaries, not transcriptions of
the resulting assembly.

## Retained WIP residue

`fn_1_2EBEC` has target/source size `0x338/0x338` and scores 99.902916%.
The relocation-aware report isolates the difference to the temporary used to
load the `-200.0f` comparison constant: the current source uses `r3`, while
the target uses `r4`. Alternate condition senses and an early-return rewrite
were compiled and rejected because they reduced the score or changed the
function size. No volatile temporary, register force, or unproved AST spelling
was retained. The function therefore remains represented WIP, not byte-exact
source.

## Party-driver accounting

| Owner | Target | Represented source | Byte-exact source |
| --- | ---: | ---: | ---: |
| `mdparty.c` | 258 functions, `0x3F424` bytes | 181 functions, `0xEAB4` bytes | 177 functions, `0xDF8C` bytes |
| `stage.c` | 57 functions, `0x73D8` bytes | 57 functions, `0x73D8` bytes | 57 functions, `0x73D8` bytes |
| Clean-C evidence total | 315 functions, `0x467FC` bytes | 238 functions, `0x15E8C` bytes | 234 functions, `0x15364` bytes |

The represented residues are:

| Function | Target/source size | Relocation-aware score |
| --- | ---: | ---: |
| `fn_1_2B0` | `0x150/0x14C` | 98.095240% |
| `fn_1_400` | `0x24C/0x248` | 98.911570% |
| `fn_1_22A8` | `0x454/0x454` | 99.444046% |
| `fn_1_2EBEC` | `0x338/0x338` | 99.902916% |

Another 77 functions remain unrepresented. Therefore `mdparty.c` stays in
the `C-not-yet-matched` bucket and no `configure.py` flip is permitted.

## Verification

The final source was checked with:

```powershell
rtk ninja -j1 build/GP6E01/src/REL/mdpartydll/mdparty.o
rtk ..\..\tools\objdiff\v3.7.2\objdiff-cli.exe diff -p . `
  -u mdpartydll/REL/mdpartydll/mdparty `
  -o build/GP6E01/src/REL/mdpartydll/mdparty_objdiff_wave84_final_object.json `
  --format json-pretty -c functionRelocDiffs=data_value
rtk ninja -j1
rtk build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1
rtk cmp.exe orig/GP6E01/sys/main.dol build/GP6E01/main.dol
rtk cmp.exe orig/GP6E01/files/dll/mdpartydll.rel `
  build/GP6E01/mdpartydll/mdpartydll.rel
```

Results:

- objdiff: 23 of the 24 Wave 84 additions and their effective relocations are
  exact; `fn_1_2EBEC` remains the documented 99.902916% residue;
- full build and explicit DTK checksum: `137 files OK`;
- `main.dol`: byte-identical, SHA-1
  `b897e6ade6b3a0cd2f9907689f38a3b19c327e70`;
- `mdpartydll.rel`: byte-identical, SHA-1
  `519debb149ef42eda1ab3b0a4d2b3132b4f3e3cc`; and
- `config/GP6E01/symbols.txt` SHA-1 before and after the final build:
  `3aaa758be28a24cc27e46fca2ab840f65168e05a`.
