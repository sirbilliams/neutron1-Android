#!/usr/bin/env python3
"""Rewrite the Neutron2.exe addresses in source files to Neutron.exe ones using addrmap.json.

    retarget.py addrmap.json report.tsv file...

Six-digit addresses (0x4xxxxx..0x6xxxxx, in code and comments) and data-macro names (g_xxxxxx)
are mapped: function starts, then instruction addresses, then data addresses. A data address
that isn't mapped itself takes the offset of mapped neighbours when the nearest mapped address
below and above it (both within 0x80) moved by the same amount (fields of one record).
Anything still unmapped is marked (`N2:0x4xxxxx`, `g_N2_xxxxxx`: a Neutron2.exe address to resolve by hand) and listed in report.tsv (file, line, address, kind),
as is every mapped function whose code was not an exact match (score < 1: check by hand).
"""
import json, re, sys, bisect

m = json.load(open(sys.argv[1]))
func = {int(a, 16): (int(v[0], 16), v[1]) for a, v in m['func'].items()}
insn = {int(a, 16): int(b, 16) for a, b in m['insn'].items()}
data = {int(a, 16): int(v[0], 16) for a, v in m['data'].items() if v[2] >= 0.6}
dkeys = sorted(data)
CODE = (0x401000, 0x4ca000)


def mapdata(a):
    if a in data: return data[a], 'data'
    i = bisect.bisect_right(dkeys, a)
    lo = dkeys[i - 1] if i else None; hi = dkeys[i] if i < len(dkeys) else None
    if lo is not None and hi is not None and a - lo <= 0x80 and hi - a <= 0x80 and data[lo] - lo == data[hi] - hi:
        return a + data[lo] - lo, 'data-near'
    return None, None


def mapaddr(a):
    if CODE[0] <= a < CODE[1]:
        if a in func: return func[a][0], 'func' if func[a][1] >= 1 else 'func~%.2f' % func[a][1]
        if a in insn: return insn[a], 'insn'
        return None, 'code'
    b, k = mapdata(a)
    return b, k or ('rdata' if a < 0x532000 else 'data')


rep = open(sys.argv[2], 'w')
tot = {}
for path in sys.argv[3:]:
    src = open(path, errors='replace').read()
    lines = src.split('\n')
    out = []
    for ln, line in enumerate(lines, 1):
        def sub(mo):
            tok = mo.group(0)
            pre = 'g_' if tok.startswith('g_') else '0x'
            a = int(tok[2:], 16)
            if not (0x401000 <= a < 0x6f9000): return tok
            b, k = mapaddr(a)
            tot[k] = tot.get(k, 0) + 1
            if b is None:
                rep.write(f'{path}\t{ln}\t{tok}\tunmapped-{k}\n')
                return f'g_N2_{a:06x}' if pre == 'g_' else f'N2:{tok}'
            if k.startswith('func~'): rep.write(f'{path}\t{ln}\t{tok}->{pre}{b:06x}\tcheck {k}\n')
            return f'{pre}{b:06x}'
        out.append(re.sub(r'\b(?:0x|g_)[4-6][0-9a-f]{5}\b', sub, line))
    new = '\n'.join(out)
    if new != src: open(path, 'w').write(new)
print(tot)
