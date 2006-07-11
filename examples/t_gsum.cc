// $Id: t_gsum.cc,v 1.1 2006-07-11 13:13:22 bjoo Exp $

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ios>

#include "qdp.h"

namespace QDP {
extern   void local_sumsq2(REAL64 *Out, REAL32 *In, int n_3vec);
};


using namespace std;
using namespace QDP;

int main(int argc, char *argv[])
{
  // Put the machine into a known state
  QDP_initialize(&argc, &argv);

  // Setup the layout
  const int foo[] = {28,28,28,64};
  multi1d<int> nrow(Nd);
  nrow = foo;  // Use only Nd elements
  Layout::setLattSize(nrow);
  Layout::create();

  LatticeFermion x; // Single precision LatticeFermion
  gaussian(x);      // Fill it with noise.
  Real xr=10.5;

  QDPIO::cout << setw(15) << xr << endl;

  LatticeFermionD x_dble = zero;  // Cast it to double

  // Convert x to doubles
  for(int i=all.start(); i <= all.end(); i++) {
    for(int spin=0; spin < Ns; spin++) { 
      for(int color=0; color < Nc; color++) { 
	x_dble.elem(i).elem(spin).elem(color).real()
	  =(REAL64) x.elem(i).elem(spin).elem(color).real();
	x_dble.elem(i).elem(spin).elem(color).imag()
	  =(REAL64) x.elem(i).elem(spin).elem(color).imag();
      }
    }
  }

  REAL64 double_local_sum = 0;
  for(int i=all.start(); i <= all.end(); i++) {
    for(int spin=0; spin < Ns; spin++) { 
      for(int color=0; color < Nc; color++) { 
	double_local_sum += x_dble.elem(i).elem(spin).elem(color).real()
	  * x_dble.elem(i).elem(spin).elem(color).real();

	double_local_sum += x_dble.elem(i).elem(spin).elem(color).imag()
	  * x_dble.elem(i).elem(spin).elem(color).imag();

      }
    }
  }

  REAL64 double_global_sum = double_local_sum;
  Internal::globalSum(double_global_sum);

  Double qdp_double_norm2 = norm2(x_dble);

  Double double_norm_diff = (qdp_double_norm2 - double_global_sum)/(double_global_sum);

  QDPIO::cout << "(QDP DP Norm2 - Handrolled DP Norm2 ) / Handrolled DP Norm2  = " << double_norm_diff << endl;

  // ----------------------------------------------------------------------//
  // Now All Double Precision sum of Single Precision vector               //
  // ----------------------------------------------------------------------//
  REAL64 local_norm2_all_in_double = 0;
  for(int i=all.start(); i <= all.end(); i++) {
    for(int spin=0; spin < Ns; spin++) { 
      for(int color=0; color < Nc; color++) { 
	local_norm2_all_in_double += (((REAL64)x.elem(i).elem(spin).elem(color).real())
	  * ((REAL64)x.elem(i).elem(spin).elem(color).real()));

	local_norm2_all_in_double += (((REAL64)x.elem(i).elem(spin).elem(color).imag())
	  * ((REAL64)x.elem(i).elem(spin).elem(color).imag()));

      }
    }
  }

  REAL64 global_norm2_all_in_double = local_norm2_all_in_double;
  Internal::globalSum(global_norm2_all_in_double);
  
  Double double_norm_diff2 = (global_norm2_all_in_double - double_global_sum) / ( double_global_sum );
  QDPIO::cout << "( Norm2 All in Double - DP Norm 2 ) /  DP Norm 2  = " << double_norm_diff2 << endl;

  // ----------------------------------------------------------------------//
  // Now All Single Precision sum of Single Precision vector               //
  // ----------------------------------------------------------------------//
  REAL32 local_norm2_all_in_single = 0;
  for(int i=all.start(); i <= all.end(); i++) {
    for(int spin=0; spin < Ns; spin++) { 
      for(int color=0; color < Nc; color++) { 
	local_norm2_all_in_single += (x.elem(i).elem(spin).elem(color).real()
	  * x.elem(i).elem(spin).elem(color).real());

	local_norm2_all_in_single += (x.elem(i).elem(spin).elem(color).imag()
	  * x.elem(i).elem(spin).elem(color).imag());

      }
    }
  }

  // Upcast to double for the global sum
  REAL64 global_norm2_in_double = (REAL64)local_norm2_all_in_single;
  Internal::globalSum(global_norm2_in_double);
  
  Double double_norm_diff3(( global_norm2_in_double - double_global_sum )/ ( double_global_sum ));
  QDPIO::cout << "( SinglePrec On Node DP Accross Nodes - DP Norm 2 ) /  DP Norm 2  = " << double_norm_diff3 << endl;

  REAL64 local_norm2_site_result;
  int nvec = (all.end() - all.start() + 1)*24;
  local_sumsq(&local_norm2_site_result, &x.elem(0).elem(0).elem(0).real(), nvec);
  
  REAL64 local_norm2_site_global_result = local_norm2_site_result;
  Internal::globalSum(local_norm2_site_global_result);
  Double double_norm_diff4(  (local_norm2_site_global_result - double_global_sum )/ ( double_global_sum ) );
  QDPIO::cout << "( local_norm2 Single SSE on Site, DP Accross Nodes - DP Norm 2 ) / ( DP Norm 2 ) = " << double_norm_diff4 << endl;

  REAL64 local_norm2_result;
  local_sumsq2(&local_norm2_result, &x.elem(0).elem(0).elem(0).real(), nvec);
  REAL64 local_norm2_2_global_result  = local_norm2_result;
  Internal::globalSum(local_norm2_2_global_result);
  Double double_norm_diff5(  (local_norm2_2_global_result - double_global_sum )/ ( double_global_sum ) );
  QDPIO::cout << "( local_norm2 Single on Node DP Accross Nodes - DP Norm 2 ) / ( DP Norm 2 ) = " << double_norm_diff5 << endl;

  // Time to bolt
  QDP_finalize();

  exit(0);
}
  
