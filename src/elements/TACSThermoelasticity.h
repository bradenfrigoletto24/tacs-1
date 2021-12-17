/*
  This file is part of TACS: The Toolkit for the Analysis of Composite
  Structures, a parallel finite-element code for structural and
  multidisciplinary design optimization.

  Copyright (C) 2014 Georgia Tech Research Corporation

  TACS is licensed under the Apache License, Version 2.0 (the
  "License"); you may not use this software except in compliance with
  the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef TACS_THERMOELASTICITY_H
#define TACS_THERMOELASTICITY_H

#include "TACSElementModel.h"
#include "TACSLinearElasticity.h"
#include "TACSPlaneStressConstitutive.h"
#include "TACSSolidConstitutive.h"

const int TACS_STEADY_STATE_MECHANICAL = 1;
const int TACS_STEADY_STATE_THERMAL = 2;

class TACSLinearThermoelasticity2D : public TACSElementModel {
 public:
  TACSLinearThermoelasticity2D( TACSPlaneStressConstitutive *_con,
                                ElementStrainType strain_type,
                                int _steady_state_flag=0 );
  ~TACSLinearThermoelasticity2D();

  int getNumParameters();
  int getVarsPerNode();
  int getDesignVarsPerNode();

  /**
    Retrieve the global design variable numbers associated with this element
  */
  int getDesignVarNums( int elemIndex, int dvLen, int dvNums[] );

  /**
    Set the element design variables from the design vector
  */
  int setDesignVars( int elemIndex, int dvLen, const TacsScalar dvs[] );

  /**
    Get the element design variables values
  */
  int getDesignVars( int elemIndex, int dvLen, TacsScalar dvs[] );

  /**
    Get the lower and upper bounds for the design variable values
  */
  int getDesignVarRange( int elemIndex, int dvLen,
                         TacsScalar lb[], TacsScalar ub[] );

  /**
    Get underlying tacs constitutive object for this model
  */
  TACSConstitutive* getConstitutive(){
    return stiff;
  }

  /**
    Evaluate the coefficients of the weak form integrand
  */
  void evalWeakIntegrand( int elemIndex, const double time,
                          int n, const double pt[],
                          const TacsScalar X[], const TacsScalar Xd[],
                          const TacsScalar Ut[], const TacsScalar Ux[],
                          TacsScalar DUt[], TacsScalar DUx[] );

  /**
     Add the derivative of the product of the adjoint and residual to
     the design vector
  */
  void addWeakAdjProduct( int elemIndex, const double time, TacsScalar scale,
                          int n, const double pt[],
                          const TacsScalar X[], const TacsScalar Xd[],
                          const TacsScalar Ut[], const TacsScalar Ux[],
                          const TacsScalar Psi[], const TacsScalar Psix[],
                          int dvLen, TacsScalar dfdx[] );

  /**
    Evaluate the spatial derivatives of the product of the adjoint
    and the residual vector.
  */
  void evalWeakAdjXptSensProduct( int elemIndex, const double time,
                                  int n, const double pt[],
                                  const TacsScalar X[], const TacsScalar Xd[],
                                  const TacsScalar Ut[], const TacsScalar Ux[],
                                  const TacsScalar Psi[], const TacsScalar Psix[],
                                  TacsScalar *product,
                                  TacsScalar dfdX[], TacsScalar dfdXd[],
                                  TacsScalar dfdUx[], TacsScalar dfdPsix[] );

  /**
    Get the non-zero pattern for the matrix
  */
  void getWeakMatrixNonzeros( ElementMatrixType matType, int elemIndex,
                              int *Jac_nnz, const int *Jac_pairs[] );

  /**
    Evaluate weak form coefficients for the specific type of matrix
  */
  void evalWeakMatrix( ElementMatrixType matType, int elemIndex,
                       const double time, int n, const double pt[],
                       const TacsScalar X[], const TacsScalar Xd[],
                       const TacsScalar Ut[], const TacsScalar Ux[],
                       TacsScalar DUt[], TacsScalar DUx[], TacsScalar Jac[] );

  /**
     Evaluate a point-wise quantity of interest at a quadrature point
  */
  int evalPointQuantity( int elemIndex, const int quantityType,
                         const double time, int n, const double pt[],
                         const TacsScalar X[], const TacsScalar Xd[],
                         const TacsScalar Ut[], const TacsScalar Ux[],
                         TacsScalar *quantity );

  /**
     Add the derivative of the quantity w.r.t. the design variables
  */
  void addPointQuantityDVSens( int elemIndex, const int quantityType,
                               const double time, TacsScalar scale,
                               int n, const double pt[],
                               const TacsScalar X[], const TacsScalar Xd[],
                               const TacsScalar Ut[], const TacsScalar Ux[],
                               const TacsScalar dfdq[],
                               int dvLen, TacsScalar dfdx[] );

  /**
     Evaluate the derivatives of the point-wise quantity of interest
     with respect to X, Ut and Ux.
  */
  void evalPointQuantitySens( int elemIndex, const int quantityType,
                              const double time, int n, const double pt[],
                              const TacsScalar X[], const TacsScalar Xd[],
                              const TacsScalar Ut[], const TacsScalar Ux[],
                              const TacsScalar dfdq[],
                              TacsScalar dfdX[], TacsScalar dfdXd[],
                              TacsScalar dfdUt[], TacsScalar dfdUx[] );

  /**
    Get the output for a single node in the mesh
  */
  void getOutputData( int elemIndex, const double time,
                      ElementType etype, int write_flag,
                      const double pt[], const TacsScalar X[],
                      const TacsScalar Ut[], const TacsScalar Ux[],
                      int ld_data, TacsScalar *data );

 private:
  int steady_state_flag;
  ElementStrainType strain_type;
  TACSPlaneStressConstitutive *stiff;

  // Constant member data
  static const int linear_Jac_pairs[54];
};


class TACSLinearThermoelasticity3D : public TACSElementModel {
 public:
  TACSLinearThermoelasticity3D( TACSSolidConstitutive *_con,
                                ElementStrainType strain_type,
                                int _steady_state_flag=0 );
  ~TACSLinearThermoelasticity3D();

  int getNumParameters();
  int getVarsPerNode();
  int getDesignVarsPerNode();

  /**
    Retrieve the global design variable numbers associated with this element
  */
  int getDesignVarNums( int elemIndex, int dvLen, int dvNums[] );

  /**
    Set the element design variables from the design vector
  */
  int setDesignVars( int elemIndex, int dvLen, const TacsScalar dvs[] );

  /**
    Get the element design variables values
  */
  int getDesignVars( int elemIndex, int dvLen, TacsScalar dvs[] );

  /**
    Get the lower and upper bounds for the design variable values
  */
  int getDesignVarRange( int elemIndex, int dvLen,
                         TacsScalar lb[], TacsScalar ub[] );

  /**
    Get underlying tacs constitutive object for this model
  */
  TACSConstitutive* getConstitutive(){
    return stiff;
  }

  /**
    Evaluate the coefficients of the weak form integrand
  */
  void evalWeakIntegrand( int elemIndex, const double time,
                          int n, const double pt[],
                          const TacsScalar X[], const TacsScalar Xd[],
                          const TacsScalar Ut[], const TacsScalar Ux[],
                          TacsScalar DUt[], TacsScalar DUx[] );

  /**
     Add the derivative of the product of the adjoint and residual to
     the design vector
  */
  void addWeakAdjProduct( int elemIndex, const double time, TacsScalar scale,
                          int n, const double pt[],
                          const TacsScalar X[], const TacsScalar Xd[],
                          const TacsScalar Ut[], const TacsScalar Ux[],
                          const TacsScalar Psi[], const TacsScalar Psix[],
                          int dvLen, TacsScalar dfdx[] );

  /**
    Evaluate the spatial derivatives of the product of the adjoint
    and the residual vector.
  */
  void evalWeakAdjXptSensProduct( int elemIndex, const double time,
                                  int n, const double pt[],
                                  const TacsScalar X[], const TacsScalar Xd[],
                                  const TacsScalar Ut[], const TacsScalar Ux[],
                                  const TacsScalar Psi[], const TacsScalar Psix[],
                                  TacsScalar *product,
                                  TacsScalar dfdX[], TacsScalar dfdXd[],
                                  TacsScalar dfdUx[], TacsScalar dfdPsix[] );

  /**
    Get the non-zero pattern for the matrix
  */
  void getWeakMatrixNonzeros( ElementMatrixType matType, int elemIndex,
                              int *Jac_nnz, const int *Jac_pairs[] );

  /**
    Evaluate weak form coefficients for the specific type of matrix
  */
  void evalWeakMatrix( ElementMatrixType matType, int elemIndex,
                       const double time, int n, const double pt[],
                       const TacsScalar X[], const TacsScalar Xd[],
                       const TacsScalar Ut[], const TacsScalar Ux[],
                       TacsScalar DUt[], TacsScalar DUx[], TacsScalar Jac[] );

  /**
     Evaluate a point-wise quantity of interest at a quadrature point
  */
  int evalPointQuantity( int elemIndex, const int quantityType,
                         const double time, int n, const double pt[],
                         const TacsScalar X[], const TacsScalar Xd[],
                         const TacsScalar Ut[], const TacsScalar Ux[],
                         TacsScalar *quantity );

  /**
     Add the derivative of the quantity w.r.t. the design variables
  */
  void addPointQuantityDVSens( int elemIndex, const int quantityType,
                               const double time, TacsScalar scale,
                               int n, const double pt[],
                               const TacsScalar X[], const TacsScalar Xd[],
                               const TacsScalar Ut[], const TacsScalar Ux[],
                               const TacsScalar dfdq[],
                               int dvLen, TacsScalar dfdx[] );

  /**
     Evaluate the derivatives of the point-wise quantity of interest
     with respect to X, Ut and Ux.
  */
  void evalPointQuantitySens( int elemIndex, const int quantityType,
                              const double time, int n, const double pt[],
                              const TacsScalar X[], const TacsScalar Xd[],
                              const TacsScalar Ut[], const TacsScalar Ux[],
                              const TacsScalar dfdq[],
                              TacsScalar dfdX[], TacsScalar dfdXd[],
                              TacsScalar dfdUt[], TacsScalar dfdUx[] );

  /**
    Get the output for a single node in the mesh
  */
  void getOutputData( int elemIndex, const double time,
                      ElementType etype, int write_flag,
                      const double pt[], const TacsScalar X[],
                      const TacsScalar Ut[], const TacsScalar Ux[],
                      int ld_data, TacsScalar *data );

 private:
  int steady_state_flag;
  ElementStrainType strain_type;
  TACSSolidConstitutive *stiff;

  // Constant member data
  static const int linear_Jac_pairs[206];
};

#endif // TACS_THERMOELASTICITY_H
