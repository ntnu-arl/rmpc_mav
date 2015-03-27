/* Include files */

#include <stddef.h>
#include "blas.h"
#include "TTR_mdl_sfun.h"
#include "c1_TTR_mdl.h"
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance->chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance->instanceNumber)
#include "TTR_mdl_sfun_debug_macros.h"
#define _SF_MEX_LISTEN_FOR_CTRL_C(S)   sf_mex_listen_for_ctrl_c(sfGlobalDebugInstanceStruct,S);

/* Type Definitions */

/* Named Constants */
#define CALL_EVENT                     (-1)

/* Variable Declarations */

/* Variable Definitions */
static const char * c1_debug_family_names[39] = { "m_B", "g", "Ixx", "Iyy",
  "Izz", "Ixy", "Ixz", "Iyz", "d_RL_x", "d_T_x", "d_RL_y", "d_T_y", "d_RL_z",
  "d_T_z", "h_RL", "h_T", "rho", "w_chord", "w_span", "w_ac_x", "w_ac_y",
  "ca_m_right", "ca_m_left", "ca_m_tail", "ca_s_right", "ca_s_left", "ca_s_tail",
  "nargin", "nargout", "u_ctrl_x", "u_ctrl_y", "u_ctrl_z", "PolyRL_U2R",
  "PolyT_U2R", "TTR_mdl_Properties_arr", "Ts_slow", "CA_M_X", "CA_S_X",
  "phi_ref" };

/* Function Declarations */
static void initialize_c1_TTR_mdl(SFc1_TTR_mdlInstanceStruct *chartInstance);
static void initialize_params_c1_TTR_mdl(SFc1_TTR_mdlInstanceStruct
  *chartInstance);
static void enable_c1_TTR_mdl(SFc1_TTR_mdlInstanceStruct *chartInstance);
static void disable_c1_TTR_mdl(SFc1_TTR_mdlInstanceStruct *chartInstance);
static void c1_update_debugger_state_c1_TTR_mdl(SFc1_TTR_mdlInstanceStruct
  *chartInstance);
static const mxArray *get_sim_state_c1_TTR_mdl(SFc1_TTR_mdlInstanceStruct
  *chartInstance);
static void set_sim_state_c1_TTR_mdl(SFc1_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c1_st);
static void finalize_c1_TTR_mdl(SFc1_TTR_mdlInstanceStruct *chartInstance);
static void sf_c1_TTR_mdl(SFc1_TTR_mdlInstanceStruct *chartInstance);
static void initSimStructsc1_TTR_mdl(SFc1_TTR_mdlInstanceStruct *chartInstance);
static void registerMessagesc1_TTR_mdl(SFc1_TTR_mdlInstanceStruct *chartInstance);
static void init_script_number_translation(uint32_T c1_machineNumber, uint32_T
  c1_chartNumber);
static const mxArray *c1_sf_marshallOut(void *chartInstanceVoid, void *c1_inData);
static real_T c1_emlrt_marshallIn(SFc1_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c1_phi_ref, const char_T *c1_identifier);
static real_T c1_b_emlrt_marshallIn(SFc1_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId);
static void c1_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static const mxArray *c1_b_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static void c1_c_emlrt_marshallIn(SFc1_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c1_CA_S_X, const char_T *c1_identifier, real_T c1_y[3]);
static void c1_d_emlrt_marshallIn(SFc1_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId, real_T c1_y[3]);
static void c1_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static const mxArray *c1_c_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static void c1_e_emlrt_marshallIn(SFc1_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId, real_T c1_y[21]);
static void c1_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static const mxArray *c1_d_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static void c1_f_emlrt_marshallIn(SFc1_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId, real_T c1_y[2]);
static void c1_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static const mxArray *c1_e_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static int32_T c1_g_emlrt_marshallIn(SFc1_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId);
static void c1_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static uint8_T c1_h_emlrt_marshallIn(SFc1_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c1_b_is_active_c1_TTR_mdl, const char_T *c1_identifier);
static uint8_T c1_i_emlrt_marshallIn(SFc1_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId);
static void init_dsm_address_info(SFc1_TTR_mdlInstanceStruct *chartInstance);

/* Function Definitions */
static void initialize_c1_TTR_mdl(SFc1_TTR_mdlInstanceStruct *chartInstance)
{
  chartInstance->c1_sfEvent = CALL_EVENT;
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  chartInstance->c1_is_active_c1_TTR_mdl = 0U;
}

static void initialize_params_c1_TTR_mdl(SFc1_TTR_mdlInstanceStruct
  *chartInstance)
{
  real_T c1_dv0[2];
  int32_T c1_i0;
  real_T c1_dv1[2];
  int32_T c1_i1;
  real_T c1_dv2[21];
  int32_T c1_i2;
  real_T c1_d0;
  sf_set_error_prefix_string(
    "Error evaluating data 'PolyRL_U2R' in the parent workspace.\n");
  sf_mex_import_named("PolyRL_U2R", sf_mex_get_sfun_param(chartInstance->S, 0, 0),
                      c1_dv0, 0, 0, 0U, 1, 0U, 2, 1, 2);
  for (c1_i0 = 0; c1_i0 < 2; c1_i0++) {
    chartInstance->c1_PolyRL_U2R[c1_i0] = c1_dv0[c1_i0];
  }

  sf_set_error_prefix_string("Stateflow Runtime Error (chart): ");
  sf_set_error_prefix_string(
    "Error evaluating data 'PolyT_U2R' in the parent workspace.\n");
  sf_mex_import_named("PolyT_U2R", sf_mex_get_sfun_param(chartInstance->S, 1, 0),
                      c1_dv1, 0, 0, 0U, 1, 0U, 2, 1, 2);
  for (c1_i1 = 0; c1_i1 < 2; c1_i1++) {
    chartInstance->c1_PolyT_U2R[c1_i1] = c1_dv1[c1_i1];
  }

  sf_set_error_prefix_string("Stateflow Runtime Error (chart): ");
  sf_set_error_prefix_string(
    "Error evaluating data 'TTR_mdl_Properties_arr' in the parent workspace.\n");
  sf_mex_import_named("TTR_mdl_Properties_arr", sf_mex_get_sfun_param
                      (chartInstance->S, 2, 0), c1_dv2, 0, 0, 0U, 1, 0U, 1, 21);
  for (c1_i2 = 0; c1_i2 < 21; c1_i2++) {
    chartInstance->c1_TTR_mdl_Properties_arr[c1_i2] = c1_dv2[c1_i2];
  }

  sf_set_error_prefix_string("Stateflow Runtime Error (chart): ");
  sf_set_error_prefix_string(
    "Error evaluating data 'Ts_slow' in the parent workspace.\n");
  sf_mex_import_named("Ts_slow", sf_mex_get_sfun_param(chartInstance->S, 3, 0),
                      &c1_d0, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c1_Ts_slow = c1_d0;
  sf_set_error_prefix_string("Stateflow Runtime Error (chart): ");
}

static void enable_c1_TTR_mdl(SFc1_TTR_mdlInstanceStruct *chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void disable_c1_TTR_mdl(SFc1_TTR_mdlInstanceStruct *chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void c1_update_debugger_state_c1_TTR_mdl(SFc1_TTR_mdlInstanceStruct
  *chartInstance)
{
}

static const mxArray *get_sim_state_c1_TTR_mdl(SFc1_TTR_mdlInstanceStruct
  *chartInstance)
{
  const mxArray *c1_st;
  const mxArray *c1_y = NULL;
  int32_T c1_i3;
  real_T c1_u[3];
  const mxArray *c1_b_y = NULL;
  int32_T c1_i4;
  real_T c1_b_u[3];
  const mxArray *c1_c_y = NULL;
  real_T c1_hoistedGlobal;
  real_T c1_c_u;
  const mxArray *c1_d_y = NULL;
  uint8_T c1_b_hoistedGlobal;
  uint8_T c1_d_u;
  const mxArray *c1_e_y = NULL;
  real_T *c1_phi_ref;
  real_T (*c1_CA_S_X)[3];
  real_T (*c1_CA_M_X)[3];
  c1_phi_ref = (real_T *)ssGetOutputPortSignal(chartInstance->S, 3);
  c1_CA_S_X = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S, 2);
  c1_CA_M_X = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S, 1);
  c1_st = NULL;
  c1_st = NULL;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_createcellarray(4), FALSE);
  for (c1_i3 = 0; c1_i3 < 3; c1_i3++) {
    c1_u[c1_i3] = (*c1_CA_M_X)[c1_i3];
  }

  c1_b_y = NULL;
  sf_mex_assign(&c1_b_y, sf_mex_create("y", c1_u, 0, 0U, 1U, 0U, 1, 3), FALSE);
  sf_mex_setcell(c1_y, 0, c1_b_y);
  for (c1_i4 = 0; c1_i4 < 3; c1_i4++) {
    c1_b_u[c1_i4] = (*c1_CA_S_X)[c1_i4];
  }

  c1_c_y = NULL;
  sf_mex_assign(&c1_c_y, sf_mex_create("y", c1_b_u, 0, 0U, 1U, 0U, 1, 3), FALSE);
  sf_mex_setcell(c1_y, 1, c1_c_y);
  c1_hoistedGlobal = *c1_phi_ref;
  c1_c_u = c1_hoistedGlobal;
  c1_d_y = NULL;
  sf_mex_assign(&c1_d_y, sf_mex_create("y", &c1_c_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c1_y, 2, c1_d_y);
  c1_b_hoistedGlobal = chartInstance->c1_is_active_c1_TTR_mdl;
  c1_d_u = c1_b_hoistedGlobal;
  c1_e_y = NULL;
  sf_mex_assign(&c1_e_y, sf_mex_create("y", &c1_d_u, 3, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c1_y, 3, c1_e_y);
  sf_mex_assign(&c1_st, c1_y, FALSE);
  return c1_st;
}

static void set_sim_state_c1_TTR_mdl(SFc1_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c1_st)
{
  const mxArray *c1_u;
  real_T c1_dv3[3];
  int32_T c1_i5;
  real_T c1_dv4[3];
  int32_T c1_i6;
  real_T *c1_phi_ref;
  real_T (*c1_CA_M_X)[3];
  real_T (*c1_CA_S_X)[3];
  c1_phi_ref = (real_T *)ssGetOutputPortSignal(chartInstance->S, 3);
  c1_CA_S_X = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S, 2);
  c1_CA_M_X = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S, 1);
  chartInstance->c1_doneDoubleBufferReInit = TRUE;
  c1_u = sf_mex_dup(c1_st);
  c1_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c1_u, 0)),
                        "CA_M_X", c1_dv3);
  for (c1_i5 = 0; c1_i5 < 3; c1_i5++) {
    (*c1_CA_M_X)[c1_i5] = c1_dv3[c1_i5];
  }

  c1_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c1_u, 1)),
                        "CA_S_X", c1_dv4);
  for (c1_i6 = 0; c1_i6 < 3; c1_i6++) {
    (*c1_CA_S_X)[c1_i6] = c1_dv4[c1_i6];
  }

  *c1_phi_ref = c1_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell
    (c1_u, 2)), "phi_ref");
  chartInstance->c1_is_active_c1_TTR_mdl = c1_h_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c1_u, 3)), "is_active_c1_TTR_mdl");
  sf_mex_destroy(&c1_u);
  c1_update_debugger_state_c1_TTR_mdl(chartInstance);
  sf_mex_destroy(&c1_st);
}

static void finalize_c1_TTR_mdl(SFc1_TTR_mdlInstanceStruct *chartInstance)
{
}

static void sf_c1_TTR_mdl(SFc1_TTR_mdlInstanceStruct *chartInstance)
{
  int32_T c1_i7;
  int32_T c1_i8;
  int32_T c1_i9;
  int32_T c1_i10;
  int32_T c1_i11;
  real_T c1_hoistedGlobal;
  real_T c1_b_hoistedGlobal;
  real_T c1_c_hoistedGlobal;
  real_T c1_d_hoistedGlobal;
  real_T c1_u_ctrl_x;
  real_T c1_u_ctrl_y;
  real_T c1_u_ctrl_z;
  int32_T c1_i12;
  real_T c1_b_PolyRL_U2R[2];
  int32_T c1_i13;
  real_T c1_b_PolyT_U2R[2];
  int32_T c1_i14;
  real_T c1_b_TTR_mdl_Properties_arr[21];
  real_T c1_b_Ts_slow;
  uint32_T c1_debug_family_var_map[39];
  real_T c1_m_B;
  real_T c1_g;
  real_T c1_Ixx;
  real_T c1_Iyy;
  real_T c1_Izz;
  real_T c1_Ixy;
  real_T c1_Ixz;
  real_T c1_Iyz;
  real_T c1_d_RL_x;
  real_T c1_d_T_x;
  real_T c1_d_RL_y;
  real_T c1_d_T_y;
  real_T c1_d_RL_z;
  real_T c1_d_T_z;
  real_T c1_h_RL;
  real_T c1_h_T;
  real_T c1_rho;
  real_T c1_w_chord;
  real_T c1_w_span;
  real_T c1_w_ac_x;
  real_T c1_w_ac_y;
  real_T c1_ca_m_right;
  real_T c1_ca_m_left;
  real_T c1_ca_m_tail;
  real_T c1_ca_s_right;
  real_T c1_ca_s_left;
  real_T c1_ca_s_tail;
  real_T c1_nargin = 7.0;
  real_T c1_nargout = 3.0;
  real_T c1_CA_M_X[3];
  real_T c1_CA_S_X[3];
  real_T c1_phi_ref;
  real_T c1_A;
  real_T c1_B;
  real_T c1_x;
  real_T c1_y;
  real_T c1_b_x;
  real_T c1_b_y;
  real_T c1_c_y;
  real_T c1_b;
  real_T c1_d_y;
  real_T c1_b_A;
  real_T c1_b_B;
  real_T c1_c_x;
  real_T c1_e_y;
  real_T c1_d_x;
  real_T c1_f_y;
  real_T c1_g_y;
  real_T c1_a;
  real_T c1_b_b;
  real_T c1_c_A;
  real_T c1_c_B;
  real_T c1_e_x;
  real_T c1_h_y;
  real_T c1_f_x;
  real_T c1_i_y;
  real_T c1_j_y;
  real_T c1_c_b;
  real_T c1_k_y;
  real_T c1_d_A;
  real_T c1_d_B;
  real_T c1_g_x;
  real_T c1_l_y;
  real_T c1_h_x;
  real_T c1_m_y;
  real_T c1_n_y;
  real_T c1_b_a;
  real_T c1_d_b;
  real_T c1_e_A;
  real_T c1_e_B;
  real_T c1_i_x;
  real_T c1_o_y;
  real_T c1_j_x;
  real_T c1_p_y;
  real_T c1_q_y;
  real_T c1_f_B;
  real_T c1_r_y;
  real_T c1_s_y;
  real_T c1_t_y;
  real_T c1_c_a;
  real_T c1_e_b;
  real_T c1_u_y;
  real_T c1_d_a;
  real_T c1_f_b;
  int32_T c1_i15;
  int32_T c1_i16;
  real_T *c1_b_phi_ref;
  real_T *c1_b_u_ctrl_z;
  real_T *c1_b_u_ctrl_y;
  real_T *c1_b_u_ctrl_x;
  real_T (*c1_b_CA_M_X)[3];
  real_T (*c1_b_CA_S_X)[3];
  c1_b_phi_ref = (real_T *)ssGetOutputPortSignal(chartInstance->S, 3);
  c1_b_CA_S_X = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S, 2);
  c1_b_CA_M_X = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S, 1);
  c1_b_u_ctrl_z = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
  c1_b_u_ctrl_y = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
  c1_b_u_ctrl_x = (real_T *)ssGetInputPortSignal(chartInstance->S, 0);
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 0U, chartInstance->c1_sfEvent);
  _SFD_DATA_RANGE_CHECK(*c1_b_u_ctrl_x, 0U);
  _SFD_DATA_RANGE_CHECK(*c1_b_u_ctrl_y, 1U);
  _SFD_DATA_RANGE_CHECK(*c1_b_u_ctrl_z, 2U);
  for (c1_i7 = 0; c1_i7 < 2; c1_i7++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c1_PolyRL_U2R[c1_i7], 3U);
  }

  for (c1_i8 = 0; c1_i8 < 2; c1_i8++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c1_PolyT_U2R[c1_i8], 4U);
  }

  for (c1_i9 = 0; c1_i9 < 21; c1_i9++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c1_TTR_mdl_Properties_arr[c1_i9], 5U);
  }

  for (c1_i10 = 0; c1_i10 < 3; c1_i10++) {
    _SFD_DATA_RANGE_CHECK((*c1_b_CA_M_X)[c1_i10], 6U);
  }

  for (c1_i11 = 0; c1_i11 < 3; c1_i11++) {
    _SFD_DATA_RANGE_CHECK((*c1_b_CA_S_X)[c1_i11], 7U);
  }

  _SFD_DATA_RANGE_CHECK(*c1_b_phi_ref, 8U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c1_Ts_slow, 9U);
  chartInstance->c1_sfEvent = CALL_EVENT;
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 0U, chartInstance->c1_sfEvent);
  c1_hoistedGlobal = *c1_b_u_ctrl_x;
  c1_b_hoistedGlobal = *c1_b_u_ctrl_y;
  c1_c_hoistedGlobal = *c1_b_u_ctrl_z;
  c1_d_hoistedGlobal = chartInstance->c1_Ts_slow;
  c1_u_ctrl_x = c1_hoistedGlobal;
  c1_u_ctrl_y = c1_b_hoistedGlobal;
  c1_u_ctrl_z = c1_c_hoistedGlobal;
  for (c1_i12 = 0; c1_i12 < 2; c1_i12++) {
    c1_b_PolyRL_U2R[c1_i12] = chartInstance->c1_PolyRL_U2R[c1_i12];
  }

  for (c1_i13 = 0; c1_i13 < 2; c1_i13++) {
    c1_b_PolyT_U2R[c1_i13] = chartInstance->c1_PolyT_U2R[c1_i13];
  }

  for (c1_i14 = 0; c1_i14 < 21; c1_i14++) {
    c1_b_TTR_mdl_Properties_arr[c1_i14] =
      chartInstance->c1_TTR_mdl_Properties_arr[c1_i14];
  }

  c1_b_Ts_slow = c1_d_hoistedGlobal;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 39U, 39U, c1_debug_family_names,
    c1_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_m_B, 0U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_g, 1U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_Ixx, 2U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_Iyy, 3U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_Izz, 4U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_Ixy, 5U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_Ixz, 6U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_Iyz, 7U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_d_RL_x, 8U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_d_T_x, 9U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_d_RL_y, 10U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_d_T_y, 11U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_d_RL_z, 12U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_d_T_z, 13U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_h_RL, 14U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_h_T, 15U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_rho, 16U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_w_chord, 17U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_w_span, 18U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_w_ac_x, 19U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_w_ac_y, 20U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_ca_m_right, 21U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_ca_m_left, 22U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_ca_m_tail, 23U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_ca_s_right, 24U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_ca_s_left, 25U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_ca_s_tail, 26U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_nargin, 27U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_nargout, 28U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_u_ctrl_x, 29U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_u_ctrl_y, 30U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_u_ctrl_z, 31U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c1_b_PolyRL_U2R, 32U, c1_d_sf_marshallOut,
    c1_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c1_b_PolyT_U2R, 33U, c1_d_sf_marshallOut,
    c1_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c1_b_TTR_mdl_Properties_arr, 34U,
    c1_c_sf_marshallOut, c1_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_b_Ts_slow, 35U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c1_CA_M_X, 36U, c1_b_sf_marshallOut,
    c1_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c1_CA_S_X, 37U, c1_b_sf_marshallOut,
    c1_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_phi_ref, 38U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 5);
  c1_m_B = c1_b_TTR_mdl_Properties_arr[0];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 6);
  c1_g = c1_b_TTR_mdl_Properties_arr[1];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 7);
  c1_Ixx = c1_b_TTR_mdl_Properties_arr[2];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 8);
  c1_Iyy = c1_b_TTR_mdl_Properties_arr[3];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 9);
  c1_Izz = c1_b_TTR_mdl_Properties_arr[4];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 10);
  c1_Ixy = c1_b_TTR_mdl_Properties_arr[5];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 11);
  c1_Ixz = c1_b_TTR_mdl_Properties_arr[6];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 12);
  c1_Iyz = c1_b_TTR_mdl_Properties_arr[7];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 13);
  c1_d_RL_x = c1_b_TTR_mdl_Properties_arr[8];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 14);
  c1_d_T_x = c1_b_TTR_mdl_Properties_arr[9];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 15);
  c1_d_RL_y = c1_b_TTR_mdl_Properties_arr[10];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 16);
  c1_d_T_y = c1_b_TTR_mdl_Properties_arr[11];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 17);
  c1_d_RL_z = c1_b_TTR_mdl_Properties_arr[12];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 18);
  c1_d_T_z = c1_b_TTR_mdl_Properties_arr[13];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 19);
  c1_h_RL = c1_b_TTR_mdl_Properties_arr[14];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 20);
  c1_h_T = c1_b_TTR_mdl_Properties_arr[15];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 21);
  c1_rho = c1_b_TTR_mdl_Properties_arr[16];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 22);
  c1_w_chord = c1_b_TTR_mdl_Properties_arr[17];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 23);
  c1_w_span = c1_b_TTR_mdl_Properties_arr[18];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 24);
  c1_w_ac_x = c1_b_TTR_mdl_Properties_arr[19];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 25);
  c1_w_ac_y = c1_b_TTR_mdl_Properties_arr[20];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 28);
  c1_A = c1_d_RL_x;
  c1_B = c1_d_T_x;
  c1_x = c1_A;
  c1_y = c1_B;
  c1_b_x = c1_x;
  c1_b_y = c1_y;
  c1_c_y = c1_b_x / c1_b_y;
  c1_b = 1.0 + c1_c_y;
  c1_d_y = 2.0 * c1_b;
  c1_b_A = c1_u_ctrl_z;
  c1_b_B = c1_d_y;
  c1_c_x = c1_b_A;
  c1_e_y = c1_b_B;
  c1_d_x = c1_c_x;
  c1_f_y = c1_e_y;
  c1_g_y = c1_d_x / c1_f_y;
  c1_a = c1_b_PolyRL_U2R[0];
  c1_b_b = c1_g_y;
  c1_ca_m_right = c1_a * c1_b_b;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 29);
  c1_c_A = c1_d_RL_x;
  c1_c_B = c1_d_T_x;
  c1_e_x = c1_c_A;
  c1_h_y = c1_c_B;
  c1_f_x = c1_e_x;
  c1_i_y = c1_h_y;
  c1_j_y = c1_f_x / c1_i_y;
  c1_c_b = 1.0 + c1_j_y;
  c1_k_y = 2.0 * c1_c_b;
  c1_d_A = c1_u_ctrl_z;
  c1_d_B = c1_k_y;
  c1_g_x = c1_d_A;
  c1_l_y = c1_d_B;
  c1_h_x = c1_g_x;
  c1_m_y = c1_l_y;
  c1_n_y = c1_h_x / c1_m_y;
  c1_b_a = c1_b_PolyRL_U2R[0];
  c1_d_b = c1_n_y;
  c1_ca_m_left = c1_b_a * c1_d_b;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 30);
  c1_e_A = c1_d_RL_x;
  c1_e_B = c1_d_T_x;
  c1_i_x = c1_e_A;
  c1_o_y = c1_e_B;
  c1_j_x = c1_i_x;
  c1_p_y = c1_o_y;
  c1_q_y = c1_j_x / c1_p_y;
  c1_f_B = 1.0 + c1_q_y;
  c1_r_y = c1_f_B;
  c1_s_y = c1_r_y;
  c1_t_y = 1.0 / c1_s_y;
  c1_c_a = c1_u_ctrl_z;
  c1_e_b = 1.0 - c1_t_y;
  c1_u_y = c1_c_a * c1_e_b;
  c1_d_a = c1_b_PolyT_U2R[0];
  c1_f_b = c1_u_y;
  c1_ca_m_tail = c1_d_a * c1_f_b;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 32);
  c1_ca_s_right = c1_u_ctrl_x;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 33);
  c1_ca_s_left = c1_u_ctrl_x;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 34);
  c1_ca_s_tail = 0.0;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 37);
  c1_CA_M_X[0] = c1_ca_m_right;
  c1_CA_M_X[1] = c1_ca_m_left;
  c1_CA_M_X[2] = c1_ca_m_tail;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 38);
  c1_CA_S_X[0] = c1_ca_s_right;
  c1_CA_S_X[1] = c1_ca_s_left;
  c1_CA_S_X[2] = c1_ca_s_tail;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 39);
  c1_phi_ref = c1_u_ctrl_y;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, -39);
  _SFD_SYMBOL_SCOPE_POP();
  for (c1_i15 = 0; c1_i15 < 3; c1_i15++) {
    (*c1_b_CA_M_X)[c1_i15] = c1_CA_M_X[c1_i15];
  }

  for (c1_i16 = 0; c1_i16 < 3; c1_i16++) {
    (*c1_b_CA_S_X)[c1_i16] = c1_CA_S_X[c1_i16];
  }

  *c1_b_phi_ref = c1_phi_ref;
  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 0U, chartInstance->c1_sfEvent);
  _SFD_CHECK_FOR_STATE_INCONSISTENCY(_TTR_mdlMachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
}

static void initSimStructsc1_TTR_mdl(SFc1_TTR_mdlInstanceStruct *chartInstance)
{
}

static void registerMessagesc1_TTR_mdl(SFc1_TTR_mdlInstanceStruct *chartInstance)
{
}

static void init_script_number_translation(uint32_T c1_machineNumber, uint32_T
  c1_chartNumber)
{
}

static const mxArray *c1_sf_marshallOut(void *chartInstanceVoid, void *c1_inData)
{
  const mxArray *c1_mxArrayOutData = NULL;
  real_T c1_u;
  const mxArray *c1_y = NULL;
  SFc1_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc1_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_u = *(real_T *)c1_inData;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", &c1_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, FALSE);
  return c1_mxArrayOutData;
}

static real_T c1_emlrt_marshallIn(SFc1_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c1_phi_ref, const char_T *c1_identifier)
{
  real_T c1_y;
  emlrtMsgIdentifier c1_thisId;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_y = c1_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_phi_ref), &c1_thisId);
  sf_mex_destroy(&c1_phi_ref);
  return c1_y;
}

static real_T c1_b_emlrt_marshallIn(SFc1_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId)
{
  real_T c1_y;
  real_T c1_d1;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_u), &c1_d1, 1, 0, 0U, 0, 0U, 0);
  c1_y = c1_d1;
  sf_mex_destroy(&c1_u);
  return c1_y;
}

static void c1_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_phi_ref;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_y;
  SFc1_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc1_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c1_phi_ref = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_y = c1_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_phi_ref), &c1_thisId);
  sf_mex_destroy(&c1_phi_ref);
  *(real_T *)c1_outData = c1_y;
  sf_mex_destroy(&c1_mxArrayInData);
}

static const mxArray *c1_b_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData = NULL;
  int32_T c1_i17;
  real_T c1_b_inData[3];
  int32_T c1_i18;
  real_T c1_u[3];
  const mxArray *c1_y = NULL;
  SFc1_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc1_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  for (c1_i17 = 0; c1_i17 < 3; c1_i17++) {
    c1_b_inData[c1_i17] = (*(real_T (*)[3])c1_inData)[c1_i17];
  }

  for (c1_i18 = 0; c1_i18 < 3; c1_i18++) {
    c1_u[c1_i18] = c1_b_inData[c1_i18];
  }

  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_u, 0, 0U, 1U, 0U, 1, 3), FALSE);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, FALSE);
  return c1_mxArrayOutData;
}

static void c1_c_emlrt_marshallIn(SFc1_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c1_CA_S_X, const char_T *c1_identifier, real_T c1_y[3])
{
  emlrtMsgIdentifier c1_thisId;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_CA_S_X), &c1_thisId, c1_y);
  sf_mex_destroy(&c1_CA_S_X);
}

static void c1_d_emlrt_marshallIn(SFc1_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId, real_T c1_y[3])
{
  real_T c1_dv5[3];
  int32_T c1_i19;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_u), c1_dv5, 1, 0, 0U, 1, 0U, 1, 3);
  for (c1_i19 = 0; c1_i19 < 3; c1_i19++) {
    c1_y[c1_i19] = c1_dv5[c1_i19];
  }

  sf_mex_destroy(&c1_u);
}

static void c1_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_CA_S_X;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_y[3];
  int32_T c1_i20;
  SFc1_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc1_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c1_CA_S_X = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_CA_S_X), &c1_thisId, c1_y);
  sf_mex_destroy(&c1_CA_S_X);
  for (c1_i20 = 0; c1_i20 < 3; c1_i20++) {
    (*(real_T (*)[3])c1_outData)[c1_i20] = c1_y[c1_i20];
  }

  sf_mex_destroy(&c1_mxArrayInData);
}

static const mxArray *c1_c_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData = NULL;
  int32_T c1_i21;
  real_T c1_b_inData[21];
  int32_T c1_i22;
  real_T c1_u[21];
  const mxArray *c1_y = NULL;
  SFc1_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc1_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  for (c1_i21 = 0; c1_i21 < 21; c1_i21++) {
    c1_b_inData[c1_i21] = (*(real_T (*)[21])c1_inData)[c1_i21];
  }

  for (c1_i22 = 0; c1_i22 < 21; c1_i22++) {
    c1_u[c1_i22] = c1_b_inData[c1_i22];
  }

  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_u, 0, 0U, 1U, 0U, 1, 21), FALSE);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, FALSE);
  return c1_mxArrayOutData;
}

static void c1_e_emlrt_marshallIn(SFc1_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId, real_T c1_y[21])
{
  real_T c1_dv6[21];
  int32_T c1_i23;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_u), c1_dv6, 1, 0, 0U, 1, 0U, 1, 21);
  for (c1_i23 = 0; c1_i23 < 21; c1_i23++) {
    c1_y[c1_i23] = c1_dv6[c1_i23];
  }

  sf_mex_destroy(&c1_u);
}

static void c1_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_b_TTR_mdl_Properties_arr;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_y[21];
  int32_T c1_i24;
  SFc1_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc1_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c1_b_TTR_mdl_Properties_arr = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_e_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_b_TTR_mdl_Properties_arr),
                        &c1_thisId, c1_y);
  sf_mex_destroy(&c1_b_TTR_mdl_Properties_arr);
  for (c1_i24 = 0; c1_i24 < 21; c1_i24++) {
    (*(real_T (*)[21])c1_outData)[c1_i24] = c1_y[c1_i24];
  }

  sf_mex_destroy(&c1_mxArrayInData);
}

static const mxArray *c1_d_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData = NULL;
  int32_T c1_i25;
  real_T c1_b_inData[2];
  int32_T c1_i26;
  real_T c1_u[2];
  const mxArray *c1_y = NULL;
  SFc1_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc1_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  for (c1_i25 = 0; c1_i25 < 2; c1_i25++) {
    c1_b_inData[c1_i25] = (*(real_T (*)[2])c1_inData)[c1_i25];
  }

  for (c1_i26 = 0; c1_i26 < 2; c1_i26++) {
    c1_u[c1_i26] = c1_b_inData[c1_i26];
  }

  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_u, 0, 0U, 1U, 0U, 2, 1, 2), FALSE);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, FALSE);
  return c1_mxArrayOutData;
}

static void c1_f_emlrt_marshallIn(SFc1_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId, real_T c1_y[2])
{
  real_T c1_dv7[2];
  int32_T c1_i27;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_u), c1_dv7, 1, 0, 0U, 1, 0U, 2, 1, 2);
  for (c1_i27 = 0; c1_i27 < 2; c1_i27++) {
    c1_y[c1_i27] = c1_dv7[c1_i27];
  }

  sf_mex_destroy(&c1_u);
}

static void c1_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_b_PolyT_U2R;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_y[2];
  int32_T c1_i28;
  SFc1_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc1_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c1_b_PolyT_U2R = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_f_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_b_PolyT_U2R), &c1_thisId,
                        c1_y);
  sf_mex_destroy(&c1_b_PolyT_U2R);
  for (c1_i28 = 0; c1_i28 < 2; c1_i28++) {
    (*(real_T (*)[2])c1_outData)[c1_i28] = c1_y[c1_i28];
  }

  sf_mex_destroy(&c1_mxArrayInData);
}

const mxArray *sf_c1_TTR_mdl_get_eml_resolved_functions_info(void)
{
  const mxArray *c1_nameCaptureInfo;
  c1_ResolvedFunctionInfo c1_info[5];
  c1_ResolvedFunctionInfo (*c1_b_info)[5];
  const mxArray *c1_m0 = NULL;
  int32_T c1_i29;
  c1_ResolvedFunctionInfo *c1_r0;
  c1_nameCaptureInfo = NULL;
  c1_nameCaptureInfo = NULL;
  c1_b_info = (c1_ResolvedFunctionInfo (*)[5])c1_info;
  (*c1_b_info)[0].context = "";
  (*c1_b_info)[0].name = "mrdivide";
  (*c1_b_info)[0].dominantType = "double";
  (*c1_b_info)[0].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p";
  (*c1_b_info)[0].fileTimeLo = 1357951548U;
  (*c1_b_info)[0].fileTimeHi = 0U;
  (*c1_b_info)[0].mFileTimeLo = 1319729966U;
  (*c1_b_info)[0].mFileTimeHi = 0U;
  (*c1_b_info)[1].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p";
  (*c1_b_info)[1].name = "rdivide";
  (*c1_b_info)[1].dominantType = "double";
  (*c1_b_info)[1].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m";
  (*c1_b_info)[1].fileTimeLo = 1346510388U;
  (*c1_b_info)[1].fileTimeHi = 0U;
  (*c1_b_info)[1].mFileTimeLo = 0U;
  (*c1_b_info)[1].mFileTimeHi = 0U;
  (*c1_b_info)[2].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m";
  (*c1_b_info)[2].name = "eml_scalexp_compatible";
  (*c1_b_info)[2].dominantType = "double";
  (*c1_b_info)[2].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_compatible.m";
  (*c1_b_info)[2].fileTimeLo = 1286818796U;
  (*c1_b_info)[2].fileTimeHi = 0U;
  (*c1_b_info)[2].mFileTimeLo = 0U;
  (*c1_b_info)[2].mFileTimeHi = 0U;
  (*c1_b_info)[3].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m";
  (*c1_b_info)[3].name = "eml_div";
  (*c1_b_info)[3].dominantType = "double";
  (*c1_b_info)[3].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_div.m";
  (*c1_b_info)[3].fileTimeLo = 1313347810U;
  (*c1_b_info)[3].fileTimeHi = 0U;
  (*c1_b_info)[3].mFileTimeLo = 0U;
  (*c1_b_info)[3].mFileTimeHi = 0U;
  (*c1_b_info)[4].context = "";
  (*c1_b_info)[4].name = "mtimes";
  (*c1_b_info)[4].dominantType = "double";
  (*c1_b_info)[4].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  (*c1_b_info)[4].fileTimeLo = 1289519692U;
  (*c1_b_info)[4].fileTimeHi = 0U;
  (*c1_b_info)[4].mFileTimeLo = 0U;
  (*c1_b_info)[4].mFileTimeHi = 0U;
  sf_mex_assign(&c1_m0, sf_mex_createstruct("nameCaptureInfo", 1, 5), FALSE);
  for (c1_i29 = 0; c1_i29 < 5; c1_i29++) {
    c1_r0 = &c1_info[c1_i29];
    sf_mex_addfield(c1_m0, sf_mex_create("nameCaptureInfo", c1_r0->context, 15,
      0U, 0U, 0U, 2, 1, strlen(c1_r0->context)), "context", "nameCaptureInfo",
                    c1_i29);
    sf_mex_addfield(c1_m0, sf_mex_create("nameCaptureInfo", c1_r0->name, 15, 0U,
      0U, 0U, 2, 1, strlen(c1_r0->name)), "name", "nameCaptureInfo", c1_i29);
    sf_mex_addfield(c1_m0, sf_mex_create("nameCaptureInfo", c1_r0->dominantType,
      15, 0U, 0U, 0U, 2, 1, strlen(c1_r0->dominantType)), "dominantType",
                    "nameCaptureInfo", c1_i29);
    sf_mex_addfield(c1_m0, sf_mex_create("nameCaptureInfo", c1_r0->resolved, 15,
      0U, 0U, 0U, 2, 1, strlen(c1_r0->resolved)), "resolved", "nameCaptureInfo",
                    c1_i29);
    sf_mex_addfield(c1_m0, sf_mex_create("nameCaptureInfo", &c1_r0->fileTimeLo,
      7, 0U, 0U, 0U, 0), "fileTimeLo", "nameCaptureInfo", c1_i29);
    sf_mex_addfield(c1_m0, sf_mex_create("nameCaptureInfo", &c1_r0->fileTimeHi,
      7, 0U, 0U, 0U, 0), "fileTimeHi", "nameCaptureInfo", c1_i29);
    sf_mex_addfield(c1_m0, sf_mex_create("nameCaptureInfo", &c1_r0->mFileTimeLo,
      7, 0U, 0U, 0U, 0), "mFileTimeLo", "nameCaptureInfo", c1_i29);
    sf_mex_addfield(c1_m0, sf_mex_create("nameCaptureInfo", &c1_r0->mFileTimeHi,
      7, 0U, 0U, 0U, 0), "mFileTimeHi", "nameCaptureInfo", c1_i29);
  }

  sf_mex_assign(&c1_nameCaptureInfo, c1_m0, FALSE);
  sf_mex_emlrtNameCapturePostProcessR2012a(&c1_nameCaptureInfo);
  return c1_nameCaptureInfo;
}

static const mxArray *c1_e_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData = NULL;
  int32_T c1_u;
  const mxArray *c1_y = NULL;
  SFc1_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc1_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_u = *(int32_T *)c1_inData;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", &c1_u, 6, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, FALSE);
  return c1_mxArrayOutData;
}

static int32_T c1_g_emlrt_marshallIn(SFc1_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId)
{
  int32_T c1_y;
  int32_T c1_i30;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_u), &c1_i30, 1, 6, 0U, 0, 0U, 0);
  c1_y = c1_i30;
  sf_mex_destroy(&c1_u);
  return c1_y;
}

static void c1_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_b_sfEvent;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  int32_T c1_y;
  SFc1_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc1_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c1_b_sfEvent = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_y = c1_g_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_b_sfEvent),
    &c1_thisId);
  sf_mex_destroy(&c1_b_sfEvent);
  *(int32_T *)c1_outData = c1_y;
  sf_mex_destroy(&c1_mxArrayInData);
}

static uint8_T c1_h_emlrt_marshallIn(SFc1_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c1_b_is_active_c1_TTR_mdl, const char_T *c1_identifier)
{
  uint8_T c1_y;
  emlrtMsgIdentifier c1_thisId;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_y = c1_i_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c1_b_is_active_c1_TTR_mdl), &c1_thisId);
  sf_mex_destroy(&c1_b_is_active_c1_TTR_mdl);
  return c1_y;
}

static uint8_T c1_i_emlrt_marshallIn(SFc1_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId)
{
  uint8_T c1_y;
  uint8_T c1_u0;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_u), &c1_u0, 1, 3, 0U, 0, 0U, 0);
  c1_y = c1_u0;
  sf_mex_destroy(&c1_u);
  return c1_y;
}

static void init_dsm_address_info(SFc1_TTR_mdlInstanceStruct *chartInstance)
{
}

/* SFunction Glue Code */
#ifdef utFree
#undef utFree
#endif

#ifdef utMalloc
#undef utMalloc
#endif

#ifdef __cplusplus

extern "C" void *utMalloc(size_t size);
extern "C" void utFree(void*);

#else

extern void *utMalloc(size_t size);
extern void utFree(void*);

#endif

void sf_c1_TTR_mdl_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(957293239U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(1757247049U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(292570765U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(4053926843U);
}

mxArray *sf_c1_TTR_mdl_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1,1,5,
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("mJNqT7T5jwlt4adrYvtKPB");
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,3,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,2,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,2,"type",mxType);
    }

    mxSetField(mxData,2,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"inputs",mxData);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,4,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(2);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(2);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(21);
      pr[1] = (double)(1);
      mxSetField(mxData,2,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,2,"type",mxType);
    }

    mxSetField(mxData,2,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,3,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,3,"type",mxType);
    }

    mxSetField(mxData,3,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"parameters",mxData);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,3,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(3);
      pr[1] = (double)(1);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(3);
      pr[1] = (double)(1);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,2,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,2,"type",mxType);
    }

    mxSetField(mxData,2,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"outputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"locals",mxCreateDoubleMatrix(0,0,mxREAL));
  }

  return(mxAutoinheritanceInfo);
}

mxArray *sf_c1_TTR_mdl_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,0);
  return(mxcell3p);
}

static const mxArray *sf_get_sim_state_info_c1_TTR_mdl(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x4'type','srcId','name','auxInfo'{{M[1],M[5],T\"CA_M_X\",},{M[1],M[12],T\"CA_S_X\",},{M[1],M[68],T\"phi_ref\",},{M[8],M[0],T\"is_active_c1_TTR_mdl\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 4, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c1_TTR_mdl_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc1_TTR_mdlInstanceStruct *chartInstance;
    chartInstance = (SFc1_TTR_mdlInstanceStruct *) ((ChartInfoStruct *)
      (ssGetUserData(S)))->chartInstance;
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (sfGlobalDebugInstanceStruct,
           _TTR_mdlMachineNumber_,
           1,
           1,
           1,
           10,
           0,
           0,
           0,
           0,
           0,
           &(chartInstance->chartNumber),
           &(chartInstance->instanceNumber),
           ssGetPath(S),
           (void *)S);
        if (chartAlreadyPresent==0) {
          /* this is the first instance */
          init_script_number_translation(_TTR_mdlMachineNumber_,
            chartInstance->chartNumber);
          sf_debug_set_chart_disable_implicit_casting
            (sfGlobalDebugInstanceStruct,_TTR_mdlMachineNumber_,
             chartInstance->chartNumber,1);
          sf_debug_set_chart_event_thresholds(sfGlobalDebugInstanceStruct,
            _TTR_mdlMachineNumber_,
            chartInstance->chartNumber,
            0,
            0,
            0);
          _SFD_SET_DATA_PROPS(0,1,1,0,"u_ctrl_x");
          _SFD_SET_DATA_PROPS(1,1,1,0,"u_ctrl_y");
          _SFD_SET_DATA_PROPS(2,1,1,0,"u_ctrl_z");
          _SFD_SET_DATA_PROPS(3,10,0,0,"PolyRL_U2R");
          _SFD_SET_DATA_PROPS(4,10,0,0,"PolyT_U2R");
          _SFD_SET_DATA_PROPS(5,10,0,0,"TTR_mdl_Properties_arr");
          _SFD_SET_DATA_PROPS(6,2,0,1,"CA_M_X");
          _SFD_SET_DATA_PROPS(7,2,0,1,"CA_S_X");
          _SFD_SET_DATA_PROPS(8,2,0,1,"phi_ref");
          _SFD_SET_DATA_PROPS(9,10,0,0,"Ts_slow");
          _SFD_STATE_INFO(0,0,2);
          _SFD_CH_SUBSTATE_COUNT(0);
          _SFD_CH_SUBSTATE_DECOMP(0);
        }

        _SFD_CV_INIT_CHART(0,0,0,0);

        {
          _SFD_CV_INIT_STATE(0,0,0,0,0,0,NULL,NULL);
        }

        _SFD_CV_INIT_TRANS(0,0,NULL,NULL,0,NULL);

        /* Initialization of MATLAB Function Model Coverage */
        _SFD_CV_INIT_EML(0,1,1,0,0,0,0,0,0,0,0);
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,1302);
        _SFD_TRANS_COV_WTS(0,0,0,1,0);
        if (chartAlreadyPresent==0) {
          _SFD_TRANS_COV_MAPS(0,
                              0,NULL,NULL,
                              0,NULL,NULL,
                              1,NULL,NULL,
                              0,NULL,NULL);
        }

        _SFD_SET_DATA_COMPILED_PROPS(0,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(1,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(2,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);

        {
          unsigned int dimVector[2];
          dimVector[0]= 1;
          dimVector[1]= 2;
          _SFD_SET_DATA_COMPILED_PROPS(3,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_d_sf_marshallOut,(MexInFcnForType)
            c1_d_sf_marshallIn);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 1;
          dimVector[1]= 2;
          _SFD_SET_DATA_COMPILED_PROPS(4,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_d_sf_marshallOut,(MexInFcnForType)
            c1_d_sf_marshallIn);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 21;
          _SFD_SET_DATA_COMPILED_PROPS(5,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_c_sf_marshallOut,(MexInFcnForType)
            c1_c_sf_marshallIn);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 3;
          _SFD_SET_DATA_COMPILED_PROPS(6,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_b_sf_marshallOut,(MexInFcnForType)
            c1_b_sf_marshallIn);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 3;
          _SFD_SET_DATA_COMPILED_PROPS(7,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_b_sf_marshallOut,(MexInFcnForType)
            c1_b_sf_marshallIn);
        }

        _SFD_SET_DATA_COMPILED_PROPS(8,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)c1_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(9,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)c1_sf_marshallIn);

        {
          real_T *c1_u_ctrl_x;
          real_T *c1_u_ctrl_y;
          real_T *c1_u_ctrl_z;
          real_T *c1_phi_ref;
          real_T (*c1_CA_M_X)[3];
          real_T (*c1_CA_S_X)[3];
          c1_phi_ref = (real_T *)ssGetOutputPortSignal(chartInstance->S, 3);
          c1_CA_S_X = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S, 2);
          c1_CA_M_X = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S, 1);
          c1_u_ctrl_z = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
          c1_u_ctrl_y = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
          c1_u_ctrl_x = (real_T *)ssGetInputPortSignal(chartInstance->S, 0);
          _SFD_SET_DATA_VALUE_PTR(0U, c1_u_ctrl_x);
          _SFD_SET_DATA_VALUE_PTR(1U, c1_u_ctrl_y);
          _SFD_SET_DATA_VALUE_PTR(2U, c1_u_ctrl_z);
          _SFD_SET_DATA_VALUE_PTR(3U, chartInstance->c1_PolyRL_U2R);
          _SFD_SET_DATA_VALUE_PTR(4U, chartInstance->c1_PolyT_U2R);
          _SFD_SET_DATA_VALUE_PTR(5U, chartInstance->c1_TTR_mdl_Properties_arr);
          _SFD_SET_DATA_VALUE_PTR(6U, *c1_CA_M_X);
          _SFD_SET_DATA_VALUE_PTR(7U, *c1_CA_S_X);
          _SFD_SET_DATA_VALUE_PTR(8U, c1_phi_ref);
          _SFD_SET_DATA_VALUE_PTR(9U, &chartInstance->c1_Ts_slow);
        }
      }
    } else {
      sf_debug_reset_current_state_configuration(sfGlobalDebugInstanceStruct,
        _TTR_mdlMachineNumber_,chartInstance->chartNumber,
        chartInstance->instanceNumber);
    }
  }
}

static const char* sf_get_instance_specialization(void)
{
  return "lnhvlkCcdCLiaAL6GJ0PEH";
}

static void sf_opaque_initialize_c1_TTR_mdl(void *chartInstanceVar)
{
  chart_debug_initialization(((SFc1_TTR_mdlInstanceStruct*) chartInstanceVar)->S,
    0);
  initialize_params_c1_TTR_mdl((SFc1_TTR_mdlInstanceStruct*) chartInstanceVar);
  initialize_c1_TTR_mdl((SFc1_TTR_mdlInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_enable_c1_TTR_mdl(void *chartInstanceVar)
{
  enable_c1_TTR_mdl((SFc1_TTR_mdlInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_disable_c1_TTR_mdl(void *chartInstanceVar)
{
  disable_c1_TTR_mdl((SFc1_TTR_mdlInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_gateway_c1_TTR_mdl(void *chartInstanceVar)
{
  sf_c1_TTR_mdl((SFc1_TTR_mdlInstanceStruct*) chartInstanceVar);
}

extern const mxArray* sf_internal_get_sim_state_c1_TTR_mdl(SimStruct* S)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_raw2high");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = (mxArray*) get_sim_state_c1_TTR_mdl((SFc1_TTR_mdlInstanceStruct*)
    chartInfo->chartInstance);         /* raw sim ctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c1_TTR_mdl();/* state var info */
  mxError = sf_mex_call_matlab(1, plhs, 4, prhs, "sfprivate");
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  mxDestroyArray(prhs[3]);
  if (mxError || plhs[0] == NULL) {
    sf_mex_error_message("Stateflow Internal Error: \nError calling 'chart_simctx_raw2high'.\n");
  }

  return plhs[0];
}

extern void sf_internal_set_sim_state_c1_TTR_mdl(SimStruct* S, const mxArray *st)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_high2raw");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = mxDuplicateArray(st);      /* high level simctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c1_TTR_mdl();/* state var info */
  mxError = sf_mex_call_matlab(1, plhs, 4, prhs, "sfprivate");
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  mxDestroyArray(prhs[3]);
  if (mxError || plhs[0] == NULL) {
    sf_mex_error_message("Stateflow Internal Error: \nError calling 'chart_simctx_high2raw'.\n");
  }

  set_sim_state_c1_TTR_mdl((SFc1_TTR_mdlInstanceStruct*)chartInfo->chartInstance,
    mxDuplicateArray(plhs[0]));
  mxDestroyArray(plhs[0]);
}

static const mxArray* sf_opaque_get_sim_state_c1_TTR_mdl(SimStruct* S)
{
  return sf_internal_get_sim_state_c1_TTR_mdl(S);
}

static void sf_opaque_set_sim_state_c1_TTR_mdl(SimStruct* S, const mxArray *st)
{
  sf_internal_set_sim_state_c1_TTR_mdl(S, st);
}

static void sf_opaque_terminate_c1_TTR_mdl(void *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc1_TTR_mdlInstanceStruct*) chartInstanceVar)->S;
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_TTR_mdl_optimization_info();
    }

    finalize_c1_TTR_mdl((SFc1_TTR_mdlInstanceStruct*) chartInstanceVar);
    utFree((void *)chartInstanceVar);
    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc1_TTR_mdl((SFc1_TTR_mdlInstanceStruct*) chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c1_TTR_mdl(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    initialize_params_c1_TTR_mdl((SFc1_TTR_mdlInstanceStruct*)(((ChartInfoStruct
      *)ssGetUserData(S))->chartInstance));
  }
}

static void mdlSetWorkWidths_c1_TTR_mdl(SimStruct *S)
{
  /* Actual parameters from chart:
     PolyRL_U2R PolyT_U2R TTR_mdl_Properties_arr Ts_slow
   */
  const char_T *rtParamNames[] = { "PolyRL_U2R", "PolyT_U2R",
    "TTR_mdl_Properties_arr", "Ts_slow" };

  ssSetNumRunTimeParams(S,ssGetSFcnParamsCount(S));

  /* registration for PolyRL_U2R*/
  ssRegDlgParamAsRunTimeParam(S, 0, 0, rtParamNames[0], SS_DOUBLE);

  /* registration for PolyT_U2R*/
  ssRegDlgParamAsRunTimeParam(S, 1, 1, rtParamNames[1], SS_DOUBLE);

  /* registration for TTR_mdl_Properties_arr*/
  ssRegDlgParamAsRunTimeParam(S, 2, 2, rtParamNames[2], SS_DOUBLE);

  /* registration for Ts_slow*/
  ssRegDlgParamAsRunTimeParam(S, 3, 3, rtParamNames[3], SS_DOUBLE);
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_TTR_mdl_optimization_info();
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable(S,sf_get_instance_specialization(),infoStruct,
      1);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,sf_rtw_info_uint_prop(S,sf_get_instance_specialization(),
                infoStruct,1,"RTWCG"));
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop(S,
      sf_get_instance_specialization(),infoStruct,1,
      "gatewayCannotBeInlinedMultipleTimes"));
    sf_update_buildInfo(S,sf_get_instance_specialization(),infoStruct,1);
    if (chartIsInlinable) {
      ssSetInputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 1, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 2, SS_REUSABLE_AND_LOCAL);
      sf_mark_chart_expressionable_inputs(S,sf_get_instance_specialization(),
        infoStruct,1,3);
      sf_mark_chart_reusable_outputs(S,sf_get_instance_specialization(),
        infoStruct,1,3);
    }

    {
      unsigned int outPortIdx;
      for (outPortIdx=1; outPortIdx<=3; ++outPortIdx) {
        ssSetOutputPortOptimizeInIR(S, outPortIdx, 1U);
      }
    }

    {
      unsigned int inPortIdx;
      for (inPortIdx=0; inPortIdx < 3; ++inPortIdx) {
        ssSetInputPortOptimizeInIR(S, inPortIdx, 1U);
      }
    }

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,1);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(3180626128U));
  ssSetChecksum1(S,(3937810243U));
  ssSetChecksum2(S,(3387145616U));
  ssSetChecksum3(S,(3879141706U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
  ssSupportsMultipleExecInstances(S,1);
}

static void mdlRTW_c1_TTR_mdl(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c1_TTR_mdl(SimStruct *S)
{
  SFc1_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc1_TTR_mdlInstanceStruct *)utMalloc(sizeof
    (SFc1_TTR_mdlInstanceStruct));
  memset(chartInstance, 0, sizeof(SFc1_TTR_mdlInstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway = sf_opaque_gateway_c1_TTR_mdl;
  chartInstance->chartInfo.initializeChart = sf_opaque_initialize_c1_TTR_mdl;
  chartInstance->chartInfo.terminateChart = sf_opaque_terminate_c1_TTR_mdl;
  chartInstance->chartInfo.enableChart = sf_opaque_enable_c1_TTR_mdl;
  chartInstance->chartInfo.disableChart = sf_opaque_disable_c1_TTR_mdl;
  chartInstance->chartInfo.getSimState = sf_opaque_get_sim_state_c1_TTR_mdl;
  chartInstance->chartInfo.setSimState = sf_opaque_set_sim_state_c1_TTR_mdl;
  chartInstance->chartInfo.getSimStateInfo = sf_get_sim_state_info_c1_TTR_mdl;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c1_TTR_mdl;
  chartInstance->chartInfo.mdlStart = mdlStart_c1_TTR_mdl;
  chartInstance->chartInfo.mdlSetWorkWidths = mdlSetWorkWidths_c1_TTR_mdl;
  chartInstance->chartInfo.extModeExec = NULL;
  chartInstance->chartInfo.restoreLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.restoreBeforeLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.storeCurrentConfiguration = NULL;
  chartInstance->S = S;
  ssSetUserData(S,(void *)(&(chartInstance->chartInfo)));/* register the chart instance with simstruct */
  init_dsm_address_info(chartInstance);
  if (!sim_mode_is_rtw_gen(S)) {
  }

  sf_opaque_init_subchart_simstructs(chartInstance->chartInfo.chartInstance);
  chart_debug_initialization(S,1);
}

void c1_TTR_mdl_method_dispatcher(SimStruct *S, int_T method, void *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c1_TTR_mdl(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c1_TTR_mdl(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c1_TTR_mdl(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c1_TTR_mdl_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
