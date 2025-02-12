#include <Kokkos_Core.hpp>
#include <cstdio>

// Create a program that does matrix multiply between a 2D View and a 1D View with at least one loop of parallelism.
// For a test case:
// a = [130, 137, 115]   b = [221]
//     [224, 158, 187]       [12]
//     [ 54, 211, 120]       [157]
// Extra credit: make a function and check for correct shape/dimensions
void matrixVectorMul(const Kokkos::View<int**>& matrix, const Kokkos::View<int*>& vector, Kokkos::View<int*>& result) {
  auto rows = matrix.extent(0);
  auto cols = matrix.extent(1);

  // Check for correct dimensions
  if(vector.extent(0) != cols) {
      printf("Dimension mismatch: Matrix columns must match Vector length!\n");
      return;
  }

  Kokkos::parallel_for("matrixVectorMultiply", rows, KOKKOS_LAMBDA(const int i) {
      int temp = 0;
      for (int j = 0; j < cols; ++j) {
          temp += matrix(i, j) * vector(j);
      }
      result(i) = temp;
  });
}

int main(int argc, char* argv[]) {
  Kokkos::initialize(argc, argv);
  {
  // Make View and add values
  

    Kokkos::View<int**> a("Matrix A", 3, 3);
    Kokkos::View<int*> b("Vector B", 3);
    Kokkos::View<int*> resultA("Result", 3);
  // Do a matrix multiply
    auto A = Kokkos::create_mirror_view(a);
    auto B = Kokkos::create_mirror_view(b);
  // Do a matrix add
    A(0,0) = 130; A(0,1) = 137; A(0,2) = 115;
    A(1,0) = 224; A(1,1) = 158; A(1,2) = 187;
    A(2,0) = 54;  A(2,1) = 211; A(2,2) = 120;
    
    B(0) = 221; B(1) = 12; B(2) = 157;
    Kokkos::deep_copy(a, A);
    Kokkos::deep_copy(b, B);
  // Output addition 

    matrixVectorMul(a, b, resultA);
  // Output addition 
    auto resultTemp = Kokkos::create_mirror_view(resultA);
    Kokkos::deep_copy(resultTemp, resultA);
    for (int i = 0; i < 3; ++i) {
        printf("%d\n", resultTemp(i)); 
    }
  
  }
  Kokkos::finalize();
}
