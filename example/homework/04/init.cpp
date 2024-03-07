#include <Kokkos_Core.hpp>
#include <cstdio>

// Do simple parallel reduce to output the maximum element in a View

int main(int argc, char* argv[]) {
  Kokkos::initialize(argc, argv);
  {
  // Make View and create values
  int n = 16;
  Kokkos::View<int*> myView("My1DView", n);
  // Do a parallel reduction
  Kokkos::parallel_for("InitializeView", n, KOKKOS_LAMBDA(const int i) {
    myView(i) = i; 
  });

  int maxVal = 0;

  Kokkos::parallel_reduce("MaxReduction", n, KOKKOS_LAMBDA(const int i, int& localMax) {
    if (myView(i) > localMax) localMax = myView(i);
  }, Kokkos::Max<int>(maxVal));
  
  printf("Maximum value in the view: %d\n", maxVal);

  }
  Kokkos::finalize();
}
