// SWIG file Gaussian_p_L_I.i

%{
#include "Gaussian_p_L_I.hxx"
%}



%include Gaussian_p_L_I.hxx
namespace OTMIXMOD { %extend Gaussian_p_L_I { Gaussian_p_L_I(const Gaussian_p_L_I & other) { return new OTMIXMOD::Gaussian_p_L_I(other); } }}
