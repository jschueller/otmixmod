/***************************************************************************
                             SRC/MIXMOD/XEMStrategy.h  description
    copyright            : (C) MIXMOD Team - 2001-2011
    email                : contact@mixmod.org
***************************************************************************/

/***************************************************************************
    This file is part of MIXMOD

    MIXMOD is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    MIXMOD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MIXMOD.  If not, see <http://www.gnu.org/licenses/>.

    All informations available on : http://www.mixmod.org
***************************************************************************/
#ifndef XEMSTRATEGY_H
#define XEMSTRATEGY_H

#include "XEMAlgo.h"
#include "XEMData.h"
#include "XEMPartition.h"
#include "XEMModel.h"

/**
   @brief Base class for Strategy(s)
   @author F Langrognet
*/

class XEMStrategy{

 public:

  /// Default constructor
  XEMStrategy();

  /// Constructor
  XEMStrategy(const XEMStrategy & strategy);



  //------------
  // Constructor (used in DCV)
  XEMStrategy(XEMStrategy * originalStrategy, XEMCVBlock & block);

  /// Destructor
  virtual ~XEMStrategy();

  ///getStrategyInit
  const XEMStrategyInit * getStrategyInit() const;

  ///getAlgo[i]
  const XEMAlgo * getAlgo(int64_t index) const;

  /// setStrategyInit
  void setStrategyInit(XEMStrategyInit * iStrategyInit);

  /// setStrategyInit
  void setStrategyInit(XEMStrategyInitName  initName,XEMData *& data, int64_t nbNbCluster, int64_t * tabNbCluster, XEMModelType ** tabModelType);

  /// setInitParam
  void setInitParam(string & paramFileName, int64_t position);

  /// setInitPartition
  void setInitPartition(string & partitionFileName, int64_t position);

  /// setInitPartition
  void setInitPartition(XEMPartition * part, int64_t position);

  /// getNbTryInInit
  const int64_t getNbTryInInit() const;

  /// setNbTryInInit
  void setNbTryInInit(int64_t nbTry);

  /// getNbIterationInInit
  const int64_t getNbIterationInInit() const;

  /// set NbIterationInInit
  void setNbIterationInInit(int64_t nbIteration);

  /// getEpsilonInInit
  const double getEpsilonInInit() const;

  /// setEpsilonInInit
  void setEpsilonInInit(double epsilon);

  /// getStopNameInInit
  const XEMAlgoStopName getStopNameInInit() const;

  /// setStopNameInInit
  void setStopNameInInit(XEMAlgoStopName stopName);




  /// setAlgo
  void setAlgo(XEMAlgoName algoName, int64_t position);

  /// removeAlgo
  void removeAlgo(int64_t position);

  /// getTabAlgo
  XEMAlgo ** getTabAlgo() const;

  /// insertAlgo
  void insertAlgo(XEMAlgo * algo, int64_t position);


  /// setAlgoStopRuleTypeValue
  void setAlgoStopRule(XEMAlgoStopName stopName, int64_t position);
  void setAlgoIteration(  int64_t position, int64_t nbIterationValue);
  void setAlgoEpsilon( int64_t position, double epsilonValue);

  ///nbTry
  const int64_t getNbTry()const;
  void setNbTry(int64_t nbTry);

  const int64_t getNbAlgo() const;


  /// Input strategy
  void input(ifstream & fi, XEMData *& data, int64_t nbNbCluster, int64_t * tabNbCluster, XEMModelType ** tabModelType);

  /// Run method
  void run(XEMModel *& model);

  bool isMAPAlgo();

  bool isMAlgo();

  bool verify();

  void edit(ofstream & oFile);


  friend ostream & operator << (ostream & fo, XEMStrategy & strategy);


 private :

  /// Number of try in the strategy
  int64_t _nbTry;

  /// strategyInit
  XEMStrategyInit * _strategyInit;

  /// Number of algorithm in the strategy
  int64_t _nbAlgo;

  /// Table of algorithm
  XEMAlgo ** _tabAlgo;    // aggregate

  void oneTry(XEMModel *& model);

};


inline const XEMStrategyInit * XEMStrategy::getStrategyInit() const{
  return _strategyInit;
}

inline const XEMAlgo * XEMStrategy::getAlgo(int64_t index) const{
  return _tabAlgo[index];
}

inline XEMAlgo ** XEMStrategy::getTabAlgo() const{
  return _tabAlgo;
}

inline const int64_t XEMStrategy::getNbAlgo() const{
  return _nbAlgo;
}
inline const int64_t XEMStrategy::getNbTry()const{
  return _nbTry;
}

inline const int64_t XEMStrategy::getNbTryInInit() const{
  return _strategyInit->getNbTry();
}

inline const int64_t XEMStrategy::getNbIterationInInit() const{
  return _strategyInit->getNbIteration();
}

inline const double XEMStrategy::getEpsilonInInit() const{
  return _strategyInit->getEpsilon();
}

inline void XEMStrategy::setNbTryInInit(int64_t nbTry){
  _strategyInit->setNbTry(nbTry);
}

inline     void XEMStrategy::setNbIterationInInit(int64_t nbIteration){
  _strategyInit->setNbIteration(nbIteration);
}

inline void XEMStrategy::setEpsilonInInit(double epsilon){
  _strategyInit->setEpsilon(epsilon);
}

inline const XEMAlgoStopName XEMStrategy::getStopNameInInit() const{
  return(_strategyInit->getStopName());
}

inline void XEMStrategy::setStopNameInInit(XEMAlgoStopName stopName){
  _strategyInit->setStopName(stopName);
}

//others functions
XEMStrategy * buildDefaultClusteringStrategy();


#endif
