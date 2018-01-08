
s = str(input("Number: "))
r = 0
for c in s[len(s) - 2::-2]:
    tmp = int(c) * 2
    r += tmp // 10 + tmp % 10
    print("%c: %d, %d" %(c, tmp, r))
for c in s[::-2]:
    r += int(c)
    print("\t%c: %d" % (c, r))

if r % 10 != 0:
    print("INVALID")
elif s[0:2] == '34' or s[0:2] == '37':
    print("AMEX")
elif s[0] == '4':
    print("VISA")
elif s[0:2] == '51' or s[0:2] == '52' or s[0:2] == '53' or s[0:2] == '54' or s[0:2] == '55':
    print("MASTERCARD")
else:
    print("INVALIDs")

