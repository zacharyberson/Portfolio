import sys
from wrapper import *


#setup
key = bytearray(raw_input("Please enter key\n"),'ascii')

if len(key) != 16 and len(key) != 24 and len(key) != 32:
   print "error: key is not 16, 24, or 32 bytes\nexiting"
   sys.exit()

user = ''
pword = ''
filename = ''
data = ''
upmac = bytearray()
filemac = bytearray()

setup()

#main loop
prompt = raw_input("\n(w)rite a file\n(r)ead a file\n(q)uit\n\n")
while(prompt != 'q'):
   if prompt == 'w' or prompt == 'r':
      user = raw_input("enter username\n")
      pword = raw_input("enter password\n")
      filename = raw_input("enter filename\n")
      upmac = getmac(user + pword, 16)
      filemac = getmac(filename, 20)

      if prompt == 'w':
         local = ''
         while local != 'e' and local != 'l': 
            local = raw_input("\n(e)nter data or (l)ocate file to encrypt\n\n")
            if local == 'e':
               with open(".data_tmp", "w") as f:
                  f.write(raw_input("enter data to encrypt\n"))
               writefile(key, upmac, filemac, ".data_tmp")
            elif local == 'l':
               filez = raw_input("enter name of file\n")
               writefile(key, upmac, filemac, filez)
            else:
               print("\ninvalid input\n")
         
      elif prompt == 'r':
         data = readfile(key, upmac, filemac)
         print "\ncontents retreived:"
         print "-----"
         print data
         print "-----\n"

   else:
      print "\ninvalid input\n"

   user = ''
   pword = ''
   filename = ''
   data = ''
   prompt = raw_input("\n(w)rite a file\n(r)ead a file\n(q)uit\n")



#cleanup
for n in key:
   n = 0
for n in upmac:
   n = 0
for n in filemac:
   n = 0
close()
sys.exit()
