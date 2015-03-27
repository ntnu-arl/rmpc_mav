/* Include files */

#include <stddef.h>
#include "blas.h"
#include "TTR_mdl_sfun.h"
#include "c41_TTR_mdl.h"
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
static const char * c41_debug_family_names[25] = { "Theta", "X_Dot", "X_Die",
  "Theta_Dot", "Theta_Die", "nargin", "nargout", "Theta_in", "X_in", "Omega_in",
  "U_in", "Omega_Dot_in", "U_Dot_in", "phi_dot", "theta_dot", "psi_dot", "x_dot",
  "y_dot", "z_dot", "phi_die", "theta_die", "psi_die", "x_die", "y_die", "z_die"
};

static const char * c41_b_debug_family_names[14] = { "s_phi", "c_phi", "s_theta",
  "c_theta", "s_psi", "c_psi", "IMUframe_2_frame", "nargin", "nargout", "v_in",
  "phi", "theta", "psi", "v_out" };

static const char * c41_c_debug_family_names[11] = { "s_phi", "c_phi", "s_theta",
  "c_theta", "IMUrates_2_frame", "nargin", "nargout", "v_in", "phi", "theta",
  "v_out" };

/* Function Declarations */
static void initialize_c41_TTR_mdl(SFc41_TTR_mdlInstanceStruct *chartInstance);
static void initialize_params_c41_TTR_mdl(SFc41_TTR_mdlInstanceStruct
  *chartInstance);
static void enable_c41_TTR_mdl(SFc41_TTR_mdlInstanceStruct *chartInstance);
static void disable_c41_TTR_mdl(SFc41_TTR_mdlInstanceStruct *chartInstance);
static void c41_update_debugger_state_c41_TTR_mdl(SFc41_TTR_mdlInstanceStruct
  *chartInstance);
static const mxArray *get_sim_state_c41_TTR_mdl(SFc41_TTR_mdlInstanceStruct
  *chartInstance);
static void set_sim_state_c41_TTR_mdl(SFc41_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c41_st);
static void finalize_c41_TTR_mdl(SFc41_TTR_mdlInstanceStruct *chartInstance);
static void sf_c41_TTR_mdl(SFc41_TTR_mdlInstanceStruct *chartInstance);
static void initSimStructsc41_TTR_mdl(SFc41_TTR_mdlInstanceStruct *chartInstance);
static void registerMessagesc41_TTR_mdl(SFc41_TTR_mdlInstanceStruct
  *chartInstance);
static void c41_Rotate_IMUframe_2_Frame(SFc41_TTR_mdlInstanceStruct
  *chartInstance, real_T c41_v_in[3], real_T c41_phi, real_T c41_theta, real_T
  c41_psi, real_T c41_v_out[3]);
static void c41_Rotate_IMUrates_2_Frame(SFc41_TTR_mdlInstanceStruct
  *chartInstance, real_T c41_v_in[3], real_T c41_phi, real_T c41_theta, real_T
  c41_v_out[3]);
static void init_script_number_translation(uint32_T c41_machineNumber, uint32_T
  c41_chartNumber);
static const mxArray *c41_sf_marshallOut(void *chartInstanceVoid, void
  *c41_inData);
static real_T c41_emlrt_marshallIn(SFc41_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c41_z_die, const char_T *c41_identifier);
static real_T c41_b_emlrt_marshallIn(SFc41_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c41_u, const emlrtMsgIdentifier *c41_parentId);
static void c41_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c41_mxArrayInData, const char_T *c41_varName, void *c41_outData);
static const mxArray *c41_b_sf_marshallOut(void *chartInstanceVoid, void
  *c41_inData);
static void c41_c_emlrt_marshallIn(SFc41_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c41_u, const emlrtMsgIdentifier *c41_parentId, real_T c41_y[3]);
static void c41_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c41_mxArrayInData, const char_T *c41_varName, void *c41_outData);
static const mxArray *c41_c_sf_marshallOut(void *chartInstanceVoid, void
  *c41_inData);
static void c41_d_emlrt_marshallIn(SFc41_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c41_u, const emlrtMsgIdentifier *c41_parentId, real_T c41_y[9]);
static void c41_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c41_mxArrayInData, const char_T *c41_varName, void *c41_outData);
static void c41_info_helper(c41_ResolvedFunctionInfo c41_info[22]);
static void c41_eml_scalar_eg(SFc41_TTR_mdlInstanceStruct *chartInstance);
static const mxArray *c41_d_sf_marshallOut(void *chartInstanceVoid, void
  *c41_inData);
static int32_T c41_e_emlrt_marshallIn(SFc41_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c41_u, const emlrtMsgIdentifier *c41_parentId);
static void c41_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c41_mxArrayInData, const char_T *c41_varName, void *c41_outData);
static uint8_T c41_f_emlrt_marshallIn(SFc41_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c41_b_is_active_c41_TTR_mdl, const char_T *c41_identifier);
static uint8_T c41_g_emlrt_marshallIn(SFc41_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c41_u, const emlrtMsgIdentifier *c41_parentId);
static void init_dsm_address_info(SFc41_TTR_mdlInstanceStruct *chartInstance);

/* Function Definitions */
static void initialize_c41_TTR_mdl(SFc41_TTR_mdlInstanceStruct *chartInstance)
{
  chartInstance->c41_sfEvent = CALL_EVENT;
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  chartInstance->c41_is_active_c41_TTR_mdl = 0U;
}

static void initialize_params_c41_TTR_mdl(SFc41_TTR_mdlInstanceStruct
  *chartInstance)
{
}

static void enable_c41_TTR_mdl(SFc41_TTR_mdlInstanceStruct *chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void disable_c41_TTR_mdl(SFc41_TTR_mdlInstanceStruct *chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void c41_update_debugger_state_c41_TTR_mdl(SFc41_TTR_mdlInstanceStruct
  *chartInstance)
{
}

static const mxArray *get_sim_state_c41_TTR_mdl(SFc41_TTR_mdlInstanceStruct
  *chartInstance)
{
  const mxArray *c41_st;
  const mxArray *c41_y = NULL;
  real_T c41_hoistedGlobal;
  real_T c41_u;
  const mxArray *c41_b_y = NULL;
  real_T c41_b_hoistedGlobal;
  real_T c41_b_u;
  const mxArray *c41_c_y = NULL;
  real_T c41_c_hoistedGlobal;
  real_T c41_c_u;
  const mxArray *c41_d_y = NULL;
  real_T c41_d_hoistedGlobal;
  real_T c41_d_u;
  const mxArray *c41_e_y = NULL;
  real_T c41_e_hoistedGlobal;
  real_T c41_e_u;
  const mxArray *c41_f_y = NULL;
  real_T c41_f_hoistedGlobal;
  real_T c41_f_u;
  const mxArray *c41_g_y = NULL;
  real_T c41_g_hoistedGlobal;
  real_T c41_g_u;
  const mxArray *c41_h_y = NULL;
  real_T c41_h_hoistedGlobal;
  real_T c41_h_u;
  const mxArray *c41_i_y = NULL;
  real_T c41_i_hoistedGlobal;
  real_T c41_i_u;
  const mxArray *c41_j_y = NULL;
  real_T c41_j_hoistedGlobal;
  real_T c41_j_u;
  const mxArray *c41_k_y = NULL;
  real_T c41_k_hoistedGlobal;
  real_T c41_k_u;
  const mxArray *c41_l_y = NULL;
  real_T c41_l_hoistedGlobal;
  real_T c41_l_u;
  const mxArray *c41_m_y = NULL;
  uint8_T c41_m_hoistedGlobal;
  uint8_T c41_m_u;
  const mxArray *c41_n_y = NULL;
  real_T *c41_phi_die;
  real_T *c41_phi_dot;
  real_T *c41_psi_die;
  real_T *c41_psi_dot;
  real_T *c41_theta_die;
  real_T *c41_theta_dot;
  real_T *c41_x_die;
  real_T *c41_x_dot;
  real_T *c41_y_die;
  real_T *c41_y_dot;
  real_T *c41_z_die;
  real_T *c41_z_dot;
  c41_z_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 12);
  c41_y_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 11);
  c41_x_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 10);
  c41_psi_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 9);
  c41_theta_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 8);
  c41_phi_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 7);
  c41_z_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 6);
  c41_y_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 5);
  c41_x_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 4);
  c41_psi_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 3);
  c41_theta_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 2);
  c41_phi_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 1);
  c41_st = NULL;
  c41_st = NULL;
  c41_y = NULL;
  sf_mex_assign(&c41_y, sf_mex_createcellarray(13), FALSE);
  c41_hoistedGlobal = *c41_phi_die;
  c41_u = c41_hoistedGlobal;
  c41_b_y = NULL;
  sf_mex_assign(&c41_b_y, sf_mex_create("y", &c41_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c41_y, 0, c41_b_y);
  c41_b_hoistedGlobal = *c41_phi_dot;
  c41_b_u = c41_b_hoistedGlobal;
  c41_c_y = NULL;
  sf_mex_assign(&c41_c_y, sf_mex_create("y", &c41_b_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c41_y, 1, c41_c_y);
  c41_c_hoistedGlobal = *c41_psi_die;
  c41_c_u = c41_c_hoistedGlobal;
  c41_d_y = NULL;
  sf_mex_assign(&c41_d_y, sf_mex_create("y", &c41_c_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c41_y, 2, c41_d_y);
  c41_d_hoistedGlobal = *c41_psi_dot;
  c41_d_u = c41_d_hoistedGlobal;
  c41_e_y = NULL;
  sf_mex_assign(&c41_e_y, sf_mex_create("y", &c41_d_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c41_y, 3, c41_e_y);
  c41_e_hoistedGlobal = *c41_theta_die;
  c41_e_u = c41_e_hoistedGlobal;
  c41_f_y = NULL;
  sf_mex_assign(&c41_f_y, sf_mex_create("y", &c41_e_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c41_y, 4, c41_f_y);
  c41_f_hoistedGlobal = *c41_theta_dot;
  c41_f_u = c41_f_hoistedGlobal;
  c41_g_y = NULL;
  sf_mex_assign(&c41_g_y, sf_mex_create("y", &c41_f_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c41_y, 5, c41_g_y);
  c41_g_hoistedGlobal = *c41_x_die;
  c41_g_u = c41_g_hoistedGlobal;
  c41_h_y = NULL;
  sf_mex_assign(&c41_h_y, sf_mex_create("y", &c41_g_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c41_y, 6, c41_h_y);
  c41_h_hoistedGlobal = *c41_x_dot;
  c41_h_u = c41_h_hoistedGlobal;
  c41_i_y = NULL;
  sf_mex_assign(&c41_i_y, sf_mex_create("y", &c41_h_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c41_y, 7, c41_i_y);
  c41_i_hoistedGlobal = *c41_y_die;
  c41_i_u = c41_i_hoistedGlobal;
  c41_j_y = NULL;
  sf_mex_assign(&c41_j_y, sf_mex_create("y", &c41_i_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c41_y, 8, c41_j_y);
  c41_j_hoistedGlobal = *c41_y_dot;
  c41_j_u = c41_j_hoistedGlobal;
  c41_k_y = NULL;
  sf_mex_assign(&c41_k_y, sf_mex_create("y", &c41_j_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c41_y, 9, c41_k_y);
  c41_k_hoistedGlobal = *c41_z_die;
  c41_k_u = c41_k_hoistedGlobal;
  c41_l_y = NULL;
  sf_mex_assign(&c41_l_y, sf_mex_create("y", &c41_k_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c41_y, 10, c41_l_y);
  c41_l_hoistedGlobal = *c41_z_dot;
  c41_l_u = c41_l_hoistedGlobal;
  c41_m_y = NULL;
  sf_mex_assign(&c41_m_y, sf_mex_create("y", &c41_l_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c41_y, 11, c41_m_y);
  c41_m_hoistedGlobal = chartInstance->c41_is_active_c41_TTR_mdl;
  c41_m_u = c41_m_hoistedGlobal;
  c41_n_y = NULL;
  sf_mex_assign(&c41_n_y, sf_mex_create("y", &c41_m_u, 3, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c41_y, 12, c41_n_y);
  sf_mex_assign(&c41_st, c41_y, FALSE);
  return c41_st;
}

static void set_sim_state_c41_TTR_mdl(SFc41_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c41_st)
{
  const mxArray *c41_u;
  real_T *c41_phi_die;
  real_T *c41_phi_dot;
  real_T *c41_psi_die;
  real_T *c41_psi_dot;
  real_T *c41_theta_die;
  real_T *c41_theta_dot;
  real_T *c41_x_die;
  real_T *c41_x_dot;
  real_T *c41_y_die;
  real_T *c41_y_dot;
  real_T *c41_z_die;
  real_T *c41_z_dot;
  c41_z_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 12);
  c41_y_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 11);
  c41_x_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 10);
  c41_psi_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 9);
  c41_theta_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 8);
  c41_phi_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 7);
  c41_z_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 6);
  c41_y_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 5);
  c41_x_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 4);
  c41_psi_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 3);
  c41_theta_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 2);
  c41_phi_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 1);
  chartInstance->c41_doneDoubleBufferReInit = TRUE;
  c41_u = sf_mex_dup(c41_st);
  *c41_phi_die = c41_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell
    (c41_u, 0)), "phi_die");
  *c41_phi_dot = c41_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell
    (c41_u, 1)), "phi_dot");
  *c41_psi_die = c41_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell
    (c41_u, 2)), "psi_die");
  *c41_psi_dot = c41_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell
    (c41_u, 3)), "psi_dot");
  *c41_theta_die = c41_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell
    (c41_u, 4)), "theta_die");
  *c41_theta_dot = c41_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell
    (c41_u, 5)), "theta_dot");
  *c41_x_die = c41_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell
    (c41_u, 6)), "x_die");
  *c41_x_dot = c41_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell
    (c41_u, 7)), "x_dot");
  *c41_y_die = c41_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell
    (c41_u, 8)), "y_die");
  *c41_y_dot = c41_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell
    (c41_u, 9)), "y_dot");
  *c41_z_die = c41_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell
    (c41_u, 10)), "z_die");
  *c41_z_dot = c41_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell
    (c41_u, 11)), "z_dot");
  chartInstance->c41_is_active_c41_TTR_mdl = c41_f_emlrt_marshallIn
    (chartInstance, sf_mex_dup(sf_mex_getcell(c41_u, 12)),
     "is_active_c41_TTR_mdl");
  sf_mex_destroy(&c41_u);
  c41_update_debugger_state_c41_TTR_mdl(chartInstance);
  sf_mex_destroy(&c41_st);
}

static void finalize_c41_TTR_mdl(SFc41_TTR_mdlInstanceStruct *chartInstance)
{
}

static void sf_c41_TTR_mdl(SFc41_TTR_mdlInstanceStruct *chartInstance)
{
  int32_T c41_i0;
  int32_T c41_i1;
  int32_T c41_i2;
  int32_T c41_i3;
  int32_T c41_i4;
  int32_T c41_i5;
  int32_T c41_i6;
  real_T c41_Theta_in[3];
  int32_T c41_i7;
  real_T c41_X_in[3];
  int32_T c41_i8;
  real_T c41_Omega_in[3];
  int32_T c41_i9;
  real_T c41_U_in[3];
  int32_T c41_i10;
  real_T c41_Omega_Dot_in[3];
  int32_T c41_i11;
  real_T c41_U_Dot_in[3];
  uint32_T c41_debug_family_var_map[25];
  real_T c41_Theta[3];
  real_T c41_X_Dot[3];
  real_T c41_X_Die[3];
  real_T c41_Theta_Dot[3];
  real_T c41_Theta_Die[3];
  real_T c41_nargin = 6.0;
  real_T c41_nargout = 12.0;
  real_T c41_phi_dot;
  real_T c41_theta_dot;
  real_T c41_psi_dot;
  real_T c41_x_dot;
  real_T c41_y_dot;
  real_T c41_z_dot;
  real_T c41_phi_die;
  real_T c41_theta_die;
  real_T c41_psi_die;
  real_T c41_x_die;
  real_T c41_y_die;
  real_T c41_z_die;
  int32_T c41_i12;
  int32_T c41_i13;
  real_T c41_b_U_in[3];
  real_T c41_dv0[3];
  int32_T c41_i14;
  int32_T c41_i15;
  real_T c41_b_U_Dot_in[3];
  real_T c41_dv1[3];
  int32_T c41_i16;
  int32_T c41_i17;
  real_T c41_b_Omega_in[3];
  real_T c41_dv2[3];
  int32_T c41_i18;
  int32_T c41_i19;
  real_T c41_b_Omega_Dot_in[3];
  real_T c41_dv3[3];
  int32_T c41_i20;
  real_T *c41_b_phi_dot;
  real_T *c41_b_theta_dot;
  real_T *c41_b_psi_dot;
  real_T *c41_b_x_dot;
  real_T *c41_b_y_dot;
  real_T *c41_b_z_dot;
  real_T *c41_b_phi_die;
  real_T *c41_b_theta_die;
  real_T *c41_b_psi_die;
  real_T *c41_b_x_die;
  real_T *c41_b_y_die;
  real_T *c41_b_z_die;
  real_T (*c41_c_U_Dot_in)[3];
  real_T (*c41_c_Omega_Dot_in)[3];
  real_T (*c41_c_U_in)[3];
  real_T (*c41_c_Omega_in)[3];
  real_T (*c41_b_X_in)[3];
  real_T (*c41_b_Theta_in)[3];
  c41_c_U_Dot_in = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 5);
  c41_c_Omega_Dot_in = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 4);
  c41_c_U_in = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 3);
  c41_c_Omega_in = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 2);
  c41_b_X_in = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 1);
  c41_b_z_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 12);
  c41_b_y_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 11);
  c41_b_x_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 10);
  c41_b_psi_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 9);
  c41_b_theta_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 8);
  c41_b_phi_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 7);
  c41_b_z_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 6);
  c41_b_y_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 5);
  c41_b_Theta_in = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 0);
  c41_b_x_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 4);
  c41_b_psi_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 3);
  c41_b_theta_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 2);
  c41_b_phi_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 1);
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 6U, chartInstance->c41_sfEvent);
  _SFD_DATA_RANGE_CHECK(*c41_b_phi_dot, 0U);
  _SFD_DATA_RANGE_CHECK(*c41_b_theta_dot, 1U);
  _SFD_DATA_RANGE_CHECK(*c41_b_psi_dot, 2U);
  _SFD_DATA_RANGE_CHECK(*c41_b_x_dot, 3U);
  for (c41_i0 = 0; c41_i0 < 3; c41_i0++) {
    _SFD_DATA_RANGE_CHECK((*c41_b_Theta_in)[c41_i0], 4U);
  }

  _SFD_DATA_RANGE_CHECK(*c41_b_y_dot, 5U);
  _SFD_DATA_RANGE_CHECK(*c41_b_z_dot, 6U);
  _SFD_DATA_RANGE_CHECK(*c41_b_phi_die, 7U);
  _SFD_DATA_RANGE_CHECK(*c41_b_theta_die, 8U);
  _SFD_DATA_RANGE_CHECK(*c41_b_psi_die, 9U);
  _SFD_DATA_RANGE_CHECK(*c41_b_x_die, 10U);
  _SFD_DATA_RANGE_CHECK(*c41_b_y_die, 11U);
  _SFD_DATA_RANGE_CHECK(*c41_b_z_die, 12U);
  for (c41_i1 = 0; c41_i1 < 3; c41_i1++) {
    _SFD_DATA_RANGE_CHECK((*c41_b_X_in)[c41_i1], 13U);
  }

  for (c41_i2 = 0; c41_i2 < 3; c41_i2++) {
    _SFD_DATA_RANGE_CHECK((*c41_c_Omega_in)[c41_i2], 14U);
  }

  for (c41_i3 = 0; c41_i3 < 3; c41_i3++) {
    _SFD_DATA_RANGE_CHECK((*c41_c_U_in)[c41_i3], 15U);
  }

  for (c41_i4 = 0; c41_i4 < 3; c41_i4++) {
    _SFD_DATA_RANGE_CHECK((*c41_c_Omega_Dot_in)[c41_i4], 16U);
  }

  for (c41_i5 = 0; c41_i5 < 3; c41_i5++) {
    _SFD_DATA_RANGE_CHECK((*c41_c_U_Dot_in)[c41_i5], 17U);
  }

  chartInstance->c41_sfEvent = CALL_EVENT;
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 6U, chartInstance->c41_sfEvent);
  for (c41_i6 = 0; c41_i6 < 3; c41_i6++) {
    c41_Theta_in[c41_i6] = (*c41_b_Theta_in)[c41_i6];
  }

  for (c41_i7 = 0; c41_i7 < 3; c41_i7++) {
    c41_X_in[c41_i7] = (*c41_b_X_in)[c41_i7];
  }

  for (c41_i8 = 0; c41_i8 < 3; c41_i8++) {
    c41_Omega_in[c41_i8] = (*c41_c_Omega_in)[c41_i8];
  }

  for (c41_i9 = 0; c41_i9 < 3; c41_i9++) {
    c41_U_in[c41_i9] = (*c41_c_U_in)[c41_i9];
  }

  for (c41_i10 = 0; c41_i10 < 3; c41_i10++) {
    c41_Omega_Dot_in[c41_i10] = (*c41_c_Omega_Dot_in)[c41_i10];
  }

  for (c41_i11 = 0; c41_i11 < 3; c41_i11++) {
    c41_U_Dot_in[c41_i11] = (*c41_c_U_Dot_in)[c41_i11];
  }

  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 25U, 25U, c41_debug_family_names,
    c41_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c41_Theta, 0U, c41_b_sf_marshallOut,
    c41_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c41_X_Dot, 1U, c41_b_sf_marshallOut,
    c41_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c41_X_Die, 2U, c41_b_sf_marshallOut,
    c41_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c41_Theta_Dot, 3U, c41_b_sf_marshallOut,
    c41_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c41_Theta_Die, 4U, c41_b_sf_marshallOut,
    c41_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_nargin, 5U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_nargout, 6U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(c41_Theta_in, 7U, c41_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c41_X_in, 8U, c41_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c41_Omega_in, 9U, c41_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c41_U_in, 10U, c41_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c41_Omega_Dot_in, 11U, c41_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c41_U_Dot_in, 12U, c41_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_phi_dot, 13U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_theta_dot, 14U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_psi_dot, 15U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_x_dot, 16U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_y_dot, 17U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_z_dot, 18U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_phi_die, 19U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_theta_die, 20U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_psi_die, 21U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_x_die, 22U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_y_die, 23U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_z_die, 24U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0U, chartInstance->c41_sfEvent, 6);
  for (c41_i12 = 0; c41_i12 < 3; c41_i12++) {
    c41_Theta[c41_i12] = c41_Theta_in[c41_i12];
  }

  _SFD_EML_CALL(0U, chartInstance->c41_sfEvent, 9);
  for (c41_i13 = 0; c41_i13 < 3; c41_i13++) {
    c41_b_U_in[c41_i13] = c41_U_in[c41_i13];
  }

  c41_Rotate_IMUframe_2_Frame(chartInstance, c41_b_U_in, c41_Theta[0],
    c41_Theta[1], 0.0, c41_dv0);
  for (c41_i14 = 0; c41_i14 < 3; c41_i14++) {
    c41_X_Dot[c41_i14] = c41_dv0[c41_i14];
  }

  _SFD_EML_CALL(0U, chartInstance->c41_sfEvent, 12);
  for (c41_i15 = 0; c41_i15 < 3; c41_i15++) {
    c41_b_U_Dot_in[c41_i15] = c41_U_Dot_in[c41_i15];
  }

  c41_Rotate_IMUframe_2_Frame(chartInstance, c41_b_U_Dot_in, c41_Theta[0],
    c41_Theta[1], 0.0, c41_dv1);
  for (c41_i16 = 0; c41_i16 < 3; c41_i16++) {
    c41_X_Die[c41_i16] = c41_dv1[c41_i16];
  }

  _SFD_EML_CALL(0U, chartInstance->c41_sfEvent, 15);
  for (c41_i17 = 0; c41_i17 < 3; c41_i17++) {
    c41_b_Omega_in[c41_i17] = c41_Omega_in[c41_i17];
  }

  c41_Rotate_IMUrates_2_Frame(chartInstance, c41_b_Omega_in, c41_Theta[0],
    c41_Theta[1], c41_dv2);
  for (c41_i18 = 0; c41_i18 < 3; c41_i18++) {
    c41_Theta_Dot[c41_i18] = c41_dv2[c41_i18];
  }

  _SFD_EML_CALL(0U, chartInstance->c41_sfEvent, 18);
  for (c41_i19 = 0; c41_i19 < 3; c41_i19++) {
    c41_b_Omega_Dot_in[c41_i19] = c41_Omega_Dot_in[c41_i19];
  }

  c41_Rotate_IMUrates_2_Frame(chartInstance, c41_b_Omega_Dot_in, c41_Theta[0],
    c41_Theta[1], c41_dv3);
  for (c41_i20 = 0; c41_i20 < 3; c41_i20++) {
    c41_Theta_Die[c41_i20] = c41_dv3[c41_i20];
  }

  _SFD_EML_CALL(0U, chartInstance->c41_sfEvent, 21);
  c41_phi_dot = c41_Theta_Dot[0];
  _SFD_EML_CALL(0U, chartInstance->c41_sfEvent, 22);
  c41_theta_dot = c41_Theta_Dot[1];
  _SFD_EML_CALL(0U, chartInstance->c41_sfEvent, 23);
  c41_psi_dot = c41_Theta_Dot[2];
  _SFD_EML_CALL(0U, chartInstance->c41_sfEvent, 24);
  c41_x_dot = c41_X_Dot[0];
  _SFD_EML_CALL(0U, chartInstance->c41_sfEvent, 25);
  c41_y_dot = c41_X_Dot[1];
  _SFD_EML_CALL(0U, chartInstance->c41_sfEvent, 26);
  c41_z_dot = c41_X_Dot[2];
  _SFD_EML_CALL(0U, chartInstance->c41_sfEvent, 27);
  c41_phi_die = c41_Theta_Die[0];
  _SFD_EML_CALL(0U, chartInstance->c41_sfEvent, 28);
  c41_theta_die = c41_Theta_Die[1];
  _SFD_EML_CALL(0U, chartInstance->c41_sfEvent, 29);
  c41_psi_die = c41_Theta_Die[2];
  _SFD_EML_CALL(0U, chartInstance->c41_sfEvent, 30);
  c41_x_die = c41_X_Die[0];
  _SFD_EML_CALL(0U, chartInstance->c41_sfEvent, 31);
  c41_y_die = c41_X_Die[1];
  _SFD_EML_CALL(0U, chartInstance->c41_sfEvent, 32);
  c41_z_die = c41_X_Die[2];
  _SFD_EML_CALL(0U, chartInstance->c41_sfEvent, -32);
  _SFD_SYMBOL_SCOPE_POP();
  *c41_b_phi_dot = c41_phi_dot;
  *c41_b_theta_dot = c41_theta_dot;
  *c41_b_psi_dot = c41_psi_dot;
  *c41_b_x_dot = c41_x_dot;
  *c41_b_y_dot = c41_y_dot;
  *c41_b_z_dot = c41_z_dot;
  *c41_b_phi_die = c41_phi_die;
  *c41_b_theta_die = c41_theta_die;
  *c41_b_psi_die = c41_psi_die;
  *c41_b_x_die = c41_x_die;
  *c41_b_y_die = c41_y_die;
  *c41_b_z_die = c41_z_die;
  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 6U, chartInstance->c41_sfEvent);
  _SFD_CHECK_FOR_STATE_INCONSISTENCY(_TTR_mdlMachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
}

static void initSimStructsc41_TTR_mdl(SFc41_TTR_mdlInstanceStruct *chartInstance)
{
}

static void registerMessagesc41_TTR_mdl(SFc41_TTR_mdlInstanceStruct
  *chartInstance)
{
}

static void c41_Rotate_IMUframe_2_Frame(SFc41_TTR_mdlInstanceStruct
  *chartInstance, real_T c41_v_in[3], real_T c41_phi, real_T c41_theta, real_T
  c41_psi, real_T c41_v_out[3])
{
  uint32_T c41_debug_family_var_map[14];
  real_T c41_s_phi;
  real_T c41_c_phi;
  real_T c41_s_theta;
  real_T c41_c_theta;
  real_T c41_s_psi;
  real_T c41_c_psi;
  real_T c41_IMUframe_2_frame[9];
  real_T c41_nargin = 4.0;
  real_T c41_nargout = 1.0;
  real_T c41_x;
  real_T c41_b_x;
  real_T c41_c_x;
  real_T c41_d_x;
  real_T c41_e_x;
  real_T c41_f_x;
  real_T c41_g_x;
  real_T c41_h_x;
  real_T c41_i_x;
  real_T c41_j_x;
  real_T c41_k_x;
  real_T c41_l_x;
  real_T c41_a;
  real_T c41_b;
  real_T c41_y;
  real_T c41_b_a;
  real_T c41_b_b;
  real_T c41_b_y;
  real_T c41_c_a;
  real_T c41_c_b;
  real_T c41_c_y;
  real_T c41_d_a;
  real_T c41_d_b;
  real_T c41_d_y;
  real_T c41_e_a;
  real_T c41_e_b;
  real_T c41_e_y;
  real_T c41_f_a;
  real_T c41_f_b;
  real_T c41_f_y;
  real_T c41_g_a;
  real_T c41_g_b;
  real_T c41_g_y;
  real_T c41_h_a;
  real_T c41_h_b;
  real_T c41_h_y;
  real_T c41_i_a;
  real_T c41_i_b;
  real_T c41_i_y;
  real_T c41_j_a;
  real_T c41_j_b;
  real_T c41_j_y;
  real_T c41_k_a;
  real_T c41_k_b;
  real_T c41_k_y;
  real_T c41_l_a;
  real_T c41_l_b;
  real_T c41_l_y;
  real_T c41_m_a;
  real_T c41_m_b;
  real_T c41_m_y;
  real_T c41_n_a;
  real_T c41_n_b;
  real_T c41_n_y;
  real_T c41_o_a;
  real_T c41_o_b;
  real_T c41_o_y;
  real_T c41_p_a;
  real_T c41_p_b;
  real_T c41_p_y;
  int32_T c41_i21;
  real_T c41_q_a[9];
  int32_T c41_i22;
  real_T c41_q_b[3];
  int32_T c41_i23;
  int32_T c41_i24;
  int32_T c41_i25;
  real_T c41_C[3];
  int32_T c41_i26;
  int32_T c41_i27;
  int32_T c41_i28;
  int32_T c41_i29;
  int32_T c41_i30;
  int32_T c41_i31;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 14U, 14U, c41_b_debug_family_names,
    c41_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_s_phi, 0U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_c_phi, 1U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_s_theta, 2U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_c_theta, 3U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_s_psi, 4U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_c_psi, 5U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c41_IMUframe_2_frame, 6U,
    c41_c_sf_marshallOut, c41_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_nargin, 7U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_nargout, 8U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c41_v_in, 9U, c41_b_sf_marshallOut,
    c41_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_phi, 10U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_theta, 11U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_psi, 12U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c41_v_out, 13U, c41_b_sf_marshallOut,
    c41_b_sf_marshallIn);
  CV_SCRIPT_FCN(0, 0);
  _SFD_SCRIPT_CALL(0U, chartInstance->c41_sfEvent, 4);
  c41_x = c41_phi;
  c41_s_phi = c41_x;
  c41_b_x = c41_s_phi;
  c41_s_phi = c41_b_x;
  c41_s_phi = muDoubleScalarSin(c41_s_phi);
  _SFD_SCRIPT_CALL(0U, chartInstance->c41_sfEvent, 4);
  c41_c_x = c41_phi;
  c41_c_phi = c41_c_x;
  c41_d_x = c41_c_phi;
  c41_c_phi = c41_d_x;
  c41_c_phi = muDoubleScalarCos(c41_c_phi);
  _SFD_SCRIPT_CALL(0U, chartInstance->c41_sfEvent, 5);
  c41_e_x = c41_theta;
  c41_s_theta = c41_e_x;
  c41_f_x = c41_s_theta;
  c41_s_theta = c41_f_x;
  c41_s_theta = muDoubleScalarSin(c41_s_theta);
  _SFD_SCRIPT_CALL(0U, chartInstance->c41_sfEvent, 5);
  c41_g_x = c41_theta;
  c41_c_theta = c41_g_x;
  c41_h_x = c41_c_theta;
  c41_c_theta = c41_h_x;
  c41_c_theta = muDoubleScalarCos(c41_c_theta);
  _SFD_SCRIPT_CALL(0U, chartInstance->c41_sfEvent, 6);
  c41_i_x = c41_psi;
  c41_s_psi = c41_i_x;
  c41_j_x = c41_s_psi;
  c41_s_psi = c41_j_x;
  c41_s_psi = muDoubleScalarSin(c41_s_psi);
  _SFD_SCRIPT_CALL(0U, chartInstance->c41_sfEvent, 6);
  c41_k_x = c41_psi;
  c41_c_psi = c41_k_x;
  c41_l_x = c41_c_psi;
  c41_c_psi = c41_l_x;
  c41_c_psi = muDoubleScalarCos(c41_c_psi);
  _SFD_SCRIPT_CALL(0U, chartInstance->c41_sfEvent, 8);
  c41_a = c41_c_psi;
  c41_b = c41_c_theta;
  c41_y = c41_a * c41_b;
  c41_b_a = c41_c_psi;
  c41_b_b = c41_s_theta;
  c41_b_y = c41_b_a * c41_b_b;
  c41_c_a = c41_b_y;
  c41_c_b = c41_s_phi;
  c41_c_y = c41_c_a * c41_c_b;
  c41_d_a = c41_s_psi;
  c41_d_b = c41_c_phi;
  c41_d_y = c41_d_a * c41_d_b;
  c41_e_a = c41_s_psi;
  c41_e_b = c41_s_phi;
  c41_e_y = c41_e_a * c41_e_b;
  c41_f_a = c41_c_psi;
  c41_f_b = c41_c_phi;
  c41_f_y = c41_f_a * c41_f_b;
  c41_g_a = c41_f_y;
  c41_g_b = c41_s_theta;
  c41_g_y = c41_g_a * c41_g_b;
  c41_h_a = c41_s_psi;
  c41_h_b = c41_c_theta;
  c41_h_y = c41_h_a * c41_h_b;
  c41_i_a = c41_c_psi;
  c41_i_b = c41_c_phi;
  c41_i_y = c41_i_a * c41_i_b;
  c41_j_a = c41_s_phi;
  c41_j_b = c41_s_theta;
  c41_j_y = c41_j_a * c41_j_b;
  c41_k_a = c41_j_y;
  c41_k_b = c41_s_psi;
  c41_k_y = c41_k_a * c41_k_b;
  c41_l_a = c41_s_theta;
  c41_l_b = c41_s_psi;
  c41_l_y = c41_l_a * c41_l_b;
  c41_m_a = c41_l_y;
  c41_m_b = c41_c_phi;
  c41_m_y = c41_m_a * c41_m_b;
  c41_n_a = c41_c_psi;
  c41_n_b = c41_s_phi;
  c41_n_y = c41_n_a * c41_n_b;
  c41_o_a = c41_c_theta;
  c41_o_b = c41_s_phi;
  c41_o_y = c41_o_a * c41_o_b;
  c41_p_a = c41_c_theta;
  c41_p_b = c41_c_phi;
  c41_p_y = c41_p_a * c41_p_b;
  c41_IMUframe_2_frame[0] = c41_y;
  c41_IMUframe_2_frame[3] = c41_c_y - c41_d_y;
  c41_IMUframe_2_frame[6] = c41_e_y + c41_g_y;
  c41_IMUframe_2_frame[1] = c41_h_y;
  c41_IMUframe_2_frame[4] = c41_i_y + c41_k_y;
  c41_IMUframe_2_frame[7] = c41_m_y - c41_n_y;
  c41_IMUframe_2_frame[2] = -c41_s_theta;
  c41_IMUframe_2_frame[5] = c41_o_y;
  c41_IMUframe_2_frame[8] = c41_p_y;
  _SFD_SCRIPT_CALL(0U, chartInstance->c41_sfEvent, 13);
  for (c41_i21 = 0; c41_i21 < 9; c41_i21++) {
    c41_q_a[c41_i21] = c41_IMUframe_2_frame[c41_i21];
  }

  for (c41_i22 = 0; c41_i22 < 3; c41_i22++) {
    c41_q_b[c41_i22] = c41_v_in[c41_i22];
  }

  c41_eml_scalar_eg(chartInstance);
  c41_eml_scalar_eg(chartInstance);
  for (c41_i23 = 0; c41_i23 < 3; c41_i23++) {
    c41_v_out[c41_i23] = 0.0;
  }

  for (c41_i24 = 0; c41_i24 < 3; c41_i24++) {
    c41_v_out[c41_i24] = 0.0;
  }

  for (c41_i25 = 0; c41_i25 < 3; c41_i25++) {
    c41_C[c41_i25] = c41_v_out[c41_i25];
  }

  for (c41_i26 = 0; c41_i26 < 3; c41_i26++) {
    c41_v_out[c41_i26] = c41_C[c41_i26];
  }

  for (c41_i27 = 0; c41_i27 < 3; c41_i27++) {
    c41_C[c41_i27] = c41_v_out[c41_i27];
  }

  for (c41_i28 = 0; c41_i28 < 3; c41_i28++) {
    c41_v_out[c41_i28] = c41_C[c41_i28];
  }

  for (c41_i29 = 0; c41_i29 < 3; c41_i29++) {
    c41_v_out[c41_i29] = 0.0;
    c41_i30 = 0;
    for (c41_i31 = 0; c41_i31 < 3; c41_i31++) {
      c41_v_out[c41_i29] += c41_q_a[c41_i30 + c41_i29] * c41_q_b[c41_i31];
      c41_i30 += 3;
    }
  }

  _SFD_SCRIPT_CALL(0U, chartInstance->c41_sfEvent, -13);
  _SFD_SYMBOL_SCOPE_POP();
}

static void c41_Rotate_IMUrates_2_Frame(SFc41_TTR_mdlInstanceStruct
  *chartInstance, real_T c41_v_in[3], real_T c41_phi, real_T c41_theta, real_T
  c41_v_out[3])
{
  uint32_T c41_debug_family_var_map[11];
  real_T c41_s_phi;
  real_T c41_c_phi;
  real_T c41_s_theta;
  real_T c41_c_theta;
  real_T c41_IMUrates_2_frame[9];
  real_T c41_nargin = 3.0;
  real_T c41_nargout = 1.0;
  real_T c41_x;
  real_T c41_b_x;
  real_T c41_c_x;
  real_T c41_d_x;
  real_T c41_e_x;
  real_T c41_f_x;
  real_T c41_g_x;
  real_T c41_h_x;
  real_T c41_A;
  real_T c41_B;
  real_T c41_i_x;
  real_T c41_y;
  real_T c41_j_x;
  real_T c41_b_y;
  real_T c41_c_y;
  real_T c41_a;
  real_T c41_b;
  real_T c41_d_y;
  real_T c41_b_A;
  real_T c41_b_B;
  real_T c41_k_x;
  real_T c41_e_y;
  real_T c41_l_x;
  real_T c41_f_y;
  real_T c41_g_y;
  real_T c41_b_a;
  real_T c41_b_b;
  real_T c41_h_y;
  real_T c41_c_A;
  real_T c41_c_B;
  real_T c41_m_x;
  real_T c41_i_y;
  real_T c41_n_x;
  real_T c41_j_y;
  real_T c41_k_y;
  real_T c41_d_A;
  real_T c41_d_B;
  real_T c41_o_x;
  real_T c41_l_y;
  real_T c41_p_x;
  real_T c41_m_y;
  real_T c41_n_y;
  int32_T c41_i32;
  real_T c41_c_a[9];
  int32_T c41_i33;
  real_T c41_c_b[3];
  int32_T c41_i34;
  int32_T c41_i35;
  int32_T c41_i36;
  real_T c41_C[3];
  int32_T c41_i37;
  int32_T c41_i38;
  int32_T c41_i39;
  int32_T c41_i40;
  int32_T c41_i41;
  int32_T c41_i42;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 11U, 11U, c41_c_debug_family_names,
    c41_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_s_phi, 0U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_c_phi, 1U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_s_theta, 2U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_c_theta, 3U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c41_IMUrates_2_frame, 4U,
    c41_c_sf_marshallOut, c41_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_nargin, 5U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_nargout, 6U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c41_v_in, 7U, c41_b_sf_marshallOut,
    c41_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_phi, 8U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c41_theta, 9U, c41_sf_marshallOut,
    c41_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c41_v_out, 10U, c41_b_sf_marshallOut,
    c41_b_sf_marshallIn);
  CV_SCRIPT_FCN(1, 0);
  _SFD_SCRIPT_CALL(1U, chartInstance->c41_sfEvent, 4);
  c41_x = c41_phi;
  c41_s_phi = c41_x;
  c41_b_x = c41_s_phi;
  c41_s_phi = c41_b_x;
  c41_s_phi = muDoubleScalarSin(c41_s_phi);
  _SFD_SCRIPT_CALL(1U, chartInstance->c41_sfEvent, 4);
  c41_c_x = c41_phi;
  c41_c_phi = c41_c_x;
  c41_d_x = c41_c_phi;
  c41_c_phi = c41_d_x;
  c41_c_phi = muDoubleScalarCos(c41_c_phi);
  _SFD_SCRIPT_CALL(1U, chartInstance->c41_sfEvent, 5);
  c41_e_x = c41_theta;
  c41_s_theta = c41_e_x;
  c41_f_x = c41_s_theta;
  c41_s_theta = c41_f_x;
  c41_s_theta = muDoubleScalarSin(c41_s_theta);
  _SFD_SCRIPT_CALL(1U, chartInstance->c41_sfEvent, 5);
  c41_g_x = c41_theta;
  c41_c_theta = c41_g_x;
  c41_h_x = c41_c_theta;
  c41_c_theta = c41_h_x;
  c41_c_theta = muDoubleScalarCos(c41_c_theta);
  _SFD_SCRIPT_CALL(1U, chartInstance->c41_sfEvent, 7);
  c41_A = c41_s_theta;
  c41_B = c41_c_theta;
  c41_i_x = c41_A;
  c41_y = c41_B;
  c41_j_x = c41_i_x;
  c41_b_y = c41_y;
  c41_c_y = c41_j_x / c41_b_y;
  c41_a = c41_s_phi;
  c41_b = c41_c_y;
  c41_d_y = c41_a * c41_b;
  c41_b_A = c41_s_theta;
  c41_b_B = c41_c_theta;
  c41_k_x = c41_b_A;
  c41_e_y = c41_b_B;
  c41_l_x = c41_k_x;
  c41_f_y = c41_e_y;
  c41_g_y = c41_l_x / c41_f_y;
  c41_b_a = c41_c_phi;
  c41_b_b = c41_g_y;
  c41_h_y = c41_b_a * c41_b_b;
  c41_c_A = c41_s_phi;
  c41_c_B = c41_c_theta;
  c41_m_x = c41_c_A;
  c41_i_y = c41_c_B;
  c41_n_x = c41_m_x;
  c41_j_y = c41_i_y;
  c41_k_y = c41_n_x / c41_j_y;
  c41_d_A = c41_c_phi;
  c41_d_B = c41_c_theta;
  c41_o_x = c41_d_A;
  c41_l_y = c41_d_B;
  c41_p_x = c41_o_x;
  c41_m_y = c41_l_y;
  c41_n_y = c41_p_x / c41_m_y;
  c41_IMUrates_2_frame[0] = 1.0;
  c41_IMUrates_2_frame[3] = c41_d_y;
  c41_IMUrates_2_frame[6] = c41_h_y;
  c41_IMUrates_2_frame[1] = 0.0;
  c41_IMUrates_2_frame[4] = c41_c_phi;
  c41_IMUrates_2_frame[7] = -c41_s_phi;
  c41_IMUrates_2_frame[2] = 0.0;
  c41_IMUrates_2_frame[5] = c41_k_y;
  c41_IMUrates_2_frame[8] = c41_n_y;
  _SFD_SCRIPT_CALL(1U, chartInstance->c41_sfEvent, 11);
  for (c41_i32 = 0; c41_i32 < 9; c41_i32++) {
    c41_c_a[c41_i32] = c41_IMUrates_2_frame[c41_i32];
  }

  for (c41_i33 = 0; c41_i33 < 3; c41_i33++) {
    c41_c_b[c41_i33] = c41_v_in[c41_i33];
  }

  c41_eml_scalar_eg(chartInstance);
  c41_eml_scalar_eg(chartInstance);
  for (c41_i34 = 0; c41_i34 < 3; c41_i34++) {
    c41_v_out[c41_i34] = 0.0;
  }

  for (c41_i35 = 0; c41_i35 < 3; c41_i35++) {
    c41_v_out[c41_i35] = 0.0;
  }

  for (c41_i36 = 0; c41_i36 < 3; c41_i36++) {
    c41_C[c41_i36] = c41_v_out[c41_i36];
  }

  for (c41_i37 = 0; c41_i37 < 3; c41_i37++) {
    c41_v_out[c41_i37] = c41_C[c41_i37];
  }

  for (c41_i38 = 0; c41_i38 < 3; c41_i38++) {
    c41_C[c41_i38] = c41_v_out[c41_i38];
  }

  for (c41_i39 = 0; c41_i39 < 3; c41_i39++) {
    c41_v_out[c41_i39] = c41_C[c41_i39];
  }

  for (c41_i40 = 0; c41_i40 < 3; c41_i40++) {
    c41_v_out[c41_i40] = 0.0;
    c41_i41 = 0;
    for (c41_i42 = 0; c41_i42 < 3; c41_i42++) {
      c41_v_out[c41_i40] += c41_c_a[c41_i41 + c41_i40] * c41_c_b[c41_i42];
      c41_i41 += 3;
    }
  }

  _SFD_SCRIPT_CALL(1U, chartInstance->c41_sfEvent, -11);
  _SFD_SYMBOL_SCOPE_POP();
}

static void init_script_number_translation(uint32_T c41_machineNumber, uint32_T
  c41_chartNumber)
{
  _SFD_SCRIPT_TRANSLATION(c41_chartNumber, 0U, sf_debug_get_script_id(
    "C:/Users/Kostas/Documents/GitHub/robust_mpc/Verified_Examples/TTR/Rotate_IMUframe_2_Frame.m"));
  _SFD_SCRIPT_TRANSLATION(c41_chartNumber, 1U, sf_debug_get_script_id(
    "C:/Users/Kostas/Documents/GitHub/robust_mpc/Verified_Examples/TTR/Rotate_IMUrates_2_Frame.m"));
}

static const mxArray *c41_sf_marshallOut(void *chartInstanceVoid, void
  *c41_inData)
{
  const mxArray *c41_mxArrayOutData = NULL;
  real_T c41_u;
  const mxArray *c41_y = NULL;
  SFc41_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc41_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c41_mxArrayOutData = NULL;
  c41_u = *(real_T *)c41_inData;
  c41_y = NULL;
  sf_mex_assign(&c41_y, sf_mex_create("y", &c41_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c41_mxArrayOutData, c41_y, FALSE);
  return c41_mxArrayOutData;
}

static real_T c41_emlrt_marshallIn(SFc41_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c41_z_die, const char_T *c41_identifier)
{
  real_T c41_y;
  emlrtMsgIdentifier c41_thisId;
  c41_thisId.fIdentifier = c41_identifier;
  c41_thisId.fParent = NULL;
  c41_y = c41_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c41_z_die),
    &c41_thisId);
  sf_mex_destroy(&c41_z_die);
  return c41_y;
}

static real_T c41_b_emlrt_marshallIn(SFc41_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c41_u, const emlrtMsgIdentifier *c41_parentId)
{
  real_T c41_y;
  real_T c41_d0;
  sf_mex_import(c41_parentId, sf_mex_dup(c41_u), &c41_d0, 1, 0, 0U, 0, 0U, 0);
  c41_y = c41_d0;
  sf_mex_destroy(&c41_u);
  return c41_y;
}

static void c41_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c41_mxArrayInData, const char_T *c41_varName, void *c41_outData)
{
  const mxArray *c41_z_die;
  const char_T *c41_identifier;
  emlrtMsgIdentifier c41_thisId;
  real_T c41_y;
  SFc41_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc41_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c41_z_die = sf_mex_dup(c41_mxArrayInData);
  c41_identifier = c41_varName;
  c41_thisId.fIdentifier = c41_identifier;
  c41_thisId.fParent = NULL;
  c41_y = c41_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c41_z_die),
    &c41_thisId);
  sf_mex_destroy(&c41_z_die);
  *(real_T *)c41_outData = c41_y;
  sf_mex_destroy(&c41_mxArrayInData);
}

static const mxArray *c41_b_sf_marshallOut(void *chartInstanceVoid, void
  *c41_inData)
{
  const mxArray *c41_mxArrayOutData = NULL;
  int32_T c41_i43;
  real_T c41_b_inData[3];
  int32_T c41_i44;
  real_T c41_u[3];
  const mxArray *c41_y = NULL;
  SFc41_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc41_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c41_mxArrayOutData = NULL;
  for (c41_i43 = 0; c41_i43 < 3; c41_i43++) {
    c41_b_inData[c41_i43] = (*(real_T (*)[3])c41_inData)[c41_i43];
  }

  for (c41_i44 = 0; c41_i44 < 3; c41_i44++) {
    c41_u[c41_i44] = c41_b_inData[c41_i44];
  }

  c41_y = NULL;
  sf_mex_assign(&c41_y, sf_mex_create("y", c41_u, 0, 0U, 1U, 0U, 1, 3), FALSE);
  sf_mex_assign(&c41_mxArrayOutData, c41_y, FALSE);
  return c41_mxArrayOutData;
}

static void c41_c_emlrt_marshallIn(SFc41_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c41_u, const emlrtMsgIdentifier *c41_parentId, real_T c41_y[3])
{
  real_T c41_dv4[3];
  int32_T c41_i45;
  sf_mex_import(c41_parentId, sf_mex_dup(c41_u), c41_dv4, 1, 0, 0U, 1, 0U, 1, 3);
  for (c41_i45 = 0; c41_i45 < 3; c41_i45++) {
    c41_y[c41_i45] = c41_dv4[c41_i45];
  }

  sf_mex_destroy(&c41_u);
}

static void c41_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c41_mxArrayInData, const char_T *c41_varName, void *c41_outData)
{
  const mxArray *c41_Theta_Die;
  const char_T *c41_identifier;
  emlrtMsgIdentifier c41_thisId;
  real_T c41_y[3];
  int32_T c41_i46;
  SFc41_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc41_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c41_Theta_Die = sf_mex_dup(c41_mxArrayInData);
  c41_identifier = c41_varName;
  c41_thisId.fIdentifier = c41_identifier;
  c41_thisId.fParent = NULL;
  c41_c_emlrt_marshallIn(chartInstance, sf_mex_dup(c41_Theta_Die), &c41_thisId,
    c41_y);
  sf_mex_destroy(&c41_Theta_Die);
  for (c41_i46 = 0; c41_i46 < 3; c41_i46++) {
    (*(real_T (*)[3])c41_outData)[c41_i46] = c41_y[c41_i46];
  }

  sf_mex_destroy(&c41_mxArrayInData);
}

static const mxArray *c41_c_sf_marshallOut(void *chartInstanceVoid, void
  *c41_inData)
{
  const mxArray *c41_mxArrayOutData = NULL;
  int32_T c41_i47;
  int32_T c41_i48;
  int32_T c41_i49;
  real_T c41_b_inData[9];
  int32_T c41_i50;
  int32_T c41_i51;
  int32_T c41_i52;
  real_T c41_u[9];
  const mxArray *c41_y = NULL;
  SFc41_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc41_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c41_mxArrayOutData = NULL;
  c41_i47 = 0;
  for (c41_i48 = 0; c41_i48 < 3; c41_i48++) {
    for (c41_i49 = 0; c41_i49 < 3; c41_i49++) {
      c41_b_inData[c41_i49 + c41_i47] = (*(real_T (*)[9])c41_inData)[c41_i49 +
        c41_i47];
    }

    c41_i47 += 3;
  }

  c41_i50 = 0;
  for (c41_i51 = 0; c41_i51 < 3; c41_i51++) {
    for (c41_i52 = 0; c41_i52 < 3; c41_i52++) {
      c41_u[c41_i52 + c41_i50] = c41_b_inData[c41_i52 + c41_i50];
    }

    c41_i50 += 3;
  }

  c41_y = NULL;
  sf_mex_assign(&c41_y, sf_mex_create("y", c41_u, 0, 0U, 1U, 0U, 2, 3, 3), FALSE);
  sf_mex_assign(&c41_mxArrayOutData, c41_y, FALSE);
  return c41_mxArrayOutData;
}

static void c41_d_emlrt_marshallIn(SFc41_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c41_u, const emlrtMsgIdentifier *c41_parentId, real_T c41_y[9])
{
  real_T c41_dv5[9];
  int32_T c41_i53;
  sf_mex_import(c41_parentId, sf_mex_dup(c41_u), c41_dv5, 1, 0, 0U, 1, 0U, 2, 3,
                3);
  for (c41_i53 = 0; c41_i53 < 9; c41_i53++) {
    c41_y[c41_i53] = c41_dv5[c41_i53];
  }

  sf_mex_destroy(&c41_u);
}

static void c41_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c41_mxArrayInData, const char_T *c41_varName, void *c41_outData)
{
  const mxArray *c41_IMUframe_2_frame;
  const char_T *c41_identifier;
  emlrtMsgIdentifier c41_thisId;
  real_T c41_y[9];
  int32_T c41_i54;
  int32_T c41_i55;
  int32_T c41_i56;
  SFc41_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc41_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c41_IMUframe_2_frame = sf_mex_dup(c41_mxArrayInData);
  c41_identifier = c41_varName;
  c41_thisId.fIdentifier = c41_identifier;
  c41_thisId.fParent = NULL;
  c41_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c41_IMUframe_2_frame),
    &c41_thisId, c41_y);
  sf_mex_destroy(&c41_IMUframe_2_frame);
  c41_i54 = 0;
  for (c41_i55 = 0; c41_i55 < 3; c41_i55++) {
    for (c41_i56 = 0; c41_i56 < 3; c41_i56++) {
      (*(real_T (*)[9])c41_outData)[c41_i56 + c41_i54] = c41_y[c41_i56 + c41_i54];
    }

    c41_i54 += 3;
  }

  sf_mex_destroy(&c41_mxArrayInData);
}

const mxArray *sf_c41_TTR_mdl_get_eml_resolved_functions_info(void)
{
  const mxArray *c41_nameCaptureInfo;
  c41_ResolvedFunctionInfo c41_info[22];
  const mxArray *c41_m0 = NULL;
  int32_T c41_i57;
  c41_ResolvedFunctionInfo *c41_r0;
  c41_nameCaptureInfo = NULL;
  c41_nameCaptureInfo = NULL;
  c41_info_helper(c41_info);
  sf_mex_assign(&c41_m0, sf_mex_createstruct("nameCaptureInfo", 1, 22), FALSE);
  for (c41_i57 = 0; c41_i57 < 22; c41_i57++) {
    c41_r0 = &c41_info[c41_i57];
    sf_mex_addfield(c41_m0, sf_mex_create("nameCaptureInfo", c41_r0->context, 15,
      0U, 0U, 0U, 2, 1, strlen(c41_r0->context)), "context", "nameCaptureInfo",
                    c41_i57);
    sf_mex_addfield(c41_m0, sf_mex_create("nameCaptureInfo", c41_r0->name, 15,
      0U, 0U, 0U, 2, 1, strlen(c41_r0->name)), "name", "nameCaptureInfo",
                    c41_i57);
    sf_mex_addfield(c41_m0, sf_mex_create("nameCaptureInfo",
      c41_r0->dominantType, 15, 0U, 0U, 0U, 2, 1, strlen(c41_r0->dominantType)),
                    "dominantType", "nameCaptureInfo", c41_i57);
    sf_mex_addfield(c41_m0, sf_mex_create("nameCaptureInfo", c41_r0->resolved,
      15, 0U, 0U, 0U, 2, 1, strlen(c41_r0->resolved)), "resolved",
                    "nameCaptureInfo", c41_i57);
    sf_mex_addfield(c41_m0, sf_mex_create("nameCaptureInfo", &c41_r0->fileTimeLo,
      7, 0U, 0U, 0U, 0), "fileTimeLo", "nameCaptureInfo", c41_i57);
    sf_mex_addfield(c41_m0, sf_mex_create("nameCaptureInfo", &c41_r0->fileTimeHi,
      7, 0U, 0U, 0U, 0), "fileTimeHi", "nameCaptureInfo", c41_i57);
    sf_mex_addfield(c41_m0, sf_mex_create("nameCaptureInfo",
      &c41_r0->mFileTimeLo, 7, 0U, 0U, 0U, 0), "mFileTimeLo", "nameCaptureInfo",
                    c41_i57);
    sf_mex_addfield(c41_m0, sf_mex_create("nameCaptureInfo",
      &c41_r0->mFileTimeHi, 7, 0U, 0U, 0U, 0), "mFileTimeHi", "nameCaptureInfo",
                    c41_i57);
  }

  sf_mex_assign(&c41_nameCaptureInfo, c41_m0, FALSE);
  sf_mex_emlrtNameCapturePostProcessR2012a(&c41_nameCaptureInfo);
  return c41_nameCaptureInfo;
}

static void c41_info_helper(c41_ResolvedFunctionInfo c41_info[22])
{
  c41_info[0].context = "";
  c41_info[0].name = "Rotate_IMUframe_2_Frame";
  c41_info[0].dominantType = "double";
  c41_info[0].resolved =
    "[E]C:/Users/Kostas/Documents/GitHub/robust_mpc/Verified_Examples/TTR/Rotate_IMUframe_2_Frame.m";
  c41_info[0].fileTimeLo = 1377894818U;
  c41_info[0].fileTimeHi = 0U;
  c41_info[0].mFileTimeLo = 0U;
  c41_info[0].mFileTimeHi = 0U;
  c41_info[1].context =
    "[E]C:/Users/Kostas/Documents/GitHub/robust_mpc/Verified_Examples/TTR/Rotate_IMUframe_2_Frame.m";
  c41_info[1].name = "sin";
  c41_info[1].dominantType = "double";
  c41_info[1].resolved = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sin.m";
  c41_info[1].fileTimeLo = 1343830386U;
  c41_info[1].fileTimeHi = 0U;
  c41_info[1].mFileTimeLo = 0U;
  c41_info[1].mFileTimeHi = 0U;
  c41_info[2].context = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sin.m";
  c41_info[2].name = "eml_scalar_sin";
  c41_info[2].dominantType = "double";
  c41_info[2].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_sin.m";
  c41_info[2].fileTimeLo = 1286818736U;
  c41_info[2].fileTimeHi = 0U;
  c41_info[2].mFileTimeLo = 0U;
  c41_info[2].mFileTimeHi = 0U;
  c41_info[3].context =
    "[E]C:/Users/Kostas/Documents/GitHub/robust_mpc/Verified_Examples/TTR/Rotate_IMUframe_2_Frame.m";
  c41_info[3].name = "cos";
  c41_info[3].dominantType = "double";
  c41_info[3].resolved = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/cos.m";
  c41_info[3].fileTimeLo = 1343830372U;
  c41_info[3].fileTimeHi = 0U;
  c41_info[3].mFileTimeLo = 0U;
  c41_info[3].mFileTimeHi = 0U;
  c41_info[4].context = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/cos.m";
  c41_info[4].name = "eml_scalar_cos";
  c41_info[4].dominantType = "double";
  c41_info[4].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cos.m";
  c41_info[4].fileTimeLo = 1286818722U;
  c41_info[4].fileTimeHi = 0U;
  c41_info[4].mFileTimeLo = 0U;
  c41_info[4].mFileTimeHi = 0U;
  c41_info[5].context =
    "[E]C:/Users/Kostas/Documents/GitHub/robust_mpc/Verified_Examples/TTR/Rotate_IMUframe_2_Frame.m";
  c41_info[5].name = "mtimes";
  c41_info[5].dominantType = "double";
  c41_info[5].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c41_info[5].fileTimeLo = 1289519692U;
  c41_info[5].fileTimeHi = 0U;
  c41_info[5].mFileTimeLo = 0U;
  c41_info[5].mFileTimeHi = 0U;
  c41_info[6].context = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c41_info[6].name = "eml_index_class";
  c41_info[6].dominantType = "";
  c41_info[6].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c41_info[6].fileTimeLo = 1323170578U;
  c41_info[6].fileTimeHi = 0U;
  c41_info[6].mFileTimeLo = 0U;
  c41_info[6].mFileTimeHi = 0U;
  c41_info[7].context = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c41_info[7].name = "eml_scalar_eg";
  c41_info[7].dominantType = "double";
  c41_info[7].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m";
  c41_info[7].fileTimeLo = 1286818796U;
  c41_info[7].fileTimeHi = 0U;
  c41_info[7].mFileTimeLo = 0U;
  c41_info[7].mFileTimeHi = 0U;
  c41_info[8].context = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c41_info[8].name = "eml_xgemm";
  c41_info[8].dominantType = "char";
  c41_info[8].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xgemm.m";
  c41_info[8].fileTimeLo = 1299076772U;
  c41_info[8].fileTimeHi = 0U;
  c41_info[8].mFileTimeLo = 0U;
  c41_info[8].mFileTimeHi = 0U;
  c41_info[9].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xgemm.m";
  c41_info[9].name = "eml_blas_inline";
  c41_info[9].dominantType = "";
  c41_info[9].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_blas_inline.m";
  c41_info[9].fileTimeLo = 1299076768U;
  c41_info[9].fileTimeHi = 0U;
  c41_info[9].mFileTimeLo = 0U;
  c41_info[9].mFileTimeHi = 0U;
  c41_info[10].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xgemm.m!below_threshold";
  c41_info[10].name = "mtimes";
  c41_info[10].dominantType = "double";
  c41_info[10].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c41_info[10].fileTimeLo = 1289519692U;
  c41_info[10].fileTimeHi = 0U;
  c41_info[10].mFileTimeLo = 0U;
  c41_info[10].mFileTimeHi = 0U;
  c41_info[11].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xgemm.m";
  c41_info[11].name = "eml_index_class";
  c41_info[11].dominantType = "";
  c41_info[11].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c41_info[11].fileTimeLo = 1323170578U;
  c41_info[11].fileTimeHi = 0U;
  c41_info[11].mFileTimeLo = 0U;
  c41_info[11].mFileTimeHi = 0U;
  c41_info[12].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xgemm.m";
  c41_info[12].name = "eml_scalar_eg";
  c41_info[12].dominantType = "double";
  c41_info[12].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m";
  c41_info[12].fileTimeLo = 1286818796U;
  c41_info[12].fileTimeHi = 0U;
  c41_info[12].mFileTimeLo = 0U;
  c41_info[12].mFileTimeHi = 0U;
  c41_info[13].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xgemm.m";
  c41_info[13].name = "eml_refblas_xgemm";
  c41_info[13].dominantType = "char";
  c41_info[13].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xgemm.m";
  c41_info[13].fileTimeLo = 1299076774U;
  c41_info[13].fileTimeHi = 0U;
  c41_info[13].mFileTimeLo = 0U;
  c41_info[13].mFileTimeHi = 0U;
  c41_info[14].context = "";
  c41_info[14].name = "Rotate_IMUrates_2_Frame";
  c41_info[14].dominantType = "double";
  c41_info[14].resolved =
    "[E]C:/Users/Kostas/Documents/GitHub/robust_mpc/Verified_Examples/TTR/Rotate_IMUrates_2_Frame.m";
  c41_info[14].fileTimeLo = 1377894818U;
  c41_info[14].fileTimeHi = 0U;
  c41_info[14].mFileTimeLo = 0U;
  c41_info[14].mFileTimeHi = 0U;
  c41_info[15].context =
    "[E]C:/Users/Kostas/Documents/GitHub/robust_mpc/Verified_Examples/TTR/Rotate_IMUrates_2_Frame.m";
  c41_info[15].name = "sin";
  c41_info[15].dominantType = "double";
  c41_info[15].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sin.m";
  c41_info[15].fileTimeLo = 1343830386U;
  c41_info[15].fileTimeHi = 0U;
  c41_info[15].mFileTimeLo = 0U;
  c41_info[15].mFileTimeHi = 0U;
  c41_info[16].context =
    "[E]C:/Users/Kostas/Documents/GitHub/robust_mpc/Verified_Examples/TTR/Rotate_IMUrates_2_Frame.m";
  c41_info[16].name = "cos";
  c41_info[16].dominantType = "double";
  c41_info[16].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/cos.m";
  c41_info[16].fileTimeLo = 1343830372U;
  c41_info[16].fileTimeHi = 0U;
  c41_info[16].mFileTimeLo = 0U;
  c41_info[16].mFileTimeHi = 0U;
  c41_info[17].context =
    "[E]C:/Users/Kostas/Documents/GitHub/robust_mpc/Verified_Examples/TTR/Rotate_IMUrates_2_Frame.m";
  c41_info[17].name = "mrdivide";
  c41_info[17].dominantType = "double";
  c41_info[17].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p";
  c41_info[17].fileTimeLo = 1357951548U;
  c41_info[17].fileTimeHi = 0U;
  c41_info[17].mFileTimeLo = 1319729966U;
  c41_info[17].mFileTimeHi = 0U;
  c41_info[18].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p";
  c41_info[18].name = "rdivide";
  c41_info[18].dominantType = "double";
  c41_info[18].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m";
  c41_info[18].fileTimeLo = 1346510388U;
  c41_info[18].fileTimeHi = 0U;
  c41_info[18].mFileTimeLo = 0U;
  c41_info[18].mFileTimeHi = 0U;
  c41_info[19].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m";
  c41_info[19].name = "eml_scalexp_compatible";
  c41_info[19].dominantType = "double";
  c41_info[19].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_compatible.m";
  c41_info[19].fileTimeLo = 1286818796U;
  c41_info[19].fileTimeHi = 0U;
  c41_info[19].mFileTimeLo = 0U;
  c41_info[19].mFileTimeHi = 0U;
  c41_info[20].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m";
  c41_info[20].name = "eml_div";
  c41_info[20].dominantType = "double";
  c41_info[20].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_div.m";
  c41_info[20].fileTimeLo = 1313347810U;
  c41_info[20].fileTimeHi = 0U;
  c41_info[20].mFileTimeLo = 0U;
  c41_info[20].mFileTimeHi = 0U;
  c41_info[21].context =
    "[E]C:/Users/Kostas/Documents/GitHub/robust_mpc/Verified_Examples/TTR/Rotate_IMUrates_2_Frame.m";
  c41_info[21].name = "mtimes";
  c41_info[21].dominantType = "double";
  c41_info[21].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c41_info[21].fileTimeLo = 1289519692U;
  c41_info[21].fileTimeHi = 0U;
  c41_info[21].mFileTimeLo = 0U;
  c41_info[21].mFileTimeHi = 0U;
}

static void c41_eml_scalar_eg(SFc41_TTR_mdlInstanceStruct *chartInstance)
{
}

static const mxArray *c41_d_sf_marshallOut(void *chartInstanceVoid, void
  *c41_inData)
{
  const mxArray *c41_mxArrayOutData = NULL;
  int32_T c41_u;
  const mxArray *c41_y = NULL;
  SFc41_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc41_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c41_mxArrayOutData = NULL;
  c41_u = *(int32_T *)c41_inData;
  c41_y = NULL;
  sf_mex_assign(&c41_y, sf_mex_create("y", &c41_u, 6, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c41_mxArrayOutData, c41_y, FALSE);
  return c41_mxArrayOutData;
}

static int32_T c41_e_emlrt_marshallIn(SFc41_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c41_u, const emlrtMsgIdentifier *c41_parentId)
{
  int32_T c41_y;
  int32_T c41_i58;
  sf_mex_import(c41_parentId, sf_mex_dup(c41_u), &c41_i58, 1, 6, 0U, 0, 0U, 0);
  c41_y = c41_i58;
  sf_mex_destroy(&c41_u);
  return c41_y;
}

static void c41_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c41_mxArrayInData, const char_T *c41_varName, void *c41_outData)
{
  const mxArray *c41_b_sfEvent;
  const char_T *c41_identifier;
  emlrtMsgIdentifier c41_thisId;
  int32_T c41_y;
  SFc41_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc41_TTR_mdlInstanceStruct *)chartInstanceVoid;
  c41_b_sfEvent = sf_mex_dup(c41_mxArrayInData);
  c41_identifier = c41_varName;
  c41_thisId.fIdentifier = c41_identifier;
  c41_thisId.fParent = NULL;
  c41_y = c41_e_emlrt_marshallIn(chartInstance, sf_mex_dup(c41_b_sfEvent),
    &c41_thisId);
  sf_mex_destroy(&c41_b_sfEvent);
  *(int32_T *)c41_outData = c41_y;
  sf_mex_destroy(&c41_mxArrayInData);
}

static uint8_T c41_f_emlrt_marshallIn(SFc41_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c41_b_is_active_c41_TTR_mdl, const char_T *c41_identifier)
{
  uint8_T c41_y;
  emlrtMsgIdentifier c41_thisId;
  c41_thisId.fIdentifier = c41_identifier;
  c41_thisId.fParent = NULL;
  c41_y = c41_g_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c41_b_is_active_c41_TTR_mdl), &c41_thisId);
  sf_mex_destroy(&c41_b_is_active_c41_TTR_mdl);
  return c41_y;
}

static uint8_T c41_g_emlrt_marshallIn(SFc41_TTR_mdlInstanceStruct *chartInstance,
  const mxArray *c41_u, const emlrtMsgIdentifier *c41_parentId)
{
  uint8_T c41_y;
  uint8_T c41_u0;
  sf_mex_import(c41_parentId, sf_mex_dup(c41_u), &c41_u0, 1, 3, 0U, 0, 0U, 0);
  c41_y = c41_u0;
  sf_mex_destroy(&c41_u);
  return c41_y;
}

static void init_dsm_address_info(SFc41_TTR_mdlInstanceStruct *chartInstance)
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

void sf_c41_TTR_mdl_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(3462249390U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(700386061U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(177490480U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(61074959U);
}

mxArray *sf_c41_TTR_mdl_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1,1,5,
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("DapHx4lTwX5mB4jJ48d3yE");
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,6,3,dataFields);

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

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(3);
      pr[1] = (double)(1);
      mxSetField(mxData,5,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,5,"type",mxType);
    }

    mxSetField(mxData,5,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"inputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"parameters",mxCreateDoubleMatrix(0,0,
                mxREAL));
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,12,3,dataFields);

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

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
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

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,5,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,5,"type",mxType);
    }

    mxSetField(mxData,5,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,6,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,6,"type",mxType);
    }

    mxSetField(mxData,6,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,7,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,7,"type",mxType);
    }

    mxSetField(mxData,7,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,8,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,8,"type",mxType);
    }

    mxSetField(mxData,8,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,9,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,9,"type",mxType);
    }

    mxSetField(mxData,9,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,10,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,10,"type",mxType);
    }

    mxSetField(mxData,10,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,11,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,11,"type",mxType);
    }

    mxSetField(mxData,11,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"outputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"locals",mxCreateDoubleMatrix(0,0,mxREAL));
  }

  return(mxAutoinheritanceInfo);
}

mxArray *sf_c41_TTR_mdl_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,0);
  return(mxcell3p);
}

static const mxArray *sf_get_sim_state_info_c41_TTR_mdl(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x10'type','srcId','name','auxInfo'{{M[1],M[41],T\"phi_die\",},{M[1],M[38],T\"phi_dot\",},{M[1],M[43],T\"psi_die\",},{M[1],M[40],T\"psi_dot\",},{M[1],M[42],T\"theta_die\",},{M[1],M[39],T\"theta_dot\",},{M[1],M[22],T\"x_die\",},{M[1],M[5],T\"x_dot\",},{M[1],M[23],T\"y_die\",},{M[1],M[20],T\"y_dot\",}}",
    "100 S1x3'type','srcId','name','auxInfo'{{M[1],M[24],T\"z_die\",},{M[1],M[21],T\"z_dot\",},{M[8],M[0],T\"is_active_c41_TTR_mdl\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 13, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c41_TTR_mdl_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc41_TTR_mdlInstanceStruct *chartInstance;
    chartInstance = (SFc41_TTR_mdlInstanceStruct *) ((ChartInfoStruct *)
      (ssGetUserData(S)))->chartInstance;
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (sfGlobalDebugInstanceStruct,
           _TTR_mdlMachineNumber_,
           41,
           1,
           1,
           18,
           0,
           0,
           0,
           0,
           2,
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
          _SFD_SET_DATA_PROPS(0,2,0,1,"phi_dot");
          _SFD_SET_DATA_PROPS(1,2,0,1,"theta_dot");
          _SFD_SET_DATA_PROPS(2,2,0,1,"psi_dot");
          _SFD_SET_DATA_PROPS(3,2,0,1,"x_dot");
          _SFD_SET_DATA_PROPS(4,1,1,0,"Theta_in");
          _SFD_SET_DATA_PROPS(5,2,0,1,"y_dot");
          _SFD_SET_DATA_PROPS(6,2,0,1,"z_dot");
          _SFD_SET_DATA_PROPS(7,2,0,1,"phi_die");
          _SFD_SET_DATA_PROPS(8,2,0,1,"theta_die");
          _SFD_SET_DATA_PROPS(9,2,0,1,"psi_die");
          _SFD_SET_DATA_PROPS(10,2,0,1,"x_die");
          _SFD_SET_DATA_PROPS(11,2,0,1,"y_die");
          _SFD_SET_DATA_PROPS(12,2,0,1,"z_die");
          _SFD_SET_DATA_PROPS(13,1,1,0,"X_in");
          _SFD_SET_DATA_PROPS(14,1,1,0,"Omega_in");
          _SFD_SET_DATA_PROPS(15,1,1,0,"U_in");
          _SFD_SET_DATA_PROPS(16,1,1,0,"Omega_Dot_in");
          _SFD_SET_DATA_PROPS(17,1,1,0,"U_Dot_in");
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
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,918);
        _SFD_CV_INIT_SCRIPT(0,1,0,0,0,0,0,0,0,0);
        _SFD_CV_INIT_SCRIPT_FCN(0,0,"Rotate_IMUframe_2_Frame",0,-1,530);
        _SFD_CV_INIT_SCRIPT(1,1,0,0,0,0,0,0,0,0);
        _SFD_CV_INIT_SCRIPT_FCN(1,0,"Rotate_IMUrates_2_Frame",0,-1,408);
        _SFD_TRANS_COV_WTS(0,0,0,1,0);
        if (chartAlreadyPresent==0) {
          _SFD_TRANS_COV_MAPS(0,
                              0,NULL,NULL,
                              0,NULL,NULL,
                              1,NULL,NULL,
                              0,NULL,NULL);
        }

        _SFD_SET_DATA_COMPILED_PROPS(0,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c41_sf_marshallOut,(MexInFcnForType)c41_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(1,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c41_sf_marshallOut,(MexInFcnForType)c41_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(2,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c41_sf_marshallOut,(MexInFcnForType)c41_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(3,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c41_sf_marshallOut,(MexInFcnForType)c41_sf_marshallIn);

        {
          unsigned int dimVector[1];
          dimVector[0]= 3;
          _SFD_SET_DATA_COMPILED_PROPS(4,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c41_b_sf_marshallOut,(MexInFcnForType)NULL);
        }

        _SFD_SET_DATA_COMPILED_PROPS(5,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c41_sf_marshallOut,(MexInFcnForType)c41_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(6,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c41_sf_marshallOut,(MexInFcnForType)c41_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(7,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c41_sf_marshallOut,(MexInFcnForType)c41_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(8,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c41_sf_marshallOut,(MexInFcnForType)c41_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(9,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c41_sf_marshallOut,(MexInFcnForType)c41_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(10,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c41_sf_marshallOut,(MexInFcnForType)c41_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(11,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c41_sf_marshallOut,(MexInFcnForType)c41_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(12,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c41_sf_marshallOut,(MexInFcnForType)c41_sf_marshallIn);

        {
          unsigned int dimVector[1];
          dimVector[0]= 3;
          _SFD_SET_DATA_COMPILED_PROPS(13,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c41_b_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 3;
          _SFD_SET_DATA_COMPILED_PROPS(14,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c41_b_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 3;
          _SFD_SET_DATA_COMPILED_PROPS(15,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c41_b_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 3;
          _SFD_SET_DATA_COMPILED_PROPS(16,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c41_b_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 3;
          _SFD_SET_DATA_COMPILED_PROPS(17,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c41_b_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          real_T *c41_phi_dot;
          real_T *c41_theta_dot;
          real_T *c41_psi_dot;
          real_T *c41_x_dot;
          real_T *c41_y_dot;
          real_T *c41_z_dot;
          real_T *c41_phi_die;
          real_T *c41_theta_die;
          real_T *c41_psi_die;
          real_T *c41_x_die;
          real_T *c41_y_die;
          real_T *c41_z_die;
          real_T (*c41_Theta_in)[3];
          real_T (*c41_X_in)[3];
          real_T (*c41_Omega_in)[3];
          real_T (*c41_U_in)[3];
          real_T (*c41_Omega_Dot_in)[3];
          real_T (*c41_U_Dot_in)[3];
          c41_U_Dot_in = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 5);
          c41_Omega_Dot_in = (real_T (*)[3])ssGetInputPortSignal
            (chartInstance->S, 4);
          c41_U_in = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 3);
          c41_Omega_in = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 2);
          c41_X_in = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 1);
          c41_z_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 12);
          c41_y_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 11);
          c41_x_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 10);
          c41_psi_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 9);
          c41_theta_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 8);
          c41_phi_die = (real_T *)ssGetOutputPortSignal(chartInstance->S, 7);
          c41_z_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 6);
          c41_y_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 5);
          c41_Theta_in = (real_T (*)[3])ssGetInputPortSignal(chartInstance->S, 0);
          c41_x_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 4);
          c41_psi_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 3);
          c41_theta_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 2);
          c41_phi_dot = (real_T *)ssGetOutputPortSignal(chartInstance->S, 1);
          _SFD_SET_DATA_VALUE_PTR(0U, c41_phi_dot);
          _SFD_SET_DATA_VALUE_PTR(1U, c41_theta_dot);
          _SFD_SET_DATA_VALUE_PTR(2U, c41_psi_dot);
          _SFD_SET_DATA_VALUE_PTR(3U, c41_x_dot);
          _SFD_SET_DATA_VALUE_PTR(4U, *c41_Theta_in);
          _SFD_SET_DATA_VALUE_PTR(5U, c41_y_dot);
          _SFD_SET_DATA_VALUE_PTR(6U, c41_z_dot);
          _SFD_SET_DATA_VALUE_PTR(7U, c41_phi_die);
          _SFD_SET_DATA_VALUE_PTR(8U, c41_theta_die);
          _SFD_SET_DATA_VALUE_PTR(9U, c41_psi_die);
          _SFD_SET_DATA_VALUE_PTR(10U, c41_x_die);
          _SFD_SET_DATA_VALUE_PTR(11U, c41_y_die);
          _SFD_SET_DATA_VALUE_PTR(12U, c41_z_die);
          _SFD_SET_DATA_VALUE_PTR(13U, *c41_X_in);
          _SFD_SET_DATA_VALUE_PTR(14U, *c41_Omega_in);
          _SFD_SET_DATA_VALUE_PTR(15U, *c41_U_in);
          _SFD_SET_DATA_VALUE_PTR(16U, *c41_Omega_Dot_in);
          _SFD_SET_DATA_VALUE_PTR(17U, *c41_U_Dot_in);
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
  return "FDHjPtkQ2WHpTUoO0sAfbB";
}

static void sf_opaque_initialize_c41_TTR_mdl(void *chartInstanceVar)
{
  chart_debug_initialization(((SFc41_TTR_mdlInstanceStruct*) chartInstanceVar)
    ->S,0);
  initialize_params_c41_TTR_mdl((SFc41_TTR_mdlInstanceStruct*) chartInstanceVar);
  initialize_c41_TTR_mdl((SFc41_TTR_mdlInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_enable_c41_TTR_mdl(void *chartInstanceVar)
{
  enable_c41_TTR_mdl((SFc41_TTR_mdlInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_disable_c41_TTR_mdl(void *chartInstanceVar)
{
  disable_c41_TTR_mdl((SFc41_TTR_mdlInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_gateway_c41_TTR_mdl(void *chartInstanceVar)
{
  sf_c41_TTR_mdl((SFc41_TTR_mdlInstanceStruct*) chartInstanceVar);
}

extern const mxArray* sf_internal_get_sim_state_c41_TTR_mdl(SimStruct* S)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_raw2high");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = (mxArray*) get_sim_state_c41_TTR_mdl((SFc41_TTR_mdlInstanceStruct*)
    chartInfo->chartInstance);         /* raw sim ctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c41_TTR_mdl();/* state var info */
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

extern void sf_internal_set_sim_state_c41_TTR_mdl(SimStruct* S, const mxArray
  *st)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_high2raw");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = mxDuplicateArray(st);      /* high level simctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c41_TTR_mdl();/* state var info */
  mxError = sf_mex_call_matlab(1, plhs, 4, prhs, "sfprivate");
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  mxDestroyArray(prhs[3]);
  if (mxError || plhs[0] == NULL) {
    sf_mex_error_message("Stateflow Internal Error: \nError calling 'chart_simctx_high2raw'.\n");
  }

  set_sim_state_c41_TTR_mdl((SFc41_TTR_mdlInstanceStruct*)
    chartInfo->chartInstance, mxDuplicateArray(plhs[0]));
  mxDestroyArray(plhs[0]);
}

static const mxArray* sf_opaque_get_sim_state_c41_TTR_mdl(SimStruct* S)
{
  return sf_internal_get_sim_state_c41_TTR_mdl(S);
}

static void sf_opaque_set_sim_state_c41_TTR_mdl(SimStruct* S, const mxArray *st)
{
  sf_internal_set_sim_state_c41_TTR_mdl(S, st);
}

static void sf_opaque_terminate_c41_TTR_mdl(void *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc41_TTR_mdlInstanceStruct*) chartInstanceVar)->S;
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_TTR_mdl_optimization_info();
    }

    finalize_c41_TTR_mdl((SFc41_TTR_mdlInstanceStruct*) chartInstanceVar);
    utFree((void *)chartInstanceVar);
    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc41_TTR_mdl((SFc41_TTR_mdlInstanceStruct*) chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c41_TTR_mdl(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    initialize_params_c41_TTR_mdl((SFc41_TTR_mdlInstanceStruct*)
      (((ChartInfoStruct *)ssGetUserData(S))->chartInstance));
  }
}

static void mdlSetWorkWidths_c41_TTR_mdl(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_TTR_mdl_optimization_info();
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable(S,sf_get_instance_specialization(),infoStruct,
      41);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,sf_rtw_info_uint_prop(S,sf_get_instance_specialization(),
                infoStruct,41,"RTWCG"));
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop(S,
      sf_get_instance_specialization(),infoStruct,41,
      "gatewayCannotBeInlinedMultipleTimes"));
    sf_update_buildInfo(S,sf_get_instance_specialization(),infoStruct,41);
    if (chartIsInlinable) {
      ssSetInputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 1, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 2, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 3, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 4, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 5, SS_REUSABLE_AND_LOCAL);
      sf_mark_chart_expressionable_inputs(S,sf_get_instance_specialization(),
        infoStruct,41,6);
      sf_mark_chart_reusable_outputs(S,sf_get_instance_specialization(),
        infoStruct,41,12);
    }

    {
      unsigned int outPortIdx;
      for (outPortIdx=1; outPortIdx<=12; ++outPortIdx) {
        ssSetOutputPortOptimizeInIR(S, outPortIdx, 1U);
      }
    }

    {
      unsigned int inPortIdx;
      for (inPortIdx=0; inPortIdx < 6; ++inPortIdx) {
        ssSetInputPortOptimizeInIR(S, inPortIdx, 1U);
      }
    }

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,41);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(692230673U));
  ssSetChecksum1(S,(4050015501U));
  ssSetChecksum2(S,(766082082U));
  ssSetChecksum3(S,(2055266992U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
  ssSupportsMultipleExecInstances(S,1);
}

static void mdlRTW_c41_TTR_mdl(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c41_TTR_mdl(SimStruct *S)
{
  SFc41_TTR_mdlInstanceStruct *chartInstance;
  chartInstance = (SFc41_TTR_mdlInstanceStruct *)utMalloc(sizeof
    (SFc41_TTR_mdlInstanceStruct));
  memset(chartInstance, 0, sizeof(SFc41_TTR_mdlInstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway = sf_opaque_gateway_c41_TTR_mdl;
  chartInstance->chartInfo.initializeChart = sf_opaque_initialize_c41_TTR_mdl;
  chartInstance->chartInfo.terminateChart = sf_opaque_terminate_c41_TTR_mdl;
  chartInstance->chartInfo.enableChart = sf_opaque_enable_c41_TTR_mdl;
  chartInstance->chartInfo.disableChart = sf_opaque_disable_c41_TTR_mdl;
  chartInstance->chartInfo.getSimState = sf_opaque_get_sim_state_c41_TTR_mdl;
  chartInstance->chartInfo.setSimState = sf_opaque_set_sim_state_c41_TTR_mdl;
  chartInstance->chartInfo.getSimStateInfo = sf_get_sim_state_info_c41_TTR_mdl;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c41_TTR_mdl;
  chartInstance->chartInfo.mdlStart = mdlStart_c41_TTR_mdl;
  chartInstance->chartInfo.mdlSetWorkWidths = mdlSetWorkWidths_c41_TTR_mdl;
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

void c41_TTR_mdl_method_dispatcher(SimStruct *S, int_T method, void *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c41_TTR_mdl(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c41_TTR_mdl(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c41_TTR_mdl(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c41_TTR_mdl_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
