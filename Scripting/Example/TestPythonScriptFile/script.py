from math import sin

def init():
    pass	

def frame(timeStamp, frameCount):
    z   = sin(timeStamp)
    self.edit_pos().setValues(0,0,z)

def changed(whichField, origin, details):
    pass
    #if(whichField & self.posFieldMask):
    #    print('Changed pos field')

def shutdown():
    pass