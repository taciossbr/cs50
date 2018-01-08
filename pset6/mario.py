import cs50
print("Height: ", end="")
h = cs50.get_int()

for i in range(2,h+2):
    print(" " * (h + 1 - i), end="")
    print("#"*i)