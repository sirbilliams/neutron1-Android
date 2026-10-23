# vt.py <Class> <Base|-> [decomp dir]: list the vtable slots <Class> overrides relative to <Base> (classes.json + raw/functions.json).
import json,sys
d=sys.argv[3] if len(sys.argv)>3 else '/home/claude/decomp1'
c=json.load(open(d+'/classes.json'))['classes']; fn=json.load(open(d+'/raw/functions.json'))
x=c[sys.argv[1]] if isinstance(c,dict) else [k for k in c if k['name']==sys.argv[1]][0]
base=sys.argv[2] if len(sys.argv)>2 and sys.argv[2]!='-' else None
bx=None
if base: bx=c[base] if isinstance(c,dict) else [k for k in c if k['name']==base][0]
for vi,vt in enumerate(x['vtables']):
  bm=[]
  if bx:
    for bv in bx['vtables']:
      if bv['this_offset']==vt['this_offset']: bm=bv['methods']
  out=[]
  for i,m in enumerate(vt['methods']):
    a=m['addr']; 
    if i<len(bm) and bm[i]['addr']==a: continue
    out.append('%d:%s:%s'%(i,a,fn.get(a,{}).get('name','?')))
  print(vt['addr'],vt['this_offset'],len(vt['methods']),' '.join(out))
