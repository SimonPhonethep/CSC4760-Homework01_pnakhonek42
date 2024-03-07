#include <Kokkos_Core.hpp>
#include <cstdio>

// Problem: Make an n ∗ m View where each index equals 1000 ∗ i ∗ j

int main(int argc, char* argv[]) {
  Kokkos::initialize(argc, argv);
  {
  // set n and m, you can change these values
    int n,m = 16;
  // Make View
    Kokkos::View<int**> myView("My2DView", n, m);
    Kokkos::deep_copy(myView, 0);
  // set values to 1000 * i * j;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            myView(i, j) = 1000.0 * i * j;
        }
    }
  }
  Kokkos::finalize();
}
