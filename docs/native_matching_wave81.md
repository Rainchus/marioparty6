# Native matching Wave 81: `mdpartydll` flow and stage expansion

Wave 81 continues the party-mode driver from frozen `fork/main` commit
`cd875cd7965f11753d79a0c03c96b8ab5440afaf`. The application and stage
owners remain `NonMatching`; their source is measured as evidence inside
fallback-linked owners and contributes nothing to the recovered-module total.
No assembly, byte packet, padding object, or matching flip was introduced.

## Clean-C accounting

Relocation-aware objdiff with `functionRelocDiffs=data_value` proves 156 of
159 represented application/stage functions and exactly `0xC000` target text
bytes:

| Owner | Target | Represented source | Byte-exact source |
| --- | ---: | ---: | ---: |
| `mdparty.c` | 258 functions, `0x3F424` bytes | 111 functions, `0x7F34` bytes | 108 functions, `0x7744` bytes |
| `stage.c` | 57 functions, `0x73D8` bytes | 48 functions, `0x48BC` bytes | 48 functions, `0x48BC` bytes |
| Clean-C total | 315 functions, `0x467FC` bytes | 159 functions, `0xC7F0` bytes | 156 functions, `0xC000` bytes |

The Wave 80 stage represented-byte total was `0x2C` low: its 29 definitions
sum to `0x26B0`, not `0x2684`. The omitted amount is the already represented
`fn_1_40A9C`. The corrected Wave 80 combined represented baseline is therefore
`0x73D0`; its exact baseline remains `0x6918`.

Against that corrected baseline, this wave adds 69 represented functions and
`0x5420` target bytes. Sixty-seven new definitions are exact; the two new
loader definitions remain WIP. The authentic stage ownership reshape also
closes the two prior stage residues, so the net exact gain is 69 functions and
`0x56E8` target bytes.

## Recovered application source

The application slice adds 50 definitions and `0x3214` represented target
bytes. Forty-eight definitions and `0x2E78` bytes are exact. The recovered
families include:

- the character-motion and board/capsule asynchronous loader, configuration
  transfer, save initialization, and party bootstrap;
- model, animation, sprite, window, camera, and transition callbacks across
  the party setup flow;
- exact selection and movement records, including the `0x38` model/animation
  entry with its target-proven `+0x30` field, two `0x30` movement records, and
  the four `0x0E` player-selection entries;
- the exact team-order table, 14-entry board-directory table, stage position
  table, six-entry sprite-bank table, diagnostic strings, and their real BSS
  owners;
- eight independently recovered leaf helpers for the `0x8C`, `0xA4`, and
  `0xD4` `HuVecF` arrays, three sprite-bank groups, and the object motion-ID
  setter/getter.

MP5 authenticates the selection field name `chrSel`, the shared sprite-bank
framework, and the `CharMotionAMemPGet(...) == 0` source expression. MP6 target
instructions, relocations, array strides, field offsets, and call arguments
prove the differing layouts and constants. The object completion field is
`mtnId[0]` at `OMOBJ+0x48`; it is not an invented `data` payload.

## Recovered stage source

The stage slice adds 19 definitions and `0x220C` exact target bytes. All 48
represented stage functions now match. The added source covers stage-effect
setup and teardown, nine texture resources, model/particle lifecycle helpers,
party-scene emitters, framebuffer motion, and the application-facing stage
coordinator.

Three source-shape facts close the former broad residues:

- stage globals are declared before code and defined at the end in reverse
  target order, reproducing `.data 0x24` and `.bss 0xE4`, including the natural
  two-byte gap before `lbl_1_bss_AF0`;
- `fn_1_44DCC` and `fn_1_4581C` call visible helper definitions, allowing the
  target's real depth-two auto-inlining instead of manually duplicating bodies;
- compound assignments remain inside the consuming call or condition where
  the target retains the updated floating-point value.

Same-game `mdseldll` authenticates the extern-before-code/reverse-definition
shape. Matching `fileseldll` authenticates the `fn_1_3FA6C`, `fn_1_3FB6C`, and
`fn_1_40020` framebuffer-motion family. MP6 object evidence proves the stage
constants, stores, inline boundaries, relocations, and final ownership.

Nine stage functions remain unrepresented rather than guessed:
`fn_1_410D4`, `fn_1_42258`, `fn_1_42F34`, `fn_1_43778`, `fn_1_4459C`,
`fn_1_45A48`, `fn_1_45C40`, `fn_1_45F3C`, and `fn_1_464A0`.

## Genuine represented residues

Only three represented functions remain divergent, all in `mdparty.c`:

| Function | Objdiff evidence |
| --- | --- |
| `fn_1_2B0` | target/source `0x150/0x14C`, 98.095240%; the sole difference is target `li r0, 0; cmplw r3, r0` versus source `cmplwi r3, 0`. |
| `fn_1_400` | target/source `0x24C/0x248`, 98.911570%; this is the same zero-comparison residue in the authentic inline copy of `fn_1_2B0`. |
| `fn_1_22A8` | target/source `0x454/0x454`, 99.444046%; instruction semantics pair, but parameter coloring and three `HuVecF` stack slots rotate. |

The MP5-authenticated unsuffixed `== 0` expression and logical negation both
collapse to the same immediate comparison under the pinned MWCC front end.
No volatile zero, fabricated local, register force, or fake operation was
introduced to manufacture the missing target instruction.

## Verification

The final object reports were regenerated after the source edits:

```powershell
rtk ninja -j1 build/GP6E01/src/REL/mdpartydll/mdparty.o `
  build/GP6E01/src/REL/mdpartydll/stage.o
rtk ..\..\tools\objdiff\v3.7.2\objdiff-cli.exe diff -p . `
  -u mdpartydll/REL/mdpartydll/mdparty `
  -o build/GP6E01/src/REL/mdpartydll/mdparty_objdiff_wave81.json `
  --format json-pretty -c functionRelocDiffs=data_value
rtk ..\..\tools\objdiff\v3.7.2\objdiff-cli.exe diff -p . `
  -u mdpartydll/REL/mdpartydll/stage `
  -o build/GP6E01/src/REL/mdpartydll/stage_objdiff_wave81.json `
  --format json-pretty -c functionRelocDiffs=data_value
```

The final serialized container gate passed:

```powershell
rtk ninja -j1
rtk build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1
rtk cmp.exe orig/GP6E01/sys/main.dol build/GP6E01/main.dol
rtk cmp.exe orig/GP6E01/files/dll/mdpartydll.rel `
  build/GP6E01/mdpartydll/mdpartydll.rel
```

Results:

- full build: `137 files OK`;
- explicit DTK checksum: `137 files OK`;
- `main.dol`: byte-identical, SHA-1
  `b897e6ade6b3a0cd2f9907689f38a3b19c327e70`;
- `mdpartydll.rel`: byte-identical, SHA-1
  `519debb149ef42eda1ab3b0a4d2b3132b4f3e3cc`;
- `config/GP6E01/symbols.txt` SHA-1 before and after the final build:
  `3aaa758be28a24cc27e46fca2ab840f65168e05a`.

The exact REL remains regression proof at this boundary, not a source-link
claim for `mdparty.c` or `stage.c`: both clean-C owners still link their
extracted fallbacks. The authenticated 19-function runtime owner is unchanged
and continues to contribute zero clean-C bytes.
