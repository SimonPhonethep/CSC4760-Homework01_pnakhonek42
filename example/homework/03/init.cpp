#include <Kokkos_Core.hpp>
#include <cstdio>

// Declare a 5 ∗ 7 ∗ 12 ∗ n View

int main(int argc, char* argv[]) {
  Kokkos::initialize(argc, argv);
  {
  // Make View
        int n = 16;
        Kokkos::View<int****> view("My4DView", 5, 7, 12, n);
  // Print name of the View
        Kokkos::parallel_for("initialize_view", Kokkos::MDRangePolicy<Kokkos::Rank<4>>({0, 0, 0, 0}, {5, 7, 12, n}), KOKKOS_LAMBDA(const int i, const int j, 
const int k, const int l) {
            view(i, j, k, l) = 1000 * i * j * k * l;
            });
        auto view_h = Kokkos::create_mirror_view(view);
        Kokkos::deep_copy(view_h, view);
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 7; ++j) {
                for (int k = 0; k < 12; ++k) {
                    for (int l = 0; l < n; ++l) {
                        // Correct format specifier for integer values
                        printf("view(%d, %d, %d, %d) = %d\n", i, j, k, l, view_h(i, j, k, l));
                    }
                }
            }
        }
   }
  Kokkos::finalize();
}
