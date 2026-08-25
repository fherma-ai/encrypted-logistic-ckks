// The logistic function of every element, over CKKS.
//
// The circuit is the LogisticFunction component from fairmath/polycircuit
// (Apache-2.0), vendored verbatim in polycircuit/ — the winning approach of
// the FHERMA Logistic challenge: OpenFHE's Chebyshev series over [-25, 25]
// up to the 59th coefficient, extended by hand-unrolled recursions of the
// odd terms up to T77, all within seven multiplicative levels. This file
// only adapts it to the four-function contract.
#include <utility>
#include <variant>

#include "solve.h"

#include "polycircuit/component/LogisticFunction/LogisticFunction.hpp"

void* solve_init(const fherma::Point& p, CryptoContext<DCRTPoly> cc) {
    return nullptr;
}

std::vector<Plaintext> solve_encoding(CryptoContext<DCRTPoly> cc,
                                      const fherma::Inputs& inp) {
    // The whole vector in one packing, slot i holding element i.
    std::vector<double> xs(inp.xs.data.begin(), inp.xs.data.end());
    return { cc->MakeCKKSPackedPlaintext(xs) };
}

std::vector<Ciphertext<DCRTPoly>> solve_run(
    void* state,
    CryptoContext<DCRTPoly> cc,
    const std::vector<Ciphertext<DCRTPoly>>& cts) {
    polycircuit::LogisticFunction<DCRTPoly> component(cc, cts[0]);
    auto answer = component.evaluate();
    return { std::get<Ciphertext<DCRTPoly>>(std::move(answer)) };
}

fherma::Outputs solve_decoding(const fherma::Point& p,
                               CryptoContext<DCRTPoly> cc,
                               const std::vector<Plaintext>& pts) {
    auto values = pts[0]->GetRealPackedValue();

    fherma::Outputs out;
    out.y.shape = { static_cast<int64_t>(p.N) };
    out.y.data.assign(values.begin(), values.begin() + p.N);
    return out;
}

void solve_free(void* state) {}
