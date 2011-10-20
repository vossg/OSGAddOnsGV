import time
from math import sin, floor

if(osg.getName(self)):
    self.get_name = osg.getName(self)
else:
    self.get_name = 'unknown'

def init():
    print('davor')
    #_fieldAccessHandler.testPointerMField()
    print('danach')
    pass	

def shutdown():
    pass

def frame(timeStamp, frameCount):
    return
    ts = int(floor(timeStamp))

    #
    # mMFVec3f:
    #
    
    if(False):
        new_vec_data = [ osg.Vec3f(0,0,ts), osg.Vec3f(0,0,ts), \
                         osg.Vec3f(0,0,ts), osg.Vec3f(0,0,ts), \
                         osg.Vec3f(0,0,ts), osg.Vec3f(0,0,ts), \
                         osg.Vec3f(0,0,ts), osg.Vec3f(0,0,ts), \
                         osg.Vec3f(0,0,ts), osg.Vec3f(0,0,ts) ]
        
        if(False):
            _fieldAccessHandler.setMField('mMFVec3f', new_vec_data, osg.Vec3f())
        self.set_mMFVec3f(new_vec_data)
    
        #
        # mMFInt32:
        #
        
        new_int_data = [ ts, ts, ts, ts, ts, ts ]
        
        if(False):
            _fieldAccessHandler.setMField('mMFInt32', new_int_data, int(0))
        self.set_mMFInt32(new_int_data)

    #
    # mMFNode:
    #
    
    self.set_mSFNode(self.findNamedComponent('ModelRoot'))
    
    self.node0 = osg.Node.create()
    osg.setName(self.node0, str(ts))
    self.node1 = osg.Node.create()
    osg.setName(self.node1, str(ts))
    
    new_node_data = [ self.node0, self.node1 ]
    #_fieldAccessHandler.setPointerMField('mMFNode', new_node_data, self.node0)
    self.set_mMFNode(new_node_data)
    
    #node = self.getPointerSField('mSFNode', osg.Node.create())
    #print('mSFNode: ' + osg.getName(node))

def changed(whichField, origin, details):
    pass
