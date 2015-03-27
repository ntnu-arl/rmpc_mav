/* Include files */

#include "TTR_mdl_sfun.h"
#include "TTR_mdl_sfun_debug_macros.h"
#include "c1_TTR_mdl.h"
#include "c2_TTR_mdl.h"
#include "c3_TTR_mdl.h"
#include "c4_TTR_mdl.h"
#include "c5_TTR_mdl.h"
#include "c6_TTR_mdl.h"
#include "c41_TTR_mdl.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */
uint32_T _TTR_mdlMachineNumber_;
real_T _sfTime_;

/* Function Declarations */

/* Function Definitions */
void TTR_mdl_initializer(void)
{
}

void TTR_mdl_terminator(void)
{
}

/* SFunction Glue Code */
unsigned int sf_TTR_mdl_method_dispatcher(SimStruct *simstructPtr, unsigned int
  chartFileNumber, const char* specsCksum, int_T method, void *data)
{
  if (chartFileNumber==1) {
    c1_TTR_mdl_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==2) {
    c2_TTR_mdl_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==3) {
    c3_TTR_mdl_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==4) {
    c4_TTR_mdl_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==5) {
    c5_TTR_mdl_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==6) {
    c6_TTR_mdl_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==41) {
    c41_TTR_mdl_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  return 0;
}

unsigned int sf_TTR_mdl_process_check_sum_call( int nlhs, mxArray * plhs[], int
  nrhs, const mxArray * prhs[] )
{

#ifdef MATLAB_MEX_FILE

  char commandName[20];
  if (nrhs<1 || !mxIsChar(prhs[0]) )
    return 0;

  /* Possible call to get the checksum */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"sf_get_check_sum"))
    return 0;
  plhs[0] = mxCreateDoubleMatrix( 1,4,mxREAL);
  if (nrhs>1 && mxIsChar(prhs[1])) {
    mxGetString(prhs[1], commandName,sizeof(commandName)/sizeof(char));
    commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
    if (!strcmp(commandName,"machine")) {
      ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(3735937830U);
      ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(2747376435U);
      ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(2904941521U);
      ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(2694502966U);
    } else if (!strcmp(commandName,"exportedFcn")) {
      ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(0U);
      ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(0U);
      ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(0U);
      ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(0U);
    } else if (!strcmp(commandName,"makefile")) {
      ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(1284502784U);
      ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(2409147313U);
      ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(2876267342U);
      ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(3429701099U);
    } else if (nrhs==3 && !strcmp(commandName,"chart")) {
      unsigned int chartFileNumber;
      chartFileNumber = (unsigned int)mxGetScalar(prhs[2]);
      switch (chartFileNumber) {
       case 1:
        {
          extern void sf_c1_TTR_mdl_get_check_sum(mxArray *plhs[]);
          sf_c1_TTR_mdl_get_check_sum(plhs);
          break;
        }

       case 2:
        {
          extern void sf_c2_TTR_mdl_get_check_sum(mxArray *plhs[]);
          sf_c2_TTR_mdl_get_check_sum(plhs);
          break;
        }

       case 3:
        {
          extern void sf_c3_TTR_mdl_get_check_sum(mxArray *plhs[]);
          sf_c3_TTR_mdl_get_check_sum(plhs);
          break;
        }

       case 4:
        {
          extern void sf_c4_TTR_mdl_get_check_sum(mxArray *plhs[]);
          sf_c4_TTR_mdl_get_check_sum(plhs);
          break;
        }

       case 5:
        {
          extern void sf_c5_TTR_mdl_get_check_sum(mxArray *plhs[]);
          sf_c5_TTR_mdl_get_check_sum(plhs);
          break;
        }

       case 6:
        {
          extern void sf_c6_TTR_mdl_get_check_sum(mxArray *plhs[]);
          sf_c6_TTR_mdl_get_check_sum(plhs);
          break;
        }

       case 41:
        {
          extern void sf_c41_TTR_mdl_get_check_sum(mxArray *plhs[]);
          sf_c41_TTR_mdl_get_check_sum(plhs);
          break;
        }

       default:
        ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(0.0);
        ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(0.0);
        ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(0.0);
        ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(0.0);
      }
    } else if (!strcmp(commandName,"target")) {
      ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(3264960176U);
      ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(3744709590U);
      ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(697759485U);
      ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(942582790U);
    } else {
      return 0;
    }
  } else {
    ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(1551938083U);
    ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(403375352U);
    ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(1151698152U);
    ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(2915507929U);
  }

  return 1;

#else

  return 0;

#endif

}

unsigned int sf_TTR_mdl_autoinheritance_info( int nlhs, mxArray * plhs[], int
  nrhs, const mxArray * prhs[] )
{

#ifdef MATLAB_MEX_FILE

  char commandName[32];
  char aiChksum[64];
  if (nrhs<3 || !mxIsChar(prhs[0]) )
    return 0;

  /* Possible call to get the autoinheritance_info */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"get_autoinheritance_info"))
    return 0;
  mxGetString(prhs[2], aiChksum,sizeof(aiChksum)/sizeof(char));
  aiChksum[(sizeof(aiChksum)/sizeof(char)-1)] = '\0';

  {
    unsigned int chartFileNumber;
    chartFileNumber = (unsigned int)mxGetScalar(prhs[1]);
    switch (chartFileNumber) {
     case 1:
      {
        if (strcmp(aiChksum, "mJNqT7T5jwlt4adrYvtKPB") == 0) {
          extern mxArray *sf_c1_TTR_mdl_get_autoinheritance_info(void);
          plhs[0] = sf_c1_TTR_mdl_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 2:
      {
        if (strcmp(aiChksum, "gXvjLcBacj9qOBIg0h7LpC") == 0) {
          extern mxArray *sf_c2_TTR_mdl_get_autoinheritance_info(void);
          plhs[0] = sf_c2_TTR_mdl_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 3:
      {
        if (strcmp(aiChksum, "KH8RpptcLmznByZcogMm5C") == 0) {
          extern mxArray *sf_c3_TTR_mdl_get_autoinheritance_info(void);
          plhs[0] = sf_c3_TTR_mdl_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 4:
      {
        if (strcmp(aiChksum, "VVPNWBOOtqWmckFbbcB3AH") == 0) {
          extern mxArray *sf_c4_TTR_mdl_get_autoinheritance_info(void);
          plhs[0] = sf_c4_TTR_mdl_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 5:
      {
        if (strcmp(aiChksum, "VVPNWBOOtqWmckFbbcB3AH") == 0) {
          extern mxArray *sf_c5_TTR_mdl_get_autoinheritance_info(void);
          plhs[0] = sf_c5_TTR_mdl_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 6:
      {
        if (strcmp(aiChksum, "VVPNWBOOtqWmckFbbcB3AH") == 0) {
          extern mxArray *sf_c6_TTR_mdl_get_autoinheritance_info(void);
          plhs[0] = sf_c6_TTR_mdl_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 41:
      {
        if (strcmp(aiChksum, "DapHx4lTwX5mB4jJ48d3yE") == 0) {
          extern mxArray *sf_c41_TTR_mdl_get_autoinheritance_info(void);
          plhs[0] = sf_c41_TTR_mdl_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     default:
      plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
    }
  }

  return 1;

#else

  return 0;

#endif

}

unsigned int sf_TTR_mdl_get_eml_resolved_functions_info( int nlhs, mxArray *
  plhs[], int nrhs, const mxArray * prhs[] )
{

#ifdef MATLAB_MEX_FILE

  char commandName[64];
  if (nrhs<2 || !mxIsChar(prhs[0]))
    return 0;

  /* Possible call to get the get_eml_resolved_functions_info */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"get_eml_resolved_functions_info"))
    return 0;

  {
    unsigned int chartFileNumber;
    chartFileNumber = (unsigned int)mxGetScalar(prhs[1]);
    switch (chartFileNumber) {
     case 1:
      {
        extern const mxArray *sf_c1_TTR_mdl_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c1_TTR_mdl_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 2:
      {
        extern const mxArray *sf_c2_TTR_mdl_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c2_TTR_mdl_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 3:
      {
        extern const mxArray *sf_c3_TTR_mdl_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c3_TTR_mdl_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 4:
      {
        extern const mxArray *sf_c4_TTR_mdl_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c4_TTR_mdl_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 5:
      {
        extern const mxArray *sf_c5_TTR_mdl_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c5_TTR_mdl_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 6:
      {
        extern const mxArray *sf_c6_TTR_mdl_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c6_TTR_mdl_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 41:
      {
        extern const mxArray *sf_c41_TTR_mdl_get_eml_resolved_functions_info
          (void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c41_TTR_mdl_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     default:
      plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
    }
  }

  return 1;

#else

  return 0;

#endif

}

unsigned int sf_TTR_mdl_third_party_uses_info( int nlhs, mxArray * plhs[], int
  nrhs, const mxArray * prhs[] )
{
  char commandName[64];
  char tpChksum[64];
  if (nrhs<3 || !mxIsChar(prhs[0]))
    return 0;

  /* Possible call to get the third_party_uses_info */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  mxGetString(prhs[2], tpChksum,sizeof(tpChksum)/sizeof(char));
  tpChksum[(sizeof(tpChksum)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"get_third_party_uses_info"))
    return 0;

  {
    unsigned int chartFileNumber;
    chartFileNumber = (unsigned int)mxGetScalar(prhs[1]);
    switch (chartFileNumber) {
     case 1:
      {
        if (strcmp(tpChksum, "lnhvlkCcdCLiaAL6GJ0PEH") == 0) {
          extern mxArray *sf_c1_TTR_mdl_third_party_uses_info(void);
          plhs[0] = sf_c1_TTR_mdl_third_party_uses_info();
          break;
        }
      }

     case 2:
      {
        if (strcmp(tpChksum, "07HcEZv9uaRVdRTv5YwfAH") == 0) {
          extern mxArray *sf_c2_TTR_mdl_third_party_uses_info(void);
          plhs[0] = sf_c2_TTR_mdl_third_party_uses_info();
          break;
        }
      }

     case 3:
      {
        if (strcmp(tpChksum, "eQcPIKpUkyaHOYePcmZ0LH") == 0) {
          extern mxArray *sf_c3_TTR_mdl_third_party_uses_info(void);
          plhs[0] = sf_c3_TTR_mdl_third_party_uses_info();
          break;
        }
      }

     case 4:
      {
        if (strcmp(tpChksum, "huTXx3sJ9CTmNjidVlHAlE") == 0) {
          extern mxArray *sf_c4_TTR_mdl_third_party_uses_info(void);
          plhs[0] = sf_c4_TTR_mdl_third_party_uses_info();
          break;
        }
      }

     case 5:
      {
        if (strcmp(tpChksum, "huTXx3sJ9CTmNjidVlHAlE") == 0) {
          extern mxArray *sf_c5_TTR_mdl_third_party_uses_info(void);
          plhs[0] = sf_c5_TTR_mdl_third_party_uses_info();
          break;
        }
      }

     case 6:
      {
        if (strcmp(tpChksum, "huTXx3sJ9CTmNjidVlHAlE") == 0) {
          extern mxArray *sf_c6_TTR_mdl_third_party_uses_info(void);
          plhs[0] = sf_c6_TTR_mdl_third_party_uses_info();
          break;
        }
      }

     case 41:
      {
        if (strcmp(tpChksum, "FDHjPtkQ2WHpTUoO0sAfbB") == 0) {
          extern mxArray *sf_c41_TTR_mdl_third_party_uses_info(void);
          plhs[0] = sf_c41_TTR_mdl_third_party_uses_info();
          break;
        }
      }

     default:
      plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
    }
  }

  return 1;
}

void TTR_mdl_debug_initialize(struct SfDebugInstanceStruct* debugInstance)
{
  _TTR_mdlMachineNumber_ = sf_debug_initialize_machine(debugInstance,"TTR_mdl",
    "sfun",0,7,0,0,0);
  sf_debug_set_machine_event_thresholds(debugInstance,_TTR_mdlMachineNumber_,0,0);
  sf_debug_set_machine_data_thresholds(debugInstance,_TTR_mdlMachineNumber_,0);
}

void TTR_mdl_register_exported_symbols(SimStruct* S)
{
}

static mxArray* sRtwOptimizationInfoStruct= NULL;
mxArray* load_TTR_mdl_optimization_info(void)
{
  if (sRtwOptimizationInfoStruct==NULL) {
    sRtwOptimizationInfoStruct = sf_load_rtw_optimization_info("TTR_mdl",
      "TTR_mdl");
    mexMakeArrayPersistent(sRtwOptimizationInfoStruct);
  }

  return(sRtwOptimizationInfoStruct);
}

void unload_TTR_mdl_optimization_info(void)
{
  if (sRtwOptimizationInfoStruct!=NULL) {
    mxDestroyArray(sRtwOptimizationInfoStruct);
    sRtwOptimizationInfoStruct = NULL;
  }
}
