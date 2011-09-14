if(osg.getName(self)):
    self.get_name = osg.getName(self)
else:
    self.get_name = 'unknown'

def init():
    pass

def shutdown():
    pass

def frame(timeStamp, frameCount):
    print('Reader: mSFInt32  = ' + str(self.get_mSFInt32()))
    print('Reader: mSFDouble = ' + str(self.get_mSFDouble()))
    print('Reader: mSFString = ' + str(self.get_mSFString()))
    print('Reader: mSFBool   = ' + str(self.get_mSFBool()))

    print('Reader: mSFPnt2f = ' + str(self.get_mSFPnt2f()))
    print('Reader: mSFPnt3f = ' + str(self.get_mSFPnt3f()))
    print('Reader: mSFPnt4f = ' + str(self.get_mSFPnt4f()))

    print('Reader: mSFVec2f = ' + str(self.get_mSFVec2f()))
    print('Reader: mSFVec2d = ' + str(self.get_mSFVec2d()))
    print('Reader: mSFVec3f = ' + str(self.get_mSFVec3f()))
    print('Reader: mSFVec3d = ' + str(self.get_mSFVec3d()))
    print('Reader: mSFVec4f = ' + str(self.get_mSFVec4f()))
    print('Reader: mSFVec4d = ' + str(self.get_mSFVec4d()))

    print('Reader: mSFMatrix   = ' + str(self.get_mSFMatrix()))
    print('Reader: mSFMatrix4d = ' + str(self.get_mSFMatrix4d()))

    print('Reader: mSFRotation = ' + str(self.get_mSFRotation()))

    print('Reader: mSFColor = ' + str(self.get_mSFColor()))
    print('Reader: mSFColorRGBA = ' + str(self.get_mSFColorRGBA()))

    #print('Reader: mSFPlane  = ' + str(self.get_mSFPlane())
    print('Reader: mSFVolume.getMin() = ' + str(self.get_mSFVolume().getMin()))

    print ('Reader: mSFTime = ' + str(self.get_mSFTime()))

def changed(whichField, origin, details):
    pass