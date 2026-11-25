"""Builds FOXBG.8xv: 4 living-room backgrounds (morning, day, evening, night), 128-color each."""
import sys
from PIL import Image
def var8xv(name,payload,archived=True):
 vd=len(payload).to_bytes(2,'little')+payload
 entry=b'\x0d\x00'+len(vd).to_bytes(2,'little')+b'\x15'+name.encode().ljust(8,b'\0')+b'\x00'+(b'\x80' if archived else b'\x00')+len(vd).to_bytes(2,'little')+vd
 return b'**TI83F*\x1a\x0a\x00'+b'Fox Home backgrounds'.ljust(42,b'\0')+len(entry).to_bytes(2,'little')+entry+(sum(entry)&0xFFFF).to_bytes(2,'little')
def main(out,files):
 pay=bytearray(b'FBG1')
 for f in files:
  im=Image.open(f).convert('RGB')
  assert im.size==(160,55),im.size
  q=im.quantize(colors=128,method=Image.Quantize.MEDIANCUT,dither=Image.Dither.NONE)
  pal=q.getpalette()[:128*3];pal+= [0]*(128*3-len(pal))
  pay+=bytes(pal)+bytes([160,55])+bytes(128+p for p in q.tobytes())
 assert len(pay)<65000
 open(out,'wb').write(var8xv('FOXBG',bytes(pay)));print(out,len(pay),'bytes')
if __name__=='__main__':main(sys.argv[1],sys.argv[2:])
