#!/usr/bin/python
import Image, sys, ImageFilter, math, struct, array

def usage():
    print "Usage: "+sys.argv[0]+" heightfield.png out.png"
    sys.exit(0)

if len(sys.argv)!=3:
    usage();
try:
    hf = Image.open(sys.argv[1])
except IOError:
    print "Can't open file: "+sys.argv[1]
    sys.exit(-1)

if hf.mode!="L":
    print "heightfield must be a 8bit greyscale image."
    sys.exit(-1)

#dx = hf.filter(ImageFilter.Kernel((3,3), [-1,0,1, -2,0,2, -1,0,1], 1))
#dy = hf.filter(ImageFilter.Kernel((3,3), [-1,-2,-1, 0,0,0, 1,2,1], 1))

#dxdata = dx.getdata()
#dydata = dy.getdata()
hfdata = hf.getdata()
result = array.array("B")
oneover255 = 1.0/255.0
# for i in xrange(0, len(dxdata)):
#     x = float(dxdata[i])*oneover255
#     y = float(dydata[i])*oneover255
#     c = 1.0/math.sqrt(x*x+y*y+1)
#     ##print math.sqrt(x*c*x*c+y*c*y*c+c*c)
#     x = 128+127*x*c
#     y = 128+127*y*c
#     z = 128+127*c
#     result.append(int(round(x)))
#     result.append(int(round(y)))
#     result.append(int(round(c)));
w,h = hf.size
for j in xrange(h):
    for i in xrange(w):
        c = float(hfdata[i+j*w])*oneover255
        cx = float(hfdata[(i+1)%w+j*w])*oneover255
        cy = float(hfdata[((j+1)%h)*w+i])*oneover255
        dcx = c-cx
        dcy = c-cy
        norm = 1.0/math.sqrt(dcx*dcx+dcy*dcy+1)
    ##print math.sqrt(x*c*x*c+y*c*y*c+c*c)
        nx = 128+127*dcy*norm
        ny = 128-127*dcx*norm
        nz = 128+127*norm
        result.append(int(round(nx)))
        result.append(int(round(ny)))
        result.append(int(round(nz)));

im = Image.frombuffer("RGB", hf.size, result)
im.save(sys.argv[2], "PNG")
#im.show()
