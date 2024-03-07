#include <Kokkos_Core.hpp>
#include <cstdio>

// Declare a 5 ∗ 7 ∗ 12 ∗ n View

int main(int argc, char* argv[]) {
  Kokkos::initialize(argc, argv);
  {
  // Make View
  int n = 18;

  Kokkos::View<int****> 4DView("My4DView", 5, 7, 12, n);

  // Print name of the View
  for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 7; ++j) {
            for (int k = 0; k < 12; ++k) {
                for (int l = 0; l < n; ++l) {
                    4DView(i, j, k, l) = 1000.0 * i * j * k * l;
                }
            }
        }
    }

  }
  for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 7; ++j) {
            for (int k = 0; k < 12; ++k) {
                for (int l = 0; l < n; ++l) {
                    printf("view(%d, %d, %d, %d) = %f\n", i, j, k, l, 4DView(i, j, k, l));
                }
            }
        }
    }
  Kokkos::finalize();
}
