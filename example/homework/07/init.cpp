#include <Kokkos_Core.hpp>
#include <cstdio>


int main(int argc, char* argv[]) {
  Kokkos::initialize(argc, argv);
  {
    int n = 16;
    Kokkos::View<double*> view("view", n);
    Kokkos::parallel_for("InitView", Kokkos::RangePolicy<Kokkos::DefaultExecutionSpace>(0, n), KOKKOS_LAMBDA(const int i) {
      view(i) = i * 0.1; 
    });

    Kokkos::View<double*> prefixSum("prefix sum", n);

    Kokkos::Timer timer;
    Kokkos::parallel_scan("Prefix Sum", Kokkos::RangePolicy<Kokkos::DefaultExecutionSpace>(0, n), KOKKOS_LAMBDA (const int i, double& update, const bool final) {
      update += view(i);
      if (final) {
          prefixSum(i) = update;
      }
    });
    double time = timer.seconds();

    printf("Prefix sum:\n");
    auto h_prefixSum = Kokkos::create_mirror_view(prefixSum);
    Kokkos::deep_copy(h_prefixSum, prefixSum);
    for (int i = 0; i < n; ++i) {
        printf("%f\n", h_prefixSum(i));
    }
    printf("Time: %f seconds\n", time);


  }
  Kokkos::finalize();
}
