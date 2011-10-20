class FieldAccessTestBase:
    """
    Base class for testing the integrity of pointer and value fields when
    accessing it with the access methods provided in the PyFieldAccessHandler
    class.
    """
    def __init__(self, fieldname, accesshandler):
        self.fieldname     = fieldname
        self.accesshandler = accesshandler
        
        self.refcounted_items = {}
        
    def get_fieldname(self):
        return self.fieldname
    
    def addRefCount(self, item):
        if item in self.refcounted_items.keys():
            self.refcounted_items[item] += 1
            print('existing refcount: ' + str(self.refcounted_items[item]))
        else:
            self.refcounted_items[item] = item.getRefCount()
            print('new refcount: ' + str(self.refcounted_items[item]))
            
    def subRefCount(self, item):
        self.refcounted_items[item] -= 1
        
    def getRefCount(self, item):
        return self.refcounted_items[item]
