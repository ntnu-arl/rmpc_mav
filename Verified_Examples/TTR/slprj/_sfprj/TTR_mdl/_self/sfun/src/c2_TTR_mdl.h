#ifndef __c2_TTR_mdl_h__
#define __c2_TTR_mdl_h__

/* Include files */
#include "sfc_sf.h"
#include "sfc_mex.h"
#include "rtwtypes.h"

/* Type Definitions */
#ifndef typedef_c2_ResolvedFunctionInfo
#define typedef_c2_ResolvedFunctionInfo

typedef struct {
  const char * context;
  const char * name;
  const char * dominantType;
  const char * resolved;
  uint32_T fileTimeLo;
  uint32_T fileTimeHi;
  uint32_T mFileTimeLo;
  uint32_T mFileTimeHi;
} c2_ResolvedFunctionInfo;

#endif                                 /*typedef_c2_ResolvedFunctionInfo*/

#ifndef typedef_SFc2_TTR_mdlInstanceStruct
#define typedef_SFc2_TTR_mdlInstanceStruct

typedef struct {
  SimStruct *S;
  ChartInfoStruct chartInfo;
  uint32_T chartNumber;
  uint32_T instanceNumber;
  int32_T c2_sfEvent;
  boolean_T c2_isStable;
  boolean_T c2_doneDoubleBufferReInit;
  uint8_T c2_is_active_c2_TTR_mdl;
  real_T c2_PolyRL_U2R[2];
  real_T c2_PolyT_U2R[2];
  real_T c2_PolyGT_U2A[2];
  real_T c2_LQR_phi[4];
  real_T c2_t_km1;
  boolean_T c2_t_km1_not_empty;
  real_T c2_CA_M_Theta_km1[3];
  boolean_T c2_CA_M_Theta_km1_not_empty;
  real_T c2_CA_S_Theta_km1[3];
  boolean_T c2_CA_S_Theta_km1_not_empty;
  real_T c2_phi_err_int;
  boolean_T c2_phi_err_int_not_empty;
  real_T c2_theta_err_int;
  boolean_T c2_theta_err_int_not_empty;
  real_T c2_psi_err_int;
  boolean_T c2_psi_err_int_not_empty;
} SFc2_TTR_mdlInstanceStruct;

#endif                                 /*typedef_SFc2_TTR_mdlInstanceStruct*/

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
extern const mxArray *sf_c2_TTR_mdl_get_eml_resolved_functions_info(void);

/* Function Definitions */
extern void sf_c2_TTR_mdl_get_check_sum(mxArray *plhs[]);
extern void c2_TTR_mdl_method_dispatcher(SimStruct *S, int_T method, void *data);

#endif
