# The logistic function of every element, elementwise — an encrypted answer

Answers `logistic/logistic@1.0.0` over ciphertexts, with openfhe (CKKS).

Yours to write: **solve.cpp** — four functions. Everything else is generated
from the signature and the config, and `--update` rewrites it without
touching yours.

| | |
| --- | --- |
| secret arguments | xs — arrive in `run` as ciphertexts |
| public arguments | none — arrive as plain values |
| measured | the `run` call alone; encoding, encryption, decryption, decoding are outside |
| keys | generated and held by the envelope; your functions never see them |
| parameters | `config.jsonc`: every parameter of every scheme, each commented |

The security level in `config.jsonc` is enforced by the library itself:
parameters that do not reach it are refused before anything is measured.
