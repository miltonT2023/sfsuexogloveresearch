# File paths
input_file = r'C:\Users\milto\OneDrive\Desktop\Exo Glove Research\ArdunioVersion\EMG_PR_code\data\NoMovement6.txt'
output_file = r'C:\Users\milto\OneDrive\Desktop\Exo Glove Research\ArdunioVersion\EMG_PR_code\EMG_PR_c\datac.c'

# Read data from the file
with open(input_file, 'r') as f:
    lines = f.readlines()

# Process the data
data = [list(map(float, line.split())) for line in lines]

# Define dimensions
num_rows = len(data)
num_cols = len(data[0])

# Write C array to file
with open(output_file, 'w') as f:
    f.write('#include <stdio.h>\n\n')
    f.write('float data[{}][{}] = {{\n'.format(num_rows, num_cols))
    
    for row in data:
        line = '    {'
        line += ', '.join(f'{value:.6f}' for value in row)
        line += '},\n'
        f.write(line)
    
    f.write('};\n')

print(f'C array has been saved to {output_file}')
