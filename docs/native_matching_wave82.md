# Native matching Wave 82: complete `mdpartydll` stage

Wave 82 completes the clean-C stage owner from frozen `fork/main` commit
`acf6ac0c0200e9e30c3639063aa974efeb95322d`.  All 57 functions and all
`0x73D8` target text bytes in `REL/mdpartydll/stage.c` are now represented and
byte-exact.  The owner is source-linked; no assembly, byte packet, padding
object, volatile register force, or other fakematch was introduced.

## Recovered source

The nine former holes contribute `0x2B1C` exact target bytes:

| Function | Target size | Recovered responsibility |
| --- | ---: | --- |
| `fn_1_410D4` | `0x548` | radial particle initialization, rotation, fade, and cache flush |
| `fn_1_42258` | `0x8E8` | normalized burst initialization and three-state particle motion |
| `fn_1_42F34` | `0x478` | five-entry texture-particle transition table and interpolation |
| `fn_1_43778` | `0x600` | four-particle-per-frame emitter and color/scale evolution |
| `fn_1_4459C` | `0x580` | single-particle emitter variant |
| `fn_1_45A48` | `0x1F8` | stage burst wrapper with a by-value color aggregate |
| `fn_1_45C40` | `0x2FC` | player-color paired particle setup |
| `fn_1_45F3C` | `0x4A4` | mode-dependent paired stage effects |
| `fn_1_464A0` | `0x35C` | color-table stage-effect dispatcher |

The target ABI proves that the color parameters of `fn_1_40AEC` and
`fn_1_40FEC` are `GXColor` values, not pointers.  Standalone PowerPC code
passes each four-byte aggregate through an address, so both spellings can look
the same in an isolated callee.  The inlined callers expose the distinction:
MWCC creates a copied aggregate formal.  Matching same-game `THPDraw.c` and
`thpmain.c` authenticate the `(GXColor) { ... }` compound-literal source
shape, and the resulting temporary/formal order reproduces the target's two
adjacent stack objects without an invented helper.

`fn_1_4459C` and `fn_1_464A0` have same-game `mdseldll` twins.  Those twins
authenticate the framework and source structure; MP6 target instructions and
data prove the party-stage constants and branch differences.  The remaining
particle functions were recovered from their target state transitions,
stores, calls, structure strides, and relocations.

## Math-pool ownership

The linked REL retains one weak `sqrtf` constant pair at module `.rodata:0`,
inside `mdparty.c`'s target range.  The reconstructed root `math.h` emits a
discardable pair for every including translation unit, but DTK cannot assign a
linker-discarded input copy to `stage.o`.  Stage therefore uses the authentic
SDK `dolphin/math.h` inline bodies while blocking the reconstructed header.
`#pragma pool_data off` preserves the target's per-symbol pool addressing
without manufacturing the discarded 16 bytes.  Matching game/board owners
authenticate the SDK-header substitution; Matching boot, file-select, and
menu owners independently authenticate owner-local `pool_data off`.

The final relocation-aware objdiff report is empty.  Target and source agree
for all 57 functions and every retained section:

| Section | Exact size |
| --- | ---: |
| `.text` | `0x73D8` |
| `.rodata` | `0x178` |
| `.data` | `0x24` |
| `.bss` | `0xE4` |

## Party-driver accounting

`mdparty.c` remains fallback-linked and is not counted as recovered source.
The application/stage evidence ledger is now:

| Owner | Target | Represented source | Byte-exact source |
| --- | ---: | ---: | ---: |
| `mdparty.c` | 258 functions, `0x3F424` bytes | 111 functions, `0x7F34` bytes | 108 functions, `0x7744` bytes |
| `stage.c` | 57 functions, `0x73D8` bytes | 57 functions, `0x73D8` bytes | 57 functions, `0x73D8` bytes |
| Clean-C evidence total | 315 functions, `0x467FC` bytes | 168 functions, `0xF30C` bytes | 165 functions, `0xEB1C` bytes |

The module as a whole remains outside the conservative fully recovered REL
done set until `mdparty.c` also source-links.  The authenticated compiler
runtime remains assembly under the exception and contributes zero clean-C
bytes.

## Verification

The object and serialized-container gates were run after the final source and
configuration edits:

```powershell
rtk ninja -j1 build/GP6E01/src/REL/mdpartydll/stage.o
rtk ..\..\tools\objdiff\v3.7.2\objdiff-cli.exe diff -p . `
  -u mdpartydll/REL/mdpartydll/stage `
  -o build/GP6E01/src/REL/mdpartydll/stage_objdiff_wave82_final_object.json `
  --format json-pretty -c functionRelocDiffs=data_value
rtk ninja -j1
rtk build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1
rtk cmp.exe orig/GP6E01/sys/main.dol build/GP6E01/main.dol
rtk cmp.exe orig/GP6E01/files/dll/mdpartydll.rel `
  build/GP6E01/mdpartydll/mdpartydll.rel
```

Results:

- objdiff: 57/57 functions, all retained data, and effective relocations exact;
- full build and explicit DTK checksum: `137 files OK`;
- `main.dol`: byte-identical, SHA-1
  `b897e6ade6b3a0cd2f9907689f38a3b19c327e70`;
- `mdpartydll.rel`: byte-identical, SHA-1
  `519debb149ef42eda1ab3b0a4d2b3132b4f3e3cc`;
- `config/GP6E01/symbols.txt` SHA-1 before and after the final build:
  `3aaa758be28a24cc27e46fca2ab840f65168e05a`.
