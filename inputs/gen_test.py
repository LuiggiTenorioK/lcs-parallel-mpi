import random

dictionary = ['A','C','G','T']
sizes = [1000,10000,100000,500000]

def generate(size):
    res = ""
    for i in range(size):
        res += dictionary[random.randint(0,len(dictionary)-1)]
    return res

for size in sizes:
    filename = f"input_{size}.txt"
    with open(filename, 'w') as file:
        file.write(f"{size}\n")
        file.write(f"{size}\n")
        file.write(generate(size))
        file.write('\n')
        file.write(generate(size))
        file.write('\n')