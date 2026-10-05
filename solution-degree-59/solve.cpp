// The logistic function of every element, over CKKS — at the degree the
// specification asks for, and no further.
//
// The challenge-winning component beside this one takes OpenFHE's Chebyshev
// series to the 59th coefficient and then adds the odd terms up to T77 by
// hand, because the challenge graded on a tighter target than this
// specification sets. This specification holds every element within 1e-3, and
// a plain Chebyshev projection of the logistic function at degree 59 reaches
// 3.4e-4 of it — inside the bar by a factor of three.
//
// So the terms above 59 are accuracy nobody asked for, and they are not free:
// degree 59 fits in six multiplicative levels where degree 77 needs seven.
// The point of this answer is to put OpenFHE and the other libraries on the
// same polynomial, so what the board compares is the library.
//
// The coefficients are the projection itself, computed from 1/(1+e^-x) on
// [-25, 25] at Chebyshev nodes. Nothing here is a fit.
#include <vector>

#include "solve.h"

namespace {

// Chebyshev coefficients of the logistic function on [-25, 25], degrees 0..59.
const std::vector<double> SERIES = {
        0.5, 0.6349347497494253, -3.469446951953614e-18,
        -0.20722691098412338, -6.938893903907228e-18, 0.11926554321258699,
        -4.85722573273506e-17, -0.08013715051153233, 1.734723475976807e-18,
        0.057579921670292085, 1.734723475976807e-18, -0.042809107375279334,
        -6.938893903907228e-18, 0.03243169763501984, 5.204170427930421e-18,
        -0.024837099973789068, 1.734723475976807e-18, 0.019142669166591894,
        -6.938893903907228e-18, -0.01481027127413813, 1.734723475976807e-17,
        0.011484877042530412, 3.469446951953614e-17, -0.008918658563008962,
        -3.469446951953614e-18, 0.006931784041012641, -6.071532165918825e-18,
        -0.00539036717502759, 1.734723475976807e-17, 0.004193062442934031,
        1.0408340855860843e-17, -0.0032623458709415513, -1.8214596497756474e-17,
        0.0025385240979381232, -7.806255641895632e-18, -0.0019754447984733896,
        0.0, 0.0015373344704775176, 1.474514954580286e-17,
        -0.001196421025293097, 7.37257477290143e-18, 0.0009311233317454148,
        -2.168404344971009e-17, -0.0007246611732315529, -2.2551405187698492e-17,
        0.0005639824772489914, -6.5052130349130266e-18, -0.00043893273508707146,
        2.6020852139652106e-18, 0.00034161065082977376, 7.806255641895632e-18,
        -0.00026586764752697944, 8.673617379884035e-19, 0.00020691881742227781,
        -7.37257477290143e-18, -0.00016104035950282886, 1.3227266504323154e-17,
        0.00012533421004378396, -1.5178830414797062e-18, -9.754491146087875e-05,
};

}  // namespace

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
    // The series is stated on [-25, 25], so OpenFHE scales the argument itself.
    return { cc->EvalChebyshevSeries(cts[0], SERIES, -25.0, 25.0) };
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
