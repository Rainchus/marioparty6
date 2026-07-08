# MP6 Exact C Recovery Import

This directory carries the source-level exact-C recovery rows exported from `_mp6_rebuild` for review in the `marioparty6` DTK repository.

- `manifest.csv` lists all 2699 source-level exact-C ledger rows.
- `summary.json` contains aggregate counts and missing-source examples.
- `source/` contains generated clean-C proof sources that still existed locally at export time.

Import status meanings:

- `already_in_repo`: the proven source is already a normal DTK source file in this repository.
- `copied_source`: the generated proof source was copied into this bundle.
- `missing_source`: the exact ledger row is included, but the historical source artifact was not present locally.

The copied generated proof sources are intentionally not added to `configure.py`: many were proven with GCC-specific attributes or register constraints, while this project links active DTK objects with MWCC. Keeping them in this bundle preserves the recovery work without breaking the current build format.
