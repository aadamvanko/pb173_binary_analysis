import sys

filename = sys.argv[1]
with open(filename) as input:
    with open("raw_bytes_" + filename, "wb") as output:
        for line in input:
            for textByte in line.split():
                number = int(textByte, 16)
                output.write(number.to_bytes(1, 'little'))
