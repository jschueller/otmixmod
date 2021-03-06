/***************************************************************************
                             SRC/MIXMOD/XEMCriterionOutput.h  description
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
#ifndef XEMCriterionOutput_H
#define XEMCriterionOutput_H

#include "XEMUtil.h"
#include "XEMCriterion.h"



/** @brief Base class for Label(s)
    @author F Langrognet & A Echenim
*/

class XEMCriterionOutput{

 public:

  /// Default constructor
  XEMCriterionOutput();

  /// constructor
  XEMCriterionOutput(XEMCriterionName criterionName);

  /// Constructor
  XEMCriterionOutput(XEMCriterionName criterionName, double criterionValue, XEMErrorType criterionErrorType);

  /// Destructor
  virtual ~XEMCriterionOutput();

  ///editType
  void editType(ofstream & oFile);

  ///editValue
  void editValue(ofstream & oFile, bool text=false);

  /// editTypeAndValue
  void editTypeAndValue(ofstream & oFile);

  //--------------
  // get
  //--------------
  /// getCriterionName
  XEMCriterionName getCriterionName() const;

  /// getValue
  double getValue() const;

  /// getError
  XEMErrorType getError() const;

  //--------------
  // set
  //--------------
  /// setCriterionName
  void  setCriterionName(XEMCriterionName criterionName);

  /// setValue
  void  setValue(double value);

  /// setError
  void setError(XEMErrorType e);


  //------
 private :
  //------
  /// Criterion value
  double _value;

  /// Error type in calculation of criterion value
  XEMErrorType _error;

  /// criterion name
  XEMCriterionName _criterionName;


};

inline XEMCriterionName XEMCriterionOutput::getCriterionName() const{
  return _criterionName;
}

inline double XEMCriterionOutput::getValue() const{
  return _value;
}

inline XEMErrorType XEMCriterionOutput::getError() const{
  return _error;
}


inline  void  XEMCriterionOutput::setCriterionName(XEMCriterionName criterionName){
  _criterionName = criterionName;
}

inline  void  XEMCriterionOutput::setValue(double value){
  _value = value;
}

inline  void XEMCriterionOutput::setError(XEMErrorType e){
  _error = e;
}

#endif
