#!BPY
"""
Name: 'SCNexporter'
Blender: 233
Group: 'Export'
Tooltip: 'Export to some format.'
"""

import Blender
from Blender import NMesh, Material, Window, Draw, BGL
from struct import *

class MeshExporter:
    def __init__(self, filename):
        self.oFile = file(filename, 'wb')
        self.filename=filename
        self.uvFmt="ff"
        self.vertFmt="fff"
        self.normFmt="fff"
        self.faceFmt="III"
        self.colorFmt="I"
        self.uv={}
        self.faces=[]
        self.vertices=[]
        self.normals=[]

    def asciiz(self, s):
        return s+"\0"

    def writeChunkHead(self, name, size):
        self.oFile.write(self.asciiz(name))
        self.oFile.write(pack("I",size))        

    def copyNormals(self, mesh):
        for v in mesh.verts:
            self.normals.append((v.no[0], v.no[1], v.no[2]))

    def saveNormals(self, mesh):
        size = len(self.normals)*calcsize(self.normFmt)
        self.writeChunkHead("NORMALS", size);
        for n in self.normals:
            self.oFile.write(pack(self.normFmt, n[0], n[1], n[2]))

    def calcNumFaces(self, faces):
        num = 0
        for f in faces:
            if len(f.v)==4:
               num+=2
            else:
               num+=1
        return num

    def saveFaces(self, mesh):
        size = len(self.faces)*calcsize(self.faceFmt)
        self.writeChunkHead("INDICES", size)
        for f in self.faces:
            self.oFile.write(pack(self.faceFmt, f[0], f[1], f[2]))

    def addFace(self, face, i, j, k):
        v=face.v
        self.faces.append(([v[i].index, v[j].index, v[k].index]))

    def distinctUV(self, uv1, uv2):
        return (uv1[0] != uv2[0]) or (uv1[1] != uv2[1])

    #move blender's faces indices into self.faces
    #and dup vertices and normals when one vertex as more than 1 uv
    def copyFaces(self, mesh):
        doUV = len(mesh.faces[0].uv)>0
        lastIndex = len(self.vertices)
        for f in mesh.faces:
            indices = []
            for i in range(0, len(f.v)):
                index = f.v[i].index
                if doUV:
                    if not self.uv.has_key(index):
                        self.uv[index] = f.uv[i]
                    elif self.distinctUV(self.uv[index], f.uv[i]):
                        self.vertices.append(self.vertices[index])
                        self.normals.append(self.normals[index])
                        self.uv[lastIndex] = f.uv[i]
                        index = lastIndex
                        lastIndex = lastIndex+1
                indices.append(index)
            self.faces.append((indices[0], indices[1], indices[2]))
            if len(indices)==4: #Triangulate quads
                self.faces.append((indices[0], indices[2], indices[3]))

    def copyVertices(self, mesh):
        for v in mesh.verts:
            self.vertices.append((v.co[0], v.co[1], v.co[2]))

    def saveVertices(self, mesh):
        size = len(self.vertices)*calcsize(self.vertFmt)
        self.writeChunkHead("VERTICES", size);
        for v in self.vertices:
            self.oFile.write(pack(self.vertFmt, v[0], v[1], v[2]))
                
    #Save per vertex uv
    def saveUV(self, mesh):
        size = len(self.uv)*calcsize(self.uvFmt)
        if size!=0:
            self.writeChunkHead("UV", size);
            for i in xrange(0, len(self.uv)):
                uv = self.uv[i]
                self.oFile.write(pack(self.uvFmt, uv[0], -uv[1]))
                     
    def export(self, object):
        #self.oFile.write(self.asciiz(object.getName()))
        mesh=object.getData()
        self.copyVertices(mesh)
        self.copyNormals(mesh)        
        self.copyFaces(mesh)        
        self.saveVertices(mesh)
        self.saveNormals(mesh)
        self.saveFaces(mesh)
        self.saveUV(mesh)
        print "UV=", len(self.uv)
        print "Vertices=", len(self.vertices)
        print "Normals=", len(self.normals)
        print "Faces=", len(self.faces)
        print "Duped vertices: ", len(self.vertices)-len(mesh.verts)
        self.oFile.close()

#--------GUI---------------------------------------------------
objects = Blender.Object.GetSelected()

def fileSelected(filename):
    meshExporter = MeshExporter(filename)
    meshExporter.export(objects[0])

#--------Main---------------------------------------------------
#Export only if one mesh object is selected
if len(objects)==1 and objects[0].getType()=="Mesh": 
    Window.FileSelector(fileSelected, 'SAVE FILE', 'unnamed.scn')
