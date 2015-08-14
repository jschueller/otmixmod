//                                               -*- C++ -*-
/**
 *  @file  t_MixtureFactory_std.cxx
 *  @brief The test file of class MixtureFactory
 *
 *  (C) Copyright 2005-20011 EADS
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License.
 *
 *  This library is distributed in the hope that it will be useful
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */
#include <cmath>
#include "OT.hxx"
#include "OTtestcode.hxx"
#include "MixtureFactory.hxx"
#include "Normal.hxx"
#include "NumericalPoint.hxx"
#include "NumericalSample.hxx"
#include "Collection.hxx"
#include "Distribution.hxx"
#include "PlatformInfo.hxx"
#include "Indices.hxx"

using namespace OTMIXMOD;

int main (int argc, char *argv[])
{
  TESTPREAMBLE;
  OT::OStream fullprint(std::cout);

  try
    {
      OT::UnsignedInteger dim(2);
      OT::UnsignedInteger size(1000);
      OT::CorrelationMatrix R(dim);
      // Create a collection of distribution
      OT::Mixture::DistributionCollection coll;

      for (OT::UnsignedInteger i = 1; i < dim; i++)
        {
          R(i, i - 1) = 0.1;
        }
      OT::Normal distribution1(OT::NumericalPoint(dim, -2.0), OT::NumericalPoint(dim, 1.2), R);
      coll.add(distribution1);
      for (OT::UnsignedInteger i = 1; i < dim; i++)
        {
          R(i, i - 1) = -0.2;
        }
      OT::Normal distribution2(OT::NumericalPoint(dim, 2.0), OT::NumericalPoint(dim, 0.8), R);
      coll.add(distribution2);
      OT::Normal distribution3(OT::NumericalPoint(dim, -5.0), OT::NumericalPoint(dim, 1.4), R);
      coll.add(distribution3);

      OT::Mixture distributionRef(coll);

      OT::NumericalSample sample(distributionRef.getSample(size));
      OT::NumericalScalar maxLL(0.0);
      OT::UnsignedInteger bestNb(0);
      for (OT::UnsignedInteger nb = 1; nb < 6; ++nb)
        {
          MixtureFactory factory(nb);
          OT::Indices labels(0);
          OT::NumericalPoint logLikelihood(0);
          OT::Distribution estimatedDistribution(factory.build(sample, labels, logLikelihood));
          if (nb == 1)
            {
              maxLL = logLikelihood[1];
              bestNb = nb;
            }
          if (logLikelihood[1] > maxLL)
            {
              maxLL = logLikelihood[1];
              bestNb = nb;
            }
        }
      fullprint << "nb clusters=" << bestNb << std::endl;

      MixtureFactory factory(bestNb);
      OT::Distribution estimatedDistribution(factory.build(sample));
      OT::NumericalPoint point(distributionRef.getMean());
      fullprint << "point=" << point << std::endl;
      fullprint << "reference pdf=" << distributionRef.computePDF(point) << std::endl;
      fullprint << "reference cdf=" << distributionRef.computeCDF(point) << std::endl;
      fullprint << "reference quantile(0.75)=" << distributionRef.computeQuantile(0.75) << std::endl;
      fullprint << "estimated pdf=" << estimatedDistribution.computePDF(point) << std::endl;
      fullprint << "estimated cdf=" << estimatedDistribution.computeCDF(point) << std::endl;
      fullprint << "estimated quantile(0.75)=" << estimatedDistribution.computeQuantile(0.75) << std::endl;
      fullprint << "done" << std::endl;
    }
  catch (OT::Test::TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return OT::Test::ExitCode::Error;
    }

  return OT::Test::ExitCode::Success;

}