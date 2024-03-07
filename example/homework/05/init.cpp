#include <Kokkos_Core.hpp>
#include <cstdio>

// Create a program that compares a parallel for loop and a standard for loop for summing rows of a View with Kokkos Timer.

int main(int argc, char* argv[]) {
  Kokkos::initialize(argc, argv);
  {
  // Make View and create values
  int row = 10000;
  int col = 1000;
  
  Kokkos::View<double**> mView("matrix view", row, col);
  // sum loops 
  Kokkos::parallel_for("initialize matrix view", Kokkos::MDRangePolicy<Kokkos::Rank<2>>({0, 0}, {numRows, numCols}), KOKKOS_LAMBDA(const int i, const int j) {
    mView(i, j) = 1.0; 
  });
  Kokkos::Timer timer;
  double standardSum = 0.0;
  for(int i = 0; i < row; ++i) {
    for(int j = 0; j < col; ++j) {
        standardSum += mView(i, j);
      }
  }
  double standardTime = timer.seconds();

  timer.reset()

  Kokkos::View<double*> rowSums("row sums", row);
  Kokkos::parallel_for("sum rows", numRows, KOKKOS_LAMBDA(const int i) {
    double sum = 0.0;
    for(int j = 0; j < col; ++j) {
      sum += matrix(i, j);
    }
    rowSums(i) = sum;
  });
  Kokkos::fence();

  double parallelSum = Kokkos::reduce_sum("reduce row sums", row, KOKKOS_LAMBDA(const int i, double& lsum) {
    lsum += rowSums(i);
  });
  double parallelTime = timer.seconds();
  
  // Output times
  printf("Standard for loop time: %f seconds, Sum: %f\n", standardTime, standardSum);
  printf("Parallel for loop time: %f seconds, Sum: %f\n", parallelTime, parallelSum);


  }
  Kokkos::finalize();
}
