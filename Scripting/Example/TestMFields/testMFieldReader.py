if(osg.getName(self)):
    self.get_name = osg.getName(self)
else:
    self.get_name = 'unknown'

def init():
    pass

def shutdown():
    pass

def frame(timeStamp, frameCount):
    if(False):
        print('Reader: mMFInt32 = ' + str(_fieldAccessHandler.myGetMField("mMFInt32", int(0))))
        print('Reader: mMFVec3f = ' + str(_fieldAccessHandler.myGetMField("mMFVec3f", osg.Vec3f())))
        print('Reader:      [3] = ' + str(_fieldAccessHandler.myGetMField("mMFVec3f", osg.Vec3f())[3]))

    print('Reader: mMFInt32 = ' + str(self.get_mMFInt32()))
    print('Reader: mMFVec3f = ' + str(self.get_mMFVec3f()))
    print('Reader:      [3] = ' + str(self.get_mMFVec3f()[3]))

def changed(whichField, origin, details):
    pass