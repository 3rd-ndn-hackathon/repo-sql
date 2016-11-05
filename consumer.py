import time
from collections import OrderedDict
from pyndn import Name
from pyndn import Interest
from pyndn import Face

def dump(*list):
    result = ""
    for element in list:
        result += (element if type(element) is str else str(element)) + " "
    print(result)

# class Counter(object):
#     def __init__(self):
#         self._callbackCount = 0

#     def onData(self, interest, data):
#         self._callbackCount += 1
#         dump("Got data packet with name", data.getName().toUri())
#         # Use join to convert each byte to chr.
#         dump(data.getContent().toRawStr())

#     def onTimeout(self, interest):
#         self._callbackCount += 1
#         dump("Time out for interest", interest.getName().toUri())

expressedInterests = OrderedDict()
firedCallbackNum = 0

class InterestCallback(object):
    def __init__(self, interest, dumpData = False):
        self._callbackTimestamp = 0
        self._dumpData = dumpData
        self._interest = interest
        self._fired = False
        self._timeOut = False

    def onData(self, interest, data):
        self.fired()
        if self._dumpData:
            dump("Got data packet with name", data.getName().toUri())
            dump(data.getContent().toRawStr())
        
    def onTimeout(self, interest):
        self.fired()
        self._timeOut = True
        if self._dumpData:
            dump("Time out for interest", interest.getName().toUri())

    def fired(self):
        global firedCallbackNum
        firedCallbackNum += 1
        self._fired = True
        self._callbackTimestamp = time.time()

def express(face, interest):
    global expressedInterests
    dump("Express name ", interest.getName().toUri())
    callback = InterestCallback(interest, False)

    expressedInterests[interest] = {'callback':callback, 'express_time': time.time()}
    #expressedInterests[interest.getName().toUri()] = {'callback':callback, 'express_time': time.time()}
    face.expressInterest(interest, callback.onData, callback.onTimeout)

def main():
    global expressedInterests
    face = Face("aleph.ndn.ucla.edu")

    # trying different interests
    # - simple
    # - MinSuffixComponent
    # - MaxSuffixComponent
    # - PublisherPublicKeyLocator
    # - Exclude-one
    # - Exclude-before
    # - Exclude-between
    # - Exclude-after
    # - ChildSelector = leftmost
    # - ChildSelector = rightmost

    # simple
    express(face, Interest(Name("/repo/A/1")))
    
    # - MinSuffixComponent
    i = Interest(Name("/repo/A"))
    i.setMinSuffixComponents(1)
    express(face, i)

    # - MaxSuffixComponent
    i = Interest(Name("/repo/A/1"))
    i.setMaxSuffixComponents(2)
    express(face, i)

    # - PublisherPublicKeyLocator


    # - Exclude-one
    i = Interest(Name("/A"))
    
    # - Exclude-before
    # - Exclude-between
    # - Exclude-after
    # - ChildSelector = leftmost
    # - ChildSelector = rightmost


    while firedCallbackNum < len(expressedInterests):
        face.processEvents()
        # We need to sleep for a few milliseconds so we don't use 100% of the CPU.
        time.sleep(0.01)

    face.shutdown()

    for k,v in expressedInterests.iteritems():
        iname = k.getName().toUri()
        processTime = (v['callback']._callbackTimestamp - v['express_time'])*1000
        status = "timeout" if v['callback']._timeOut else "data"
        print "{0}\t{1:.2f}ms\t{2}".format(iname, processTime, status)

main()