// SWIG file Gaussian_p_Lk_Bk.i

%{
#include "Gaussian_p_Lk_Bk.hxx"
%}



%include Gaussian_p_Lk_Bk.hxx
namespace OTMIXMOD { %extend Gaussian_p_Lk_Bk { Gaussian_p_Lk_Bk(const Gaussian_p_Lk_Bk & other) { return new OTMIXMOD::Gaussian_p_Lk_Bk(other); } }}
