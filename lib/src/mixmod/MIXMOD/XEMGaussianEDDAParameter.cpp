/***************************************************************************
                             SRC/MIXMOD/XEMGaussianEDDAParameter.cpp  description
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
#include "XEMGaussianEDDAParameter.h"
#include "XEMGaussianParameter.h"
#include "XEMGaussianData.h"
#include "XEMModel.h"



#include "XEMGaussianGeneralParameter.h"
#include "XEMUtil.h"
#include "XEMGaussianData.h"
#include "XEMRandom.h"

#include "XEMMatrix.h"
#include "XEMDiagMatrix.h"
#include "XEMSphericalMatrix.h"
#include "XEMSymmetricMatrix.h"
#include "XEMGeneralMatrix.h"

//-------------------- Constructors--------------

XEMGaussianEDDAParameter::XEMGaussianEDDAParameter():XEMGaussianParameter(){
  throw wrongConstructorType;
}


//--------------
// constructor
//-------------
XEMGaussianEDDAParameter::XEMGaussianEDDAParameter(XEMModel * iModel, XEMModelType * iModelType): XEMGaussianParameter(iModel, iModelType){
  _tabInvSqrtDetSigma = new double [_nbCluster];
  for (int64_t  k=0; k<_nbCluster; k++){
    _tabInvSqrtDetSigma[k] = 1.0;
  }
  _tabInvSigma        = new XEMMatrix* [_nbCluster];
  _tabSigma           = new XEMMatrix* [_nbCluster];
}




//------------------------------------------
// constructor called if USER_initialisation
//-------------------------------------------
XEMGaussianEDDAParameter::XEMGaussianEDDAParameter(int64_t  iNbCluster, int64_t  iPbDimension, XEMModelType * iModelType) : XEMGaussianParameter(iNbCluster, iPbDimension, iModelType){
  _tabInvSqrtDetSigma = new double [_nbCluster];
  for (int64_t  k=0; k<_nbCluster; k++){
    _tabInvSqrtDetSigma[k] = 0.0;
  }
  _tabInvSigma        = new XEMMatrix* [_nbCluster];
  _tabSigma           = new XEMMatrix* [_nbCluster];
}



//-----------------
// copy constructor
//-----------------
XEMGaussianEDDAParameter::XEMGaussianEDDAParameter(const XEMGaussianEDDAParameter * iParameter):XEMGaussianParameter(iParameter){
  int64_t  k;
  _tabInvSqrtDetSigma   = new double[_nbCluster];
  double * iTabInvSqrtDetSigma = iParameter->getTabInvSqrtDetSigma();
  for (k=0; k<_nbCluster; k++){
    _tabInvSqrtDetSigma[k] = iTabInvSqrtDetSigma[k];
  }
  _tabInvSigma        = new XEMMatrix* [_nbCluster];
  _tabSigma           = new XEMMatrix* [_nbCluster];
}





/**************/
/* Destructor */
/**************/
XEMGaussianEDDAParameter::~XEMGaussianEDDAParameter(){
  if (_tabInvSqrtDetSigma){
    delete[] _tabInvSqrtDetSigma;
    _tabInvSqrtDetSigma = NULL;
  }

  if(_tabInvSigma){
    delete[] _tabInvSigma;
    _tabInvSigma = NULL;
  }

  if(_tabSigma){
    delete[] _tabSigma;
    _tabSigma = NULL;
  }

}


//------------------------
// reset to default values
//------------------------
void XEMGaussianEDDAParameter::reset(){
  for (int64_t  k=0; k<_nbCluster; k++){
    _tabInvSqrtDetSigma[k] = 0.0;
    *(_tabSigma[k]) = 1.0;
    *(_tabInvSigma[k]) = 1.0;
  }
  XEMGaussianParameter::reset();
}




void XEMGaussianEDDAParameter::getAllPdf(double** tabFik,double* tabProportion)const{

  XEMGaussianData * data        = (XEMGaussianData*)(_model->getData());
  int64_t  nbSample = _model->getNbSample();
  double * muk ;                            // to store mu_k
  double InvPiInvDetProp;                   // 1/(pi)^(d/2) * 1/det(\Sigma_k) ^ (.5) * p_k
  XEMMatrix * SigmakMoins1;
  double ** matrix = data->getYStore();         // to store x_i i=1,...,n

  double ** p_matrix;                       // pointeur : parcours de y
  double ** p_tabFik_i;                     // pointeur : parcours de tabFik

  double norme;

  // parcours
  int64_t  i;   // parcours des individus
  int64_t  k;    // cluster index

  double * xiMoinsMuk = data->getTmpTabOfSizePbDimension();
  double * xi;
  int64_t  p;

  for(k=0 ; k<_nbCluster ; k++){
    InvPiInvDetProp = data->getInv2PiPow() * _tabInvSqrtDetSigma[k] * tabProportion[k];
    muk             = _tabMean[k];
    SigmakMoins1    = _tabInvSigma[k];

    //computing
    p_matrix = matrix; // pointe sur la premiere ligne des donnees
    p_tabFik_i = tabFik;
    for(i=0 ; i<nbSample ; i++){
      xi =   *p_matrix;
      for(p=0; p<_pbDimension; p++){
        xiMoinsMuk[p] = xi[p] - muk[p];
      }
      // calcul de la norme (x-muk)'Sigma_k^{-1} (x-muk)
      norme            = SigmakMoins1->norme(xiMoinsMuk);  // virtual !!
      (*p_tabFik_i)[k] = InvPiInvDetProp * exp(-0.5 * norme);

      p_matrix++;
      p_tabFik_i++;
    } // end for i
  } // end for k

}


//-------
// getPdf
//-------
double XEMGaussianEDDAParameter::getPdf(XEMSample * x, int64_t  kCluster)const{

  XEMGaussianData * data        = (XEMGaussianData*)(_model->getData());

  double normPdf     = 0.0;                   /* Normal pdf                  */
  double invPi       = data->getInv2PiPow() ; /* Inverse of (2*Pi)^(d/2)     */

  /* Compute (x-m)*inv(S)*(x-m)' */
  double * ligne     = ((XEMGaussianSample*)x)->getTabValue();

  XEMMatrix * Sigma_kMoins1 = _tabInvSigma[kCluster];

  double norme;
  double * muk = _tabMean[kCluster];
  double * xiMoinsMuk = data->getTmpTabOfSizePbDimension();
  int64_t  p;

  for(p=0; p<_pbDimension; p++){
    xiMoinsMuk[p] = ligne[p] - muk[p];
  }

  // calcul de la norme (x-muk)'Sigma_k^{-1} (x-muk)
  norme = Sigma_kMoins1->norme(xiMoinsMuk); // virtual

  /* Compute normal density */
  normPdf = invPi * _tabInvSqrtDetSigma[kCluster] * exp( -0.5 * norme);

  return normPdf;
}





double XEMGaussianEDDAParameter::getPdf(int64_t  iSample, int64_t  kCluster)const{

  XEMGaussianData * data        = (XEMGaussianData*)(_model->getData());
  double * x                    = (data->getYStore())[iSample];

  XEMMatrix * Sigma_kCluster_moins1  = _tabInvSigma[kCluster];

  double norme;

  double * xiMoinsMuk = data->getTmpTabOfSizePbDimension();
  double * muk = _tabMean[kCluster];

  int64_t  p;

  for(p=0; p<_pbDimension; p++){
    xiMoinsMuk[p] = x[p] - muk[p];
  }

  // calcul de la norme (x-muk)'Sigma_k^{-1} (x-muk)
  norme = Sigma_kCluster_moins1->norme(xiMoinsMuk);  // virtual

  double normPdf;

  // Compute normal density
  normPdf = data->getInv2PiPow() * _tabInvSqrtDetSigma[kCluster] * exp(-0.5*norme);

  return normPdf;
}




void XEMGaussianEDDAParameter::updateTabInvSigmaAndDet(){
  int64_t  k;
  double detSigma;
  for (k=0; k<_nbCluster; k++){
    detSigma                 = _tabSigma[k]->determinant(minDeterminantSigmaValueError);
    _tabSigma[k]->inverse(_tabInvSigma[k]);
    _tabInvSqrtDetSigma[k]   = 1.0 / sqrt(detSigma);
  }

}


//---------
// MAP step
//---------
// update _tabWk and _W.
// No need for this algo but if CV is the criterion, these values
// must be updated
/*void XEMGaussianEDDAParameter::MAPStep(){
  computeTabWkW();
  }*/



//--------------------
// init USER_PARTITION
//--------------------
void XEMGaussianEDDAParameter::initForInitUSER_PARTITION(int64_t  & nbInitializedCluster, bool * tabNotInitializedCluster, XEMPartition * initPartition){
  // init : tabSigma, _tabInvSigma, _tabInvSqrtDetSigma,
  XEMDiagMatrix * matrixDataVar = new XEMDiagMatrix(_pbDimension,0.0);
  computeGlobalDiagDataVariance(matrixDataVar);
  // Vaiance matrix initialization to variance matrix of data (Symmetric, Diag or Spherical)
  for (int64_t  k=0; k<_nbCluster; k++){
    (*_tabSigma[k]) = matrixDataVar;
  }
  updateTabInvSigmaAndDet();
  delete matrixDataVar;

  // _tabMean
  computeTabMeanInitUSER_PARTITION(nbInitializedCluster, tabNotInitializedCluster, initPartition);
}





void  XEMGaussianEDDAParameter::initUSER(XEMParameter * iParam){
  /*
    updated : _tabMean, _tabWk, _tabSigma, _tabProportion, _tabShape, _tabOrientation, _tabInvSigma, _tabInvSqrtDetSigma
  */

  // recuperation
  // we got an XEMGaussianGeneralParameter
  // because of the implementation in class XEMStrategyType
  // in gaussian cases the init parameters are allways General
  XEMGaussianGeneralParameter * param = (XEMGaussianGeneralParameter *) iParam;
  double ** iTabMean            = param->getTabMean();
  double * iTabProportion       = param->getTabProportion();
  XEMMatrix **  iTabWk          = param->getTabWk();
  XEMMatrix **  iTabSigma       = param->getTabSigma();
  int64_t  k;
  for (k=0; k<_nbCluster; k++){
    recopyTab(iTabMean[k], _tabMean[k], _pbDimension);
    (* _tabWk[k]) = iTabWk[k];
    (* _tabSigma[k]) = iTabSigma[k];
    if (hasFreeProportion(_modelType->_nameModel)) {
      _tabProportion[k]       = iTabProportion[k];
    }
    else{
      _tabProportion[k]       =  1.0 / _nbCluster;
    }
  }
}



void  XEMGaussianEDDAParameter::input(ifstream & fi){
  // verify paramFile
  double tmp;
  int64_t  cpt=0;
  fi.clear();
  fi.seekg(0, ios::beg);
  //while (!fi.eof()){
  while (fi>>tmp){ // retourne NULL si on est �la fin
    cpt++;
  }
  int64_t  goodValue = _nbCluster * (1 + _pbDimension + _pbDimension*_pbDimension);

  if(cpt != goodValue){
    throw errorInitParameter;
  }

  fi.clear();
  fi.seekg(0, ios::beg);

  int64_t  j,k;
  double * muk;
  for (k=0; k<_nbCluster; k++){
    muk          = _tabMean[k];
    // Proportions //
    fi >> _tabProportion[k];
    // Center (mean) //
    for (j=0; j<_pbDimension; j++){
      fi >> muk[j];
    }
    // Variance matrix //
    _tabSigma[k]->input(fi) ; // virtual method
  } // end for k
}





void XEMGaussianEDDAParameter::updateForCV(XEMModel * originalModel, XEMCVBlock & CVBlock){
  XEMGaussianParameter::updateForCV(originalModel,CVBlock);
  computeTabSigma();
}



/*-------------------------------------------------------------------------------------------
  M step
  ------

  already updated :
  - _model (_tabFik, _tabTik, _tabCik, _tabNk) if Estep is done before
  - _model->_tabCik, _model->_tabNk if USER_PARTITION is done before (Disciminant analysis)
  In all cases, only  _tabCik and _tabNk are needed

  updated in this method :
  - in XEMGaussianParameter::MStep :
  -     _tabProportion
  - _tabMean
  - _tabWk
  - _W
  - here :
  - _tabSigma
  - _tabInvSigma
  - _tabInvSqrtDetSigma
  --------------------------------------------------------------------------------------------*/
void XEMGaussianEDDAParameter::MStep(){
  XEMGaussianParameter::MStep();
  computeTabSigma();
}






//--------------------------------------------
// initialize attributes before an InitRandom
//--------------------------------------------
void  XEMGaussianEDDAParameter::initForInitRANDOM(){
  XEMDiagMatrix * matrixDataVar = new XEMDiagMatrix(_pbDimension,0.0);
  computeGlobalDiagDataVariance(matrixDataVar);

  // Vaiance matrix initialization to variance matrix of data (Symmetric, Diag or Spherical)
  // init : tabSigma, _tabInvSigma, _tabInvSqrtDetSigma,
  for (int64_t  k=0; k<_nbCluster; k++){
    (*_tabSigma[k]) = matrixDataVar;
  }
  updateTabInvSigmaAndDet();

  delete matrixDataVar;
}





//-----------------------------------------
// computeTik when underflow
// -model->_tabSumF[i] pour ith sample = 0
// i : 0 ->_nbSample-1
//-----------------------------------------
void XEMGaussianEDDAParameter::computeTikUnderflow(int64_t  i, double ** tabTik){

  XEMGaussianData * data        = (XEMGaussianData*)(_model->getData());
  int64_t  * lnFk                 = new int64_t [_nbCluster];
  long double  * lnFkPrim             = new long double[_nbCluster];
  long double * fkPrim               = new long double[_nbCluster];
  int64_t  k,k0;
  long double lnFkMax, fkTPrim;
  long double detSigma;

  double * ligne     = (data->getYStore())[i];
  long double norme;
  double ** p_tabMean;
  double *  tabTik_i = tabTik[i];

  double * xiMoinsMuk = data->getTmpTabOfSizePbDimension();
  double * muk;

  p_tabMean = _tabMean;
  int64_t  p;
  for (k=0; k<_nbCluster; k++){
    detSigma      = (_tabSigma[k])->determinant(minDeterminantSigmaValueError); // virtual method
    muk = *p_tabMean;
    for(p=0; p<_pbDimension; p++){
      xiMoinsMuk[p] = ligne[p]  - muk[p];
    }
    norme         = (_tabInvSigma[k])->norme(xiMoinsMuk); // virtual method

    // compute lnFik[k]

    lnFk[k]       =  log(_tabProportion[k]) - data->getHalfPbDimensionLog2Pi() - 0.5*log(detSigma) - 0.5*norme;

    p_tabMean++;
  } // end for k

  lnFkMax = lnFk[0];
  for (k=1; k<_nbCluster; k++){
    if (lnFk[k] > lnFkMax){
      lnFkMax = lnFk[k];
    }
  }


  fkTPrim = 0.0;
  for (k=0; k<_nbCluster; k++){
    lnFkPrim[k] = lnFk[k] - lnFkMax;
    fkPrim[k]   = exp(lnFkPrim[k]);
    fkTPrim    += fkPrim[k];
  }

  // compute tabTik
  if (fkTPrim == 0){
    // reset tabTik
    initToZero(tabTik_i, _nbCluster);
    k0           = XEMGaussianParameter::computeClassAssigment(i);
    tabTik_i[k0] = 1.0;
  }
  else{
    for (k=0; k<_nbCluster; k++){
      tabTik_i[k] = fkPrim[k] / fkTPrim;
    }
  }

  delete [] lnFk;
  delete [] lnFkPrim;
  delete [] fkPrim;

}




void XEMGaussianEDDAParameter::edit(){
  int64_t  k;
  for (k=0; k<_nbCluster; k++){
    cout<<"\tcomponent : " << k << endl;
    cout<<"\t\tproportion : " << _tabProportion[k]<<endl;
    editTab(_tabMean+k,1,_pbDimension," ","\t\tmean : ",cout);
    cout<<"\t\tsigma : "<<endl;
    _tabSigma[k]->edit(cout, "\t\t\t");

    cout<<"\t\tWk : "<<endl;
    _tabWk[k]->edit(cout,"\t\t\t");

    cout<<"\t\tinvSigma : "<<endl;
    _tabInvSigma[k]->edit(cout,"\t\t\t");

    cout<<"\t\ttabInvSqrtDetSigma : "<<_tabInvSqrtDetSigma[k]<<endl;
  }

  cout<<"\tW : "<<endl;
  _W->edit(cout, "\t\t");

}






//------
// Edit
//-------
void XEMGaussianEDDAParameter::edit(ofstream & oFile, bool text){
  int64_t  k;
  if (text){
    for (k=0; k<_nbCluster; k++){
      oFile<<"\t\t\tComponent "<<k+1<<endl;
      oFile<<"\t\t\t---------"<<endl;
      oFile<<"\t\t\tMixing proportion : "<<_tabProportion[k]<<endl;
      editTab(_tabMean+k,1,_pbDimension," ","\t\t\tMean : ",oFile);

      oFile<<"\t\t\tCovariance matrix : "<<endl;
      _tabSigma[k]->edit(oFile, "\t\t\t\t");

      oFile<<endl;
    }
    oFile<<endl;
  }
  else{
    for (k=0; k<_nbCluster; k++){
      oFile<<_tabProportion[k]<<endl;
      editTab(_tabMean+k,1,_pbDimension," ","",oFile);
      _tabSigma[k]->edit(oFile, "");
      oFile<<endl;

    }
    oFile<<endl;

  }
}


void XEMGaussianEDDAParameter::recopy(XEMParameter * otherParameter){

  XEMGaussianEDDAParameter * iParameter = (XEMGaussianEDDAParameter *)otherParameter;

  recopyTab(iParameter->getTabMean(), _tabMean, _nbCluster, _pbDimension);
  //  _W->recopy(iParameter->getW());
  (*_W) = iParameter->getW();

  int64_t  k;
  XEMMatrix ** iTabSigma    = iParameter->getTabSigma();
  XEMMatrix ** iTabInvSigma = iParameter->getTabInvSigma();
  XEMMatrix ** iTabWk       = iParameter->getTabWk();
  for(k=0; k<_nbCluster; k++){
    // _tabSigma[k]->recopy(iTabSigma[k]);
    (* _tabSigma[k]) = iTabSigma[k];
    // _tabInvSigma[k]->recopy(iTabInvSigma[k]);
    (*_tabInvSigma[k]) = iTabInvSigma[k];
    // _tabWk[k]->recopy(iTabWk[k]);
    (* _tabWk[k]) = iTabWk[k];
  }
  recopyTab(iParameter->getTabInvSqrtDetSigma(),  _tabInvSqrtDetSigma, _nbCluster);

}

