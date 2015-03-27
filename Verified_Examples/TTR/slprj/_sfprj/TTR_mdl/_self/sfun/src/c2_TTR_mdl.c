/* Include files */

#include <stddef.h>
#include "blas.h"
#include "TTR_mdl_sfun.h"
#include "c2_TTR_mdl.h"
#include "mwmathutil.h"
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance->chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance->instanceNumber)
#include "TTR_mdl_sfun_debug_macros.h"
#define _SF_MEX_LISTEN_FOR_CTRL_C(S)   sf_mex_listen_for_ctrl_c(sfGlobalDebugInstanceStruct,S);

/* Type Definitions */

/* Named Constants */
#define CALL_EVENT                     (-1)

/* Variable Declarations */

/* Variable Definitions */
static const char * c2_debug_family_names[79] = { "Ts", "Ki_roll", "Kp_roll_LU",
  "Kd_roll", "Kg_roll", "Ki_pitch", "Kp_pitch_LU", "Kd_pitch", "Kg_pitch",
  "Ki_yaw", "Kp_yaw", "Kd_yaw", "Kg_yaw", "phi", "theta", "psi", "phi_dot",
  "theta_dot", "psi_dot", "phi_die", "theta_die", "psi_die", "phi_d", "theta_d",
  "psi_d", "phi_err", "phi_dot_err", "phi_die_err", "theta_err", "theta_dot_err",
  "theta_die_err", "psi_err", "psi_dot_err", "psi_die_err", "Int_Leakage_Theta",
  "integrator_switch", "Yaw_Error_constraint", "Phi_Error_Int_constraint",
  "Theta_Error_Int_constraint", "Psi_Error_Int_constraint",
  "Phi_Integrator_constraint", "Theta_Integrator_constraint",
  "Psi_Integrator_constraint", "a", "phi_err_intcnstr", "theta_err_intcnstr",
  "psi_err_intcnstr", "LUx", "Kp_roll", "Kp_pitch", "roll_control",
  "pitch_control", "yaw_control", "PHI_ctrl", "ca_m_right", "ca_m_left",
  "ca_m_tail", "ca_s_right", "ca_s_left", "ca_s_tail", "nargin", "nargout", "t",
  "Theta", "Theta_Dot", "Theta_Die", "Theta_d", "PolyRL_U2R", "PolyT_U2R",
  "PolyGT_U2A", "LQR_phi", "CA_M_Theta", "CA_S_Theta", "t_km1", "CA_M_Theta_km1",
  "CA_S_Theta_km1", "phi_err_int", "theta_err_int", "psi_err_int" };

/* Function Declarations */
static void initialize_c2_TTR_mdl(SFc2_TTR_mdlInstanceStruct *chartInstance);
static void initialize_params_c2_TTR_mdl(SFc2_TTR_mdlInstanceStruct
  *chartInstance);
static void enable_c2_TTR_mdl(SFc2_TTR_mdlInstanceStruct *chartInstance);
static void disable_c2_TTR_mdl(SFc2_TTR_mdlInstanceStruct *chartInstance);
static void c2_update_debugger_state_c2_TTR_mdl(SFc2_TTR_mdlInstanceStruct
  *chartInstance);
static const mxArray *get_sim_state_c2_TTR_mdl(SFc2_TTR_mdlInstanceStruct
  *chartInstance);
static void set_sim_state_c2_TTR_mdl(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_st);
static void finalize_c2_TTR_mdl(SFc2_TTR_mdlInstanceStruct *chartInstance);
static void sf_c2_TTR_mdl(SFc2_TTR_mdlInstanceStruct *chartInstance);
static void c2_chartstep_c2_TTR_mdl(SFc2_TTR_mdlInstanceStruct *chartInstance);
static void initSimStructsc2_TTR_mdl(SFc2_TTR_mdlInstanceStruct *chartInstance);
static void registerMessagesc2_TTR_mdl(SFc2_TTR_mdlInstanceStruct *chartInstance);
static void init_script_number_translation(uint32_T c2_machineNumber, uint32_T
  c2_chartNumber);
static const mxArray *c2_sf_marshallOut(void *chartInstanceVoid, void *c2_inData);
static real_T c2_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_b_psi_err_int, const char_T *c2_identifier);
static real_T c2_b_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId);
static void c2_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_b_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static real_T c2_c_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_b_theta_err_int, const char_T *c2_identifier);
static real_T c2_d_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId);
static void c2_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_c_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static real_T c2_e_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_b_phi_err_int, const char_T *c2_identifier);
static real_T c2_f_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId);
static void c2_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_d_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static void c2_g_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_b_CA_S_Theta_km1, const char_T *c2_identifier, real_T c2_y[3]);
static void c2_h_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId, real_T c2_y[3]);
static void c2_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_e_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static void c2_i_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_b_CA_M_Theta_km1, const char_T *c2_identifier, real_T c2_y[3]);
static void c2_j_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId, real_T c2_y[3]);
static void c2_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_f_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static real_T c2_k_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_b_t_km1, const char_T *c2_identifier);
static real_T c2_l_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId);
static void c2_f_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_g_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static void c2_m_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_CA_S_Theta, const char_T *c2_identifier, real_T c2_y[3]);
static void c2_n_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId, real_T c2_y[3]);
static void c2_g_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_h_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static void c2_o_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId, real_T c2_y[4]);
static void c2_h_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_i_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static void c2_p_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId, real_T c2_y[2]);
static void c2_i_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_j_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static real_T c2_q_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId);
static void c2_j_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_k_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static void c2_r_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId, real_T c2_y[5]);
static void c2_k_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_l_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static void c2_info_helper(c2_ResolvedFunctionInfo c2_info[46]);
static real_T c2_abs(SFc2_TTR_mdlInstanceStruct *chartInstance, real_T c2_x);
static real_T c2_interp1(SFc2_TTR_mdlInstanceStruct *chartInstance, real_T
  c2_varargin_1[3], real_T c2_varargin_2[3], real_T c2_varargin_3, real_T
  c2_varargin_5);
static void c2_eml_error(SFc2_TTR_mdlInstanceStruct *chartInstance);
static void c2_b_eml_error(SFc2_TTR_mdlInstanceStruct *chartInstance);
static void c2_eml_scalar_eg(SFc2_TTR_mdlInstanceStruct *chartInstance);
static const mxArray *c2_m_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static int32_T c2_s_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId);
static void c2_l_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static uint8_T c2_t_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_b_is_active_c2_TTR_mdl, const char_T *c2_identifier);
static uint8_T c2_u_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId);
static void init_dsm_address_info(SFc2_TTR_mdlInstanceStruct *chartInstance);

/* Function Definitions */
static void initialize_c2_TTR_mdl(SFc2_TTR_mdlInstanceStruct *chartInstance)
{
  chartInstance->c2_sfEvent = CALL_EVENT;
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  chartInstance->c2_t_km1_not_empty = FALSE;
  chartInstance->c2_CA_M_Theta_km1_not_empty = FALSE;
  chartInstance->c2_CA_S_Theta_km1_not_empty = FALSE;
  chartInstance->c2_phi_err_int_not_empty = FALSE;
  chartInstance->c2_theta_err_int_not_empty = FALSE;
  chartInstance->c2_psi_err_int_not_empty = FALSE;
  chartInstance->c2_is_active_c2_TTR_mdl = 0U;
}

static void initialize_params_c2_TTR_mdl(SFc2_TTR_mdlInstanceStruct
  *chartInstance)
{
  real_T c2_dv0[2];
  int32_T c2_i0;
  real_T c2_dv1[2];
  int32_T c2_i1;
  real_T c2_dv2[2];
  int32_T c2_i2;
  real_T c2_dv3[4];
  int32_T c2_i3;
  sf_set_error_prefix_string(
    "Error evaluating data 'PolyRL_U2R' in the parent workspace.\n");
  sf_mex_import_named("PolyRL_U2R", sf_mex_get_sfun_param(chartInstance->S, 2, 0),
                      c2_dv0, 0, 0, 0U, 1, 0U, 2, 1, 2);
  for (c2_i0 = 0; c2_i0 < 2; c2_i0++) {
    chartInstance->c2_PolyRL_U2R[c2_i0] = c2_dv0[c2_i0];
  }

  sf_set_error_prefix_string("Stateflow Runtime Error (chart): ");
  sf_set_error_prefix_string(
    "Error evaluating data 'PolyT_U2R' in the parent workspace.\n");
  sf_mex_import_named("PolyT_U2R", sf_mex_get_sfun_param(chartInstance->S, 3, 0),
                      c2_dv1, 0, 0, 0U, 1, 0U, 2, 1, 2);
  for (c2_i1 = 0; c2_i1 < 2; c2_i1++) {
    chartInstance->c2_PolyT_U2R[c2_i1] = c2_dv1[c2_i1];
  }

  sf_set_error_prefix_string("Stateflow Runtime Error (chart): ");
  sf_set_error_prefix_string(
    "Error evaluating data 'PolyGT_U2A' in the parent workspace.\n");
  sf_mex_import_named("PolyGT_U2A", sf_mex_get_sfun_param(chartInstance->S, 1, 0),
                      c2_dv2, 0, 0, 0U, 1, 0U, 2, 1, 2);
  for (c2_i2 = 0; c2_i2 < 2; c2_i2++) {
    chartInstance->c2_PolyGT_U2A[c2_i2] = c2_dv2[c2_i2];
  }

  sf_set_error_prefix_string("Stateflow Runtime Error (chart): ");
  sf_set_error_prefix_string(
    "Error evaluating data 'LQR_phi' in the parent workspace.\n");
  sf_mex_import_named("LQR_phi", sf_mex_get_sfun_param(chartInstance->S, 0, 0),
                      c2_dv3, 0, 0, 0U, 1, 0U, 2, 1, 4);
  for (c2_i3 = 0; c2_i3 < 4; c2_i3++) {
    chartInstance->c2_LQR_phi[c2_i3] = c2_dv3[c2_i3];
  }

  sf_set_error_prefix_string("Stateflow Runtime Error (chart): ");
}

static void enable_c2_TTR_mdl(SFc2_TTR_mdlInstanceStruct *chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void disable_c2_TTR_mdl(SFc2_TTR_mdlInstanceStruct *chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void c2_update_debugger_state_c2_TTR_mdl(SFc2_TTR_mdlInstanceStruct
  *chartInstance)
{
}

static const mxArray *get_sim_state_c2_TTR_mdl(SFc2_TTR_mdlInstanceStruct
  *chartInstance)
{
  const mxArray *c2_st;
  const mxArray *c2_y = NULL;
  int32_T c2_i4;
  real_T c2_u[3];
  const mxArray *c2_b_y = NULL;
  int32_T c2_i5;
  real_T c2_b_u[3];
  const mxArray *c2_c_y = NULL;
  int32_T c2_i6;
  real_T c2_c_u[3];
  const mxArray *c2_d_y = NULL;
  int32_T c2_i7;
  real_T c2_d_u[3];
  const mxArray *c2_e_y = NULL;
  real_T c2_hoistedGlobal;
  real_T c2_e_u;
  const mxArray *c2_f_y = NULL;
  real_T c2_b_hoistedGlobal;
  real_T c2_f_u;
  const mxArray *c2_g_y = NULL;
  real_T c2_c_hoistedGlobal;
  real_T c2_g_u;
  const mxArray *c2_h_y = NULL;
  real_T c2_d_hoistedGlobal;
  real_T c2_h_u;
  const mxArray *c2_i_y = NULL;
  uint8_T c2_e_hoistedGlobal;
  uint8_T c2_i_u;
  const mxArray *c2_j_y = NULL;
  real_T (*c2_CA_S_Theta)[3];
  real_T (*c2_CA_M_Theta)[3];
  c2_CA_S_Theta = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S, 2);
  c2_CA_M_Theta = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S, 1);
  c2_st = NULL;
  c2_st = NULL;
  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_createcellarray(9), FALSE);
  for (c2_i4 = 0; c2_i4 < 3; c2_i4++) {
    c2_u[c2_i4] = (*c2_CA_M_Theta)[c2_i4];
  }

  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", c2_u, 0, 0U, 1U, 0U, 1, 3), FALSE);
  sf_mex_setcell(c2_y, 0, c2_b_y);
  for (c2_i5 = 0; c2_i5 < 3; c2_i5++) {
    c2_b_u[c2_i5] = (*c2_CA_S_Theta)[c2_i5];
  }

  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", c2_b_u, 0, 0U, 1U, 0U, 1, 3), FALSE);
  sf_mex_setcell(c2_y, 1, c2_c_y);
  for (c2_i6 = 0; c2_i6 < 3; c2_i6++) {
    c2_c_u[c2_i6] = chartInstance->c2_CA_M_Theta_km1[c2_i6];
  }

  c2_d_y = NULL;
  if (!chartInstance->c2_CA_M_Theta_km1_not_empty) {
    sf_mex_assign(&c2_d_y, sf_mex_create("y", NULL, 0, 0U, 1U, 0U, 2, 0, 0),
                  FALSE);
  } else {
    sf_mex_assign(&c2_d_y, sf_mex_create("y", c2_c_u, 0, 0U, 1U, 0U, 1, 3),
                  FALSE);
  }

  sf_mex_setcell(c2_y, 2, c2_d_y);
  for (c2_i7 = 0; c2_i7 < 3; c2_i7++) {
    c2_d_u[c2_i7] = chartInstance->c2_CA_S_Theta_km1[c2_i7];
  }

  c2_e_y = NULL;
  if (!chartInstance->c2_CA_S_Theta_km1_not_empty) {
    sf_mex_assign(&c2_e_y, sf_mex_create("y", NULL, 0, 0U, 1U, 0U, 2, 0, 0),
                  FALSE);
  } else {
    sf_mex_assign(&c2_e_y, sf_mex_create("y", c2_d_u, 0, 0U, 1U, 0U, 1, 3),
                  FALSE);
  }

  sf_mex_setcell(c2_y, 3, c2_e_y);
  c2_hoistedGlobal = chartInstance->c2_phi_err_int;
  c2_e_u = c2_hoistedGlobal;
  c2_f_y = NULL;
  if (!chartInstance->c2_phi_err_int_not_empty) {
    sf_mex_assign(&c2_f_y, sf_mex_create("y", NULL, 0, 0U, 1U, 0U, 2, 0, 0),
                  FALSE);
  } else {
    sf_mex_assign(&c2_f_y, sf_mex_create("y", &c2_e_u, 0, 0U, 0U, 0U, 0), FALSE);
  }

  sf_mex_setcell(c2_y, 4, c2_f_y);
  c2_b_hoistedGlobal = chartInstance->c2_psi_err_int;
  c2_f_u = c2_b_hoistedGlobal;
  c2_g_y = NULL;
  if (!chartInstance->c2_psi_err_int_not_empty) {
    sf_mex_assign(&c2_g_y, sf_mex_create("y", NULL, 0, 0U, 1U, 0U, 2, 0, 0),
                  FALSE);
  } else {
    sf_mex_assign(&c2_g_y, sf_mex_create("y", &c2_f_u, 0, 0U, 0U, 0U, 0), FALSE);
  }

  sf_mex_setcell(c2_y, 5, c2_g_y);
  c2_c_hoistedGlobal = chartInstance->c2_t_km1;
  c2_g_u = c2_c_hoistedGlobal;
  c2_h_y = NULL;
  if (!chartInstance->c2_t_km1_not_empty) {
    sf_mex_assign(&c2_h_y, sf_mex_create("y", NULL, 0, 0U, 1U, 0U, 2, 0, 0),
                  FALSE);
  } else {
    sf_mex_assign(&c2_h_y, sf_mex_create("y", &c2_g_u, 0, 0U, 0U, 0U, 0), FALSE);
  }

  sf_mex_setcell(c2_y, 6, c2_h_y);
  c2_d_hoistedGlobal = chartInstance->c2_theta_err_int;
  c2_h_u = c2_d_hoistedGlobal;
  c2_i_y = NULL;
  if (!chartInstance->c2_theta_err_int_not_empty) {
    sf_mex_assign(&c2_i_y, sf_mex_create("y", NULL, 0, 0U, 1U, 0U, 2, 0, 0),
                  FALSE);
  } else {
    sf_mex_assign(&c2_i_y, sf_mex_create("y", &c2_h_u, 0, 0U, 0U, 0U, 0), FALSE);
  }

  sf_mex_setcell(c2_y, 7, c2_i_y);
  c2_e_hoistedGlobal = chartInstance->c2_is_active_c2_TTR_mdl;
  c2_i_u = c2_e_hoistedGlobal;
  c2_j_y = NULL;
  sf_mex_assign(&c2_j_y, sf_mex_create("y", &c2_i_u, 3, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c2_y, 8, c2_j_y);
  sf_mex_assign(&c2_st, c2_y, FALSE);
  return c2_st;
}

static void set_sim_state_c2_TTR_mdl(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_st)
{
  const mxArray *c2_u;
  real_T c2_dv4[3];
  int32_T c2_i8;
  real_T c2_dv5[3];
  int32_T c2_i9;
  real_T c2_dv6[3];
  int32_T c2_i10;
  real_T c2_dv7[3];
  int32_T c2_i11;
  real_T (*c2_CA_M_Theta)[3];
  real_T (*c2_CA_S_Theta)[3];
  c2_CA_S_Theta = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S, 2);
  c2_CA_M_Theta = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S, 1);
  chartInstance->c2_doneDoubleBufferReInit = TRUE;
  c2_u = sf_mex_dup(c2_st);
  c2_m_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c2_u, 0)),
                        "CA_M_Theta", c2_dv4);
  for (c2_i8 = 0; c2_i8 < 3; c2_i8++) {
    (*c2_CA_M_Theta)[c2_i8] = c2_dv4[c2_i8];
  }

  c2_m_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c2_u, 1)),
                        "CA_S_Theta", c2_dv5);
  for (c2_i9 = 0; c2_i9 < 3; c2_i9++) {
    (*c2_CA_S_Theta)[c2_i9] = c2_dv5[c2_i9];
  }

  c2_i_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c2_u, 2)),
                        "CA_M_Theta_km1", c2_dv6);
  for (c2_i10 = 0; c2_i10 < 3; c2_i10++) {
    chartInstance->c2_CA_M_Theta_km1[c2_i10] = c2_dv6[c2_i10];
  }

  c2_g_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c2_u, 3)),
                        "CA_S_Theta_km1", c2_dv7);
  for (c2_i11 = 0; c2_i11 < 3; c2_i11++) {
    chartInstance->c2_CA_S_Theta_km1[c2_i11] = c2_dv7[c2_i11];
  }

  chartInstance->c2_phi_err_int = c2_e_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 4)), "phi_err_int");
  chartInstance->c2_psi_err_int = c2_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c2_u, 5)), "psi_err_int");
  chartInstance->c2_t_km1 = c2_k_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c2_u, 6)), "t_km1");
  chartInstance->c2_theta_err_int = c2_c_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 7)), "theta_err_int");
  chartInstance->c2_is_active_c2_TTR_mdl = c2_t_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 8)), "is_active_c2_TTR_mdl");
  sf_mex_destroy(&c2_u);
  c2_update_debugger_state_c2_TTR_mdl(chartInstance);
  sf_mex_destroy(&c2_st);
}

static void finalize_c2_TTR_mdl(SFc2_TTR_mdlInstanceStruct *chartInstance)
{
}

static void sf_c2_TTR_mdl(SFc2_TTR_mdlInstanceStruct *chartInstance)
{
  int32_T c2_i12;
  int32_T c2_i13;
  int32_T c2_i14;
  int32_T c2_i15;
  int32_T c2_i16;
  int32_T c2_i17;
  int32_T c2_i18;
  int32_T c2_i19;
  int32_T c2_i20;
  int32_T c2_i21;
  real_T *c2_t;
  real_T (*c2_CA_S_Theta)[3];
  real_T (*c2_Theta_d)[3];
  real_T (*c2_CA_M_Theta)[3];
  real_T (*c2_Theta_Die)[3];
  real_T (*c2_Theta_Dot)[3];
  real_T (*c2_Theta)[3];
  c2_CA_S_Theta = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S, 2);
  c2_Theta_d = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 4);
  c2_CA_M_Theta = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S, 1);
  c2_Theta_Die = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 3);
  c2_Theta_Dot = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 2);
  c2_Theta = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 1);
  c2_t = (real_T *)ssGetInputPortSignal(chartInstance->S, 0);
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 1U, chartInstance->c2_sfEvent);
  _SFD_DATA_RANGE_CHECK(*c2_t, 0U);
  for (c2_i12 = 0; c2_i12 < 3; c2_i12++) {
    _SFD_DATA_RANGE_CHECK((*c2_Theta)[c2_i12], 1U);
  }

  for (c2_i13 = 0; c2_i13 < 3; c2_i13++) {
    _SFD_DATA_RANGE_CHECK((*c2_Theta_Dot)[c2_i13], 2U);
  }

  for (c2_i14 = 0; c2_i14 < 3; c2_i14++) {
    _SFD_DATA_RANGE_CHECK((*c2_Theta_Die)[c2_i14], 3U);
  }

  for (c2_i15 = 0; c2_i15 < 3; c2_i15++) {
    _SFD_DATA_RANGE_CHECK((*c2_CA_M_Theta)[c2_i15], 4U);
  }

  for (c2_i16 = 0; c2_i16 < 3; c2_i16++) {
    _SFD_DATA_RANGE_CHECK((*c2_Theta_d)[c2_i16], 5U);
  }

  for (c2_i17 = 0; c2_i17 < 2; c2_i17++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c2_PolyRL_U2R[c2_i17], 6U);
  }

  for (c2_i18 = 0; c2_i18 < 2; c2_i18++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c2_PolyT_U2R[c2_i18], 7U);
  }

  for (c2_i19 = 0; c2_i19 < 3; c2_i19++) {
    _SFD_DATA_RANGE_CHECK((*c2_CA_S_Theta)[c2_i19], 8U);
  }

  for (c2_i20 = 0; c2_i20 < 2; c2_i20++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c2_PolyGT_U2A[c2_i20], 9U);
  }

  for (c2_i21 = 0; c2_i21 < 4; c2_i21++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c2_LQR_phi[c2_i21], 10U);
  }

  chartInstance->c2_sfEvent = CALL_EVENT;
  c2_chartstep_c2_TTR_mdl(chartInstance);
  _SFD_CHECK_FOR_STATE_INCONSISTENCY(_TTR_mdlMachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
}

static void c2_chartstep_c2_TTR_mdl(SFc2_TTR_mdlInstanceStruct *chartInstance)
{
  real_T c2_hoistedGlobal;
  real_T c2_t;
  int32_T c2_i22;
  real_T c2_Theta[3];
  int32_T c2_i23;
  real_T c2_Theta_Dot[3];
  int32_T c2_i24;
  real_T c2_Theta_Die[3];
  int32_T c2_i25;
  real_T c2_Theta_d[3];
  int32_T c2_i26;
  real_T c2_b_PolyRL_U2R[2];
  int32_T c2_i27;
  real_T c2_b_PolyT_U2R[2];
  int32_T c2_i28;
  real_T c2_b_PolyGT_U2A[2];
  int32_T c2_i29;
  real_T c2_b_LQR_phi[4];
  uint32_T c2_debug_family_var_map[79];
  real_T c2_Ts;
  real_T c2_Ki_roll;
  real_T c2_Kp_roll_LU[3];
  real_T c2_Kd_roll;
  real_T c2_Kg_roll;
  real_T c2_Ki_pitch;
  real_T c2_Kp_pitch_LU[3];
  real_T c2_Kd_pitch;
  real_T c2_Kg_pitch;
  real_T c2_Ki_yaw;
  real_T c2_Kp_yaw;
  real_T c2_Kd_yaw;
  real_T c2_Kg_yaw;
  real_T c2_phi;
  real_T c2_theta;
  real_T c2_psi;
  real_T c2_phi_dot;
  real_T c2_theta_dot;
  real_T c2_psi_dot;
  real_T c2_phi_die;
  real_T c2_theta_die;
  real_T c2_psi_die;
  real_T c2_phi_d;
  real_T c2_theta_d;
  real_T c2_psi_d;
  real_T c2_phi_err;
  real_T c2_phi_dot_err;
  real_T c2_phi_die_err;
  real_T c2_theta_err;
  real_T c2_theta_dot_err;
  real_T c2_theta_die_err;
  real_T c2_psi_err;
  real_T c2_psi_dot_err;
  real_T c2_psi_die_err;
  real_T c2_Int_Leakage_Theta;
  real_T c2_integrator_switch;
  real_T c2_Yaw_Error_constraint;
  real_T c2_Phi_Error_Int_constraint;
  real_T c2_Theta_Error_Int_constraint;
  real_T c2_Psi_Error_Int_constraint;
  real_T c2_Phi_Integrator_constraint;
  real_T c2_Theta_Integrator_constraint;
  real_T c2_Psi_Integrator_constraint;
  real_T c2_a;
  real_T c2_phi_err_intcnstr;
  real_T c2_theta_err_intcnstr;
  real_T c2_psi_err_intcnstr;
  real_T c2_LUx[3];
  real_T c2_Kp_roll;
  real_T c2_Kp_pitch;
  real_T c2_roll_control;
  real_T c2_pitch_control;
  real_T c2_yaw_control;
  real_T c2_PHI_ctrl[5];
  real_T c2_ca_m_right;
  real_T c2_ca_m_left;
  real_T c2_ca_m_tail;
  real_T c2_ca_s_right;
  real_T c2_ca_s_left;
  real_T c2_ca_s_tail;
  real_T c2_nargin = 9.0;
  real_T c2_nargout = 2.0;
  real_T c2_CA_M_Theta[3];
  real_T c2_CA_S_Theta[3];
  int32_T c2_i30;
  int32_T c2_i31;
  int32_T c2_i32;
  static real_T c2_dv8[3] = { 140.0, 120.0, 90.0 };

  int32_T c2_i33;
  static real_T c2_dv9[3] = { 190.0, 170.0, 140.0 };

  real_T c2_b_hoistedGlobal;
  real_T c2_b_a;
  real_T c2_y;
  real_T c2_c_hoistedGlobal;
  real_T c2_c_a;
  real_T c2_b_y;
  real_T c2_d_hoistedGlobal;
  real_T c2_d_a;
  real_T c2_c_y;
  int32_T c2_i34;
  int32_T c2_i35;
  real_T c2_dv10[3];
  int32_T c2_i36;
  real_T c2_dv11[3];
  int32_T c2_i37;
  real_T c2_dv12[3];
  int32_T c2_i38;
  real_T c2_dv13[3];
  real_T c2_e_hoistedGlobal;
  real_T c2_b;
  real_T c2_d_y;
  real_T c2_e_a;
  real_T c2_b_b;
  real_T c2_e_y;
  real_T c2_c_b;
  real_T c2_f_y;
  real_T c2_d_b;
  real_T c2_g_y;
  real_T c2_f_hoistedGlobal;
  real_T c2_e_b;
  real_T c2_h_y;
  real_T c2_f_a;
  real_T c2_f_b;
  real_T c2_i_y;
  real_T c2_g_b;
  real_T c2_j_y;
  real_T c2_h_b;
  real_T c2_k_y;
  real_T c2_g_hoistedGlobal;
  real_T c2_i_b;
  real_T c2_l_y;
  real_T c2_j_b;
  real_T c2_m_y;
  real_T c2_k_b;
  real_T c2_n_y;
  real_T c2_l_b;
  real_T c2_o_y;
  real_T c2_g_a[5];
  int32_T c2_i39;
  int32_T c2_i40;
  real_T c2_m_b[5];
  int32_T c2_k;
  int32_T c2_b_k;
  real_T c2_h_a;
  real_T c2_n_b;
  real_T c2_i_a;
  real_T c2_o_b;
  real_T c2_j_a;
  real_T c2_p_b;
  real_T c2_k_a;
  real_T c2_q_b;
  int32_T c2_i41;
  int32_T c2_i42;
  int32_T c2_i43;
  int32_T c2_i44;
  int32_T c2_i45;
  int32_T c2_i46;
  real_T *c2_b_t;
  real_T (*c2_b_CA_M_Theta)[3];
  real_T (*c2_b_CA_S_Theta)[3];
  real_T (*c2_b_Theta_d)[3];
  real_T (*c2_b_Theta_Die)[3];
  real_T (*c2_b_Theta_Dot)[3];
  real_T (*c2_b_Theta)[3];
  boolean_T guard1 = FALSE;
  boolean_T guard2 = FALSE;
  boolean_T guard3 = FALSE;
  boolean_T guard4 = FALSE;
  boolean_T guard5 = FALSE;
  boolean_T guard6 = FALSE;
  boolean_T guard7 = FALSE;
  boolean_T guard8 = FALSE;
  boolean_T guard9 = FALSE;
  c2_b_CA_S_Theta = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S, 2);
  c2_b_Theta_d = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 4);
  c2_b_CA_M_Theta = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S, 1);
  c2_b_Theta_Die = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 3);
  c2_b_Theta_Dot = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 2);
  c2_b_Theta = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 1);
  c2_b_t = (real_T *)ssGetInputPortSignal(chartInstance->S, 0);
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 1U, chartInstance->c2_sfEvent);
  c2_hoistedGlobal = *c2_b_t;
  c2_t = c2_hoistedGlobal;
  for (c2_i22 = 0; c2_i22 < 3; c2_i22++) {
    c2_Theta[c2_i22] = (*c2_b_Theta)[c2_i22];
  }

  for (c2_i23 = 0; c2_i23 < 3; c2_i23++) {
    c2_Theta_Dot[c2_i23] = (*c2_b_Theta_Dot)[c2_i23];
  }

  for (c2_i24 = 0; c2_i24 < 3; c2_i24++) {
    c2_Theta_Die[c2_i24] = (*c2_b_Theta_Die)[c2_i24];
  }

  for (c2_i25 = 0; c2_i25 < 3; c2_i25++) {
    c2_Theta_d[c2_i25] = (*c2_b_Theta_d)[c2_i25];
  }

  for (c2_i26 = 0; c2_i26 < 2; c2_i26++) {
    c2_b_PolyRL_U2R[c2_i26] = chartInstance->c2_PolyRL_U2R[c2_i26];
  }

  for (c2_i27 = 0; c2_i27 < 2; c2_i27++) {
    c2_b_PolyT_U2R[c2_i27] = chartInstance->c2_PolyT_U2R[c2_i27];
  }

  for (c2_i28 = 0; c2_i28 < 2; c2_i28++) {
    c2_b_PolyGT_U2A[c2_i28] = chartInstance->c2_PolyGT_U2A[c2_i28];
  }

  for (c2_i29 = 0; c2_i29 < 4; c2_i29++) {
    c2_b_LQR_phi[c2_i29] = chartInstance->c2_LQR_phi[c2_i29];
  }

  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 79U, 79U, c2_debug_family_names,
    c2_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_Ts, 0U, c2_j_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_Ki_roll, 1U, c2_j_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c2_Kp_roll_LU, 2U, c2_l_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_Kd_roll, 3U, c2_j_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_Kg_roll, 4U, c2_j_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_Ki_pitch, 5U, c2_j_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c2_Kp_pitch_LU, 6U, c2_l_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_Kd_pitch, 7U, c2_j_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_Kg_pitch, 8U, c2_j_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_Ki_yaw, 9U, c2_j_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_Kp_yaw, 10U, c2_j_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_Kd_yaw, 11U, c2_j_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_Kg_yaw, 12U, c2_j_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_phi, 13U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_theta, 14U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_psi, 15U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_phi_dot, 16U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_theta_dot, 17U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_psi_dot, 18U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_phi_die, 19U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_theta_die, 20U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_psi_die, 21U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_phi_d, 22U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_theta_d, 23U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_psi_d, 24U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_phi_err, 25U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_phi_dot_err, 26U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_phi_die_err, 27U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_theta_err, 28U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_theta_dot_err, 29U,
    c2_j_sf_marshallOut, c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_theta_die_err, 30U,
    c2_j_sf_marshallOut, c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_psi_err, 31U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_psi_dot_err, 32U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_psi_die_err, 33U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_Int_Leakage_Theta, 34U, c2_j_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_integrator_switch, 35U, c2_j_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Yaw_Error_constraint, 36U,
    c2_j_sf_marshallOut, c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Phi_Error_Int_constraint, 37U,
    c2_j_sf_marshallOut, c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Theta_Error_Int_constraint, 38U,
    c2_j_sf_marshallOut, c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Psi_Error_Int_constraint, 39U,
    c2_j_sf_marshallOut, c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Phi_Integrator_constraint, 40U,
    c2_j_sf_marshallOut, c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Theta_Integrator_constraint, 41U,
    c2_j_sf_marshallOut, c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Psi_Integrator_constraint, 42U,
    c2_j_sf_marshallOut, c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_a, 43U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_phi_err_intcnstr, 44U,
    c2_j_sf_marshallOut, c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_theta_err_intcnstr, 45U,
    c2_j_sf_marshallOut, c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_psi_err_intcnstr, 46U,
    c2_j_sf_marshallOut, c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(c2_LUx, 47U, c2_l_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Kp_roll, 48U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Kp_pitch, 49U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_roll_control, 50U,
    c2_j_sf_marshallOut, c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_pitch_control, 51U,
    c2_j_sf_marshallOut, c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_yaw_control, 52U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_PHI_ctrl, 53U, c2_k_sf_marshallOut,
    c2_k_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_ca_m_right, 54U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_ca_m_left, 55U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_ca_m_tail, 56U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_ca_s_right, 57U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_ca_s_left, 58U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_ca_s_tail, 59U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargin, 60U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargout, 61U, c2_j_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_t, 62U, c2_j_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c2_Theta, 63U, c2_g_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c2_Theta_Dot, 64U, c2_g_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c2_Theta_Die, 65U, c2_g_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c2_Theta_d, 66U, c2_g_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_b_PolyRL_U2R, 67U, c2_i_sf_marshallOut,
    c2_i_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_b_PolyT_U2R, 68U, c2_i_sf_marshallOut,
    c2_i_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_b_PolyGT_U2A, 69U, c2_i_sf_marshallOut,
    c2_i_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_b_LQR_phi, 70U, c2_h_sf_marshallOut,
    c2_h_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_CA_M_Theta, 71U, c2_g_sf_marshallOut,
    c2_g_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_CA_S_Theta, 72U, c2_g_sf_marshallOut,
    c2_g_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&chartInstance->c2_t_km1, 73U,
    c2_f_sf_marshallOut, c2_f_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(chartInstance->c2_CA_M_Theta_km1, 74U,
    c2_e_sf_marshallOut, c2_e_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(chartInstance->c2_CA_S_Theta_km1, 75U,
    c2_d_sf_marshallOut, c2_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&chartInstance->c2_phi_err_int, 76U,
    c2_c_sf_marshallOut, c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&chartInstance->c2_theta_err_int, 77U,
    c2_b_sf_marshallOut, c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&chartInstance->c2_psi_err_int, 78U,
    c2_sf_marshallOut, c2_sf_marshallIn);
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 4);
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 4);
  if (CV_EML_IF(0, 1, 0, !chartInstance->c2_t_km1_not_empty)) {
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 4);
    chartInstance->c2_t_km1 = -0.02;
    chartInstance->c2_t_km1_not_empty = TRUE;
  }

  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 5);
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 5);
  if (CV_EML_IF(0, 1, 1, !chartInstance->c2_CA_M_Theta_km1_not_empty)) {
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 5);
    for (c2_i30 = 0; c2_i30 < 3; c2_i30++) {
      chartInstance->c2_CA_M_Theta_km1[c2_i30] = 0.0;
    }

    chartInstance->c2_CA_M_Theta_km1_not_empty = TRUE;
  }

  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 6);
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 6);
  if (CV_EML_IF(0, 1, 2, !chartInstance->c2_CA_S_Theta_km1_not_empty)) {
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 6);
    for (c2_i31 = 0; c2_i31 < 3; c2_i31++) {
      chartInstance->c2_CA_S_Theta_km1[c2_i31] = 0.0;
    }

    chartInstance->c2_CA_S_Theta_km1_not_empty = TRUE;
  }

  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 7);
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 7);
  if (CV_EML_IF(0, 1, 3, !chartInstance->c2_phi_err_int_not_empty)) {
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 7);
    chartInstance->c2_phi_err_int = 0.0;
    chartInstance->c2_phi_err_int_not_empty = TRUE;
  }

  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 8);
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 8);
  if (CV_EML_IF(0, 1, 4, !chartInstance->c2_theta_err_int_not_empty)) {
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 8);
    chartInstance->c2_theta_err_int = 0.0;
    chartInstance->c2_theta_err_int_not_empty = TRUE;
  }

  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 9);
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 9);
  if (CV_EML_IF(0, 1, 5, !chartInstance->c2_psi_err_int_not_empty)) {
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 9);
    chartInstance->c2_psi_err_int = 0.0;
    chartInstance->c2_psi_err_int_not_empty = TRUE;
  }

  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 11);
  c2_Ts = 0.01;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 12);
  if (CV_EML_IF(0, 1, 6, c2_t - chartInstance->c2_t_km1 >= c2_Ts)) {
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 13);
    chartInstance->c2_t_km1 = c2_t;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 16);
    c2_Ki_roll = 0.0;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 17);
    for (c2_i32 = 0; c2_i32 < 3; c2_i32++) {
      c2_Kp_roll_LU[c2_i32] = c2_dv8[c2_i32];
    }

    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 18);
    c2_Kd_roll = 17.5;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 19);
    c2_Kg_roll = 0.8;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 21);
    c2_Ki_pitch = 60.0;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 22);
    for (c2_i33 = 0; c2_i33 < 3; c2_i33++) {
      c2_Kp_pitch_LU[c2_i33] = c2_dv9[c2_i33];
    }

    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 23);
    c2_Kd_pitch = 22.5;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 24);
    c2_Kg_pitch = 1.25;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 26);
    c2_Ki_yaw = 60.0;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 27);
    c2_Kp_yaw = 300.0;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 28);
    c2_Kd_yaw = 150.0;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 29);
    c2_Kg_yaw = 0.0;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 32);
    c2_phi = c2_Theta[0];
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 33);
    c2_theta = c2_Theta[1];
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 34);
    c2_psi = c2_Theta[2];
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 35);
    c2_phi_dot = c2_Theta_Dot[0];
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 36);
    c2_theta_dot = c2_Theta_Dot[1];
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 37);
    c2_psi_dot = c2_Theta_Dot[2];
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 38);
    c2_phi_die = c2_Theta_Die[0];
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 39);
    c2_theta_die = c2_Theta_Die[1];
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 40);
    c2_psi_die = c2_Theta_Die[2];
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 42);
    c2_phi_d = c2_Theta_d[0];
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 43);
    c2_theta_d = c2_Theta_d[1];
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 44);
    c2_psi_d = c2_Theta_d[2];
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 48);
    c2_phi_err = c2_phi_d - c2_phi;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 49);
    c2_phi_dot_err = 0.0 - c2_phi_dot;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 50);
    c2_phi_die_err = 0.0 - c2_phi_die;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 51);
    c2_theta_err = c2_theta_d - c2_theta;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 52);
    c2_theta_dot_err = 0.0 - c2_theta_dot;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 53);
    c2_theta_die_err = 0.0 - c2_theta_die;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 54);
    c2_psi_err = c2_psi_d - c2_psi;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 55);
    c2_psi_dot_err = 0.0 - c2_psi_dot;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 56);
    c2_psi_die_err = 0.0 - c2_psi_die;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 60);
    c2_Int_Leakage_Theta = 3.0;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 61);
    c2_integrator_switch = 1.0;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 62);
    c2_Yaw_Error_constraint = 0.3490658503988659;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 66);
    if (CV_EML_IF(0, 1, 7, c2_psi_err < -c2_Yaw_Error_constraint)) {
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 67);
      c2_psi_err = -c2_Yaw_Error_constraint;
    } else {
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 68);
      if (CV_EML_IF(0, 1, 8, c2_psi_err > c2_Yaw_Error_constraint)) {
        _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 69);
        c2_psi_err = c2_Yaw_Error_constraint;
      }
    }

    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 74);
    c2_Phi_Error_Int_constraint = 0.017453292519943295;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 75);
    c2_Theta_Error_Int_constraint = 0.017453292519943295;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 76);
    c2_Psi_Error_Int_constraint = 0.017453292519943295;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 77);
    c2_Phi_Integrator_constraint = 0.52359877559829882;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 78);
    c2_Theta_Integrator_constraint = 0.52359877559829882;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 79);
    c2_Psi_Integrator_constraint = 0.52359877559829882;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 80);
    c2_a = 0.0016638935108153079;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 82);
    CV_EML_IF(0, 1, 9, c2_integrator_switch == 0.0);
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 87);
    if (CV_EML_IF(0, 1, 10, c2_phi_err < -c2_Phi_Error_Int_constraint)) {
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 88);
      c2_phi_err_intcnstr = -c2_Phi_Error_Int_constraint;
    } else {
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 89);
      if (CV_EML_IF(0, 1, 11, c2_phi_err > c2_Phi_Error_Int_constraint)) {
        _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 90);
        c2_phi_err_intcnstr = c2_Phi_Error_Int_constraint;
      } else {
        _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 92);
        c2_phi_err_intcnstr = c2_phi_err;
      }
    }

    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 94);
    if (CV_EML_IF(0, 1, 12, c2_theta_err < -c2_Theta_Error_Int_constraint)) {
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 95);
      c2_theta_err_intcnstr = -c2_Theta_Error_Int_constraint;
    } else {
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 96);
      if (CV_EML_IF(0, 1, 13, c2_theta_err > c2_Theta_Error_Int_constraint)) {
        _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 97);
        c2_theta_err_intcnstr = c2_Theta_Error_Int_constraint;
      } else {
        _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 99);
        c2_theta_err_intcnstr = c2_theta_err;
      }
    }

    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 101);
    if (CV_EML_IF(0, 1, 14, c2_psi_err < -c2_Psi_Error_Int_constraint)) {
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 102);
      c2_psi_err_intcnstr = -c2_Psi_Error_Int_constraint;
    } else {
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 103);
      if (CV_EML_IF(0, 1, 15, c2_psi_err > c2_Psi_Error_Int_constraint)) {
        _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 104);
        c2_psi_err_intcnstr = c2_Psi_Error_Int_constraint;
      } else {
        _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 106);
        c2_psi_err_intcnstr = c2_psi_err;
      }
    }

    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 109);
    if (CV_EML_IF(0, 1, 16, c2_abs(chartInstance, chartInstance->c2_phi_err_int)
                  <= c2_Phi_Integrator_constraint)) {
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 111);
      guard7 = FALSE;
      guard8 = FALSE;
      guard9 = FALSE;
      if (CV_EML_COND(0, 1, 0, c2_phi_d > c2_phi)) {
        if (CV_EML_COND(0, 1, 1, c2_phi_dot <= 0.0)) {
          guard8 = TRUE;
        } else {
          guard9 = TRUE;
        }
      } else {
        guard9 = TRUE;
      }

      if (guard9 == TRUE) {
        if (CV_EML_COND(0, 1, 2, c2_phi_d < c2_phi)) {
          if (CV_EML_COND(0, 1, 3, c2_phi_dot >= -0.0)) {
            guard8 = TRUE;
          } else {
            guard7 = TRUE;
          }
        } else {
          guard7 = TRUE;
        }
      }

      if (guard8 == TRUE) {
        CV_EML_MCDC(0, 1, 0, TRUE);
        CV_EML_IF(0, 1, 17, TRUE);
        _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 112);
        c2_b_hoistedGlobal = chartInstance->c2_phi_err_int;
        c2_b_a = c2_phi_err_intcnstr;
        c2_y = c2_b_a * 0.01;
        chartInstance->c2_phi_err_int = c2_b_hoistedGlobal + c2_y;
      }

      if (guard7 == TRUE) {
        CV_EML_MCDC(0, 1, 0, FALSE);
        CV_EML_IF(0, 1, 17, FALSE);
      }
    } else {
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 115);
      if (CV_EML_IF(0, 1, 18, chartInstance->c2_phi_err_int >
                    c2_Phi_Integrator_constraint)) {
        _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 116);
        chartInstance->c2_phi_err_int = c2_Phi_Integrator_constraint;
      } else {
        _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 117);
        if (CV_EML_IF(0, 1, 19, chartInstance->c2_phi_err_int <
                      -c2_Phi_Integrator_constraint)) {
          _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 118);
          chartInstance->c2_phi_err_int = -c2_Phi_Integrator_constraint;
        }
      }
    }

    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 120);
    if (CV_EML_IF(0, 1, 20, c2_abs(chartInstance,
          chartInstance->c2_theta_err_int) <= c2_Theta_Integrator_constraint)) {
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 122);
      guard4 = FALSE;
      guard5 = FALSE;
      guard6 = FALSE;
      if (CV_EML_COND(0, 1, 4, c2_theta_d > c2_theta)) {
        if (CV_EML_COND(0, 1, 5, c2_theta_dot <= 0.0)) {
          guard5 = TRUE;
        } else {
          guard6 = TRUE;
        }
      } else {
        guard6 = TRUE;
      }

      if (guard6 == TRUE) {
        if (CV_EML_COND(0, 1, 6, c2_theta_d < c2_theta)) {
          if (CV_EML_COND(0, 1, 7, c2_theta_dot >= -0.0)) {
            guard5 = TRUE;
          } else {
            guard4 = TRUE;
          }
        } else {
          guard4 = TRUE;
        }
      }

      if (guard5 == TRUE) {
        CV_EML_MCDC(0, 1, 1, TRUE);
        CV_EML_IF(0, 1, 21, TRUE);
        _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 123);
        c2_c_hoistedGlobal = chartInstance->c2_theta_err_int;
        c2_c_a = c2_theta_err_intcnstr;
        c2_b_y = c2_c_a * 0.01;
        chartInstance->c2_theta_err_int = c2_c_hoistedGlobal + c2_b_y;
      }

      if (guard4 == TRUE) {
        CV_EML_MCDC(0, 1, 1, FALSE);
        CV_EML_IF(0, 1, 21, FALSE);
      }
    } else {
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 126);
      if (CV_EML_IF(0, 1, 22, chartInstance->c2_theta_err_int >
                    c2_Theta_Integrator_constraint)) {
        _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, MAX_int8_T);
        chartInstance->c2_theta_err_int = c2_Theta_Integrator_constraint;
      } else {
        _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 128U);
        if (CV_EML_IF(0, 1, 23, chartInstance->c2_theta_err_int <
                      -c2_Theta_Integrator_constraint)) {
          _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 129U);
          chartInstance->c2_theta_err_int = -c2_Theta_Integrator_constraint;
        }
      }
    }

    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 131U);
    if (CV_EML_IF(0, 1, 24, c2_abs(chartInstance, chartInstance->c2_psi_err_int)
                  <= c2_Psi_Integrator_constraint)) {
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 133U);
      guard1 = FALSE;
      guard2 = FALSE;
      guard3 = FALSE;
      if (CV_EML_COND(0, 1, 8, c2_psi_d > c2_psi)) {
        if (CV_EML_COND(0, 1, 9, c2_psi_dot <= 0.0)) {
          guard2 = TRUE;
        } else {
          guard3 = TRUE;
        }
      } else {
        guard3 = TRUE;
      }

      if (guard3 == TRUE) {
        if (CV_EML_COND(0, 1, 10, c2_psi_d < c2_psi)) {
          if (CV_EML_COND(0, 1, 11, c2_psi_dot >= -0.0)) {
            guard2 = TRUE;
          } else {
            guard1 = TRUE;
          }
        } else {
          guard1 = TRUE;
        }
      }

      if (guard2 == TRUE) {
        CV_EML_MCDC(0, 1, 2, TRUE);
        CV_EML_IF(0, 1, 25, TRUE);
        _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 134U);
        c2_d_hoistedGlobal = chartInstance->c2_psi_err_int;
        c2_d_a = c2_psi_err_intcnstr;
        c2_c_y = c2_d_a * 0.01;
        chartInstance->c2_psi_err_int = c2_d_hoistedGlobal + c2_c_y;
      }

      if (guard1 == TRUE) {
        CV_EML_MCDC(0, 1, 2, FALSE);
        CV_EML_IF(0, 1, 25, FALSE);
      }
    } else {
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 137U);
      if (CV_EML_IF(0, 1, 26, chartInstance->c2_psi_err_int >
                    c2_Psi_Integrator_constraint)) {
        _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 138U);
        chartInstance->c2_psi_err_int = c2_Psi_Integrator_constraint;
      } else {
        _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 139U);
        if (CV_EML_IF(0, 1, 27, chartInstance->c2_psi_err_int <
                      -c2_Psi_Integrator_constraint)) {
          _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 140U);
          chartInstance->c2_psi_err_int = -c2_Psi_Integrator_constraint;
        }
      }
    }

    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 146U);
    for (c2_i34 = 0; c2_i34 < 3; c2_i34++) {
      c2_LUx[c2_i34] = 0.087266462599716474 * (real_T)c2_i34;
    }

    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 147U);
    for (c2_i35 = 0; c2_i35 < 3; c2_i35++) {
      c2_dv10[c2_i35] = 0.087266462599716474 * (real_T)c2_i35;
    }

    for (c2_i36 = 0; c2_i36 < 3; c2_i36++) {
      c2_dv11[c2_i36] = c2_dv8[c2_i36];
    }

    c2_Kp_roll = c2_interp1(chartInstance, c2_dv10, c2_dv11, c2_abs
      (chartInstance, c2_phi_err), 90.0);
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 148U);
    if (CV_EML_IF(0, 1, 28, c2_Kp_roll < 0.0)) {
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 148U);
      c2_Kp_roll = 0.0;
    }

    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 149U);
    for (c2_i37 = 0; c2_i37 < 3; c2_i37++) {
      c2_dv12[c2_i37] = 0.087266462599716474 * (real_T)c2_i37;
    }

    for (c2_i38 = 0; c2_i38 < 3; c2_i38++) {
      c2_dv13[c2_i38] = c2_dv9[c2_i38];
    }

    c2_Kp_pitch = c2_interp1(chartInstance, c2_dv12, c2_dv13, c2_abs
      (chartInstance, c2_theta_err), 140.0);
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 150U);
    if (CV_EML_IF(0, 1, 29, c2_Kp_pitch < 0.0)) {
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 150U);
      c2_Kp_pitch = 0.0;
    }

    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 153U);
    c2_e_hoistedGlobal = chartInstance->c2_phi_err_int;
    c2_b = c2_e_hoistedGlobal;
    c2_d_y = 0.0 * c2_b;
    c2_e_a = c2_Kp_roll;
    c2_b_b = c2_phi_err;
    c2_e_y = c2_e_a * c2_b_b;
    c2_c_b = c2_phi_dot_err;
    c2_f_y = 17.5 * c2_c_b;
    c2_d_b = c2_phi_die_err;
    c2_g_y = 0.8 * c2_d_b;
    c2_roll_control = ((c2_d_y + c2_e_y) + c2_f_y) + c2_g_y;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 154U);
    c2_f_hoistedGlobal = chartInstance->c2_theta_err_int;
    c2_e_b = c2_f_hoistedGlobal;
    c2_h_y = 60.0 * c2_e_b;
    c2_f_a = c2_Kp_pitch;
    c2_f_b = c2_theta_err;
    c2_i_y = c2_f_a * c2_f_b;
    c2_g_b = c2_theta_dot_err;
    c2_j_y = 22.5 * c2_g_b;
    c2_h_b = c2_theta_die_err;
    c2_k_y = 1.25 * c2_h_b;
    c2_pitch_control = ((c2_h_y + c2_i_y) + c2_j_y) + c2_k_y;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 155U);
    c2_g_hoistedGlobal = chartInstance->c2_psi_err_int;
    c2_i_b = c2_g_hoistedGlobal;
    c2_l_y = 60.0 * c2_i_b;
    c2_j_b = c2_psi_err;
    c2_m_y = 300.0 * c2_j_b;
    c2_k_b = c2_psi_dot_err;
    c2_n_y = 150.0 * c2_k_b;
    c2_l_b = c2_psi_die_err;
    c2_o_y = 0.0 * c2_l_b;
    c2_yaw_control = ((c2_l_y + c2_m_y) + c2_n_y) + c2_o_y;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 157U);
    c2_PHI_ctrl[0] = chartInstance->c2_phi_err_int;
    c2_PHI_ctrl[1] = c2_phi_err;
    c2_PHI_ctrl[2] = c2_phi_dot_err;
    c2_PHI_ctrl[3] = c2_phi_die_err;
    c2_PHI_ctrl[4] = 0.0;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 158U);
    c2_g_a[0] = c2_Ki_roll;
    for (c2_i39 = 0; c2_i39 < 4; c2_i39++) {
      c2_g_a[c2_i39 + 1] = c2_b_LQR_phi[c2_i39];
    }

    for (c2_i40 = 0; c2_i40 < 5; c2_i40++) {
      c2_m_b[c2_i40] = c2_PHI_ctrl[c2_i40];
    }

    c2_eml_scalar_eg(chartInstance);
    c2_eml_scalar_eg(chartInstance);
    c2_roll_control = 0.0;
    for (c2_k = 1; c2_k < 6; c2_k++) {
      c2_b_k = c2_k;
      c2_roll_control += c2_g_a[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
        _SFD_INTEGER_CHECK("", (real_T)c2_b_k), 1, 5, 1, 0) - 1] *
        c2_m_b[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK("",
        (real_T)c2_b_k), 1, 5, 1, 0) - 1];
    }

    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 161U);
    c2_h_a = c2_b_PolyRL_U2R[0];
    c2_n_b = -c2_roll_control + c2_pitch_control;
    c2_ca_m_right = c2_h_a * c2_n_b;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 162U);
    c2_i_a = c2_b_PolyRL_U2R[0];
    c2_o_b = c2_roll_control + c2_pitch_control;
    c2_ca_m_left = c2_i_a * c2_o_b;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 163U);
    c2_j_a = c2_b_PolyT_U2R[0];
    c2_p_b = -c2_pitch_control;
    c2_ca_m_tail = c2_j_a * c2_p_b;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 165U);
    c2_ca_s_right = 0.0;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 166U);
    c2_ca_s_left = 0.0;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 167U);
    c2_k_a = c2_b_PolyGT_U2A[0];
    c2_q_b = -c2_yaw_control;
    c2_ca_s_tail = c2_k_a * c2_q_b;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 170U);
    c2_CA_M_Theta[0] = c2_ca_m_right;
    c2_CA_M_Theta[1] = c2_ca_m_left;
    c2_CA_M_Theta[2] = c2_ca_m_tail;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 171U);
    c2_CA_S_Theta[0] = c2_ca_s_right;
    c2_CA_S_Theta[1] = c2_ca_s_left;
    c2_CA_S_Theta[2] = c2_ca_s_tail;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 173U);
    for (c2_i41 = 0; c2_i41 < 3; c2_i41++) {
      chartInstance->c2_CA_M_Theta_km1[c2_i41] = c2_CA_M_Theta[c2_i41];
    }

    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 174U);
    for (c2_i42 = 0; c2_i42 < 3; c2_i42++) {
      chartInstance->c2_CA_S_Theta_km1[c2_i42] = c2_CA_S_Theta[c2_i42];
    }
  } else {
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 177U);
    for (c2_i43 = 0; c2_i43 < 3; c2_i43++) {
      c2_CA_M_Theta[c2_i43] = chartInstance->c2_CA_M_Theta_km1[c2_i43];
    }

    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 178U);
    for (c2_i44 = 0; c2_i44 < 3; c2_i44++) {
      c2_CA_S_Theta[c2_i44] = chartInstance->c2_CA_S_Theta_km1[c2_i44];
    }
  }

  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, -178);
  _SFD_SYMBOL_SCOPE_POP();
  for (c2_i45 = 0; c2_i45 < 3; c2_i45++) {
    (*c2_b_CA_M_Theta)[c2_i45] = c2_CA_M_Theta[c2_i45];
  }

  for (c2_i46 = 0; c2_i46 < 3; c2_i46++) {
    (*c2_b_CA_S_Theta)[c2_i46] = c2_CA_S_Theta[c2_i46];
  }

  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 1U, chartInstance->c2_sfEvent);
}

static void initSimStructsc2_TTR_mdl(SFc2_TTR_mdlInstanceStruct *chartInstance)
{
}

static void registerMessagesc2_TTR_mdl(SFc2_TTR_mdlInstanceStruct *chartInstance)
{
}

static void init_script_number_translation(uint32_T c2_machineNumber, uint32_T
  c2_chartNumber)
{
}

static const mxArray *c2_sf_marshallOut(void *chartInstanceVoid, void *c2_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  real_T c2_u;
  const mxArray *c2_y = NULL;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_u = *(real_T *)c2_inData;
  c2_y = NULL;
  if (!chartInstance->c2_psi_err_int_not_empty) {
    sf_mex_assign(&c2_y, sf_mex_create("y", NULL, 0, 0U, 1U, 0U, 2, 0, 0), FALSE);
  } else {
    sf_mex_assign(&c2_y, sf_mex_create("y", &c2_u, 0, 0U, 0U, 0U, 0), FALSE);
  }

  sf_mex_assign(&c2_mxArrayOutData, c2_y, FALSE);
  return c2_mxArrayOutData;
}

static real_T c2_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_b_psi_err_int, const char_T *c2_identifier)
{
  real_T c2_y;
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_y = c2_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_psi_err_int),
    &c2_thisId);
  sf_mex_destroy(&c2_b_psi_err_int);
  return c2_y;
}

static real_T c2_b_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId)
{
  real_T c2_y;
  real_T c2_d0;
  if (mxIsEmpty(c2_u)) {
    chartInstance->c2_psi_err_int_not_empty = FALSE;
  } else {
    chartInstance->c2_psi_err_int_not_empty = TRUE;
    sf_mex_import(c2_parentId, sf_mex_dup(c2_u), &c2_d0, 1, 0, 0U, 0, 0U, 0);
    c2_y = c2_d0;
  }

  sf_mex_destroy(&c2_u);
  return c2_y;
}

static void c2_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_psi_err_int;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_y;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_b_psi_err_int = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_y = c2_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_psi_err_int),
    &c2_thisId);
  sf_mex_destroy(&c2_b_psi_err_int);
  *(real_T *)c2_outData = c2_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_b_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  real_T c2_u;
  const mxArray *c2_y = NULL;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_u = *(real_T *)c2_inData;
  c2_y = NULL;
  if (!chartInstance->c2_theta_err_int_not_empty) {
    sf_mex_assign(&c2_y, sf_mex_create("y", NULL, 0, 0U, 1U, 0U, 2, 0, 0), FALSE);
  } else {
    sf_mex_assign(&c2_y, sf_mex_create("y", &c2_u, 0, 0U, 0U, 0U, 0), FALSE);
  }

  sf_mex_assign(&c2_mxArrayOutData, c2_y, FALSE);
  return c2_mxArrayOutData;
}

static real_T c2_c_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_b_theta_err_int, const char_T *c2_identifier)
{
  real_T c2_y;
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_y = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_theta_err_int),
    &c2_thisId);
  sf_mex_destroy(&c2_b_theta_err_int);
  return c2_y;
}

static real_T c2_d_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId)
{
  real_T c2_y;
  real_T c2_d1;
  if (mxIsEmpty(c2_u)) {
    chartInstance->c2_theta_err_int_not_empty = FALSE;
  } else {
    chartInstance->c2_theta_err_int_not_empty = TRUE;
    sf_mex_import(c2_parentId, sf_mex_dup(c2_u), &c2_d1, 1, 0, 0U, 0, 0U, 0);
    c2_y = c2_d1;
  }

  sf_mex_destroy(&c2_u);
  return c2_y;
}

static void c2_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_theta_err_int;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_y;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_b_theta_err_int = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_y = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_theta_err_int),
    &c2_thisId);
  sf_mex_destroy(&c2_b_theta_err_int);
  *(real_T *)c2_outData = c2_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_c_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  real_T c2_u;
  const mxArray *c2_y = NULL;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_u = *(real_T *)c2_inData;
  c2_y = NULL;
  if (!chartInstance->c2_phi_err_int_not_empty) {
    sf_mex_assign(&c2_y, sf_mex_create("y", NULL, 0, 0U, 1U, 0U, 2, 0, 0), FALSE);
  } else {
    sf_mex_assign(&c2_y, sf_mex_create("y", &c2_u, 0, 0U, 0U, 0U, 0), FALSE);
  }

  sf_mex_assign(&c2_mxArrayOutData, c2_y, FALSE);
  return c2_mxArrayOutData;
}

static real_T c2_e_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_b_phi_err_int, const char_T *c2_identifier)
{
  real_T c2_y;
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_y = c2_f_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_phi_err_int),
    &c2_thisId);
  sf_mex_destroy(&c2_b_phi_err_int);
  return c2_y;
}

static real_T c2_f_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId)
{
  real_T c2_y;
  real_T c2_d2;
  if (mxIsEmpty(c2_u)) {
    chartInstance->c2_phi_err_int_not_empty = FALSE;
  } else {
    chartInstance->c2_phi_err_int_not_empty = TRUE;
    sf_mex_import(c2_parentId, sf_mex_dup(c2_u), &c2_d2, 1, 0, 0U, 0, 0U, 0);
    c2_y = c2_d2;
  }

  sf_mex_destroy(&c2_u);
  return c2_y;
}

static void c2_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_phi_err_int;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_y;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_b_phi_err_int = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_y = c2_f_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_phi_err_int),
    &c2_thisId);
  sf_mex_destroy(&c2_b_phi_err_int);
  *(real_T *)c2_outData = c2_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_d_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  int32_T c2_i47;
  real_T c2_b_inData[3];
  int32_T c2_i48;
  real_T c2_u[3];
  const mxArray *c2_y = NULL;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  for (c2_i47 = 0; c2_i47 < 3; c2_i47++) {
    c2_b_inData[c2_i47] = (*(real_T (*)[3])c2_inData)[c2_i47];
  }

  for (c2_i48 = 0; c2_i48 < 3; c2_i48++) {
    c2_u[c2_i48] = c2_b_inData[c2_i48];
  }

  c2_y = NULL;
  if (!chartInstance->c2_CA_S_Theta_km1_not_empty) {
    sf_mex_assign(&c2_y, sf_mex_create("y", NULL, 0, 0U, 1U, 0U, 2, 0, 0), FALSE);
  } else {
    sf_mex_assign(&c2_y, sf_mex_create("y", c2_u, 0, 0U, 1U, 0U, 1, 3), FALSE);
  }

  sf_mex_assign(&c2_mxArrayOutData, c2_y, FALSE);
  return c2_mxArrayOutData;
}

static void c2_g_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_b_CA_S_Theta_km1, const char_T *c2_identifier, real_T c2_y[3])
{
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_h_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_CA_S_Theta_km1),
                        &c2_thisId, c2_y);
  sf_mex_destroy(&c2_b_CA_S_Theta_km1);
}

static void c2_h_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId, real_T c2_y[3])
{
  real_T c2_dv14[3];
  int32_T c2_i49;
  if (mxIsEmpty(c2_u)) {
    chartInstance->c2_CA_S_Theta_km1_not_empty = FALSE;
  } else {
    chartInstance->c2_CA_S_Theta_km1_not_empty = TRUE;
    sf_mex_import(c2_parentId, sf_mex_dup(c2_u), c2_dv14, 1, 0, 0U, 1, 0U, 1, 3);
    for (c2_i49 = 0; c2_i49 < 3; c2_i49++) {
      c2_y[c2_i49] = c2_dv14[c2_i49];
    }
  }

  sf_mex_destroy(&c2_u);
}

static void c2_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_CA_S_Theta_km1;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_y[3];
  int32_T c2_i50;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_b_CA_S_Theta_km1 = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_h_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_CA_S_Theta_km1),
                        &c2_thisId, c2_y);
  sf_mex_destroy(&c2_b_CA_S_Theta_km1);
  for (c2_i50 = 0; c2_i50 < 3; c2_i50++) {
    (*(real_T (*)[3])c2_outData)[c2_i50] = c2_y[c2_i50];
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_e_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  int32_T c2_i51;
  real_T c2_b_inData[3];
  int32_T c2_i52;
  real_T c2_u[3];
  const mxArray *c2_y = NULL;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  for (c2_i51 = 0; c2_i51 < 3; c2_i51++) {
    c2_b_inData[c2_i51] = (*(real_T (*)[3])c2_inData)[c2_i51];
  }

  for (c2_i52 = 0; c2_i52 < 3; c2_i52++) {
    c2_u[c2_i52] = c2_b_inData[c2_i52];
  }

  c2_y = NULL;
  if (!chartInstance->c2_CA_M_Theta_km1_not_empty) {
    sf_mex_assign(&c2_y, sf_mex_create("y", NULL, 0, 0U, 1U, 0U, 2, 0, 0), FALSE);
  } else {
    sf_mex_assign(&c2_y, sf_mex_create("y", c2_u, 0, 0U, 1U, 0U, 1, 3), FALSE);
  }

  sf_mex_assign(&c2_mxArrayOutData, c2_y, FALSE);
  return c2_mxArrayOutData;
}

static void c2_i_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_b_CA_M_Theta_km1, const char_T *c2_identifier, real_T c2_y[3])
{
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_j_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_CA_M_Theta_km1),
                        &c2_thisId, c2_y);
  sf_mex_destroy(&c2_b_CA_M_Theta_km1);
}

static void c2_j_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId, real_T c2_y[3])
{
  real_T c2_dv15[3];
  int32_T c2_i53;
  if (mxIsEmpty(c2_u)) {
    chartInstance->c2_CA_M_Theta_km1_not_empty = FALSE;
  } else {
    chartInstance->c2_CA_M_Theta_km1_not_empty = TRUE;
    sf_mex_import(c2_parentId, sf_mex_dup(c2_u), c2_dv15, 1, 0, 0U, 1, 0U, 1, 3);
    for (c2_i53 = 0; c2_i53 < 3; c2_i53++) {
      c2_y[c2_i53] = c2_dv15[c2_i53];
    }
  }

  sf_mex_destroy(&c2_u);
}

static void c2_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_CA_M_Theta_km1;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_y[3];
  int32_T c2_i54;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_b_CA_M_Theta_km1 = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_j_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_CA_M_Theta_km1),
                        &c2_thisId, c2_y);
  sf_mex_destroy(&c2_b_CA_M_Theta_km1);
  for (c2_i54 = 0; c2_i54 < 3; c2_i54++) {
    (*(real_T (*)[3])c2_outData)[c2_i54] = c2_y[c2_i54];
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_f_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  real_T c2_u;
  const mxArray *c2_y = NULL;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_u = *(real_T *)c2_inData;
  c2_y = NULL;
  if (!chartInstance->c2_t_km1_not_empty) {
    sf_mex_assign(&c2_y, sf_mex_create("y", NULL, 0, 0U, 1U, 0U, 2, 0, 0), FALSE);
  } else {
    sf_mex_assign(&c2_y, sf_mex_create("y", &c2_u, 0, 0U, 0U, 0U, 0), FALSE);
  }

  sf_mex_assign(&c2_mxArrayOutData, c2_y, FALSE);
  return c2_mxArrayOutData;
}

static real_T c2_k_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_b_t_km1, const char_T *c2_identifier)
{
  real_T c2_y;
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_y = c2_l_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_t_km1), &c2_thisId);
  sf_mex_destroy(&c2_b_t_km1);
  return c2_y;
}

static real_T c2_l_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId)
{
  real_T c2_y;
  real_T c2_d3;
  if (mxIsEmpty(c2_u)) {
    chartInstance->c2_t_km1_not_empty = FALSE;
  } else {
    chartInstance->c2_t_km1_not_empty = TRUE;
    sf_mex_import(c2_parentId, sf_mex_dup(c2_u), &c2_d3, 1, 0, 0U, 0, 0U, 0);
    c2_y = c2_d3;
  }

  sf_mex_destroy(&c2_u);
  return c2_y;
}

static void c2_f_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_t_km1;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_y;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_b_t_km1 = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_y = c2_l_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_t_km1), &c2_thisId);
  sf_mex_destroy(&c2_b_t_km1);
  *(real_T *)c2_outData = c2_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_g_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  int32_T c2_i55;
  real_T c2_b_inData[3];
  int32_T c2_i56;
  real_T c2_u[3];
  const mxArray *c2_y = NULL;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  for (c2_i55 = 0; c2_i55 < 3; c2_i55++) {
    c2_b_inData[c2_i55] = (*(real_T (*)[3])c2_inData)[c2_i55];
  }

  for (c2_i56 = 0; c2_i56 < 3; c2_i56++) {
    c2_u[c2_i56] = c2_b_inData[c2_i56];
  }

  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_create("y", c2_u, 0, 0U, 1U, 0U, 1, 3), FALSE);
  sf_mex_assign(&c2_mxArrayOutData, c2_y, FALSE);
  return c2_mxArrayOutData;
}

static void c2_m_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_CA_S_Theta, const char_T *c2_identifier, real_T c2_y[3])
{
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_n_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_CA_S_Theta), &c2_thisId,
                        c2_y);
  sf_mex_destroy(&c2_CA_S_Theta);
}

static void c2_n_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId, real_T c2_y[3])
{
  real_T c2_dv16[3];
  int32_T c2_i57;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), c2_dv16, 1, 0, 0U, 1, 0U, 1, 3);
  for (c2_i57 = 0; c2_i57 < 3; c2_i57++) {
    c2_y[c2_i57] = c2_dv16[c2_i57];
  }

  sf_mex_destroy(&c2_u);
}

static void c2_g_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_CA_S_Theta;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_y[3];
  int32_T c2_i58;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_CA_S_Theta = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_n_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_CA_S_Theta), &c2_thisId,
                        c2_y);
  sf_mex_destroy(&c2_CA_S_Theta);
  for (c2_i58 = 0; c2_i58 < 3; c2_i58++) {
    (*(real_T (*)[3])c2_outData)[c2_i58] = c2_y[c2_i58];
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_h_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  int32_T c2_i59;
  real_T c2_b_inData[4];
  int32_T c2_i60;
  real_T c2_u[4];
  const mxArray *c2_y = NULL;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  for (c2_i59 = 0; c2_i59 < 4; c2_i59++) {
    c2_b_inData[c2_i59] = (*(real_T (*)[4])c2_inData)[c2_i59];
  }

  for (c2_i60 = 0; c2_i60 < 4; c2_i60++) {
    c2_u[c2_i60] = c2_b_inData[c2_i60];
  }

  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_create("y", c2_u, 0, 0U, 1U, 0U, 2, 1, 4), FALSE);
  sf_mex_assign(&c2_mxArrayOutData, c2_y, FALSE);
  return c2_mxArrayOutData;
}

static void c2_o_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId, real_T c2_y[4])
{
  real_T c2_dv17[4];
  int32_T c2_i61;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), c2_dv17, 1, 0, 0U, 1, 0U, 2, 1, 4);
  for (c2_i61 = 0; c2_i61 < 4; c2_i61++) {
    c2_y[c2_i61] = c2_dv17[c2_i61];
  }

  sf_mex_destroy(&c2_u);
}

static void c2_h_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_LQR_phi;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_y[4];
  int32_T c2_i62;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_b_LQR_phi = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_o_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_LQR_phi), &c2_thisId,
                        c2_y);
  sf_mex_destroy(&c2_b_LQR_phi);
  for (c2_i62 = 0; c2_i62 < 4; c2_i62++) {
    (*(real_T (*)[4])c2_outData)[c2_i62] = c2_y[c2_i62];
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_i_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  int32_T c2_i63;
  real_T c2_b_inData[2];
  int32_T c2_i64;
  real_T c2_u[2];
  const mxArray *c2_y = NULL;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  for (c2_i63 = 0; c2_i63 < 2; c2_i63++) {
    c2_b_inData[c2_i63] = (*(real_T (*)[2])c2_inData)[c2_i63];
  }

  for (c2_i64 = 0; c2_i64 < 2; c2_i64++) {
    c2_u[c2_i64] = c2_b_inData[c2_i64];
  }

  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_create("y", c2_u, 0, 0U, 1U, 0U, 2, 1, 2), FALSE);
  sf_mex_assign(&c2_mxArrayOutData, c2_y, FALSE);
  return c2_mxArrayOutData;
}

static void c2_p_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId, real_T c2_y[2])
{
  real_T c2_dv18[2];
  int32_T c2_i65;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), c2_dv18, 1, 0, 0U, 1, 0U, 2, 1, 2);
  for (c2_i65 = 0; c2_i65 < 2; c2_i65++) {
    c2_y[c2_i65] = c2_dv18[c2_i65];
  }

  sf_mex_destroy(&c2_u);
}

static void c2_i_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_PolyGT_U2A;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_y[2];
  int32_T c2_i66;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_b_PolyGT_U2A = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_p_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_PolyGT_U2A), &c2_thisId,
                        c2_y);
  sf_mex_destroy(&c2_b_PolyGT_U2A);
  for (c2_i66 = 0; c2_i66 < 2; c2_i66++) {
    (*(real_T (*)[2])c2_outData)[c2_i66] = c2_y[c2_i66];
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_j_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  real_T c2_u;
  const mxArray *c2_y = NULL;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_u = *(real_T *)c2_inData;
  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_create("y", &c2_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c2_mxArrayOutData, c2_y, FALSE);
  return c2_mxArrayOutData;
}

static real_T c2_q_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId)
{
  real_T c2_y;
  real_T c2_d4;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), &c2_d4, 1, 0, 0U, 0, 0U, 0);
  c2_y = c2_d4;
  sf_mex_destroy(&c2_u);
  return c2_y;
}

static void c2_j_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_nargout;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_y;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_nargout = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_y = c2_q_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_nargout), &c2_thisId);
  sf_mex_destroy(&c2_nargout);
  *(real_T *)c2_outData = c2_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_k_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  int32_T c2_i67;
  real_T c2_b_inData[5];
  int32_T c2_i68;
  real_T c2_u[5];
  const mxArray *c2_y = NULL;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  for (c2_i67 = 0; c2_i67 < 5; c2_i67++) {
    c2_b_inData[c2_i67] = (*(real_T (*)[5])c2_inData)[c2_i67];
  }

  for (c2_i68 = 0; c2_i68 < 5; c2_i68++) {
    c2_u[c2_i68] = c2_b_inData[c2_i68];
  }

  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_create("y", c2_u, 0, 0U, 1U, 0U, 1, 5), FALSE);
  sf_mex_assign(&c2_mxArrayOutData, c2_y, FALSE);
  return c2_mxArrayOutData;
}

static void c2_r_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId, real_T c2_y[5])
{
  real_T c2_dv19[5];
  int32_T c2_i69;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), c2_dv19, 1, 0, 0U, 1, 0U, 1, 5);
  for (c2_i69 = 0; c2_i69 < 5; c2_i69++) {
    c2_y[c2_i69] = c2_dv19[c2_i69];
  }

  sf_mex_destroy(&c2_u);
}

static void c2_k_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_PHI_ctrl;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_y[5];
  int32_T c2_i70;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_PHI_ctrl = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_r_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_PHI_ctrl), &c2_thisId, c2_y);
  sf_mex_destroy(&c2_PHI_ctrl);
  for (c2_i70 = 0; c2_i70 < 5; c2_i70++) {
    (*(real_T (*)[5])c2_outData)[c2_i70] = c2_y[c2_i70];
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_l_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  int32_T c2_i71;
  real_T c2_b_inData[3];
  int32_T c2_i72;
  real_T c2_u[3];
  const mxArray *c2_y = NULL;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  for (c2_i71 = 0; c2_i71 < 3; c2_i71++) {
    c2_b_inData[c2_i71] = (*(real_T (*)[3])c2_inData)[c2_i71];
  }

  for (c2_i72 = 0; c2_i72 < 3; c2_i72++) {
    c2_u[c2_i72] = c2_b_inData[c2_i72];
  }

  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_create("y", c2_u, 0, 0U, 1U, 0U, 2, 1, 3), FALSE);
  sf_mex_assign(&c2_mxArrayOutData, c2_y, FALSE);
  return c2_mxArrayOutData;
}

const mxArray *sf_c2_TTR_mdl_get_eml_resolved_functions_info(void)
{
  const mxArray *c2_nameCaptureInfo;
  c2_ResolvedFunctionInfo c2_info[46];
  const mxArray *c2_m0 = NULL;
  int32_T c2_i73;
  c2_ResolvedFunctionInfo *c2_r0;
  c2_nameCaptureInfo = NULL;
  c2_nameCaptureInfo = NULL;
  c2_info_helper(c2_info);
  sf_mex_assign(&c2_m0, sf_mex_createstruct("nameCaptureInfo", 1, 46), FALSE);
  for (c2_i73 = 0; c2_i73 < 46; c2_i73++) {
    c2_r0 = &c2_info[c2_i73];
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", c2_r0->context, 15,
      0U, 0U, 0U, 2, 1, strlen(c2_r0->context)), "context", "nameCaptureInfo",
                    c2_i73);
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", c2_r0->name, 15, 0U,
      0U, 0U, 2, 1, strlen(c2_r0->name)), "name", "nameCaptureInfo", c2_i73);
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", c2_r0->dominantType,
      15, 0U, 0U, 0U, 2, 1, strlen(c2_r0->dominantType)), "dominantType",
                    "nameCaptureInfo", c2_i73);
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", c2_r0->resolved, 15,
      0U, 0U, 0U, 2, 1, strlen(c2_r0->resolved)), "resolved", "nameCaptureInfo",
                    c2_i73);
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", &c2_r0->fileTimeLo,
      7, 0U, 0U, 0U, 0), "fileTimeLo", "nameCaptureInfo", c2_i73);
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", &c2_r0->fileTimeHi,
      7, 0U, 0U, 0U, 0), "fileTimeHi", "nameCaptureInfo", c2_i73);
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", &c2_r0->mFileTimeLo,
      7, 0U, 0U, 0U, 0), "mFileTimeLo", "nameCaptureInfo", c2_i73);
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", &c2_r0->mFileTimeHi,
      7, 0U, 0U, 0U, 0), "mFileTimeHi", "nameCaptureInfo", c2_i73);
  }

  sf_mex_assign(&c2_nameCaptureInfo, c2_m0, FALSE);
  sf_mex_emlrtNameCapturePostProcessR2012a(&c2_nameCaptureInfo);
  return c2_nameCaptureInfo;
}

static void c2_info_helper(c2_ResolvedFunctionInfo c2_info[46])
{
  c2_info[0].context = "";
  c2_info[0].name = "mrdivide";
  c2_info[0].dominantType = "double";
  c2_info[0].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p";
  c2_info[0].fileTimeLo = 1357951548U;
  c2_info[0].fileTimeHi = 0U;
  c2_info[0].mFileTimeLo = 1319729966U;
  c2_info[0].mFileTimeHi = 0U;
  c2_info[1].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p";
  c2_info[1].name = "rdivide";
  c2_info[1].dominantType = "double";
  c2_info[1].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m";
  c2_info[1].fileTimeLo = 1346510388U;
  c2_info[1].fileTimeHi = 0U;
  c2_info[1].mFileTimeLo = 0U;
  c2_info[1].mFileTimeHi = 0U;
  c2_info[2].context = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m";
  c2_info[2].name = "eml_scalexp_compatible";
  c2_info[2].dominantType = "double";
  c2_info[2].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_compatible.m";
  c2_info[2].fileTimeLo = 1286818796U;
  c2_info[2].fileTimeHi = 0U;
  c2_info[2].mFileTimeLo = 0U;
  c2_info[2].mFileTimeHi = 0U;
  c2_info[3].context = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m";
  c2_info[3].name = "eml_div";
  c2_info[3].dominantType = "double";
  c2_info[3].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_div.m";
  c2_info[3].fileTimeLo = 1313347810U;
  c2_info[3].fileTimeHi = 0U;
  c2_info[3].mFileTimeLo = 0U;
  c2_info[3].mFileTimeHi = 0U;
  c2_info[4].context = "";
  c2_info[4].name = "mtimes";
  c2_info[4].dominantType = "double";
  c2_info[4].resolved = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c2_info[4].fileTimeLo = 1289519692U;
  c2_info[4].fileTimeHi = 0U;
  c2_info[4].mFileTimeLo = 0U;
  c2_info[4].mFileTimeHi = 0U;
  c2_info[5].context = "";
  c2_info[5].name = "abs";
  c2_info[5].dominantType = "double";
  c2_info[5].resolved = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m";
  c2_info[5].fileTimeLo = 1343830366U;
  c2_info[5].fileTimeHi = 0U;
  c2_info[5].mFileTimeLo = 0U;
  c2_info[5].mFileTimeHi = 0U;
  c2_info[6].context = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m";
  c2_info[6].name = "eml_scalar_abs";
  c2_info[6].dominantType = "double";
  c2_info[6].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_abs.m";
  c2_info[6].fileTimeLo = 1286818712U;
  c2_info[6].fileTimeHi = 0U;
  c2_info[6].mFileTimeLo = 0U;
  c2_info[6].mFileTimeHi = 0U;
  c2_info[7].context = "";
  c2_info[7].name = "length";
  c2_info[7].dominantType = "double";
  c2_info[7].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/length.m";
  c2_info[7].fileTimeLo = 1303146206U;
  c2_info[7].fileTimeHi = 0U;
  c2_info[7].mFileTimeLo = 0U;
  c2_info[7].mFileTimeHi = 0U;
  c2_info[8].context = "";
  c2_info[8].name = "interp1";
  c2_info[8].dominantType = "double";
  c2_info[8].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/polyfun/interp1.m";
  c2_info[8].fileTimeLo = 1332168672U;
  c2_info[8].fileTimeHi = 0U;
  c2_info[8].mFileTimeLo = 0U;
  c2_info[8].mFileTimeHi = 0U;
  c2_info[9].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/polyfun/interp1.m!interp1_work";
  c2_info[9].name = "eml_index_class";
  c2_info[9].dominantType = "";
  c2_info[9].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c2_info[9].fileTimeLo = 1323170578U;
  c2_info[9].fileTimeHi = 0U;
  c2_info[9].mFileTimeLo = 0U;
  c2_info[9].mFileTimeHi = 0U;
  c2_info[10].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/polyfun/interp1.m!interp1_work";
  c2_info[10].name = "eml_scalar_eg";
  c2_info[10].dominantType = "double";
  c2_info[10].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m";
  c2_info[10].fileTimeLo = 1286818796U;
  c2_info[10].fileTimeHi = 0U;
  c2_info[10].mFileTimeLo = 0U;
  c2_info[10].mFileTimeHi = 0U;
  c2_info[11].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/polyfun/interp1.m!interp1_work";
  c2_info[11].name = "eml_int_forloop_overflow_check";
  c2_info[11].dominantType = "";
  c2_info[11].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_int_forloop_overflow_check.m";
  c2_info[11].fileTimeLo = 1346510340U;
  c2_info[11].fileTimeHi = 0U;
  c2_info[11].mFileTimeLo = 0U;
  c2_info[11].mFileTimeHi = 0U;
  c2_info[12].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_int_forloop_overflow_check.m!eml_int_forloop_overflow_check_helper";
  c2_info[12].name = "intmax";
  c2_info[12].dominantType = "char";
  c2_info[12].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/intmax.m";
  c2_info[12].fileTimeLo = 1311255316U;
  c2_info[12].fileTimeHi = 0U;
  c2_info[12].mFileTimeLo = 0U;
  c2_info[12].mFileTimeHi = 0U;
  c2_info[13].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/polyfun/interp1.m!interp1_work";
  c2_info[13].name = "isnan";
  c2_info[13].dominantType = "double";
  c2_info[13].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isnan.m";
  c2_info[13].fileTimeLo = 1286818760U;
  c2_info[13].fileTimeHi = 0U;
  c2_info[13].mFileTimeLo = 0U;
  c2_info[13].mFileTimeHi = 0U;
  c2_info[14].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/polyfun/interp1.m!interp1_work";
  c2_info[14].name = "eml_error";
  c2_info[14].dominantType = "char";
  c2_info[14].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_error.m";
  c2_info[14].fileTimeLo = 1343830358U;
  c2_info[14].fileTimeHi = 0U;
  c2_info[14].mFileTimeLo = 0U;
  c2_info[14].mFileTimeHi = 0U;
  c2_info[15].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/polyfun/interp1.m!interp1_work";
  c2_info[15].name = "fliplr";
  c2_info[15].dominantType = "double";
  c2_info[15].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/fliplr.m";
  c2_info[15].fileTimeLo = 1286818688U;
  c2_info[15].fileTimeHi = 0U;
  c2_info[15].mFileTimeLo = 0U;
  c2_info[15].mFileTimeHi = 0U;
  c2_info[16].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/fliplr.m";
  c2_info[16].name = "eml_index_class";
  c2_info[16].dominantType = "";
  c2_info[16].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c2_info[16].fileTimeLo = 1323170578U;
  c2_info[16].fileTimeHi = 0U;
  c2_info[16].mFileTimeLo = 0U;
  c2_info[16].mFileTimeHi = 0U;
  c2_info[17].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/fliplr.m";
  c2_info[17].name = "eml_index_rdivide";
  c2_info[17].dominantType = "double";
  c2_info[17].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_rdivide.m";
  c2_info[17].fileTimeLo = 1286818780U;
  c2_info[17].fileTimeHi = 0U;
  c2_info[17].mFileTimeLo = 0U;
  c2_info[17].mFileTimeHi = 0U;
  c2_info[18].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_rdivide.m";
  c2_info[18].name = "eml_index_class";
  c2_info[18].dominantType = "";
  c2_info[18].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c2_info[18].fileTimeLo = 1323170578U;
  c2_info[18].fileTimeHi = 0U;
  c2_info[18].mFileTimeLo = 0U;
  c2_info[18].mFileTimeHi = 0U;
  c2_info[19].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/fliplr.m";
  c2_info[19].name = "eml_index_minus";
  c2_info[19].dominantType = "coder.internal.indexInt";
  c2_info[19].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_minus.m";
  c2_info[19].fileTimeLo = 1286818778U;
  c2_info[19].fileTimeHi = 0U;
  c2_info[19].mFileTimeLo = 0U;
  c2_info[19].mFileTimeHi = 0U;
  c2_info[20].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_minus.m";
  c2_info[20].name = "eml_index_class";
  c2_info[20].dominantType = "";
  c2_info[20].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c2_info[20].fileTimeLo = 1323170578U;
  c2_info[20].fileTimeHi = 0U;
  c2_info[20].mFileTimeLo = 0U;
  c2_info[20].mFileTimeHi = 0U;
  c2_info[21].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/fliplr.m";
  c2_info[21].name = "eml_index_plus";
  c2_info[21].dominantType = "double";
  c2_info[21].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_plus.m";
  c2_info[21].fileTimeLo = 1286818778U;
  c2_info[21].fileTimeHi = 0U;
  c2_info[21].mFileTimeLo = 0U;
  c2_info[21].mFileTimeHi = 0U;
  c2_info[22].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_plus.m";
  c2_info[22].name = "eml_index_class";
  c2_info[22].dominantType = "";
  c2_info[22].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c2_info[22].fileTimeLo = 1323170578U;
  c2_info[22].fileTimeHi = 0U;
  c2_info[22].mFileTimeLo = 0U;
  c2_info[22].mFileTimeHi = 0U;
  c2_info[23].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/polyfun/interp1.m!interp1_work";
  c2_info[23].name = "eml_bsearch";
  c2_info[23].dominantType = "double";
  c2_info[23].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_bsearch.m";
  c2_info[23].fileTimeLo = 1286818696U;
  c2_info[23].fileTimeHi = 0U;
  c2_info[23].mFileTimeLo = 0U;
  c2_info[23].mFileTimeHi = 0U;
  c2_info[24].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_bsearch.m";
  c2_info[24].name = "eml_index_class";
  c2_info[24].dominantType = "";
  c2_info[24].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c2_info[24].fileTimeLo = 1323170578U;
  c2_info[24].fileTimeHi = 0U;
  c2_info[24].mFileTimeLo = 0U;
  c2_info[24].mFileTimeHi = 0U;
  c2_info[25].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_bsearch.m";
  c2_info[25].name = "eml_unsigned_class";
  c2_info[25].dominantType = "char";
  c2_info[25].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_unsigned_class.m";
  c2_info[25].fileTimeLo = 1323170580U;
  c2_info[25].fileTimeHi = 0U;
  c2_info[25].mFileTimeLo = 0U;
  c2_info[25].mFileTimeHi = 0U;
  c2_info[26].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_unsigned_class.m";
  c2_info[26].name = "eml_index_class";
  c2_info[26].dominantType = "";
  c2_info[26].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c2_info[26].fileTimeLo = 1323170578U;
  c2_info[26].fileTimeHi = 0U;
  c2_info[26].mFileTimeLo = 0U;
  c2_info[26].mFileTimeHi = 0U;
  c2_info[27].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_bsearch.m";
  c2_info[27].name = "intmax";
  c2_info[27].dominantType = "char";
  c2_info[27].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/intmax.m";
  c2_info[27].fileTimeLo = 1311255316U;
  c2_info[27].fileTimeHi = 0U;
  c2_info[27].mFileTimeLo = 0U;
  c2_info[27].mFileTimeHi = 0U;
  c2_info[28].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/polyfun/interp1.m!interp1_work";
  c2_info[28].name = "mtimes";
  c2_info[28].dominantType = "coder.internal.indexInt";
  c2_info[28].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c2_info[28].fileTimeLo = 1289519692U;
  c2_info[28].fileTimeHi = 0U;
  c2_info[28].mFileTimeLo = 0U;
  c2_info[28].mFileTimeHi = 0U;
  c2_info[29].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/polyfun/interp1.m!interp1_work";
  c2_info[29].name = "mtimes";
  c2_info[29].dominantType = "double";
  c2_info[29].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c2_info[29].fileTimeLo = 1289519692U;
  c2_info[29].fileTimeHi = 0U;
  c2_info[29].mFileTimeLo = 0U;
  c2_info[29].mFileTimeHi = 0U;
  c2_info[30].context = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c2_info[30].name = "eml_index_class";
  c2_info[30].dominantType = "";
  c2_info[30].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c2_info[30].fileTimeLo = 1323170578U;
  c2_info[30].fileTimeHi = 0U;
  c2_info[30].mFileTimeLo = 0U;
  c2_info[30].mFileTimeHi = 0U;
  c2_info[31].context = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c2_info[31].name = "eml_scalar_eg";
  c2_info[31].dominantType = "double";
  c2_info[31].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m";
  c2_info[31].fileTimeLo = 1286818796U;
  c2_info[31].fileTimeHi = 0U;
  c2_info[31].mFileTimeLo = 0U;
  c2_info[31].mFileTimeHi = 0U;
  c2_info[32].context = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c2_info[32].name = "eml_xdotu";
  c2_info[32].dominantType = "double";
  c2_info[32].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xdotu.m";
  c2_info[32].fileTimeLo = 1299076772U;
  c2_info[32].fileTimeHi = 0U;
  c2_info[32].mFileTimeLo = 0U;
  c2_info[32].mFileTimeHi = 0U;
  c2_info[33].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xdotu.m";
  c2_info[33].name = "eml_blas_inline";
  c2_info[33].dominantType = "";
  c2_info[33].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_blas_inline.m";
  c2_info[33].fileTimeLo = 1299076768U;
  c2_info[33].fileTimeHi = 0U;
  c2_info[33].mFileTimeLo = 0U;
  c2_info[33].mFileTimeHi = 0U;
  c2_info[34].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xdotu.m";
  c2_info[34].name = "eml_xdot";
  c2_info[34].dominantType = "double";
  c2_info[34].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xdot.m";
  c2_info[34].fileTimeLo = 1299076772U;
  c2_info[34].fileTimeHi = 0U;
  c2_info[34].mFileTimeLo = 0U;
  c2_info[34].mFileTimeHi = 0U;
  c2_info[35].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xdot.m";
  c2_info[35].name = "eml_blas_inline";
  c2_info[35].dominantType = "";
  c2_info[35].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_blas_inline.m";
  c2_info[35].fileTimeLo = 1299076768U;
  c2_info[35].fileTimeHi = 0U;
  c2_info[35].mFileTimeLo = 0U;
  c2_info[35].mFileTimeHi = 0U;
  c2_info[36].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xdot.m";
  c2_info[36].name = "eml_index_class";
  c2_info[36].dominantType = "";
  c2_info[36].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c2_info[36].fileTimeLo = 1323170578U;
  c2_info[36].fileTimeHi = 0U;
  c2_info[36].mFileTimeLo = 0U;
  c2_info[36].mFileTimeHi = 0U;
  c2_info[37].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xdot.m";
  c2_info[37].name = "eml_refblas_xdot";
  c2_info[37].dominantType = "double";
  c2_info[37].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xdot.m";
  c2_info[37].fileTimeLo = 1299076772U;
  c2_info[37].fileTimeHi = 0U;
  c2_info[37].mFileTimeLo = 0U;
  c2_info[37].mFileTimeHi = 0U;
  c2_info[38].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xdot.m";
  c2_info[38].name = "eml_refblas_xdotx";
  c2_info[38].dominantType = "char";
  c2_info[38].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xdotx.m";
  c2_info[38].fileTimeLo = 1299076774U;
  c2_info[38].fileTimeHi = 0U;
  c2_info[38].mFileTimeLo = 0U;
  c2_info[38].mFileTimeHi = 0U;
  c2_info[39].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xdotx.m";
  c2_info[39].name = "eml_scalar_eg";
  c2_info[39].dominantType = "double";
  c2_info[39].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m";
  c2_info[39].fileTimeLo = 1286818796U;
  c2_info[39].fileTimeHi = 0U;
  c2_info[39].mFileTimeLo = 0U;
  c2_info[39].mFileTimeHi = 0U;
  c2_info[40].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xdotx.m";
  c2_info[40].name = "eml_index_class";
  c2_info[40].dominantType = "";
  c2_info[40].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c2_info[40].fileTimeLo = 1323170578U;
  c2_info[40].fileTimeHi = 0U;
  c2_info[40].mFileTimeLo = 0U;
  c2_info[40].mFileTimeHi = 0U;
  c2_info[41].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xdotx.m";
  c2_info[41].name = "eml_index_minus";
  c2_info[41].dominantType = "double";
  c2_info[41].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_minus.m";
  c2_info[41].fileTimeLo = 1286818778U;
  c2_info[41].fileTimeHi = 0U;
  c2_info[41].mFileTimeLo = 0U;
  c2_info[41].mFileTimeHi = 0U;
  c2_info[42].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xdotx.m";
  c2_info[42].name = "eml_index_times";
  c2_info[42].dominantType = "coder.internal.indexInt";
  c2_info[42].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_times.m";
  c2_info[42].fileTimeLo = 1286818780U;
  c2_info[42].fileTimeHi = 0U;
  c2_info[42].mFileTimeLo = 0U;
  c2_info[42].mFileTimeHi = 0U;
  c2_info[43].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_times.m";
  c2_info[43].name = "eml_index_class";
  c2_info[43].dominantType = "";
  c2_info[43].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c2_info[43].fileTimeLo = 1323170578U;
  c2_info[43].fileTimeHi = 0U;
  c2_info[43].mFileTimeLo = 0U;
  c2_info[43].mFileTimeHi = 0U;
  c2_info[44].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xdotx.m";
  c2_info[44].name = "eml_index_plus";
  c2_info[44].dominantType = "coder.internal.indexInt";
  c2_info[44].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_plus.m";
  c2_info[44].fileTimeLo = 1286818778U;
  c2_info[44].fileTimeHi = 0U;
  c2_info[44].mFileTimeLo = 0U;
  c2_info[44].mFileTimeHi = 0U;
  c2_info[45].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xdotx.m";
  c2_info[45].name = "eml_int_forloop_overflow_check";
  c2_info[45].dominantType = "";
  c2_info[45].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_int_forloop_overflow_check.m";
  c2_info[45].fileTimeLo = 1346510340U;
  c2_info[45].fileTimeHi = 0U;
  c2_info[45].mFileTimeLo = 0U;
  c2_info[45].mFileTimeHi = 0U;
}

static real_T c2_abs(SFc2_TTR_mdlInstanceStruct *chartInstance, real_T c2_x)
{
  real_T c2_b_x;
  c2_b_x = c2_x;
  return muDoubleScalarAbs(c2_b_x);
}

static real_T c2_interp1(SFc2_TTR_mdlInstanceStruct *chartInstance, real_T
  c2_varargin_1[3], real_T c2_varargin_2[3], real_T c2_varargin_3, real_T
  c2_varargin_5)
{
  real_T c2_yi;
  real_T c2_exin;
  int32_T c2_i74;
  real_T c2_y[3];
  real_T c2_xi;
  int32_T c2_i75;
  real_T c2_x[3];
  real_T c2_extrapval;
  int32_T c2_k;
  int32_T c2_b_k;
  real_T c2_b_x;
  boolean_T c2_b;
  real_T c2_xtmp;
  real_T c2_b_xtmp;
  int32_T c2_c_k;
  real_T c2_xlo;
  real_T c2_xhi;
  real_T c2_b_xi;
  int32_T c2_low_i;
  int32_T c2_low_ip1;
  int32_T c2_high_i;
  int32_T c2_b_low_i;
  int32_T c2_b_high_i;
  int32_T c2_mid_i;
  int32_T c2_n;
  real_T c2_xn;
  real_T c2_xnp1;
  real_T c2_r;
  real_T c2_onemr;
  real_T c2_y1;
  real_T c2_y2;
  real_T c2_a;
  real_T c2_b_b;
  real_T c2_b_y;
  real_T c2_b_a;
  real_T c2_c_b;
  real_T c2_c_y;
  int32_T exitg1;
  c2_exin = c2_varargin_5;
  for (c2_i74 = 0; c2_i74 < 3; c2_i74++) {
    c2_y[c2_i74] = c2_varargin_2[c2_i74];
  }

  c2_xi = c2_varargin_3;
  for (c2_i75 = 0; c2_i75 < 3; c2_i75++) {
    c2_x[c2_i75] = c2_varargin_1[c2_i75];
  }

  c2_extrapval = c2_exin;
  c2_yi = c2_extrapval;
  c2_k = 1;
  do {
    exitg1 = 0;
    if (c2_k < 4) {
      c2_b_k = c2_k;
      c2_b_x = c2_x[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK(
        "", (real_T)c2_b_k), 1, 3, 1, 0) - 1];
      c2_b = muDoubleScalarIsNaN(c2_b_x);
      if (c2_b) {
        c2_eml_error(chartInstance);
        exitg1 = 1;
      } else {
        c2_k++;
      }
    } else {
      if (c2_x[1] < c2_x[0]) {
        c2_xtmp = c2_x[0];
        c2_x[0] = c2_x[2];
        c2_x[2] = c2_xtmp;
        c2_b_xtmp = c2_y[0];
        c2_y[0] = c2_y[2];
        c2_y[2] = c2_b_xtmp;
      }

      for (c2_c_k = 2; c2_c_k < 4; c2_c_k++) {
        c2_b_k = c2_c_k;
        if (c2_x[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK("",
              (real_T)c2_b_k), 1, 3, 1, 0) - 1] <=
            c2_x[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK("",
              (real_T)(c2_b_k - 1)), 1, 3, 1, 0) - 1]) {
          c2_b_eml_error(chartInstance);
        }
      }

      c2_xlo = c2_x[0];
      c2_xhi = c2_x[2];
      if (c2_xi > c2_xhi) {
      } else if (c2_xi < c2_xlo) {
      } else {
        c2_b_xi = c2_xi;
        c2_low_i = 1;
        c2_low_ip1 = 1;
        c2_high_i = 3;
        while (c2_high_i > c2_low_ip1 + 1) {
          c2_b_low_i = c2_low_i;
          c2_b_high_i = c2_high_i;
          c2_mid_i = (c2_b_low_i + c2_b_high_i) >> 1;
          if (c2_b_xi >= c2_x[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
               _SFD_INTEGER_CHECK("", (real_T)c2_mid_i), 1, 3, 1, 0) - 1]) {
            c2_low_i = c2_mid_i;
            c2_low_ip1 = c2_low_i;
          } else {
            c2_high_i = c2_mid_i;
          }
        }

        c2_n = c2_low_i;
        c2_xn = c2_x[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK
          ("", (real_T)c2_n), 1, 3, 1, 0) - 1];
        c2_xnp1 = c2_x[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
          _SFD_INTEGER_CHECK("", (real_T)(c2_n + 1)), 1, 3, 1, 0) - 1];
        if (c2_n == 0) {
        } else {
          c2_r = (c2_xi - c2_xn) / (c2_xnp1 - c2_xn);
          c2_onemr = 1.0 - c2_r;
          c2_y1 = c2_y[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
            _SFD_INTEGER_CHECK("", (real_T)c2_n), 1, 3, 1, 0) - 1];
          c2_y2 = c2_y[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
            _SFD_INTEGER_CHECK("", (real_T)(c2_n + 1)), 1, 3, 1, 0) - 1];
          if (c2_y1 == c2_y2) {
            c2_yi = c2_y1;
          } else {
            c2_a = c2_onemr;
            c2_b_b = c2_y1;
            c2_b_y = c2_a * c2_b_b;
            c2_b_a = c2_r;
            c2_c_b = c2_y2;
            c2_c_y = c2_b_a * c2_c_b;
            c2_yi = c2_b_y + c2_c_y;
          }
        }
      }

      exitg1 = 1;
    }
  } while (exitg1 == 0);

  return c2_yi;
}

static void c2_eml_error(SFc2_TTR_mdlInstanceStruct *chartInstance)
{
  int32_T c2_i76;
  static char_T c2_cv0[21] = { 'M', 'A', 'T', 'L', 'A', 'B', ':', 'i', 'n', 't',
    'e', 'r', 'p', '1', ':', 'N', 'a', 'N', 'i', 'n', 'X' };

  char_T c2_u[21];
  const mxArray *c2_y = NULL;
  for (c2_i76 = 0; c2_i76 < 21; c2_i76++) {
    c2_u[c2_i76] = c2_cv0[c2_i76];
  }

  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_create("y", c2_u, 10, 0U, 1U, 0U, 2, 1, 21), FALSE);
  sf_mex_call_debug("error", 0U, 1U, 14, sf_mex_call_debug("message", 1U, 1U, 14,
    c2_y));
}

static void c2_b_eml_error(SFc2_TTR_mdlInstanceStruct *chartInstance)
{
  int32_T c2_i77;
  static char_T c2_cv1[35] = { 'C', 'o', 'd', 'e', 'r', ':', 't', 'o', 'o', 'l',
    'b', 'o', 'x', ':', 'i', 'n', 't', 'e', 'r', 'p', '1', '_', 'n', 'o', 'n',
    'M', 'o', 'n', 'o', 't', 'o', 'n', 'i', 'c', 'X' };

  char_T c2_u[35];
  const mxArray *c2_y = NULL;
  for (c2_i77 = 0; c2_i77 < 35; c2_i77++) {
    c2_u[c2_i77] = c2_cv1[c2_i77];
  }

  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_create("y", c2_u, 10, 0U, 1U, 0U, 2, 1, 35), FALSE);
  sf_mex_call_debug("error", 0U, 1U, 14, sf_mex_call_debug("message", 1U, 1U, 14,
    c2_y));
}

static void c2_eml_scalar_eg(SFc2_TTR_mdlInstanceStruct *chartInstance)
{
}

static const mxArray *c2_m_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  int32_T c2_u;
  const mxArray *c2_y = NULL;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_u = *(int32_T *)c2_inData;
  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_create("y", &c2_u, 6, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c2_mxArrayOutData, c2_y, FALSE);
  return c2_mxArrayOutData;
}

static int32_T c2_s_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId)
{
  int32_T c2_y;
  int32_T c2_i78;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), &c2_i78, 1, 6, 0U, 0, 0U, 0);
  c2_y = c2_i78;
  sf_mex_destroy(&c2_u);
  return c2_y;
}

static void c2_l_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_sfEvent;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  int32_T c2_y;
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c2_b_sfEvent = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_y = c2_s_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_sfEvent),
    &c2_thisId);
  sf_mex_destroy(&c2_b_sfEvent);
  *(int32_T *)c2_outData = c2_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static uint8_T c2_t_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_b_is_active_c2_TTR_mdl, const char_T *c2_identifier)
{
  uint8_T c2_y;
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_y = c2_u_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c2_b_is_active_c2_TTR_mdl), &c2_thisId);
  sf_mex_destroy(&c2_b_is_active_c2_TTR_mdl);
  return c2_y;
}

static uint8_T c2_u_emlrt_marshallIn(SFc2_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId)
{
  uint8_T c2_y;
  uint8_T c2_u0;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), &c2_u0, 1, 3, 0U, 0, 0U, 0);
  c2_y = c2_u0;
  sf_mex_destroy(&c2_u);
  return c2_y;
}

static void init_dsm_address_info(SFc2_TTR_mdlInstanceStruct *chartInstance)
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

void sf_c2_TTR_mdl_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(1612841743U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(2412551204U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(1604828572U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(1512177069U);
}

mxArray *sf_c2_TTR_mdl_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1,1,5,
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("gXvjLcBacj9qOBIg0h7LpC");
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,5,3,dataFields);

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
      pr[0] = (double)(3);
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
      pr[0] = (double)(3);
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

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(3);
      pr[1] = (double)(1);
      mxSetField(mxData,4,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,4,"type",mxType);
    }

    mxSetField(mxData,4,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"inputs",mxData);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,4,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(4);
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
      pr[0] = (double)(1);
      pr[1] = (double)(2);
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
      pr[1] = (double)(2);
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

    mxArray *mxData = mxCreateStructMatrix(1,2,3,dataFields);

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
    mxSetField(mxAutoinheritanceInfo,0,"outputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"locals",mxCreateDoubleMatrix(0,0,mxREAL));
  }

  return(mxAutoinheritanceInfo);
}

mxArray *sf_c2_TTR_mdl_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,0);
  return(mxcell3p);
}

static const mxArray *sf_get_sim_state_info_c2_TTR_mdl(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x9'type','srcId','name','auxInfo'{{M[1],M[5],T\"CA_M_Theta\",},{M[1],M[12],T\"CA_S_Theta\",},{M[4],M[0],T\"CA_M_Theta_km1\",S'l','i','p'{{M1x2[222 236],M[0],}}},{M[4],M[0],T\"CA_S_Theta_km1\",S'l','i','p'{{M1x2[308 322],M[0],}}},{M[4],M[0],T\"phi_err_int\",S'l','i','p'{{M1x2[394 405],M[0],}}},{M[4],M[0],T\"psi_err_int\",S'l','i','p'{{M1x2[548 559],M[0],}}},{M[4],M[0],T\"t_km1\",S'l','i','p'{{M1x2[150 155],M[0],}}},{M[4],M[0],T\"theta_err_int\",S'l','i','p'{{M1x2[468 481],M[0],}}},{M[8],M[0],T\"is_active_c2_TTR_mdl\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 9, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c2_TTR_mdl_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc2_TTR_mdlInstanceStruct *chartInstance;
    chartInstance = (SFc2_TTR_mdlInstanceStruct *) ((ChartInfoStruct *)
      (ssGetUserData(S)))->chartInstance;
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (sfGlobalDebugInstanceStruct,
           _TTR_mdlMachineNumber_,
           2,
           1,
           1,
           11,
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
          _SFD_SET_DATA_PROPS(0,1,1,0,"t");
          _SFD_SET_DATA_PROPS(1,1,1,0,"Theta");
          _SFD_SET_DATA_PROPS(2,1,1,0,"Theta_Dot");
          _SFD_SET_DATA_PROPS(3,1,1,0,"Theta_Die");
          _SFD_SET_DATA_PROPS(4,2,0,1,"CA_M_Theta");
          _SFD_SET_DATA_PROPS(5,1,1,0,"Theta_d");
          _SFD_SET_DATA_PROPS(6,10,0,0,"PolyRL_U2R");
          _SFD_SET_DATA_PROPS(7,10,0,0,"PolyT_U2R");
          _SFD_SET_DATA_PROPS(8,2,0,1,"CA_S_Theta");
          _SFD_SET_DATA_PROPS(9,10,0,0,"PolyGT_U2A");
          _SFD_SET_DATA_PROPS(10,10,0,0,"LQR_phi");
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
        _SFD_CV_INIT_EML(0,1,1,30,0,0,0,0,0,12,3);
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,6045);
        _SFD_CV_INIT_EML_IF(0,1,0,162,179,-1,209);
        _SFD_CV_INIT_EML_IF(0,1,1,238,264,-1,295);
        _SFD_CV_INIT_EML_IF(0,1,2,324,350,-1,381);
        _SFD_CV_INIT_EML_IF(0,1,3,408,431,-1,455);
        _SFD_CV_INIT_EML_IF(0,1,4,484,509,-1,535);
        _SFD_CV_INIT_EML_IF(0,1,5,562,585,-1,609);
        _SFD_CV_INIT_EML_IF(0,1,6,624,642,5973,6043);
        _SFD_CV_INIT_EML_IF(0,1,7,1549,1585,1624,1665);
        _SFD_CV_INIT_EML_IF(0,1,8,1624,1663,-1,1665);
        _SFD_CV_INIT_EML_IF(0,1,9,2115,2140,2206,4826);
        _SFD_CV_INIT_EML_IF(0,1,10,2211,2251,2302,2433);
        _SFD_CV_INIT_EML_IF(0,1,11,2302,2345,2395,2433);
        _SFD_CV_INIT_EML_IF(0,1,12,2434,2478,2533,2676);
        _SFD_CV_INIT_EML_IF(0,1,13,2533,2580,2634,2676);
        _SFD_CV_INIT_EML_IF(0,1,14,2677,2717,2768,2899);
        _SFD_CV_INIT_EML_IF(0,1,15,2768,2811,2861,2899);
        _SFD_CV_INIT_EML_IF(0,1,16,2901,2949,3199,3245);
        _SFD_CV_INIT_EML_IF(0,1,17,3005,3087,-1,3152);
        _SFD_CV_INIT_EML_IF(0,1,18,3199,3245,3359,3524);
        _SFD_CV_INIT_EML_IF(0,1,19,3359,3406,-1,3406);
        _SFD_CV_INIT_EML_IF(0,1,20,3525,3577,3855,3905);
        _SFD_CV_INIT_EML_IF(0,1,21,3639,3733,-1,3804);
        _SFD_CV_INIT_EML_IF(0,1,22,3855,3905,4023,4196);
        _SFD_CV_INIT_EML_IF(0,1,23,4023,4074,-1,4074);
        _SFD_CV_INIT_EML_IF(0,1,24,4199,4247,4497,4543);
        _SFD_CV_INIT_EML_IF(0,1,25,4303,4385,-1,4450);
        _SFD_CV_INIT_EML_IF(0,1,26,4497,4543,4657,4822);
        _SFD_CV_INIT_EML_IF(0,1,27,4657,4704,-1,4704);
        _SFD_CV_INIT_EML_IF(0,1,28,4964,4978,-1,4998);
        _SFD_CV_INIT_EML_IF(0,1,29,5096,5111,-1,5132);

        {
          static int condStart[] = { 3009, 3024, 3051, 3066 };

          static int condEnd[] = { 3018, 3043, 3060, 3086 };

          static int pfixExpr[] = { 0, 1, -3, 2, 3, -3, -2 };

          _SFD_CV_INIT_EML_MCDC(0,1,0,3008,3087,4,0,&(condStart[0]),&(condEnd[0]),
                                7,&(pfixExpr[0]));
        }

        {
          static int condStart[] = { 3643, 3662, 3691, 3710 };

          static int condEnd[] = { 3656, 3683, 3704, 3732 };

          static int pfixExpr[] = { 0, 1, -3, 2, 3, -3, -2 };

          _SFD_CV_INIT_EML_MCDC(0,1,1,3642,3733,4,4,&(condStart[0]),&(condEnd[0]),
                                7,&(pfixExpr[0]));
        }

        {
          static int condStart[] = { 4307, 4322, 4349, 4364 };

          static int condEnd[] = { 4316, 4341, 4358, 4384 };

          static int pfixExpr[] = { 0, 1, -3, 2, 3, -3, -2 };

          _SFD_CV_INIT_EML_MCDC(0,1,2,4306,4385,4,8,&(condStart[0]),&(condEnd[0]),
                                7,&(pfixExpr[0]));
        }

        _SFD_TRANS_COV_WTS(0,0,0,1,0);
        if (chartAlreadyPresent==0) {
          _SFD_TRANS_COV_MAPS(0,
                              0,NULL,NULL,
                              0,NULL,NULL,
                              1,NULL,NULL,
                              0,NULL,NULL);
        }

        _SFD_SET_DATA_COMPILED_PROPS(0,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_j_sf_marshallOut,(MexInFcnForType)NULL);

        {
          unsigned int dimVector[1];
          dimVector[0]= 3;
          _SFD_SET_DATA_COMPILED_PROPS(1,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_g_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 3;
          _SFD_SET_DATA_COMPILED_PROPS(2,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_g_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 3;
          _SFD_SET_DATA_COMPILED_PROPS(3,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_g_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 3;
          _SFD_SET_DATA_COMPILED_PROPS(4,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_g_sf_marshallOut,(MexInFcnForType)
            c2_g_sf_marshallIn);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 3;
          _SFD_SET_DATA_COMPILED_PROPS(5,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_g_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 1;
          dimVector[1]= 2;
          _SFD_SET_DATA_COMPILED_PROPS(6,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_i_sf_marshallOut,(MexInFcnForType)
            c2_i_sf_marshallIn);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 1;
          dimVector[1]= 2;
          _SFD_SET_DATA_COMPILED_PROPS(7,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_i_sf_marshallOut,(MexInFcnForType)
            c2_i_sf_marshallIn);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 3;
          _SFD_SET_DATA_COMPILED_PROPS(8,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_g_sf_marshallOut,(MexInFcnForType)
            c2_g_sf_marshallIn);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 1;
          dimVector[1]= 2;
          _SFD_SET_DATA_COMPILED_PROPS(9,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_i_sf_marshallOut,(MexInFcnForType)
            c2_i_sf_marshallIn);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 1;
          dimVector[1]= 4;
          _SFD_SET_DATA_COMPILED_PROPS(10,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_h_sf_marshallOut,(MexInFcnForType)
            c2_h_sf_marshallIn);
        }

        {
          real_T *c2_t;
          real_T (*c2_Theta)[3];
          real_T (*c2_Theta_Dot)[3];
          real_T (*c2_Theta_Die)[3];
          real_T (*c2_CA_M_Theta)[3];
          real_T (*c2_Theta_d)[3];
          real_T (*c2_CA_S_Theta)[3];
          c2_CA_S_Theta = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S,
            2);
          c2_Theta_d = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 4);
          c2_CA_M_Theta = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S,
            1);
          c2_Theta_Die = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 3);
          c2_Theta_Dot = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 2);
          c2_Theta = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 1);
          c2_t = (real_T *)ssGetInputPortSignal(chartInstance->S, 0);
          _SFD_SET_DATA_VALUE_PTR(0U, c2_t);
          _SFD_SET_DATA_VALUE_PTR(1U, *c2_Theta);
          _SFD_SET_DATA_VALUE_PTR(2U, *c2_Theta_Dot);
          _SFD_SET_DATA_VALUE_PTR(3U, *c2_Theta_Die);
          _SFD_SET_DATA_VALUE_PTR(4U, *c2_CA_M_Theta);
          _SFD_SET_DATA_VALUE_PTR(5U, *c2_Theta_d);
          _SFD_SET_DATA_VALUE_PTR(6U, chartInstance->c2_PolyRL_U2R);
          _SFD_SET_DATA_VALUE_PTR(7U, chartInstance->c2_PolyT_U2R);
          _SFD_SET_DATA_VALUE_PTR(8U, *c2_CA_S_Theta);
          _SFD_SET_DATA_VALUE_PTR(9U, chartInstance->c2_PolyGT_U2A);
          _SFD_SET_DATA_VALUE_PTR(10U, chartInstance->c2_LQR_phi);
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
  return "07HcEZv9uaRVdRTv5YwfAH";
}

static void sf_opaque_initialize_c2_TTR_mdl(void *chartInstanceVar)
{
  chart_debug_initialization(((SFc2_TTR_mdlInstanceStruct*) chartInstanceVar)->S,
    0);
  initialize_params_c2_TTR_mdl((SFc2_TTR_mdlInstanceStruct*) chartInstanceVar);
  initialize_c2_TTR_mdl((SFc2_TTR_mdlInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_enable_c2_TTR_mdl(void *chartInstanceVar)
{
  enable_c2_TTR_mdl((SFc2_TTR_mdlInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_disable_c2_TTR_mdl(void *chartInstanceVar)
{
  disable_c2_TTR_mdl((SFc2_TTR_mdlInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_gateway_c2_TTR_mdl(void *chartInstanceVar)
{
  sf_c2_TTR_mdl((SFc2_TTR_mdlInstanceStruct*) chartInstanceVar);
}

extern const mxArray* sf_internal_get_sim_state_c2_TTR_mdl(SimStruct* S)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_raw2high");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = (mxArray*) get_sim_state_c2_TTR_mdl((SFc2_TTR_mdlInstanceStruct*)
    chartInfo->chartInstance);         /* raw sim ctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c2_TTR_mdl();/* state var info */
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

extern void sf_internal_set_sim_state_c2_TTR_mdl(SimStruct* S, const mxArray *st)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_high2raw");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = mxDuplicateArray(st);      /* high level simctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c2_TTR_mdl();/* state var info */
  mxError = sf_mex_call_matlab(1, plhs, 4, prhs, "sfprivate");
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  mxDestroyArray(prhs[3]);
  if (mxError || plhs[0] == NULL) {
    sf_mex_error_message("Stateflow Internal Error: \nError calling 'chart_simctx_high2raw'.\n");
  }

  set_sim_state_c2_TTR_mdl((SFc2_TTR_mdlInstanceStruct*)chartInfo->chartInstance,
    mxDuplicateArray(plhs[0]));
  mxDestroyArray(plhs[0]);
}

static const mxArray* sf_opaque_get_sim_state_c2_TTR_mdl(SimStruct* S)
{
  return sf_internal_get_sim_state_c2_TTR_mdl(S);
}

static void sf_opaque_set_sim_state_c2_TTR_mdl(SimStruct* S, const mxArray *st)
{
  sf_internal_set_sim_state_c2_TTR_mdl(S, st);
}

static void sf_opaque_terminate_c2_TTR_mdl(void *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc2_TTR_mdlInstanceStruct*) chartInstanceVar)->S;
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_TTR_mdl_optimization_info();
    }

    finalize_c2_TTR_mdl((SFc2_TTR_mdlInstanceStruct*) chartInstanceVar);
    utFree((void *)chartInstanceVar);
    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc2_TTR_mdl((SFc2_TTR_mdlInstanceStruct*) chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c2_TTR_mdl(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    initialize_params_c2_TTR_mdl((SFc2_TTR_mdlInstanceStruct*)(((ChartInfoStruct
      *)ssGetUserData(S))->chartInstance));
  }
}

static void mdlSetWorkWidths_c2_TTR_mdl(SimStruct *S)
{
  /* Actual parameters from chart:
     LQR_phi PolyGT_U2A PolyRL_U2R PolyT_U2R
   */
  const char_T *rtParamNames[] = { "LQR_phi", "PolyGT_U2A", "PolyRL_U2R",
    "PolyT_U2R" };

  ssSetNumRunTimeParams(S,ssGetSFcnParamsCount(S));

  /* registration for LQR_phi*/
  ssRegDlgParamAsRunTimeParam(S, 0, 0, rtParamNames[0], SS_DOUBLE);

  /* registration for PolyGT_U2A*/
  ssRegDlgParamAsRunTimeParam(S, 1, 1, rtParamNames[1], SS_DOUBLE);

  /* registration for PolyRL_U2R*/
  ssRegDlgParamAsRunTimeParam(S, 2, 2, rtParamNames[2], SS_DOUBLE);

  /* registration for PolyT_U2R*/
  ssRegDlgParamAsRunTimeParam(S, 3, 3, rtParamNames[3], SS_DOUBLE);
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_TTR_mdl_optimization_info();
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable(S,sf_get_instance_specialization(),infoStruct,
      2);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,sf_rtw_info_uint_prop(S,sf_get_instance_specialization(),
                infoStruct,2,"RTWCG"));
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop(S,
      sf_get_instance_specialization(),infoStruct,2,
      "gatewayCannotBeInlinedMultipleTimes"));
    sf_update_buildInfo(S,sf_get_instance_specialization(),infoStruct,2);
    if (chartIsInlinable) {
      ssSetInputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 1, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 2, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 3, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 4, SS_REUSABLE_AND_LOCAL);
      sf_mark_chart_expressionable_inputs(S,sf_get_instance_specialization(),
        infoStruct,2,5);
      sf_mark_chart_reusable_outputs(S,sf_get_instance_specialization(),
        infoStruct,2,2);
    }

    {
      unsigned int outPortIdx;
      for (outPortIdx=1; outPortIdx<=2; ++outPortIdx) {
        ssSetOutputPortOptimizeInIR(S, outPortIdx, 1U);
      }
    }

    {
      unsigned int inPortIdx;
      for (inPortIdx=0; inPortIdx < 5; ++inPortIdx) {
        ssSetInputPortOptimizeInIR(S, inPortIdx, 1U);
      }
    }

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,2);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(2638955431U));
  ssSetChecksum1(S,(1710611794U));
  ssSetChecksum2(S,(2847296210U));
  ssSetChecksum3(S,(3622121974U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
  ssSupportsMultipleExecInstances(S,1);
}

static void mdlRTW_c2_TTR_mdl(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c2_TTR_mdl(SimStruct *S)
{
  SFc2_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc2_TTR_mdlInstanceStruct *)utMalloc(sizeof
    (SFc2_TTR_mdlInstanceStruct));
  memset(chartInstance, 0, sizeof(SFc2_TTR_mdlInstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway = sf_opaque_gateway_c2_TTR_mdl;
  chartInstance->chartInfo.initializeChart = sf_opaque_initialize_c2_TTR_mdl;
  chartInstance->chartInfo.terminateChart = sf_opaque_terminate_c2_TTR_mdl;
  chartInstance->chartInfo.enableChart = sf_opaque_enable_c2_TTR_mdl;
  chartInstance->chartInfo.disableChart = sf_opaque_disable_c2_TTR_mdl;
  chartInstance->chartInfo.getSimState = sf_opaque_get_sim_state_c2_TTR_mdl;
  chartInstance->chartInfo.setSimState = sf_opaque_set_sim_state_c2_TTR_mdl;
  chartInstance->chartInfo.getSimStateInfo = sf_get_sim_state_info_c2_TTR_mdl;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c2_TTR_mdl;
  chartInstance->chartInfo.mdlStart = mdlStart_c2_TTR_mdl;
  chartInstance->chartInfo.mdlSetWorkWidths = mdlSetWorkWidths_c2_TTR_mdl;
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

void c2_TTR_mdl_method_dispatcher(SimStruct *S, int_T method, void *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c2_TTR_mdl(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c2_TTR_mdl(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c2_TTR_mdl(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c2_TTR_mdl_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
