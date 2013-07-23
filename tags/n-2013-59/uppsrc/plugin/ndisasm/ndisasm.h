#ifndef _plugin_ndisasm_ndisasm_h_
#define _plugin_ndisasm_ndisasm_h_

NAMESPACE_UPP

static const int NDISASM_INSN_MAX = 32;     // one instruction can't be longer than this
static const int NDISASM_OUTBUF_SIZE = 256; // maximum length of disassembly output

int NDisassemble(char *output, const byte *data, long offset, bool x64 = false); // returns instruction length

END_UPP_NAMESPACE

#endif
