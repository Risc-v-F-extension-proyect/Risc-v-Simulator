#include "rv_f_ext.h" // Asegúrate de que el include sea el de F, no el de M

namespace Ripes {
namespace RVISA {
namespace ExtF {

void enableExt(
    const ISAInfoBase *isa,
    InstrVec & instructions,
    PseudoInstrVec &)
{
  using namespace ExtF;

  enableInstructions<

      Flw_s, Fsw_s,                                 // Carga y almacenamiento
      Fadd_s, Fsub_s, Fmul_s, Fdiv_s, Fsqrt_s,      // Aritmeticas
      Fmin_s, Fmax_s,                               // Min-max
      Fmadd_s, Fmsub_s, Fnmadd_s, Fnmsub_s,         // R4
      Fcvt_s_wu_s, Fcvt_s_w_s, Fcvt_w_s, Fcvt_wu_s, // Conversion int - float

      Feq_s, Flt_s, Fle_s,                          // Comparacion
      Fmv_x_w_s, Fmv_w_x_s,                         // Movimiento
      Fsgnj_s, Fsgnjn_s, Fsgnjx_s,                  // Inyeccion de signo
      Fclass_s                                      // Clasificacion
      >(instructions);

  if(isa->bits() == 64){
  }
}

} //namespace ExtF
} //namespace RVISA
} //namespace Ripes
