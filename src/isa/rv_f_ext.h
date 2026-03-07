#pragma once

#include "pseudoinstruction.h"
#include "rv_i_ext.h"
#include "rvisainfo_common.h"

namespace Ripes {
namespace RVISA {
namespace ExtF {

enum class Funct5 : unsigned {
  FADD    = 0b00000,
  FSUB    = 0b00001,
  FMUL    = 0b00010,
  FDIV    = 0b00011,
  FSQRT   = 0b01011,

  FMINMAX = 0b00101,

  FEQ     = 0b10100,
  FLE     = 0b10101,
  FLT     = 0b10110,

  FCVT_W_S  = 0b11000,
  FCVT_WU_S = 0b11000,
  FCVT_S_W  = 0b11010,
  FCVT_S_WU = 0b11010,

  FCLASS  = 0b11100,

  FMV_X_W = 0b11100,
  FMV_W_X = 0b11110,

  FSGNJ   = 0b00100,

  FMADD   = 0b00000,
  FMSUB   = 0b00000,
  FNMADD  = 0b00000,
  FNMSUB  = 0b00000
};

enum class Fmt : unsigned {
  S = 0b00,
  D = 0b01,
  Q = 0b11
};

enum class Rm : unsigned {
  RNE = 0b000,
  RTZ = 0b001,
  RDN = 0b010,
  RUP = 0b011,
  RMM = 0b100,
  DYN = 0b111,
  UNDEFINED = 0xff
};

enum struct Rs2 : unsigned {
  ZERO = 0,
  ONE = 1,
  UNDEFINED = 0xff
};

template <unsigned V>
using OpPartRs2 = OpPart<V, BitRange<20,24>>;

using RVISA::OpcodeID;


template<typename InstrImpl, OpcodeID opcodeID, Rm width>
struct F_Lw : public RV_Instruction<InstrImpl> {
  struct Opcode : public OpcodeSet<
                      OpPartOpcode<opcodeID>,
                      OpPartRm<static_cast<unsigned>(width)>
                      > {};
  struct Fields : public FieldSet<RegRd, RegRs1, Imm12> {};
};


template<typename InstrImpl, OpcodeID opcodeID, Rm width>
struct F_Sw : public RV_Instruction<InstrImpl> {
  struct Opcode : public OpcodeSet<
                      OpPartOpcode<opcodeID>,
                      OpPartRm<static_cast<unsigned>(width)>
                      > {};
  struct Fields : public FieldSet<RegRs1, RegRs2, Imm12s> {};
};


template <typename InstrImpl, OpcodeID opcodeID, Fmt fmt, Funct5 funct5, Rm rm = Rm::UNDEFINED, Rs2 rs2 = Rs2::UNDEFINED>
struct F_R : public RV_Instruction<InstrImpl> {

  // --- Lógica del Opcode (Se queda igual, está bien) ---
  using ResolveOpcode = typename std::conditional<
      rm == Rm::UNDEFINED,
      typename std::conditional<
          rs2 == Rs2::UNDEFINED,
          OpcodeSet<OpPartOpcode<opcodeID>, OpPartFmt<static_cast<unsigned>(fmt)>, OpPartFunct5<static_cast<unsigned>(funct5)>>,
          OpcodeSet<OpPartOpcode<opcodeID>, OpPartFmt<static_cast<unsigned>(fmt)>, OpPartFunct5<static_cast<unsigned>(funct5)>, OpPartRs2<static_cast<unsigned>(rs2)>>
          >::type,
      OpcodeSet<OpPartOpcode<opcodeID>, OpPartRm<static_cast<unsigned>(rm)>, OpPartFmt<static_cast<unsigned>(fmt)>, OpPartFunct5<static_cast<unsigned>(funct5)>>
      >::type;

  struct Opcode : public ResolveOpcode {};

  using ResolveFields = typename std::conditional<
      rm == Rm::UNDEFINED,
      typename std::conditional<
          rs2 == Rs2::UNDEFINED,
          FieldSet<RegRd, RegRm, RegRs1, RegRs2>,
          FieldSet<RegRd, RegRm, RegRs1>
          >::type,
      typename std::conditional<
          rs2 == Rs2::UNDEFINED,
          FieldSet<RegRd, RegRs1, RegRs2>,
          FieldSet<RegRd, RegRs1>
          >::type
      >::type;

  struct Fields : public ResolveFields {};
};

template <typename InstrImpl, OpcodeID opcodeID, Fmt fmt>
struct F_R4 : public RV_Instruction<InstrImpl> {

  using ResolveOpcode =
      OpcodeSet<
          OpPartOpcode<opcodeID>,
          OpPartFmt<static_cast<unsigned>(fmt)>
          >;

  struct Opcode : public ResolveOpcode {};

  struct Fields : FieldSet<RegRd, RegRm, RegRs1, RegRs2, RegRs3> {};
};


// Aritméticas
struct Fadd_s : public F_R<
                    Fadd_s,
                    OpcodeID::FP,
                    Fmt::S,
                    Funct5::FADD
                    > {
  constexpr static std::string_view NAME = "fadd.s";
};

struct Fsub_s : public F_R<
                    Fsub_s,
                    OpcodeID::FP,
                    Fmt::S,
                    Funct5::FSUB
                    > {
  constexpr static std::string_view NAME = "fsub.s";
};

struct Fmul_s : public F_R<
                    Fmul_s,
                    OpcodeID::FP,
                    Fmt::S,
                    Funct5::FMUL
                    > {
  constexpr static std::string_view NAME = "fmul.s";
};

struct Fdiv_s : public F_R<
                    Fdiv_s,
                    OpcodeID::FP,
                    Fmt::S,
                    Funct5::FDIV
                    > {
  constexpr static std::string_view NAME = "fdiv.s";
};

struct Fsqrt_s : public F_R<
                     Fsqrt_s,
                     OpcodeID::FP,
                     Fmt::S,
                     Funct5::FSQRT
                     > {
  constexpr static std::string_view NAME = "fsqrt.s";
};

// Min-Max
struct Fmin_s : public F_R<
                    Fmin_s,
                    OpcodeID::FP,
                    Fmt::S,
                    Funct5::FMINMAX,
                    Rm::RNE
                    > {
  constexpr static std::string_view NAME = "fmin.s";
};

struct Fmax_s : public F_R<
                    Fmax_s,
                    OpcodeID::FP,
                    Fmt::S,
                    Funct5::FMINMAX,
                    Rm::RTZ
                    > {
  constexpr static std::string_view NAME = "fmax.s";
};

// Comparaciones
struct Feq_s : public F_R<
                   Feq_s,
                   OpcodeID::FP,
                   Fmt::S,
                   Funct5::FEQ,
                   Rm::RUP
                   > {
  constexpr static std::string_view NAME = "feq.s";
};

struct Flt_s : public F_R<
                   Flt_s,
                   OpcodeID::FP,
                   Fmt::S,
                   Funct5::FLT,
                   Rm::RTZ
                   > {
  constexpr static std::string_view NAME = "flt.s";
};

struct Fle_s : public F_R<
                   Fle_s,
                   OpcodeID::FP,
                   Fmt::S,
                   Funct5::FLE,
                   Rm::RDN
                   > {
  constexpr static std::string_view NAME = "fle.s";
};

// Conversión
struct Fcvt_w_s : public F_R<
                      Fcvt_w_s,
                      OpcodeID::FP,
                      Fmt::S,
                      Funct5::FCVT_W_S,
                      Rm::UNDEFINED,
                      Rs2::ZERO
                      > {
  constexpr static std::string_view NAME = "fcvt.w.s";
};

struct Fcvt_wu_s : public F_R<
                       Fcvt_wu_s,
                       OpcodeID::FP,
                       Fmt::S,
                       Funct5::FCVT_WU_S,
                       Rm::UNDEFINED,
                       Rs2::ONE
                       > {
  constexpr static std::string_view NAME = "fcvt.wu.s";
};

struct Fcvt_s_w_s : public F_R<
                        Fcvt_s_w_s,
                        OpcodeID::FP,
                        Fmt::S,
                        Funct5::FCVT_S_W,
                        Rm::UNDEFINED,
                        Rs2::ZERO
                        > {
  constexpr static std::string_view NAME = "fcvt.s.w";
};

struct Fcvt_s_wu_s : public F_R<
                         Fcvt_s_wu_s,
                         OpcodeID::FP,
                         Fmt::S,
                         Funct5::FCVT_S_WU,
                         Rm::UNDEFINED,
                         Rs2::ONE
                         > {
  constexpr static std::string_view NAME = "fcvt.s.wu";
};

// Clasificación y Movimiento
struct Fclass_s : public F_R<
                      Fclass_s,
                      OpcodeID::FP,
                      Fmt::S,
                      Funct5::FCLASS,
                      Rm::RTZ
                      > {
  constexpr static std::string_view NAME = "fclass.s";
};

struct Fmv_x_w_s : public F_R<
                       Fmv_x_w_s,
                       OpcodeID::FP,
                       Fmt::S,
                       Funct5::FMV_X_W,
                       Rm::RNE
                       > {
  constexpr static std::string_view NAME = "fmv.x.w";
};

struct Fmv_w_x_s : public F_R<
                       Fmv_w_x_s,
                       OpcodeID::FP,
                       Fmt::S,
                       Funct5::FMV_W_X,
                       Rm::RNE
                       > {
  constexpr static std::string_view NAME = "fmv.w.x";
};

// Signo
struct Fsgnj_s : public F_R<
                     Fsgnj_s,
                     OpcodeID::FP,
                     Fmt::S,
                     Funct5::FSGNJ,
                     Rm::RNE
                     > {
  constexpr static std::string_view NAME = "fsgnj.s";
};

struct Fsgnjn_s : public F_R<
                      Fsgnjn_s,
                      OpcodeID::FP,
                      Fmt::S,
                      Funct5::FSGNJ,
                      Rm::RTZ
                      > {
  constexpr static std::string_view NAME = "fsgnjn.s";
};

struct Fsgnjx_s : public F_R<
                      Fsgnjx_s,
                      OpcodeID::FP,
                      Fmt::S,
                      Funct5::FSGNJ,
                      Rm::RDN
                      > {
  constexpr static std::string_view NAME = "fsgnjx.s";
};

// FMA (R4)
struct Fmadd_s : public F_R4<
                     Fmadd_s,
                     OpcodeID::FP_MADD,
                     Fmt::S
                     > {
  constexpr static std::string_view NAME = "fmadd.s";
};

struct Fmsub_s : public F_R4<
                     Fmsub_s,
                     OpcodeID::FP_MSUB,
                     Fmt::S
                     > {
  constexpr static std::string_view NAME = "fmsub.s";
};

struct Fnmadd_s : public F_R4<
                      Fnmadd_s,
                      OpcodeID::FP_NMADD,
                      Fmt::S
                      > {
  constexpr static std::string_view NAME = "fnmadd.s";
};

struct Fnmsub_s : public F_R4<
                      Fnmsub_s,
                      OpcodeID::FP_NMSUB,
                      Fmt::S
                      > {
  constexpr static std::string_view NAME = "fnmsub.s";
};

// Carga y Almacenamiento
struct Flw_s : public F_Lw<
                   Flw_s,
                   OpcodeID::FP_LW,
                   Rm::RDN
                   > {
  constexpr static std::string_view NAME = "flw";
};

struct Fsw_s : public F_Sw<
                   Fsw_s,
                   OpcodeID::FP_SW,
                   Rm::RDN
                   > {
  constexpr static std::string_view NAME = "fsw";
};

//void enableExt(const ISAInfoBase *isa, InstrVec &instructions, PseudoInstrVec &pseudoInstructions);

} // namespace ExtF
} // namespace RVISA
} // namespace Ripes

