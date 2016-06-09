# $Id: GCCDD.py,v 1.1 2001/11/05 19:53:33 zeller Exp $
# Using delta debugging on GCC input

import DD
import string
import subprocess


class MyDD(DD.DD):
    def __init__(self):
        DD.DD.__init__(self)

    def _test(self, deltas):
        print('222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222')
        # Build input
        input = ""
        for (index, character) in deltas:
            input = input + character

        # Write input to `input.c'
        out = open('input.c', 'w')
        out.write(input)
        out.close()

        print(self.coerce(deltas))

        # Invoke GCC
        pipe = subprocess.Popen("(gcc -o tempProgram input.c dominion.c rngs.c) 2>&1", shell=True, universal_newlines=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        output = str.join("", pipe.stdout.readlines())
        status = pipe.wait()

        # Determine outcome
        if status == 0:
            pipe = subprocess.Popen("(./tempProgram) 2>&1", shell=True, universal_newlines=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            output = str.join("", pipe.stdout.readlines())
            status = pipe.wait()
            print('============================================')
            print('============================================')
            print(output)
            print('============================================')
            print(status)
            print('============================================')
            print('============================================')
            if status == 0:
                return self.PASS
            elif status == -1:
                return self.FAIL
            return self.UNRESOLVED
        return self.UNRESOLVED

    def coerce(self, deltas):
        # Pretty-print the configuration
        input = ""
        for (index, character) in deltas:
            input = input + character
        return input


if __name__ == '__main__':
    #I will want to set the deltas to the functions input


    # Load deltas from `initializeGameBug.c'
    deltas = []
    index = 1
    for character in open('initializeGameTest.c').read():
        deltas.append((index, character))
        index = index + 1

    mydd = MyDD()

    print('Simplifying failure-inducing input...')
    c = mydd.ddmin(deltas)              # Invoke DDMIN
    print('The 1-minimal failure-inducing input is', mydd.coerce(c))
    print('Removing any element will make the failure go away.')

    # print

    #print("Isolating the failure-inducing difference...")
    #(c, c1, c2) = mydd.dd(deltas) # Invoke DD
    #print("The 1-minimal failure-inducing difference is", c)
    #print(mydd.coerce(c1), "passes,", mydd.coerce(c2), "fails")




# Local Variables:
# mode: python
# End:
