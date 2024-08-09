#!/usr/bin/env python3

import os
import argparse


def createHeader(inletCount, outletCount):

   text = "autowatch = 1;\n"
   text += '\n'

   text += '// inlets and outlets\n'
   text += f'inlets = {inletCount};\n'
   for index in range(inletCount):
      text += f'setinletassist({index}, "text");\n'
   text += '\n'

   text += f'outlets = {outletCount};\n'
   for index in range(outletCount):
      text += f'setoutletassist({index}, "text");\n'
   text += '\n'

   return text


def createStorage():

   text += '//////////////////////////////////////////\n'
   text += '\n'
   text += 'var settings = {};\n'
   text += '\n'

   text += 'function getvalueof() {\n'
   text += 'var text = JSON.stringify(settings);\n'
   text += 'return text;\n'
   text += '}\n'
   text += '\n'

   text += 'function setvalueof(value) {\n'
   text += 'settings = JSON.parse(value);\n'
   text += '}\n'
   text += '\n'

   text += '//////////////////////////////////////////\n'
   text += '\n'

   return text


def createBody():

   text = 'function bang(){\n'
   text += '   debug("bang");\n'
   text += '}\n'
   text += '\n'

   return text


def main():

   parser = argparse.ArgumentParser(description='Create new Max javascript.')
   parser.add_argument('scriptnames', metavar='SCRIPTS', type=str, nargs='+', help='name of script to create')
   parser.add_argument('-i', '--inlet', metavar='COUNT', nargs='?', default=1, help='number of inlets')
   parser.add_argument('-o', '--outlet', metavar='COUNT', nargs='?', default=1, help='number of outlets')

   args = parser.parse_args()  # will quit here if help is called

   scriptnames = args.scriptnames
   inletCount = int(args.inlet)
   outletCount = int(args.outlet)

   for scriptname in scriptnames:
      if not scriptname.endswith('.js'):
         scriptname += '.js'

      if os.path.exists(scriptname):
         print(f'script {scriptname} does already exist')
         return

      text = createHeader(inletCount, outletCount)
      text += createStorage()
      text += createBody()

      with open(scriptname, 'w') as outfile:
         outfile.write(text)


if __name__ == '__main__':
   main()
