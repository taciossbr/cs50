import sys
import crypt

if len(sys.argv) != 2:
    print("Usage: ./crack hash")
    exit(1)

for i in range(ord('a'), ord('z') + 1):
    for j in range(ord('a'), ord('z') + 1):
        for k in range(ord('a'), ord('z') + 1):
            for l in range(ord('a'), ord('z') + 1):
                s = chr(i) + chr(j) + chr(k) + chr(l)
                if crypt.crypt(s, '50') == sys.argv[1]:
                    print(s)
                    sys.exit(0)

for i in range(ord('A'), ord('Z') + 1):
    for j in range(ord('a'), ord('z') + 1):
        for k in range(ord('a'), ord('z') + 1):
            for l in range(ord('a'), ord('z') + 1):
                s = chr(i) + chr(j) + chr(k) + chr(l)
                if crypt.crypt(s, '50') == sys.argv[1]:
                    print(s)
                    sys.exit(0)