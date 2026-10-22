#!/usr/bin/env python3
"""Renumber `// SLOT n` comments (CGameObject-family vtable slots) from Negatron's numbering to
Neutron.exe's (tools/decomp/go_bg2n2.json: BG slot -> N2 slot). A slot Neutron.exe does not have
becomes `// N2SLOT n` (Negatron only). Names stay as they are.

    n2slots_to_bg.py files...         (edits in place)
    n2slots_to_bg.py --names out.json game/src   (BG slot -> {"name": tree name}, for gen_class.py --names)
"""
import json, os, re, sys
m = json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'go_bg2n2.json')))
inv = {v: int(k) for k, v in m.items() if v is not None}
def fix(t):
    def r(x):
        n = int(x.group(2))
        if n in inv: return '%sSLOT %d%s' % (x.group(1), inv[n], ' ' * max(0, len(x.group(2)) - len(str(inv[n]))))
        return '%sN2SLOT %d' % (x.group(1), n)
    return re.sub(r'(//\s*)SLOT\s+(\d+)', r, t)
def names(src):
    """BG slot -> {"name": the tree's name of the matching Negatron slot} (gen_class.py --names)."""
    import glob
    n2 = {}
    for f in [src + '/CGameObject.h', src + '/CLocalGameObject.h', src + '/gen/CGameObject_slots.inc', src + '/gen/CLocalGameObject_slots.inc']:
        if not os.path.exists(f): continue
        for l in open(f):
            x = re.search(r'virtual\s+[^;{]*?\b(\w+)\s*\([^)]*\)[^\n]*//\s*(?:N2)?(?:SLOT|slot)\s+(\d+)', l)
            if x:
                n = int(x.group(2))
                # a renumbered header carries BG numbers: map them back
                if 'N2SLOT' not in l and '/gen/' not in f and RENUMBERED: n = m.get(str(n), n)
                n2.setdefault(n, x.group(1))
    out = {}
    for k, v in m.items():
        out[k] = {'name': n2.get(v, 'v%d' % v) if v is not None else 'bg_v%s' % k}
    return out
RENUMBERED = True
if __name__ == '__main__':
    if sys.argv[1] == '--names':
        json.dump(names(sys.argv[3]), open(sys.argv[2], 'w'), indent=0); sys.exit(0)
    for f in sys.argv[1:]:
        t = open(f).read(); u = fix(t)
        if u != t: open(f, 'w').write(u); print('renumbered', f)
