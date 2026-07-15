# Native matching wave 62: Masu engine-core recovery

Wave 62 advances one DOL owner, `board/masu.c`, from 103/119 to 115/119
strict-exact functions. The owner remains `NonMatching`; four functions still
diverge, so none of its fallback-linked bytes are counted as decompiled source
and no full-DOL gate is claimed for this WIP slice.

## Method

The target object, symbols, relocations, section bytes, and objdiff instruction
alignment are authoritative. Matching Mario Party 5 `game/board/masu.c`
authenticates the shared matrix declaration order and atlas calculation.
`repos/mwcc-decomp` `StackFrame.c`, `Coloring.c`, and `CInline.c` were read
before changing local lifetimes or condition shapes. No dummy locals, padding
objects, register forcing, byte blobs, or assembly were introduced.

## Exact closures

Twelve functions become strict-exact:

- `mbMasuDataRead`;
- `MasuDisplayListMake` and `MasuDisplayListKaoMake`;
- `MasuNextMain`, `MasuNextDraw`, and `mbMasuNextSet`;
- `mbev_MasuMove`, `mbev_MasuCapStop`, and `mbev_MasuStop`;
- `mbev_MasuMasuStart` and `mbev_MasuMasuEnd`;
- `mbMasuCornerPosGet`.

Five of those were also the remaining data-value residuals, raising that gate
from 110/119 to 115/119. The other seven had already matched under relocation
value comparison; suppressing the unrelated weak math constants restores the
target `.sdata2` owner and closes them under the strict gate.

The recovered source shape includes:

- the target `starNum` lifetime in the data reader;
- `GX_LIGHT_NULL`, the MP5-authenticated shared matrix order, and the original
  no-cast `(patNo % 4) / 4.0f` atlas expression;
- target switch order and party snapshots in all three space-event paths;
- target-proven `int` return ownership for `mbMasuCapsuleGet` and typed capsule
  event declarations;
- the target `.data` definition order around the `mbev_MasuStop` jump table.

## Object evidence

The final serialized object compile and both objdiff modes report:

- 115/119 functions exact in strict and
  `functionRelocDiffs=data_value` comparison;
- target/source `.text` `0x5DC8/0x5DB0`, 99.614460%;
- `.rodata 0x40`, `.data 0x128`, `.bss 0x700`, `.sdata 0x10`, and
  `.sdata2 0x90` exact;
- semantic `.sbss` exact, with target/source raw sizes `0x78/0x74` because the
  target retains its four-byte alignment tail.

The four remaining functions are:

| Function | Target/source | Score |
| --- | ---: | ---: |
| `MasuDraw` | `0xB54/0xB54` | 99.484140% |
| `MasuFind` | `0x62C/0x62C` | 99.154430% |
| `mbMasuPlayerColorSet` | `0x260/0x260` | 99.868420% |
| `mbMasuPKinokoValueGet` | `0x7EC/0x7D4` | 96.871796% |

`MasuDraw` now has target-identical size and frame after using the unary inline
team getter; PlayerColor differs only in two compiler-temporary register
colors. PKinoko still lacks two target compiler spills. None was imitated with
fake storage. Because these residuals remain, `configure.py` is unchanged and
the owner is not promoted.
