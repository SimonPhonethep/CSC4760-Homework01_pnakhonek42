#include <Kokkos_Core.hpp>
#include <cstdio>

// Create a program that does matrix addition between a 2D View and a 1D View with at least one loop of parallelism.
// For a test case:
// a = [130, 137, 115]   b = [221]
//     [224, 158, 187]       [12]
//     [ 54, 211, 120]       [157]
// Extra credit: make a function and check for correct shape/dimensions
void matrixVectorAdd(const Kokkos::View<int**>& matrix, const Kokkos::View<int*>& vector, Kokkos::View<int**>& result) {
  auto row = matrix.extent(0);
  auto col = matrix.extent(1);
  
  // Check for correct dimensions
  if(vector.extent(0) != row) {
    std::cerr << "Dimension mismatch!" << std::endl;
    return;
  }
  Kokkos::parallel_for("matrixVectorAdd", row, KOKKOS_LAMBDA(const int i) {
    for(int j = 0; j < col; ++j) {
      result(i, j) = matrix(i, j) + vector(i);
    }
  });
}

int main(int argc, char* argv[]) {
  Kokkos::initialize(argc, argv);
  {
  // Make View and add values
    Kokkos::View<int**> a("Matrix A", 3, 3);
    Kokkos::View<int*> b("Vector B", 3);
    Kokkos::View<int**> result("Result", 3, 3);

    auto A = Kokkos::create_mirror_view(a);
    auto B = Kokkos::create_mirror_view(b);
  // Do a matrix add
    A(0,0) = 130; A(0,1) = 137; A(0,2) = 115;
    A(1,0) = 224; A(1,1) = 158; A(1,2) = 187;
    A(2,0) = 54;  A(2,1) = 211; A(2,2) = 120;
    
    B(0) = 221; B(1) = 12; B(2) = 157;
    Kokkos::deep_copy(a, A);
    Kokkos::deep_copy(b, B)

    matrixVectorAdd(a, b, result);
  // Output addition 
    auto resultTemp = Kokkos::create_mirror_view(result);
    Kokkos::deep_copy(resultTemp, result);
    for(int i = 0; i < 3; ++i) {
      for(int j = 0; j < 3; ++j) {
        printf("%d ", resultTemp(i, j));
      }
      printf("\n");
    }
  }
  Kokkos::finalize();
}
