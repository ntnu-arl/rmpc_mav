#ifndef __c41_TTR_mdl_h__
#define __c41_TTR_mdl_h__

/* Include files */
#include "sfc_sf.h"
#include "sfc_mex.h"
#include "rtwtypes.h"

/* Type Definitions */
#ifndef typedef_c41_ResolvedFunctionInfo
#define typedef_c41_ResolvedFunctionInfo

typedef struct {
  const char * context;
  const char * name;
  const char * dominantType;
  const char * resolved;
  uint32_T fileTimeLo;
  uint32_T fileTimeHi;
  uint32_T mFileTimeLo;
  uint32_T mFileTimeHi;
} c41_ResolvedFunctionInfo;

#endif                                 /*typedef_c41_ResolvedFunctionInfo*/

#ifndef typedef_SFc41_TTR_mdlInstanceStruct
#define typedef_SFc41_TTR_mdlInstanceStruct

typedef struct {
  SimStruct *S;
  ChartInfoStruct chartInfo;
  uint32_T chartNumber;
  uint32_T instanceNumber;
  int32_T c41_sfEvent;
  boolean_T c41_isStable;
  boolean_T c41_doneDoubleBufferReInit;
  uint8_T c41_is_active_c41_TTR_mdl;
} SFc41_TTR_mdlInstanceStruct;

#endif                                 /*typedef_SFc41_TTR_mdlInstanceStruct*/

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
extern const mxArray *sf_c41_TTR_mdl_get_eml_resolved_functions_info(void);

/* Function Definitions */
extern void sf_c41_TTR_mdl_get_check_sum(mxArray *plhs[]);
extern void c41_TTR_mdl_method_dispatcher(SimStruct *S, int_T method, void *data);

#endif
