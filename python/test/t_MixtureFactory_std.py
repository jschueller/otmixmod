
from __future__ import print_function
from openturns import *
from otmixmod import *

dim = 3
size = 10000
R = CorrelationMatrix(dim)
# Create a collection of distribution
coll = DistributionCollection(0)

for i in range(1, dim):
    R[i, i - 1] = 0.3

distribution1 = Normal(NumericalPoint(dim, -2.0), NumericalPoint(dim, 1.2), R)
coll.add(Distribution(distribution1))
for i in range(1, dim):
    R[i, i - 1] = -0.5

distribution2 = Normal(NumericalPoint(dim, 2.0), NumericalPoint(dim, 0.8), R)
coll.add(Distribution(distribution2))
distribution3 = Normal(NumericalPoint(dim, -5.0), NumericalPoint(dim, 1.4), R)
coll.add(Distribution(distribution3))
distributionRef = Mixture(coll)
sample = NumericalSample(distributionRef.getSample(size))

#for covarianceModel in [Gaussian_pk_L_B(), Gaussian_pk_L_Bk(), Gaussian_pk_L_C(), Gaussian_pk_L_Ck(), Gaussian_pk_L_D_Ak_D(), Gaussian_pk_L_Dk_A_Dk(), Gaussian_pk_L_I(), Gaussian_pk_Lk_B(), Gaussian_pk_Lk_Bk(), Gaussian_pk_Lk_C(), Gaussian_pk_Lk_Ck(), Gaussian_pk_Lk_D_Ak_D(), Gaussian_pk_Lk_Dk_A_Dk(), Gaussian_pk_Lk_I(), Gaussian_p_L_B(), Gaussian_p_L_Bk(), Gaussian_p_L_C(), Gaussian_p_L_Ck(), Gaussian_p_L_D_Ak_D(), Gaussian_p_L_Dk_A_Dk(), Gaussian_p_L_I(), Gaussian_p_Lk_B(), Gaussian_p_Lk_Bk(), Gaussian_p_Lk_C(), Gaussian_p_Lk_Ck(), Gaussian_p_Lk_D_Ak_D(), Gaussian_p_Lk_Dk_A_Dk(), Gaussian_p_Lk_I()]:
for covarianceModel in [Gaussian_pk_L_B(), Gaussian_pk_L_Bk()]:
    factory = MixtureFactory(3, covarianceModel)
    estimatedDistribution, labels, BICLogLikelihood = factory.build(sample)
    print(covarianceModel.getClassName(), "PDF=%.6f" % estimatedDistribution.computePDF(NumericalPoint(dim, 1.7)))
