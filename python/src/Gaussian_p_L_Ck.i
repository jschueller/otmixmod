// SWIG file Gaussian_p_L_Ck.i

%{
#include "Gaussian_p_L_Ck.hxx"
%}



%include Gaussian_p_L_Ck.hxx
namespace OTMIXMOD { %extend Gaussian_p_L_Ck { Gaussian_p_L_Ck(const Gaussian_p_L_Ck & other) { return new OTMIXMOD::Gaussian_p_L_Ck(other); } }}
