# Takes n byte stream of k sized byte blocks, pads with PKCS#7
from conv import *

def pad(stream, k):
   b = bytearray()
   topad = k - (len(stream) % k)
   for i in range(0,topad):
      stream = stream + chr(topad)
   b.extend(stream)
   return b

def unpad(stream, k):
   length = len(stream)
   if (length % k) != 0:
      print "invalid block size"
      return ''
   c1 = stream[length - 1]
   size = 1
   for i in range(1,k):
      c2 = stream[length - 1 - i]
      if c1 != c2:
         break
      size += 1
   if size != ord(c1):
#      print "invalid padding"
      return ''
   return str(stream [:(length - size)])
