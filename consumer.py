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
        self._data = data
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
    callback = InterestCallback(interest, False)
    
    if callback._dumpData: dump("Express name ", interestToStr(interest))

    expressedInterests[interest] = {'callback':callback, 'express_time': time.time()}
    #expressedInterests[interest.getName().toUri()] = {'callback':callback, 'express_time': time.time()}
    face.expressInterest(interest, callback.onData, callback.onTimeout)

def interestToStr(i):
    iname = i.getName().toUri()
    if i.getMinSuffixComponents(): iname += " MinSuffixComponent: "+str(i.getMinSuffixComponents())
    if i.getMaxSuffixComponents(): iname += " MaxSuffixComponent: "+str(i.getMaxSuffixComponents())
    #if i.getKeyLocator(): iname += " KeyLocator.keyName: "+str(i.getKeyLocator().getKeyName())
    if i.getExclude():
        iname += " Exclude: " 
        for idx in range(0,i.getExclude().size()):
            if i.getExclude().get(idx).getComponent():
                iname += str(i.getExclude().get(idx).getComponent())+", "
            else:
                iname += "*"
    if i.getChildSelector(): iname += " ChildSelector: "+str(i.getChildSelector())
    return iname

def printResults():
    idx = 0
    for k,v in expressedInterests.iteritems():
        iname = interestToStr(k)
        processTime = (v['callback']._callbackTimestamp - v['express_time'])*1000
        status = "timeout" if v['callback']._timeOut else "data"
        print str(idx)+":\t{0:.2f}ms\t{1}\tinterest: {2}\tdata: {3}".format(processTime, status, iname, v['callback']._data.getName())
        idx += 1

def main():
    global expressedInterests
    face = Face("localhost")

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
    #express(face, Interest(Name("/repo/A/1")))
    
    # /repo/A/1
    # /repo/A/2
    # /repo/A/3
    # /repo/A/4
    # /repo/A/5
    # /repo/A/6
    # /repo/A/7
    # /repo/A/8
    # /repo/B/1
    # /repo/B/2
    # /repo/B/3
    # /repo/A/9/A/B/C
    # /repo/A/9/A/B
    # /repo/A/9/A
    # /repo/C/1             -- keylocator keyname == keyname1
    # /repo/C/2             -- keylocator keyname == keyname2


    # - MinSuffixComponent
    i = Interest(Name("/repo/A"))
    i.setMinSuffixComponents(2)
    express(face, i)

    # - MaxSuffixComponent
    i = Interest(Name("/repo/A/1"))
    i.setMaxSuffixComponents(2)
    express(face, i)

    # - PublisherPublicKeyLocator
    # i = Interest(Name("/repo/C"))
    # i.getKeyLocator().setKeyName("/keyname")
    # express(face,i)

    # - Exclude-one
    i = Interest(Name("/repo/A"))
    i.getExclude().appendComponent("1")
    express(face, i)

    # - Exclude-before
    i = Interest(Name("/repo/A"))
    i.getExclude().appendAny().appendComponent("5")
    express(face, i)

    # - Exclude-between
    i = Interest(Name("/repo/A"))
    i.getExclude().appendComponent("3").appendComponent("7")
    express(face, i)

    # - Exclude-after
    i = Interest(Name("/repo/A"))
    i.getExclude().appendComponent("5").appendAny()
    express(face, i)

    # - ChildSelector = leftmost
    i = Interest(Name("/repo/A"))
    i.setChildSelector(0)
    express(face, i)

    # - ChildSelector = rightmost
    i = Interest(Name("/repo/A"))
    i.setChildSelector(1)
    i.setMaxSuffixComponents(2)
    express(face, i)

    while firedCallbackNum < len(expressedInterests):
        face.processEvents()
        # We need to sleep for a few milliseconds so we don't use 100% of the CPU.
        time.sleep(0.01)

    face.shutdown()
    printResults()

main()