import time
from math import sin

if(osg.getName(self)):
    self.get_name = osg.getName(self)
else:
    self.get_name = 'unknown'

def init():
    pass	

def shutdown():
    pass

def frame(timeStamp, frameCount):

    ts = timeStamp;

    self.set_mSFInt32(frameCount)
    #self.set_mSFFloat(timeStamp)
    self.set_mSFDouble(timeStamp)
    self.set_mSFString('mystring ' + str(frameCount))
    if frameCount%2 == 0:
        self.set_mSFBool(False)
    else:
        self.set_mSFBool(True)

    self.set_mSFPnt2f(osg.Pnt2f(ts,ts))
    self.set_mSFPnt3f(osg.Pnt3f(ts,ts,ts))
    self.set_mSFPnt4f(osg.Pnt4f(ts,ts,ts,ts))

    self.set_mSFVec2f(osg.Vec2f(ts,ts))
    self.set_mSFVec2d(osg.Vec2d(ts,ts))
    self.set_mSFVec3f(osg.Vec3f(ts,ts,ts))
    self.set_mSFVec3d(osg.Vec3d(ts,ts,ts))
    self.set_mSFVec4f(osg.Vec4f(ts,ts,ts,ts))
    self.set_mSFVec4d(osg.Vec4d(ts,ts,ts,ts))

    self.set_mSFMatrix  (osg.Matrix  (ts,ts,ts,ts,ts,ts,ts,ts,ts,ts,ts,ts,ts,ts,ts,ts))
    self.set_mSFMatrix4d(osg.Matrix4d(ts,ts,ts,ts,ts,ts,ts,ts,ts,ts,ts,ts,ts,ts,ts,ts))

    self.set_mSFRotation(osg.Quaternion(osg.Vec3f(ts,ts,ts), ts))

    self.set_mSFColor(osg.Color3f(ts,ts,ts))
    self.set_mSFColorRGBA(osg.Color4f(ts,ts,ts,1))

    #self.get_mSFPlane(osg.Plane(frameCount, frameCount+1, frameCount+2)
    self.set_mSFVolume (osg.BoxVolume(osg.Vec3f(ts,ts,ts), osg.Vec3f(ts,ts,ts)))

    self.set_mSFTime(ts)

def changed(whichField, origin, details):
    pass