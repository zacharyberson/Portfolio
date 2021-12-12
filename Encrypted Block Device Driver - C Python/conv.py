import base64

def int32(x):
   return int(0xFFFFFFFF & int(x))

def asciitob64(string):
   encoded = base64.b64encode(string)
   return encoded

def b64toascii(string):
   decoded = base64.b64decode(string)
   return decoded

def asciitohex(string):
   encoded = base64.b16encode(string)
   return encoded

def hextoascii(string):
   decoded = base64.b16decode(string)
   return decoded

def b64tohex(string):
   string = b64toascii(string)
   encoded = base64.b16encode(string)
   return encoded

def hextob64(string):
   string = base64.b16decode(string)
   decoded = asciitob64(string)
   return decoded

def bit32tob64(num):
   a = chr((0xFF000000 & int(num)) >> 24)
   b = chr((0xFF0000 & int(num)) >> 16)
   c = chr((0xFF00 & int(num)) >> 8)
   d = chr(0xFF & int(num))
   ans = ''.join([a, b, c, d])
   ans = asciitob64(ans)
   return ans
