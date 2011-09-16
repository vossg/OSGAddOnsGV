import time
from math import sin, floor

if(osg.getName(self)):
    self.get_name = osg.getName(self)
else:
    self.get_name = 'unknown'

def init():
    pass	

def shutdown():
    pass

def frame(timeStamp, frameCount):
    ts = int(floor(timeStamp))

    new_vec_data = [ osg.Vec3f(0,0,ts), osg.Vec3f(0,0,ts), \
                     osg.Vec3f(0,0,ts), osg.Vec3f(0,0,ts), \
                     osg.Vec3f(0,0,ts), osg.Vec3f(0,0,ts), \
                     osg.Vec3f(0,0,ts), osg.Vec3f(0,0,ts), \
                     osg.Vec3f(0,0,ts), osg.Vec3f(0,0,ts) ]
    
    if(False):
        _fieldAccessHandler.setMField('mMFVec3f', new_vec_data, osg.Vec3f())
        
    self.set_mMFVec3f(new_vec_data)

    new_int_data = [ ts, ts, ts, ts, ts, ts ]
    
    if(False):
        _fieldAccessHandler.setMField('mMFInt32', new_int_data, int(0))
    self.set_mMFInt32(new_int_data)

    #node = self.getPointerSField('mSFNode', osg.Node.create())
    #print('mSFNode: ' + osg.getName(node))

def changed(whichField, origin, details):
    pass
