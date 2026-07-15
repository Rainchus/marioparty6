# Native matching wave 61: Branch and M2S owner closure

Wave 61 closes two clean-C DOL owners and recovers sixteen further exact MIC
functions. `board/branch.c` and `dolphin/mic/m2s.c` are promoted only after
strict/data-value object proof and the full DOL/container gate.
`dolphin/mic/mic.c` remains `NonMatching`: five functions and three target text
relocations still diverge, so none of its fallback-linked bytes are counted as
decompiled source.

## Target scope and method

The active target is the byte-identical non-minigame game loop. At this gate:

- DOL `game/` plus `board/` is `696000 / 1431136` Matching `.text` bytes
  (48.63%), with 70/97 owners Matching: all 57 Game owners and 13/40 Board
  owners.
- The conservative fully source-linked required-REL set is `5084 / ~3100000`
  `.text` bytes (approximately 0.16%): `actmanDLL` plus `sequencedll`.
  `bootDll`, `selmenuDll`, and `fileseldll` are excluded from the numerator
  because each still links a fallback runtime owner.
- Minigame, instruction, minigame-mode, and mic-quiz DLLs are excluded.

DTK is pinned at 0.9.2 (`4d039140f2d2ed80572b1949b76a5ff9b3094e06`)
and objdiff-cli at 3.7.3. Target instructions, CFGs, symbols, relocations,
section bytes, and final linked containers are authoritative. `repos/gc-mic`
corroborates the MIC transaction family. `repos/mwcc-decomp` was inspected
read-only; in particular, `StackFrame.c::compress_data_area` proves that an
unused local is removed before final stack assignment, ruling out a fake
padding-local fix for the last MIC async slot differences.

No inline assembly, standalone assembly, byte blobs, dummy locals, fake
storage, or forced instruction encodings are introduced.

## `board/branch.c`: 17/17 exact

The two repeated countdowns now use the authentic compact shape:

```c
if (--choiceTime == 0) {
    btnDown = PAD_BUTTON_A;
}
```

MWCC's loop-weighted variable usage makes this shape assign `choice` and
`choiceTime` to the target registers without dead uses or register forcing.
Restoring the target-proven small-BSS declaration order closes the remaining
global layout.

Final object evidence:

- 17/17 functions exact; `ev_Branch` is `0x8F0/0x8F0` with zero differences;
- `.text 0x10E0`, `.data 0x12`, `.bss 0x10`, `.sbss 0xC`, and
  `.sdata2 0x68` exact;
- all 210 text relocations pair.

The old split ends included a six-byte `.data` alignment tail and a four-byte
`.sbss` tail. Narrowing those ends assigns only semantic bytes to Branch; no
padding object is added.

## `dolphin/mic/m2s.c`: 14/14 exact

`M2SAdvanceBuffer` is recovered by indexing the target-proven source buffer,
testing the debug buffer directly, and forming its indexed destination after
the cached index. It is now `0x13C/0x13C`, 100%, with zero differences.

Moving `__M2SCalibration` after the public functions, with only a forward
declaration above them, restores the target translation-unit order:
public functions occupy `0x0..0x9F8` and calibration begins at `0x9F8` with
size `0x358`.

Final object evidence:

- 14/14 functions and `.text 0xD50` exact;
- all 143 text relocations and `.sbss 0x20` exact;
- target/source `.bss` are `0x260/0x25C` in the raw objects.

The four target-only BSS bytes are DTK's named
`gap_08_802B6214_bss`. Both M2S BSS and the following fallback
`msm/msmsys.o` BSS have eight-byte ELF alignment, so the linker regenerates
that gap before the next owner at `0x802B6218`. Source padding would be false
storage and is deliberately absent.

## `dolphin/mic/mic.c`: 36/41 exact

Target-CFG recovery adds sixteen exact functions:

- public control paths: `MICMountAsync`, `MICMount`, `MICUnmount`,
  `MICSetGainAsync`, `MICSetGain`, `MICStart`, and `MICStop`;
- callbacks/control: `__MICMountCallback`, `__MICTxHandler`,
  `__MICTimeoutCallback`, `__MICGetControlBlock`, and `__MICUpdateButton`;
- raw transactions: `__MICRawReset`, `__MICRawReadStatus`,
  `__MICRawWriteStatus`, and `__MICRawReadDataAsync`.

The recovered owner shape includes a real `u32 [4][4]` timeout table, an
instruction-proven signed timeout field, separate command/data locals for the
status read, split write-command construction, early interrupt restoration,
and explicit failure exits. Final target/source `.text` is `0x2020/0x1FC0` at
98.147860% in both strict and data-value modes. `.data 0x40` and `.bss 0x150`
are exact; source `.sbss 0x4` covers the named value before the target alignment
tail.

Five functions remain:

| Function | Target/source result |
| --- | ---: |
| `MICStartAsync` | `0x118/0x118`, 99.885710% |
| `MICStopAsync` | `0xC4/0xC4`, 99.918365% |
| `__MICDoMount` | `0x178/0x164`, 89.031910% |
| `__MICExiHandler` | `0x1E4/0x1C0`, 87.148760% |
| `__MICUpdateStatus` | `0x25C/0x234`, 91.986755% |

The first two differ only in eight/four references to one stack slot, offset
by four bytes. MWCC's final stack compression rules out a dead scalar, and no
authenticated SDK/sibling source supplies the only compatible alternative—a
live aggregate containing the control-block pointer at member offset four.
They remain unfaked near-matches.

## Rejected audio probe

`board/audio.c::mbMusBoardFadeOut` remains `0x3F0/0x3F0` at 99.952380%, with
12 stack-slot operands and no CFG, size, or relocation differences. A
MWCC-motivated nested allocator assignment regressed it to `0x3EC` and
98.670630%, so it was reverted. The owner remains fallback-linked at 48/49
exact functions.

## Gate

The final integrated gate passes:

- `ninja -j1`: `137 files OK`;
- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`;
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical;
- DTK progress: 9.24% code/31.37% data overall and 47.42% code/74.59% data
  in the DOL, with 303/895 and 294/396 Matching owners respectively;
- `config/GP6E01/symbols.txt` is unchanged after the final build.

