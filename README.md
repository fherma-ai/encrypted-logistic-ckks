# Logistic over CKKS — a Chebyshev series to T₇₇

> **The winning approach of the FHERMA Logistic challenge, by
> [Aikata](https://www.iaik.tugraz.at/person/aikata-aikata/) (TU Graz),
> published as the LogisticFunction component of
> [polycircuit](https://github.com/fairmath/polycircuit) (Apache-2.0)** —
> vendored verbatim in `solution/polycircuit/`, adapted to the FHERMA kernel
> contract. Answers
> [`logistic` / `f64@1.0.0`](https://fherma.io/kernels/logistic) on the
> FHERMA kernel catalogue.

σ(x) = 1/(1+e⁻ˣ) for every element of an encrypted vector, x ∈ [−25, 25].
OpenFHE's built-in Chebyshev series carries the approximation to the 59th
coefficient; the odd terms beyond it are unrolled by hand with the recursion
T_m = 2·T_i·T_j − T_k up to T₇₇ — seven multiplicative levels, no rotation
keys, and 99.9988% of the validation vector within 10⁻³.

```text
kernel logistic<N: uint>(
    %xs: secret<tensor<N x f64>>,
) -> %y: secret<tensor<N x f64>>
```

## Layout

| | |
| --- | --- |
| [`solution/`](solution/) | the measured project: `solve.cpp` (the adaptation), `polycircuit/` (the vendored component), `config.jsonc` |
| [`solution/README.md`](solution/README.md) | the contract: what is measured, what is reviewed, how to run |

## Running it

```sh
cd solution
docker run --rm -v "$PWD":/solution -w /solution fherma/openfhe:1.5.1 \
    sh -c "cmake -B build && cmake --build build -j && ./build/solution <point-dir>"
```

## License

Apache-2.0, as published in polycircuit.
