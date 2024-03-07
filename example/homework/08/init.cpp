#include <Kokkos_Core.hpp>
#include <cstdio>

// Create a program that does matrix multiply between a 2D View and a 1D View with at least one loop of parallelism.
// For a test case:
// a = [130, 137, 115]   b = [221]
//     [224, 158, 187]       [12]
//     [ 54, 211, 120]       [157]
// Extra credit: make a function and check for correct shape/dimensions
void matrixVectorMul(const Kokkos::View<int**>& matrix, const Kokkos::View<int*>& vector, Kokkos::View<int**>& result) {
  auto row = matrix.extent(0);
  auto col = matrix.extent(1);
  
  // Check for correct dimensions
  if(vector.extent(0) != row) {
    std::cerr << "Dimension mismatch!" << std::endl;
    return;
  }

  Kokkos::parallel_for("matrixVectorMultiply", row, KOKKOS_LAMBDA(const int i) {
  int temp = 0;
  for(int j = 0; j < numCols; ++j) {
    temp += matrix(i, j) * vector(j);
  }
  result(i) = temp;
  });
}

int main(int argc, char* argv[]) {
  Kokkos::initialize(argc, argv);
  {
  // Make View and add values
  
    int result = 0;
    Kokkos::View<int**> a("Matrix A", 3, 3);
    Kokkos::View<int*> b("Vector B", 3);
    Kokkos::View<int*> resultA("Result", result);
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

    matrixVectorMul(a, b, result);
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
  }
  Kokkos::finalize();
}
