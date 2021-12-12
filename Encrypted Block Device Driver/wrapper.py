from Crypto.Cipher import AES
from Crypto.Hash import SHA
from Crypto.Util import number
from padding import *
from conv import *
import os
import random

def setup():
   os.system("make > /dev/null 2>&1")
   return

def readfile(key, upmac, filemac):
   filename = asciitohex(filemac)[:16]
   with open(".script_tmp", "w") as f:
      f.write("1\nopenrw\n2\nread\n" + filename + "\n1\nclose\n")
   os.system('./libDisk .script_tmp > /dev/null 2>&1')
   os.system('rm .script_tmp > /dev/null 2>&1')

   if(os.path.isfile('.out' + filename)):
      data = ''
      with open(".out" + filename, "r") as f:
         iv1 = f.read(16)
         c2 = f.read(32)
         cipher = AES.new(bytes(upmac), AES.MODE_CBC, iv1)
         c1 = cipher.decrypt(c2)
         c1 += f.read()
      cipher = AES.new(bytes(key), AES.MODE_CBC, c1[:16])
      data = cipher.decrypt(c1[16:])
      ans = bytes(unpad(data, 16))
      if(len(ans) == 0):
         ans = '<no data>'
   else:
      ans = '<file not found>'
   return ans;


def writefile(key, upmac, filemac, local):
   filename = asciitohex(filemac)[:16]
   with open(local, "r") as f:
      data = f.read()
   pdata = pad(data, 16)
   iv1 = os.urandom(16)
   iv2 = os.urandom(16)
   cipher = AES.new(bytes(key), AES.MODE_CBC, iv1)
   enc = iv1 + cipher.encrypt(bytes(pdata))
   cipher = AES.new(bytes(upmac), AES.MODE_CBC, iv2)
   enc2 = iv2 + cipher.encrypt(enc[:32])
   enc = enc2 + enc[32:]
   
   with open('.tempy', 'w') as f:
      f.write(enc)

   with open(".script_tmp", "w") as f:
      f.write("1\nopenrw\n3\nwrite\n" + filename + "\n.tempy\n1\nclose\n")
   os.system('./libDisk .script_tmp > /dev/null 2>&1')
   os.system('rm .script_tmp .tempy > /dev/null 2>&1')
   return

def close():
   os.system('rm .script_tmp .out* .data_tmp > /dev/null 2>&1')
   return

def getmac(string, length):
   ans = bytearray(length)

   h = SHA.new()
   h.update(bytes(string))
   d = h.digest()

   for i in range(0, length):
      ans[i] = d[i + (20 - length)]

   return ans
