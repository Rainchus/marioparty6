# Native matching wave 67: Mode-select REL lifecycle

Wave 67 opens the Priority 1a `mdseldll.rel` owner and recovers the complete
entry, transition, window-message, and initial mode-flow spine as ordinary C.
Sixteen target functions are represented and 15 are byte-exact under the
required `functionRelocDiffs=data_value` comparison. The exact functions
account for `0x1660` target text bytes. Target/source `.text` is
`0x10010/0x17BC` at 9.266537% while the application owner remains
`NonMatching`; none of its fallback-linked bytes are counted as decompiled
source.

## Ownership and compiler evidence

The MP6 target REL, DTK 0.9.2 split object, relocations, retained data, and
same-game consumers are authoritative. `include/ovl_table.h` establishes
`mdseldll` as overlay 93, and the Matching `selmenuDll` source routes the
mode-select entry directly to that overlay. Mario Party 5 commit
`e246f9d9850ff53ac684b971068fbf87fdcf6acb` supplies the authenticated
`mdsel` framework and module-entry source family; MP6 instructions and data
establish every retained MP6-specific call, overlay, state value, message,
camera coordinate, and transition constant.

The first proven split separates application text at `0x00000..0x10010` from
the compiler runtime at `0x10010..0x10A54`, with runtime rodata at
`0x260..0x278`. The runtime code and rodata are byte-identical to the
same-game `fileseldll` and `selmenuDll` runtime blocks. Its authentic bodies
are assembly in the sibling Runtime source, so this remains an
`original-was-asm` fallback owner and is not clean-C recovery. The application
owner is `C-not-yet-matched` work.

The application object uses the canonical REL compiler recipe: GC/1.3.2,
`cflags_rel`, `-inline auto`, `-O0,p`, unsigned `char`, disabled FP
contraction, and zero small-data areas. The MP6 object proves the important
definition-order shape:

- data and BSS declarations are visible as externals while the functions are
  compiled, preventing false address-base folding; their recovered definitions
  are emitted later in the owner;
- `fn_1_FEC` is inlined into `fn_1_B304`;
- `fn_1_2080` is recursively inlined through `fn_1_23E0`;
- `fn_1_23E0`, `fn_1_2288`, and `fn_1_21DC` are inlined into `fn_1_B414`;
- `fn_1_B304` and `fn_1_B414` are inlined into `fn_1_E7B0`;
- the report helper and overlay dispatcher are inlined into `fn_1_A310`.

The register/slot decisions were checked against the local MWCC source oracle,
including `CInline.c`, `Coloring.c`, and `StackFrame.c`. No inline assembly,
opaque byte body, artificial padding, or forced-register construct was added.

## Exact closures

These 15 functions are byte-exact:

- module and setup: `_prolog` (`0x54`), `_epilog` (`0x4C`),
  `fn_1_B0C8` (`0x20`), and the sibling-authenticated `ObjectSetup`
  (`0x17C`);
- transition/report flow: `fn_1_1B4` (`0x118`), `fn_1_2CC` (`0xB0`),
  `fn_1_37C` (`0x90`), and `fn_1_A310` (`0x2C4`);
- camera target and opening flow: `fn_1_FEC` (`0x1E8`) and
  `fn_1_B304` (`0x110`);
- window-message flow: `fn_1_21DC` (`0xAC`), `fn_1_2288` (`0x60`), and
  `fn_1_23E0` (`0x218`);
- mode initialization/orchestration: `fn_1_B414` (`0x3F0`) and
  `fn_1_E7B0` (`0x4FC`).

`fn_1_2080` is fully represented at the exact `0x15C` size and 99.540230%.
Its remaining differences are confined to the two nonvolatile registers used
for the old and new window IDs. The source order that makes its nested copy in
`fn_1_23E0` exact is retained; no register forcing is accepted.

## Recovered data and globals

The source restores the target-backed six-entry camera offset bank
`lbl_1_data_28`, the six-entry mode-grid permutation `lbl_1_data_70`, the
window/message cache values at `lbl_1_data_7C`, `lbl_1_data_150`, and
`lbl_1_data_154`, all report strings used by the recovered functions, the
four-vector camera current/target bank at `lbl_1_bss_19AC`, the stream handle,
the three-`s16` selected-mode/grid state, and the four-window ID bank.

All nine recovered string/vector/table objects pair byte-for-byte in the
object comparison. Source `.data` is `0x14A` against target `0x370`, source
`.bss` is `0x4C` against target `0x1A4E`, and source `.rodata` is `0x2C`
against target `0x260`; the remaining ranges belong to unrecovered menu
objects and effects, not padding.

## Validation

The final object comparison is:

```text
ninja build/GP6E01/src/REL/mdseldll/mdsel.o
objdiff-cli diff -p . -u mdseldll/REL/mdseldll/mdsel \
  -c functionRelocDiffs=data_value
```

It reports 16 paired functions, 15 exact, target/source `.text
0x10010/0x17BC` at 9.266537%, `.rodata 0x260/0x2C` at 13.496932%, `.data
0x370/0x14A` at 54.380165%, and `.bss 0x1A4E/0x4C` at 4.347826%.

Because this wave adds REL split/config plumbing, a serialized full `ninja`
was also run after the final source change. It completed and the manifest check
reported `137 files OK`, leaving the fallback-linked `main.dol` and
`mdseldll.rel` exact. No object was flipped to `Matching`, so no clean-C or
assembly-exception promotion is claimed.
