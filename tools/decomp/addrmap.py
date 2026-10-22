#!/usr/bin/env python3
"""Map Neutron2.exe addresses (functions and data) to the matching ones in another build of the
game's code (Neutron.exe, Boy Genius), so code reconstructed from one exe can be retargeted.

    addrmap.py A.exe A_functions.json A_classes.json B.exe B_functions.json B_classes.json out.json

Anchors: vtable slots of classes with the same name (same slot index), and functions whose
normalised code (immediates that are addresses masked) is identical and unique in both exes.
Then propagate: in every matched pair, align the normalised instruction streams (difflib) and
map the call targets and data addresses at aligned positions; repeat until nothing new.
Output: {"func": {a: [b, score]}, "data": {a: [b, votes]}} (hex strings), score = code similarity.
"""
import json, sys, difflib, collections
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM


class Exe:
    def __init__(self, path, funcs_json, classes_json):
        p = pefile.PE(path)
        self.base = p.OPTIONAL_HEADER.ImageBase
        self.img = p.get_memory_mapped_image()
        t = [s for s in p.sections if s.Name.startswith(b'.text')][0]
        self.tlo, self.thi = self.base + t.VirtualAddress, self.base + t.VirtualAddress + t.Misc_VirtualSize
        self.lo, self.hi = self.base + 0x1000, self.base + len(self.img)
        fj = json.load(open(funcs_json))
        if isinstance(fj, dict):
            self.funcs = {int(a, 16): v.get('size', 0) for a, v in fj.items()}
        else:
            self.funcs = {f['addr']: f.get('size', 0) for f in fj}
        self.classes = {c['name']: c for c in json.load(open(classes_json))['classes']}
        self.md = Cs(CS_ARCH_X86, CS_MODE_32); self.md.detail = True
        self.cache = {}

    def code(self, a):
        """[(norm, calltarget|None, [data addrs])] for the function at a."""
        if a in self.cache: return self.cache[a]
        size = self.funcs.get(a) or 0
        out = []
        if self.tlo <= a < self.thi:
            size = max(size, 1) if size else 4096
            buf = self.img[a - self.base:a - self.base + min(size, 65536)]
            for i in self.md.disasm(buf, a):
                call = None; data = []
                ops = []
                for o in i.operands:
                    if o.type == X86_OP_IMM:
                        v = o.imm & 0xffffffff
                        if self.lo <= v < self.hi:
                            if i.mnemonic in ('call', 'jmp') and self.tlo <= v < self.thi and i.mnemonic == 'call':
                                call = v; ops.append('F')
                            elif i.mnemonic.startswith('j') or i.mnemonic == 'jmp':
                                ops.append('J')
                            else:
                                data.append(v); ops.append('A')
                        else:
                            ops.append(hex(v))
                    elif o.type == X86_OP_MEM:
                        d = o.mem.disp & 0xffffffff
                        if o.mem.base == 0 and o.mem.index == 0 and self.lo <= d < self.hi:
                            data.append(d); ops.append('[A]')
                        elif o.mem.base == 0 and self.lo <= d < self.hi:
                            data.append(d); ops.append('[A+r]')
                        else:
                            ops.append('m%d' % o.mem.disp if abs(o.mem.disp) < 0x10000 else 'm?')
                    else:
                        ops.append(i.reg_name(o.reg) if o.type == 1 else '?')
                out.append((i.mnemonic + ' ' + ','.join(ops), call, data, i.address))
                if not self.funcs.get(a) and i.mnemonic == 'ret': break
        self.cache[a] = out
        return out


def main():
    A = Exe(*sys.argv[1:4]); B = Exe(*sys.argv[4:7]); outp = sys.argv[7]
    fmap = {}; score = {}; dvotes = collections.defaultdict(collections.Counter); imap = {}
    queue = []

    def add(a, b, s, why):
        if a in fmap or b in used: return
        fmap[a] = b; used.add(b); score[a] = (s, why); queue.append(a)
    used = set()

    def sim(a, b):
        x = [t[0] for t in A.code(a)]; y = [t[0] for t in B.code(b)]
        if not x or not y: return 0.0
        return difflib.SequenceMatcher(None, x, y, autojunk=False).ratio()

    # anchors 1: identical normalised code, unique on both sides (skip tiny functions)
    def key(e, a):
        c = e.code(a)
        return tuple(t[0] for t in c) if len(c) >= 6 else None
    ka = collections.defaultdict(list); kb = collections.defaultdict(list)
    for a in A.funcs: k = key(A, a); k and ka[k].append(a)
    for b in B.funcs: k = key(B, b); k and kb[k].append(b)
    for k, la in ka.items():
        lb = kb.get(k)
        if lb and len(la) == 1 and len(lb) == 1: add(la[0], lb[0], 1.0, 'hash')
    print('hash anchors', len(fmap), flush=True)
    # anchors 2: vtable slots of same-named classes (slots may be inserted: best match in a window)
    for name, ca in A.classes.items():
        cb = B.classes.get(name)
        if not cb: continue
        for va, vb in zip(ca['vtables'], cb['vtables']):
            mb = [int(m['addr'], 16) if not m['imported'] else None for m in vb['methods']]
            for i, ma in enumerate(va['methods']):
                if ma['imported']: continue
                a = int(ma['addr'], 16)
                if a in fmap: continue
                best = (0, None)
                for j in range(max(0, i - 16), min(len(mb), i + 17)):
                    b = mb[j]
                    if b is None or b in used: continue
                    sc = sim(a, b) - 0.002 * abs(i - j)
                    if sc > best[0]: best = (sc, b)
                if best[1] and best[0] >= (0.3 if best[1] == (mb[i] if i < len(mb) else None) else 0.55):
                    add(a, best[1], best[0], 'vtable ' + name)
    print('with vtables', len(fmap), flush=True)
    for x in sys.argv[8:]:                       # manual seeds a=b
        a, b = (int(v, 16) for v in x.split('='))
        add(a, b, sim(a, b), 'manual')

    def propagate():
        while queue:
            a = queue.pop(); b = fmap[a]
            ca, cb = A.code(a), B.code(b)
            sm = difflib.SequenceMatcher(None, [t[0] for t in ca], [t[0] for t in cb], autojunk=False)
            for tag, i1, i2, j1, j2 in sm.get_opcodes():
                if tag == 'equal' or (tag == 'replace' and i2 - i1 == j2 - j1):
                    for i, j in zip(range(i1, i2), range(j1, j2)):
                        ta, tb = ca[i], cb[j]
                        imap.setdefault(ta[3], tb[3])
                        if ta[1] and tb[1] and ta[1] not in fmap:
                            s = sim(ta[1], tb[1])
                            if s >= (0.4 if tag == 'equal' else 0.6): add(ta[1], tb[1], s, 'call from %x' % a)
                        if tag == 'equal' and len(ta[2]) == len(tb[2]):
                            for da, db in zip(ta[2], tb[2]): dvotes[da][db] += 1
    propagate()
    print('propagated', len(fmap), flush=True)
    # rounds: candidates for an unmatched function = B functions that use the mapped data / call the
    # mapped callees / are called by the mapped callers of it
    bdata = collections.defaultdict(set); bcall = collections.defaultdict(set); bcallers = collections.defaultdict(set)
    acallers = collections.defaultdict(set)
    for b in B.funcs:
        for t in B.code(b):
            for d in t[2]: bdata[d].add(b)
            if t[1]: bcall[t[1]].add(b); bcallers[t[1]].add(b)
    for a in A.funcs:
        for t in A.code(a):
            if t[1]: acallers[t[1]].add(a)
    for rnd in range(4):
        before = len(fmap)
        dmap = {da: c.most_common(1)[0][0] for da, c in dvotes.items()}
        for a in A.funcs:
            if a in fmap: continue
            ca = A.code(a)
            if len(ca) < 4: continue
            cand = collections.Counter()
            for t in ca:
                for d in t[2]:
                    if d in dmap and len(bdata[dmap[d]]) < 40:
                        for b in bdata[dmap[d]]: cand[b] += 1
                if t[1] in fmap and len(bcall[fmap[t[1]]]) < 40:
                    for b in bcall[fmap[t[1]]]: cand[b] += 1
            for caller in acallers[a]:
                if caller in fmap:
                    for t in B.code(fmap[caller]):
                        if t[1]: cand[t[1]] += 1
            best = (0, None)
            xa = [t[0] for t in ca]
            for b, n in cand.most_common(12):
                if b in used: continue
                xb = [t[0] for t in B.code(b)]
                if not xb or min(len(xa), len(xb)) / max(len(xa), len(xb)) < 0.5: continue
                sm = difflib.SequenceMatcher(None, xa, xb, autojunk=False)
                if sm.quick_ratio() < 0.6: continue
                r = sm.ratio()
                if r > best[0]: best = (r, b)
            if best[1] and best[0] >= 0.7: add(a, best[1], best[0], 'round %d' % rnd)
        propagate()
        print('round', rnd, len(fmap), flush=True)
        if len(fmap) == before: break
    print('propagated', len(fmap), 'of', len(A.funcs), flush=True)
    data = {}
    for da, c in dvotes.items():
        (db, n), = c.most_common(1)
        tot = sum(c.values())
        data[hex(da)] = [hex(db), n, round(n / tot, 2)]
    json.dump(dict(func={hex(a): [hex(b), round(score[a][0], 3), score[a][1]] for a, b in sorted(fmap.items())},
                   data=dict(sorted(data.items())),
                   insn={hex(a): hex(b) for a, b in sorted(imap.items())}), open(outp, 'w'), indent=0)
    print('data addresses', len(data))


if __name__ == '__main__':
    main()
