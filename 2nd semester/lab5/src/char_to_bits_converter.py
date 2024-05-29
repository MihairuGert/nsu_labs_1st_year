def byte_to_binary(byte):
    return format(byte, '08b')

def print_file_in_binary(filename):
    with open(filename, 'rb') as file:
        byte = file.read(1)
        while byte:
            print(byte_to_binary(byte[0]), end=' ')
            byte = file.read(1)

print_file_in_binary('out.txt')

